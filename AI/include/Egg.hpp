/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** Egg
*/

#ifndef EGG_HPP_
#define EGG_HPP_

#include <map>
#include "Position.hpp"

class Egg {
    public:
        Egg(int x, int y);
        ~Egg();
        void run();
        void cry();
        void setPos(int x, int y);
        Position *getPos();
        void death();
        bool isAlive();
        void hatched();
        bool isHatched();
        void newConnexionForEgg();

    protected:
    private:
        bool _isAlive;
        Position *_pos;
        bool _isHatched;
        int _food;

};

#endif /* !EGG_HPP_ */