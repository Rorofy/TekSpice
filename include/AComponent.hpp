/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** AComponent
*/

#ifndef ACOMPONENT_HPP_
#define ACOMPONENT_HPP_

#include <string>
#include "IComponent.hpp"

class PinMap
{
    public:
        std::map<std::size_t, std::tuple<std::size_t, nts::IComponent *>> _entry;
        std::map<std::size_t, std::tuple<std::size_t, nts::IComponent *>> _outputs;
        std::map<std::size_t, std::tuple<std::size_t, std::size_t>> _mapping;
};

class AComponent : public nts::IComponent
{
    public:
        AComponent() : nts::IComponent() {};
        AComponent(const std::string &name, nts::Tristate (*_operation)(nts::Tristate tris, nts::Tristate right), std::size_t n, std::size_t pinMap[]);
        ~AComponent();

        void simulate(std::size_t tick) override;
        virtual nts::Tristate compute(std::size_t pin) override;
        virtual void setLink(std::size_t pin, nts::IComponent& other,
            std::size_t otherPin) override;
        void dump() const override;

        std::map<std::size_t, nts::Tristate> &getChipset() override;
        const std::string &getName() const override;
        const std::map<std::size_t, std::tuple<std::size_t, nts::IComponent *>> &getOuputs() const override {return this->_pinMap._outputs;};
        const std::map<std::size_t, std::tuple<std::size_t, nts::IComponent *>> &getInputs() const override {return this->_pinMap._entry;};
        virtual void setValue(const std::size_t pin, nts::Tristate value) override;
        nts::Tristate getValue(const std::size_t pin) override;

    protected:
        nts::Tristate (*_operation)(nts::Tristate tris, nts::Tristate right);
        std::map<std::size_t, nts::Tristate> _chipset;
        std::map<std::size_t, std::string> _pinsName;
        const std::string _name;
        PinMap _pinMap;
};

class InputValue : public AComponent
{
    public:
        InputValue(const std::string &name) : AComponent(name, nullptr, 1, new std::size_t[3] {1, 2, 2}) {};

        nts::Tristate compute(std::size_t pin) override {return this->_chipset[1];};
};

class OutputValue : public AComponent
{
    public:
        OutputValue(const std::string &name) : AComponent(name, nullptr, 1, new std::size_t[3] {2, 1, 1}) {};

        nts::Tristate compute(std::size_t pin) override {if (_pinMap._entry.find(1) != _pinMap._entry.end()) _chipset[2] = std::get<1>(_pinMap._entry[1])->compute(std::get<0>(_pinMap._entry[1])); return _chipset[2];};
};

class TrueValue : public AComponent
{
    public:
        TrueValue(const std::string &name) : AComponent(name, nullptr, 1, new std::size_t[3] {1, 0, 0}) {this->_chipset[1] = nts::TRUE;};

        void setValue(const std::size_t pin, nts::Tristate value) override {};
        nts::Tristate compute(std::size_t pin) override {return nts::TRUE;};
};

class FalseValue : public AComponent
{
    public:
        FalseValue(const std::string &name) : AComponent(name, nullptr, 1, new std::size_t[3] {1, 0, 0}) {this->_chipset[1] = nts::FALSE;};

        void setValue(const std::size_t pin, nts::Tristate value) override {};
        nts::Tristate compute(std::size_t pin) override {return nts::FALSE;};
};

class ClockValue : public AComponent
{
    public:
        ClockValue(const std::string &name) : AComponent(name, nullptr, 1, new std::size_t[3] {1, 0, 0}) {this->_chipset[1] = nts::UNDEFINED;};

        nts::Tristate compute(std::size_t pin) override {_chipset[1] = _chipset[1] == nts::TRUE ? nts::FALSE : (_chipset[1] == nts::FALSE ? nts::TRUE : nts::UNDEFINED); return _chipset[1];};
};

nts::Tristate and_op(nts::Tristate left, nts::Tristate right);
class And : public AComponent
{
    public:
        And(const std::string &name) : AComponent(name, &and_op, 4, new std::size_t[12] {3, 4, 11, 10, 1, 2, 5, 6, 13, 12, 9, 8}) {};
};

nts::Tristate or_op(nts::Tristate left, nts::Tristate right);
class Or : public AComponent
{
    public:
        Or(const std::string &name) : AComponent(name, &or_op, 4, new std::size_t[12] {3, 4, 11, 10, 1, 2, 5, 6, 13, 12, 9, 8}) {};
};

class Not : public AComponent
{
    public:
        Not(const std::string &name) : AComponent(name, nullptr, 6, new std::size_t[18] {2, 4, 6, 8, 10, 12, 1, 0, 3, 0, 5, 0, 9, 0, 11, 0, 13, 0}) {}

        nts::Tristate compute(std::size_t pin) override {if (_pinMap._entry.find(std::get<0>(_pinMap._mapping[pin])) != _pinMap._entry.end()) {
                nts::Tristate t = std::get<1>(_pinMap._entry[ std::get<0>(_pinMap._mapping[pin])])->compute(std::get<0>(_pinMap._entry[ std::get<0>(_pinMap._mapping[pin])]));
                _chipset[pin] = t == nts::UNDEFINED ? nts::UNDEFINED : (t == nts::TRUE ? nts::FALSE : nts::TRUE);} return _chipset[pin];};
};

nts::Tristate nor_op(nts::Tristate left, nts::Tristate right);
class Nor : public AComponent
{
    public:
        Nor(const std::string &name) : AComponent(name, &nor_op, 4, new std::size_t[12] {3, 4, 10, 11, 1, 2, 5, 6, 8, 9, 12, 13}) {}
};

nts::Tristate nand_op(nts::Tristate left, nts::Tristate right);
class Nand : public AComponent
{
    public:
        Nand(const std::string &name) : AComponent(name, &nand_op, 4, new std::size_t[12] {3, 4, 10, 11, 1, 2, 5, 6, 8, 9, 12, 13}) {}
};

nts::Tristate xor_op(nts::Tristate left, nts::Tristate right);
class Xor : public AComponent
{
    public:
        Xor(const std::string &name) : AComponent(name, &xor_op, 4, new std::size_t[12] {3, 4, 10, 11, 1, 2, 5, 6, 8, 9, 12, 13}) {}
};

#endif /* !ACOMPONENT_HPP_ */