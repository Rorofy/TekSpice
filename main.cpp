/*
** EPITECH PROJECT, 2021
** OOP [WSL: Ubuntu]
** File description:
** main
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include "Parser.hpp"
#include "Shell.hpp"

int main(int ac, char **av)
{
    std::ifstream t(av[1]);
    std::stringstream buffer;
    buffer << t.rdbuf();

    try {
        Parser parse(buffer.str());

        nts::Shell shell(parse.parse());
        shell.start();
    } catch (ABasicError &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }

    return 0;
}