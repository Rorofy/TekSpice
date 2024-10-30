/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Parser
*/

#include <sstream>
#include <algorithm>
#include "Parser.hpp"
#include "AComponent.hpp"

Parser::Parser(const std::string &str)
{
    stream = new std::istringstream(str);
    factory.insert({"true", [](const std::string &type) -> nts::IComponent * {return new TrueValue(type);}});
    factory.insert({"false", [](const std::string &type) -> nts::IComponent * {return new FalseValue(type);}});
    factory.insert({"input", [](const std::string &type) -> nts::IComponent * {return new InputValue(type);}});
    factory.insert({"output", [](const std::string &type) -> nts::IComponent * {return new OutputValue(type);}});
    factory.insert({"clock", [](const std::string &type) -> nts::IComponent * {return new ClockValue(type);}});
    factory.insert({"4001", [](const std::string &type) -> nts::IComponent * {return new Nor(type);}});
    factory.insert({"4011", [](const std::string &type) -> nts::IComponent * {return new Nand(type);}});
    factory.insert({"4071", [](const std::string &type) -> nts::IComponent * {return new Or(type);}});
    factory.insert({"4081", [](const std::string &type) -> nts::IComponent * {return new And(type);}});
    factory.insert({"4069", [](const std::string &type) -> nts::IComponent * {return new Not(type);}});
    factory.insert({"4030", [](const std::string &type) -> nts::IComponent * {return new Xor(type);}});/*
    factory.insert({"4008", [](const std::string &type) -> nts::IComponent * {return new Adder(type);}});
    factory.insert({"4013", [](const std::string &type) -> nts::IComponent * {return new DualFlipFlop(type);}});
    factory.insert({"4017", [](const std::string &type) -> nts::IComponent * {return new Johnson(type);}});
    factory.insert({"4040", [](const std::string &type) -> nts::IComponent * {return new Counter(type);}});
    factory.insert({"4094", [](const std::string &type) -> nts::IComponent * {return new Shift(type);}});
    factory.insert({"4512", [](const std::string &type) -> nts::IComponent * {return new DataSelector(type);}});
    factory.insert({"4514", [](const std::string &type) -> nts::IComponent * {return new Decoder(type);}});
    factory.insert({"4801", [](const std::string &type) -> nts::IComponent * {return new Ram(type);}});
    factory.insert({"2716", [](const std::string &type) -> nts::IComponent * {return new Rom(type);}});
    */
}

Parser::~Parser()
{
    if (stream != nullptr)
        delete stream;
}

nts::IComponent *Parser::create_comp(std::string &type, std::string &name)
{
    nts::IComponent *comp = nullptr;

    if (factory.find(type) == factory.end())
        throw Parser::UnknowComponentError(type);
    comp = factory[type](name);
    if (type == "input" || type == "false" || type == "true" || type == "clock") this->circuit->inputs.push_back(comp);
    if (type == "output") this->circuit->outputs.push_back(comp);
    return comp;
}

void Parser::parse_comp(std::string &line)
{
    nts::IComponent *comp = nullptr;
    std::vector<std::string> words;
    std::istringstream streamline(line);

    for(std::string s; streamline >> s;)
        words.push_back(s);
    if (words.size() != 2)
        throw Parser::GrammarError("Find " + std::to_string(words.size()) + " keywords instead of 2 : " + line);
    comp = create_comp(words[0], words[1]);
    if (comp == nullptr)
        throw Parser::UnknowComponentError(words[0]);
    this->comps.push_back(comp);
}

void Parser::parse_comps(void)
{
    std::string line;

    while (std::getline(*stream, line)) {
        if (line.empty() || line.rfind("#", 0) == 0)
            continue;
        if (line == ".chipsets:")
            break;
        throw Parser::GrammarError("Didn't find .chipsets:");
    }
    while (std::getline(*stream, line)) {
        if (line.empty() || line.rfind("#", 0) == 0)
            continue;
        if (line == ".links:")
            return;
        parse_comp(line);
    }
}

static std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string str;

    while(std::getline(ss, str, delim))
        elems.push_back(str);
    return elems;
}

void Parser::parse_link(std::string &lword, std::string &rword)
{
    std::vector<std::string> lwords;
    std::vector<std::string> rwords;
    nts::IComponent *lcomp;
    nts::IComponent *rcomp;

    lwords = split(lword, ':');
    rwords = split(rword, ':');
    if (lwords.size() != 2 || rwords.size() != 2)
        throw Parser::GrammarError("Didn't find two keywords : " + (lwords.size() != 2 ? lwords[0] + ":" + lwords[1] : rwords[0] + ":" + rwords[1]));
    lcomp = *std::find_if(comps.begin(), comps.end(), [&]
        (nts::IComponent *s){return s->getName() == lwords[0];});
    rcomp = *std::find_if(comps.begin(), comps.end(), [&]
        (nts::IComponent *s){return s->getName() == rwords[0];});
    if (lcomp == nullptr || rcomp == nullptr)
        throw Parser::NotFindComponentError(lcomp == nullptr ? lwords[0] : rwords[0]);
    lcomp->setLink(std::stoi(lwords[1]), *rcomp, std::stoi(rwords[1]));
    rcomp->setLink(std::stoi(rwords[1]), *lcomp, std::stoi(lwords[1]));
}

void Parser::parse_links(void)
{
    std::string line;

    while (std::getline(*stream, line)) {
        std::istringstream streamline(line);
        std::vector<std::string> words;

        if (line.rfind("#", 0) == 0)
            continue;
        for(std::string s; streamline >> s;)
            words.push_back(s);
        if (words.size() != 2)
            throw Parser::GrammarError("Find " + std::to_string(words.size()) + "keywords instead of 2 : " + line);
        parse_link(words[0], words[1]);
    }
}

Circuit *Parser::parse(void)
{
    circuit = new Circuit();

    parse_comps();
    if (this->comps.empty()) throw Parser::EmptyChipsetError("The circuit is empty");
    parse_links();
    return circuit;
}