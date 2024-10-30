/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Shell
*/

#ifndef SHELL_HPP_
#define SHELL_HPP_

#include "Circuit.hpp"

namespace nts
{
    class Shell {
        public:
            Shell(Circuit *circuit);
            ~Shell();

            void static loop_stop(int signal);
            void start();

        protected:
            Circuit *circuit;
    		static bool loop_running;

            void display();
            void input(const std::string &line);
            void input();
            void loop();
            void dump();

    };
}

#endif /* !SHELL_HPP_ */