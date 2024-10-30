/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Shell
*/

#include <fstream>
#include <set>
#include <csignal>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "Shell.hpp"
#include "AComponent.hpp"

nts::Shell::Shell(Circuit *circuit) :
    circuit(circuit)
{
}

nts::Shell::~Shell()
{
    if (circuit != nullptr)
        delete circuit;
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

bool nts::Shell::loop_running{false};

void nts::Shell::loop_stop(int signal)
{
    loop_running = false;
}

void nts::Shell::loop()
{
    loop_running = true;

	std::signal(SIGINT, Shell::loop_stop);
    while (loop_running) {
        circuit->simulation();
        display();
    }
	std::signal(SIGINT, SIG_DFL);
}

void nts::Shell::dump()
{
}

void nts::Shell::display()
{
    std::cout << "tick: " << this->circuit->getTick() << std::endl;
    std::cout << "input(s):" << std::endl;

    for (nts::IComponent *c : circuit->getInputs()) {
        nts::Tristate state = c->getValue(1);
        std::cout << "  " << c->getName() << ": " << (state == nts::TRUE ? "1" : (state == nts::FALSE ? "0" : "U")) << std::endl;
    }
    std::cout << "output(s):" << std::endl;
    for (nts::IComponent *c : circuit->getOutputs()) {
        nts::Tristate state = c->getValue(2);
        std::cout << "  " << c->getName() << ": " << (state == nts::TRUE ? "1" : (state == nts::FALSE ? "0" : "U")) << std::endl;
    }
}

void nts::Shell::input(const std::string &line)
{
    std::vector<std::string> lwords;
    Circuit::Buffer *buffer;

    lwords = split(line, '=');
    if (lwords.size() != 2 || (lwords[1] != "0" && lwords[1] != "1" && lwords[1] != "U"))
        return;
    auto in = *std::find_if(circuit->inputs.begin(), circuit->inputs.end(), [&]
        (nts::IComponent *s){return s->getName() == lwords[0];});
    if (in == nullptr)
        return;
    buffer = new Circuit::Buffer(in, 1, lwords[1] == "0" ? nts::FALSE : (lwords[1] == "1" ? nts::TRUE : nts::UNDEFINED));
    if (dynamic_cast<ClockValue*>(in) != nullptr) buffer->_state = buffer->_state == nts::TRUE ? nts::FALSE : (buffer->_state == nts::FALSE ? nts::TRUE : nts::UNDEFINED);
    circuit->buffer.push_back(buffer);
}

void nts::Shell::start()
{
    std::cout << "> ";
    for (std::string line; std::getline(std::cin, line);){
        if (line == "simulate")
            circuit->simulation();
        else if (line == "display")
            display();
        else if (line == "dump")
            dump();
        else if (line == "loop")
            loop();
        else if (line == "exit")
            return;
        else
            input(line);
        std::cout << "> ";
    }
    std::cout << "(CTRL+D)";
}