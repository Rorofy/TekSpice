/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Circuit
*/

#include <unordered_map>
#include "Circuit.hpp"

Circuit::Circuit() :
    tick(0)
{
}

Circuit::~Circuit()
{
}

size_t Circuit::getTick()
{
    return this->tick;
}

void Circuit::simulation()
{
    tick++;
    for (Circuit::Buffer *buf : this->buffer) {
        buf->_comp->setValue(buf->_pin, buf->_state);
        delete buf;
    }
    buffer.clear();
    for (nts::IComponent *c : this->outputs)
        c->simulate(tick);
}

const std::vector<nts::IComponent *> &Circuit::getInputs() const
{
    return this->inputs;
}

const std::vector<nts::IComponent *> &Circuit::getOutputs() const
{
    return this->outputs;
}