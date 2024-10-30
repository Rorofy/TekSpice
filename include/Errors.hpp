/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Errors
*/

#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <exception>
#include <string>

class ABasicError : public std::exception
{
    public:
        std::string _info;

        ABasicError(std::string name, std::string info) : std::exception() {_info = name + ": " + info;};

        const char *what() const throw () {return _info.c_str();}
};

#endif /* !ERRORS_HPP_ */
