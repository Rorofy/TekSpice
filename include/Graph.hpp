/*
** EPITECH PROJECT, 2021
** B-OOP-400-PAR-4-1-tekspice-francois.zhao [WSL]
** File description:
** Graph
*/

#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include <vector>

template<typename T>
class Graph {
    public:
        Graph(T value);

        void push_back(T const &child);
        void pop_back();
        bool empty() const;
        std::vector<Graph<T>> &getChilds() const;

        T value;
        std::vector<Graph<T>> childs;
    protected:
    private:
};

#endif /* !GRAPH_HPP_ */