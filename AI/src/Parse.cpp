/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** main
*/

#include "Parse.hpp"

Parse::Parse()
{
    _machine = "127.0.0.1";
    _port = 0;
}

int Parse::usage(char *bin)
{
    std::cout << "USAGE: " << bin << " -p port -n name -h machine\n\t\
    port is the port number\n\t\
    name is the name of the team\n\t\
    machine is the name of the machine; localhost by default\n" << std::endl;
    return (0);
}

void Parse::init(char **av)
{
    std::vector<std::string> tab;

    for (size_t i = 0; av[i]; i++)
        tab.push_back(std::string(av[i]));
    if (find_arguments(tab) != 0)
        throw Error("Missing argument.");
}

int Parse::find_arguments(std::vector<std::string> tab)
{
    for (size_t i = 0; i < tab.size(); i++) {
        if (tab.at(i) == "-p" && i < tab.size() - 1 && std::atoi(tab.at(i + 1).c_str()) != 0) {
            _port = std::atoi(tab.at(i + 1).c_str());
        } else if (tab.at(i) == "-m" && i < tab.size() - 1) {
            _machine = tab.at(i + 1);
        } else if (tab.at(i) == "-n" && i < tab.size() - 1) {
            _name = tab.at(i + 1);
        }
    }
    if (_port == 0 || _name.empty())
        return (1);
    return (0);
}

std::string Parse::getMachine()const
{
    return (_machine);
}

std::string Parse::getName()const
{
    return (_name);
}

int Parse::getPort()const
{
    return (_port);
}