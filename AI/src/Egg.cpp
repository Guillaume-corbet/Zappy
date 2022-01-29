/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** Egg
*/

#include "Egg.hpp"

Egg::Egg(int x, int y)
{
    _isAlive = true;
    _isHatched = false;
    _pos = new Position(x, y);
    _food = 10;
}

Egg::~Egg()
{
}

void Egg::run()
{

}

void Egg::cry()
{

}

void Egg::setPos(int x, int y)
{
    _pos->x = x;
    _pos->y = y;
}

Position *Egg::getPos()
{
    return (_pos);
}

void Egg::death()
{
    _isAlive = false;
}

bool Egg::isAlive()
{
    return (_isAlive);
}

void Egg::hatched()
{
    _isHatched = true;
}

bool Egg::isHatched()
{
    return (_isHatched);
}

void Egg::newConnexionForEgg()
{
    _pos->~Position();
    std::exit(0);
}