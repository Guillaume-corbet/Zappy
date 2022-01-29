/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** Parse
*/

#ifndef PARSE_HPP_
#define PARSE_HPP_

#include "Ai.hpp"
#include "Error.hpp"

class Parse {
    public:
        Parse();
        ~Parse();
        int usage(char *bin);
        void init(char **av);
        int find_arguments(std::vector<std::string> tab);
        std::string getMachine()const;
        std::string getName()const;
        int getPort()const;

    protected:
    private:
        std::string _machine;
        std::string _name;
        int _port;
};

#endif /* !PARSE_HPP_ */