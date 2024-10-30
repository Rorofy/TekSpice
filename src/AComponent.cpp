/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** AComponent
*/

#include "AComponent.hpp"

AComponent::AComponent(const std::string &name, nts::Tristate (*operation)(nts::Tristate tris, nts::Tristate right), std::size_t n, std::size_t pinMap[]) :
    _name(name),
    _operation(operation)
{
    for (std::size_t i = 0; i < n; i++) {
        this->_pinMap._mapping[pinMap[i]] = {pinMap[n+i*2], pinMap[n+i*2+1]};
        this->_chipset[pinMap[i]] = nts::UNDEFINED;
        this->_pinMap._outputs[pinMap[i]] = {0, nullptr};
        if (pinMap[n+i*2] != 0) this->_chipset[pinMap[n+i*2]] = nts::UNDEFINED;
        if (pinMap[n+i*2+1] != 0) this->_chipset[pinMap[n+i*2+1]] = nts::UNDEFINED;
    }
    delete[] pinMap;
}

AComponent::~AComponent()
{
}

void AComponent::simulate(std::size_t tick)
{
    for (auto i : this->getOuputs())
        compute(i.first);
}

nts::Tristate AComponent::compute(std::size_t pin)
{
    int lpin = std::get<0>(_pinMap._mapping[pin]);
    int rpin = std::get<1>(_pinMap._mapping[pin]);

    _chipset[pin] = _operation(std::get<1>(_pinMap._entry[lpin])->compute(std::get<0>(_pinMap._entry[lpin])),
                    std::get<1>(_pinMap._entry[rpin])->compute(std::get<0>(_pinMap._entry[rpin])));
    return _chipset[pin];
}

void AComponent::setLink(std::size_t pin, nts::IComponent &other,
    std::size_t otherPin)
{
    if (this->_chipset.find(pin) != _chipset.end())
        if (this->_pinMap._mapping.find(pin) != this->_pinMap._mapping.end())
            this->_pinMap._outputs.insert({pin, {otherPin, &other}});
        else
            this->_pinMap._entry.insert({pin, {otherPin, &other}});
    else
        throw nts::LinkPinError(this->_name);
}

void AComponent::setValue(const std::size_t pin, nts::Tristate value)
{
    this->_chipset[pin] = value;
}


void AComponent::dump() const
{
}

std::map<std::size_t, nts::Tristate> &AComponent::getChipset()
{
    return this->_chipset;
}

const std::string &AComponent::getName() const
{
    return this->_name;
}

nts::Tristate AComponent::getValue(const std::size_t pin)
{
    return this->_chipset[pin];
}

nts::Tristate and_op(nts::Tristate left, nts::Tristate right)
{
    return (left == nts::FALSE || right == nts::FALSE) ? nts::FALSE : (left == nts::TRUE && right == nts::TRUE ? nts::TRUE : nts::UNDEFINED);
}

nts::Tristate or_op(nts::Tristate left, nts::Tristate right)
{
    return (left == nts::TRUE || right == nts::TRUE) ? nts::TRUE : ((left == nts::UNDEFINED || right == nts::UNDEFINED) ? nts::UNDEFINED : nts::FALSE);
}

nts::Tristate nor_op(nts::Tristate left, nts::Tristate right)
{
    return (left == nts::UNDEFINED || right == nts::UNDEFINED) ? nts::UNDEFINED : ((left == nts::FALSE && right == nts::FALSE) ? nts::TRUE : nts::FALSE);
}

nts::Tristate nand_op(nts::Tristate left, nts::Tristate right)
{
    return (left == nts::TRUE && right == nts::TRUE) ? nts::FALSE : ((left == nts::FALSE || right == nts::FALSE) ? nts::TRUE : nts::UNDEFINED);
}

nts::Tristate xor_op(nts::Tristate left, nts::Tristate right)
{
    return (left == nts::UNDEFINED || right == nts::UNDEFINED) ? nts::UNDEFINED : (((left != nts::TRUE || right != nts::TRUE) && (left == nts::TRUE || right == nts::TRUE)) ? nts::TRUE : nts::FALSE);
}