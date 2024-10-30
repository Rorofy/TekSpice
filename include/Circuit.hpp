/*
** EPITECH PROJECT, 2021
** OOP [WSL: Ubuntu]
** File description:
** Circuit
*/

#pragma once
#include <vector>
#include "Graph.hpp"
#include "IComponent.hpp"

class Circuit
{
    public :
        class Buffer
        {
            public :
                Buffer(nts::IComponent *comp, size_t pin, nts::Tristate state) : 
                    _comp(comp),
                    _pin(pin),
                    _state(state){};

                nts::IComponent *_comp;
                size_t _pin;
                nts::Tristate _state;
        };

        Circuit();
        ~Circuit();

        std::vector<Buffer *> buffer;
        std::vector<nts::IComponent *> outputs;
        std::vector<nts::IComponent *> inputs;
        std::size_t tick;

        void simulation();
        size_t getTick();
        const std::vector<nts::IComponent *> &getOutputs() const;
        const std::vector<nts::IComponent *> &getInputs() const;

};