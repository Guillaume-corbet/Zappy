/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** main
*/

#include "Parse.hpp"

int main(int ac, char **av)
{
    Parse *parsor = new Parse();

    if (ac == 2 && av[1] == "-help")
        return (parsor->usage(av[0]));
    else if (ac == 5 || ac == 7) {
        try {
            parsor->init(av);
            Ai *client = new Ai(parsor->getMachine(), parsor->getPort(), parsor->getName());
            client->run();
        }
        catch (Error const &error) {
            std::cerr << error.err << std::endl;
            return (84);
        }
    }
    else {
        std::cerr << "Wrong arguments." << std::endl;
        return (84);
    }
    return (0);
}