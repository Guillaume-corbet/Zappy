/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** Ai
*/

#ifndef AI_HPP_
#define AI_HPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>
#include <uuid/uuid.h>
#include "TcpConnexion.hpp"
#include "Egg.hpp"

class Ai {
    public:
        enum Orientation {None, North, East, South, West};
        Ai(std::string machine, int port, std::string name);
        Ai(std::string machine, int port, std::string name, int test);
        ~Ai();
        void createResourcesNamesVect();
        void createResourcesMap();
        std::vector<std::string> parse(std::string cmd, char c);

        std::string definePriority();
        bool isIncantationRecipeGood();
        bool needStone();
        std::string isOneItemPresent(std::string cnt, std::string itemList);
        int calcDistance(int pos);
        void goSomewhere(int value, std::string to_find);
        void goSomewhereAndTakeAll(int value, std::string necessaryStones);
        void followBroadcast();
        void run();
        int seeItem(char item);
        void findBlindFood();
        void findFood();
        void incantation();
        void findStone();

        void forward();
        void right();
        void left();
        void look();
        void inventory();
        void broadcast(std::string msg);
        void connect_nbr();
        void fork_player(std::vector<std::string> tab);
        void eject();
        void take(std::string obj);
        void put(std::string obj);
        void launchIncantation();

        void createInterpretationMap();
        void createResourceNeedVect();
        void receive(std::string received);
        void getMapSize(std::string resp);
        void getForward(std::string resp);
        void getRight(std::string resp);
        void getLeft(std::string resp);
        void getLook(std::string resp);
        void getInventory(std::string resp);
        void getBroadcast(std::string resp);
        void getConnectNbr(std::string resp);
        void getForkPlayer(std::string resp);
        void getEjection(std::string resp);
        void getDeath(std::string resp);
        void getTake(std::string resp);
        void getPut(std::string resp);
        void getIncantation(std::string resp);

    protected:
        bool _debug;
    private:
        TcpConnexion *_sock;
        std::string _teamName;
        int _level;
        int _clientNum;
        std::vector<std::string> _nameResources;
        std::map<std::string, int> _resources;
        std::vector<std::map<std::string, int>> _elevationNeed;
        Position *_sizeMap;
        Position *_pos;
        Orientation _orientation;
        int _isIncant;
        bool _isAlive;
        std::vector<int> _broadNb;
        size_t _countForInventory;
        size_t _forwardedNbr;
        std::queue<std::string> _queue;
        Egg *_egg;
        std::map<std::string, void (Ai::*)(std::string param)> _behave;
        std::vector<std::string> _vision;
        std::pair<std::string, int> _lastBroad;
        std::pair<int, std::string> _broadToFollow;
        std::vector<std::string> _stoneToFind;
};

#endif /* !AI_HPP_ */