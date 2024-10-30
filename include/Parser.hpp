/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Parser
*/

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <string>
#include "IComponent.hpp"
#include "Circuit.hpp"

typedef std::function<nts::IComponent *(const std::string &type)> comp_factory;

class Parser {
    public:
        class GrammarError : public ABasicError
        {
            public:
                GrammarError(std::string info) : ABasicError("LexicalSyntaticError", info) {};
        };

        class UnknowComponentError : public ABasicError
        {
            public:
               UnknowComponentError(std::string info) : ABasicError("Unknow Component", info) {};
        };

        class NotFindComponentError : public ABasicError
        {
            public:
                NotFindComponentError(std::string info) : ABasicError("Didn't find the component", info) {};
        };

        class SharedNameComponent : public ABasicError
        {
            public:
                SharedNameComponent(std::string info) : ABasicError("SharedComponentName, two component with the same name", info) {};
        };

        class EmptyChipsetError : public ABasicError
        {
            public:
                EmptyChipsetError(std::string info) : ABasicError("EmptyChipsetError", info) {};
        };

        Parser(const std::string &str);
        ~Parser();

        Circuit *parse(void);
    protected:
        std::istringstream *stream;
        std::vector<nts::IComponent *> comps;
        Circuit *circuit;

        std::unordered_map<std::string, comp_factory> factory;
        nts::IComponent *create_comp(std::string &type, std::string &name);
        void parse_links(void);
        void parse_comps(void);
        void parse_comp(std::string &line);
        void parse_link(std::string &lcomp, std::string &rcomp);
};

#endif /* !PARSER_HPP_ */