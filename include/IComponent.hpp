/*
** EPITECH PROJECT, 2021
** OOP [WSL: Ubuntu]
** File description:
** IComponent
*/

#pragma once

#include <memory>
#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include "Errors.hpp"

namespace nts
{
    enum Tristate {
        UNDEFINED = (-true),
        TRUE = true,
        FALSE = false
    };

    class LinkPinError : public ABasicError
    {
        public:
            LinkPinError(std::string info) : ABasicError("LinkPinError", info) {};
    };

    class IComponent
    {
        public :
            virtual ~IComponent() = default;

            virtual void simulate(std::size_t tick) = 0;
            virtual nts::Tristate compute(std::size_t pin) = 0;
            virtual void setLink(std::size_t pin, nts::IComponent& other,
                std::size_t otherPin) = 0;
            virtual void dump() const = 0;

            virtual std::map<std::size_t, nts::Tristate> &getChipset() = 0;
            virtual const std::string &getName() const = 0;
            virtual const std::map<std::size_t, std::tuple<std::size_t, nts::IComponent *>> &getOuputs() const = 0;
            virtual const std::map<std::size_t, std::tuple<std::size_t, nts::IComponent *>> &getInputs() const = 0;
            virtual void setValue(const std::size_t pin, nts::Tristate value) = 0;
            virtual nts::Tristate getValue(const std::size_t pin) = 0;
    };
};