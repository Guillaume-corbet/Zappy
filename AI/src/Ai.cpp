/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** Ai
*/

#include "Ai.hpp"

// INIT //

Ai::Ai(std::string machine, int port, std::string name)
{
    std::string tmp;

    _debug = false;
    _teamName = name;
    _sock = new TcpConnexion(machine, port, name, false);
    _level = 1;
    usleep(100);
    tmp = _sock->force_receive();
    std::cout << tmp << std::endl;
    if (tmp == "ko\n") {
        _sock->destroy("Team is full");
    }
    _clientNum = std::stoi(tmp);
    createResourcesMap();
    createInterpretationMap();
    createResourceNeedVect();
    createResourcesNamesVect();
    _pos = new Position(0, 0);
    _orientation = (Orientation)(rand() % 4 + 1);
    _isIncant = 0;
    _isAlive = true;
    _forwardedNbr = 0;
    _egg = NULL;
    _countForInventory = 0;
    tmp = tmp.substr(tmp.find('\n') + 1);
    _sizeMap = new Position(std::stoi(tmp.substr(0, tmp.find(' '))), std::stoi(tmp.substr(tmp.find(' ') + 1)));
    while (name == "dumb") {
        tmp = _sock->receive_msg();
        if (tmp == "dead\n")
            _sock->destroy("player is dead.");
        sleep(10);
        _sock->send_msg("Bleug blblbl bleug\n");
    }
}

Ai::Ai(std::string machine, int port, std::string name, int test)
{
    std::string tmp;
    std::string input;

    _debug = true;
    _teamName = name;
    _sock = new TcpConnexion(machine, port, name, _debug);
    _level = 1;
    std::cout << "client NUM" << std::endl;
    _clientNum = std::stoi(_sock->force_receive());
    createResourcesMap();
    createInterpretationMap();
    createResourceNeedVect();
    createResourcesNamesVect();
    _pos = new Position(0, 0);
    _orientation = (Orientation)(rand() % 4 + 1);
    _isIncant = 0;
    _isAlive = true;
    _forwardedNbr = 0;
    _egg = NULL;
    _countForInventory = 0;
    std::cout << "map size" << std::endl;
    tmp = _sock->force_receive();
    _sizeMap = new Position(std::stoi(tmp.substr(0, tmp.find(' '))), std::stoi(tmp.substr(tmp.find(' ') + 1)));

}

Ai::~Ai()
{
}

void Ai::createResourcesNamesVect()
{
    _nameResources.push_back("linemate");
    _nameResources.push_back("deraumere");
    _nameResources.push_back("sibur");
    _nameResources.push_back("mendiane");
    _nameResources.push_back("phiras");
    _nameResources.push_back("thystame");
    for (size_t i = 0; i < 7; i++) {
        _broadNb.push_back(0);
    }
}

void Ai::createResourcesMap()
{
    _resources.insert(std::pair<std::string, int>("food", 10));
    _resources.insert(std::pair<std::string, int>("linemate", 0));
    _resources.insert(std::pair<std::string, int>("deraumere", 0));
    _resources.insert(std::pair<std::string, int>("sibur", 0));
    _resources.insert(std::pair<std::string, int>("mendiane", 0));
    _resources.insert(std::pair<std::string, int>("phiras", 0));
    _resources.insert(std::pair<std::string, int>("thystame", 0));
}

std::vector<std::string> Ai::parse(std::string cmd, char c)
{
    std::vector<std::string> res;
    std::string tmp;
    size_t i = 0;

    while (i <= cmd.size()) {
        tmp = cmd.substr(i, cmd.find(c, i) - i);
        if (tmp.empty())
            res.push_back("null");
        else
            res.push_back(tmp);
        if (cmd.find(c, i) != std::string::npos)
            i = cmd.find(c, i) + 1;
        else {
            break;
        }
        tmp.clear();
    }
    return (res);
}


// PRIORITIES //

std::string Ai::definePriority()
{
    _stoneToFind.clear();
    if (_level < 8) {
        for (size_t i = 0; i < _nameResources.size(); i++) {
            if (_resources[_nameResources.at(i)] < _elevationNeed.at(_level - 1)[_nameResources.at(i)]) {
                _stoneToFind.push_back(_nameResources.at(i));
            }
        }
    }
    if (_countForInventory > 5) {
        _countForInventory = 0;
        inventory();
    }
    if (!_isAlive)
        return ("dead");
    else if (_queue.size() > 0)
        return ("wait");
    else if ((_resources["food"] <= 5 && _isIncant) || (_resources["food"] <= 9 && !_isIncant))
        return ("food");
    else if (_egg && _egg->isAlive() && !_isIncant)
        return ("egg");
    else if (isIncantationRecipeGood() || (_isIncant && _resources["food"] > 4))
        return ("incantation");
    else if (_level > 3 && _resources["food"] > 5 && !_isIncant && _clientNum > 0 && false)
        return ("reproduce");
    else if (needStone())
        return ("stone");
    return ("food");
}

bool Ai::isIncantationRecipeGood()
{
    bool check = true;

    if (_level < 8) {
        for (size_t i = 0; i < _nameResources.size(); i++) {
            if (_resources[_nameResources.at(i)] < _elevationNeed.at(_level - 1)[_nameResources.at(i)]) {
                check = false;
            }
        }
    } else {
        return (false);
    }
    if (_resources["food"] <= 5)
        check = false;
    if (check && !_isIncant)
        _isIncant = 1;
    return (check);
}

bool Ai::needStone()
{
    if (_resources["food"] > 4 && _level < 8 && _stoneToFind.size() > 0) {
        return (true);
    }
    return (false);
}

std::string Ai::isOneItemPresent(std::string cnt, std::string itemList)
{
    if (cnt.empty() || cnt == "null" || itemList.empty())
        return ("");
    for (size_t i = 0; i < _nameResources.size(); i++) {
        if (std::strchr(cnt.c_str(), _nameResources.at(i).at(0)) && std::strchr(itemList.c_str(), _nameResources.at(i).at(0)))
            return (_nameResources.at(i));
    }
    return ("");
}


// MOVEMENTS //

int Ai::calcDistance(int pos)
{
    int lineSize = 3;
    int countTiles = 0;
    int lines = 0;

    while (pos > countTiles) {
        countTiles += lineSize;
        lineSize += 2;
        lines++;
    }
    countTiles = (countTiles - (lineSize - 2) / 2);
    if (pos < countTiles) {
        while (pos != countTiles) {
            countTiles--;
            lines++;
        }
    }
    else if (pos > countTiles) {
        while (pos != countTiles) {
            countTiles++;
            lines++;
        }
    }
    return (lines);
}

void Ai::goSomewhere(int value, std::string to_find)
{
    int lineSize = 3;
    int countTiles = 0;
    int opti = 0;
    int relativePos = 0;

    if (!to_find.empty())
        opti = _queue.size() - calcDistance(value);
    while (value > countTiles) {
        countTiles += lineSize;
        relativePos += lineSize - 1;
        lineSize += 2;
        forward();
        if (opti > 0 && std::strchr(_vision.at(relativePos).c_str(), to_find.at(0))) {
            take(to_find);
            opti--;
        }
    }
    countTiles = (countTiles - (lineSize - 2) / 2);
    if (value < countTiles) {
        left();
        while (value != countTiles) {
            if (opti > 0 && std::strchr(_vision.at(relativePos).c_str(), to_find.at(0))) {
                take(to_find);
                opti--;
            }
            countTiles--;
            relativePos--;
            forward();
        }
    }
    else if (value > countTiles) {
        right();
        while (value != countTiles) {
            if (opti > 0 && std::strchr(_vision.at(relativePos).c_str(), to_find.at(0))) {
                take(to_find);
                opti--;
            }
            countTiles++;
            relativePos++;
            forward();
        }
    }
}

void Ai::goSomewhereAndTakeAll(int value, std::string necessaryStones)
{
    int lineSize = 3;
    int countTiles = 0;
    int relativePos = 0;
    std::string item;

    while (value > countTiles) {
        if (!(item = isOneItemPresent(_vision.at(relativePos), necessaryStones)).empty()) {
            take(item);
        }
        countTiles += lineSize;
        relativePos += lineSize - 1;
        lineSize += 2;
        forward();
    }
    countTiles = (countTiles - (lineSize - 2) / 2);
    if (value < countTiles) {
        left();
        while (value != countTiles) {
            if (!(item = isOneItemPresent(_vision.at(relativePos), necessaryStones)).empty()) {
                take(item);
            }
            countTiles--;
            relativePos--;
            forward();
        }
    }
    else if (value > countTiles) {
        right();
        while (value != countTiles) {
            if (!(item = isOneItemPresent(_vision.at(relativePos), necessaryStones)).empty()) {
                take(item);
            }
            countTiles++;
            relativePos++;
            forward();
        }
    }
}

void Ai::followBroadcast()
{
    if (_broadToFollow.second.empty() || _broadToFollow.second == "")
        return;
    if (_broadToFollow.first == 0) {
        broadcast("here " + _lastBroad.first);
        _broadToFollow.second.clear();
        _broadToFollow.second = "";
        _isIncant = 5;
        return;
    } if (_broadToFollow.first == 5) {
        left();
        left();
    } else if (_broadToFollow.first == 6 || _broadToFollow.first == 7) {
        right();
    } else if (_broadToFollow.first == 3 || _broadToFollow.first == 4) {
        left();
    } forward();
    _broadToFollow.second.clear();
    _broadToFollow.second = "";
}


// RUN //

void Ai::run()
{
    std::string priority;
    std::string fromServer;

    while (true) {
        fromServer = _sock->receive_msg();
        if (!fromServer.empty())
            receive(fromServer);
        priority = definePriority();
        if (_debug && priority != "wait")
            std::cout << "Priority: " << priority << std::endl;
        if (priority == "dead") {
            _sock->destroy("player is dead.");
        } else if (priority == "wait") {
            continue;
        } _countForInventory++;
        if (priority == "food") {
            if (_level != 1)
                findFood();
            else
                findBlindFood();
        } else if (priority == "egg") {
            continue;
        } else if (priority == "reproduce") {
            continue;
        } else if (priority == "incantation") {
            launchIncantation();
        } else if (priority == "stone") {
            findStone();
        }
    }
}


// FOOD //

int Ai::seeItem(char item)
{
    int foodDistance = 100;
    int lastFoodSeen = -1;
    int distance;

    for (size_t i = 0; i < _vision.size(); i++) {
        if (_vision.at(i) != "null" && _vision.at(i).find(item) != std::string::npos) {
            if ((distance = calcDistance(i)) < foodDistance) {
                lastFoodSeen = i;
                foodDistance = distance;
            }
        }
    }
    if (_queue.size() + lastFoodSeen >= 9)
        return (-1);
    return (lastFoodSeen);
}

void Ai::findBlindFood()
{
    if (_vision.empty())
        look();
    else {
        if (_vision.at(0).find('f') != std::string::npos)
            take("food");
        if (_vision.at(0).find('l', 6) != std::string::npos && _resources["linemate"] == 0)
            take("linemate");
        forward();
        _vision.clear();
    }
}

void Ai::findFood()
{
    int foodPos;
    std::string stones = "f";

    for (size_t i = 0; i < _stoneToFind.size() && i < _resources["food"] - 2; i++) {
        stones += _stoneToFind.at(i).at(0);
    }
    if (_vision.empty())
        look();
    else {
        if ((foodPos = seeItem('f')) != -1) {
            goSomewhereAndTakeAll(foodPos, stones);
            take("food");
            if (foodPos != 0)
                forward();
            _vision.clear();
        }
        else {
            forward();
            _vision.clear();
        }
    }
}


// INCANTATION //

std::string generateUUID()
{
    char *id = static_cast<char *>(std::malloc(37));
    uuid_t nb;
    std::string res;

    uuid_generate(nb);
    uuid_unparse(nb, id);
    res = std::string(id);
    std::free(id);
    return (res);
}

void Ai::launchIncantation()
{
    if (_debug)
        std::cout << "incant nb:" << _isIncant << std::endl;
    if (_level == 1) {
        incantation();
        return;
    } if (_isIncant == 1) {
        broadcast("inc " + generateUUID() + " " + std::to_string(_level + 1));
        _isIncant = 2;
    } else if (_isIncant == 2) {
        findFood();
    } else if (_isIncant == 3) {
        if (_lastBroad.second == _elevationNeed.at(_level - 1)["player"]) {
            broadcast("launch " + _lastBroad.first + " " + std::to_string(_level + 1));
            _isIncant = 6;
        } else {
            broadcast("go " + _lastBroad.first + " " + std::to_string(_level + 1));
        }
    } else if (_isIncant == 4) {
        followBroadcast();
    } else if (_isIncant == 5) {
        return;
    } else if (_isIncant == 6) {
        incantation();
    }
}


// STONES //

void Ai::findStone()
{
    int stonePos = -1;
    std::string necessaryStones = "";
    int i = 0;
    int found;

    if (_vision.empty())
        look();
    else {
        for (size_t j = 0; j < _stoneToFind.size(); j++) {
            necessaryStones += _stoneToFind.at(j).at(0);
        } necessaryStones += "f";
        while (i < necessaryStones.size() && (stonePos = seeItem(necessaryStones.at(i))) == -1) {
            i++;
        }
        if (stonePos != -1) {
            goSomewhereAndTakeAll(stonePos, necessaryStones);
            if (i == necessaryStones.size() - 1)
                take("food");
            else
                take(_stoneToFind.at(i));
            if (stonePos != 0 || _level == 1) {
                if (_level == 1)
                    forward();
                _vision.clear();
            } else if ((found = _vision.at(0).find(necessaryStones.at(i))) != std::string::npos) {
                _vision.at(0).at(found) = 'x';
            }
        }
        else {
            forward();
            if ((_forwardedNbr >= _sizeMap->y - 1 && _orientation % 2 == 1) || ((_forwardedNbr >= _sizeMap->x - 1 && _orientation % 2 == 0)))
                left();
            _vision.clear();
        }
    }
}


// ACTIONS //

void Ai::forward()
{
    if (_sock)
        _sock->send_msg("Forward\n");
    if (_debug)
        std::cout << "Forward" << std::endl;
    _forwardedNbr++;
    if (rand() % 6 == 0)
        _forwardedNbr++;
    if ((_forwardedNbr >= _sizeMap->y - 1 && _orientation % 2 == 1) || ((_forwardedNbr >= _sizeMap->x - 1 && _orientation % 2 == 0))) {
        if (rand() % 2 == 0)
            left();
        else
            right();
        _forwardedNbr = 0;
    }
    _queue.push("Forward");
}

void Ai::right()
{
    if (_sock)
        _sock->send_msg("Right\n");
    if (_debug)
        std::cout << "Right" << std::endl;
    _forwardedNbr = 0;
    _queue.push("Right");
}

void Ai::left()
{
    if (_sock)
        _sock->send_msg("Left\n");
    if (_debug)
        std::cout << "Left" << std::endl;
    _forwardedNbr = 0;
    _queue.push("Left");
}

void Ai::look()
{
    if (_sock)
        _sock->send_msg("Look\n");
    if (_debug)
        std::cout << "Look" << std::endl;
    _queue.push("Look");
}

void Ai::inventory()
{
    if (_sock)
        _sock->send_msg("Inventory\n");
    if (_debug)
        std::cout << "Inventory" << std::endl;
    _queue.push("Inventory");
}

void Ai::broadcast(std::string msg)
{
    if (_sock)
        _sock->send_msg("Broadcast " + msg + "\n");
    if (_debug || true)
        std::cout << "Broadcast " << msg << std::endl;
    _queue.push("Broadcast");
}

void Ai::connect_nbr()
{
    if (_sock)
        _sock->send_msg("Connect_nbr\n");
    if (_debug)
        std::cout << "Connect_nbr" << std::endl;
    _queue.push("Connect_nbr");
}

void Ai::fork_player(std::vector<std::string> tab)
{
    if (_sock)
        _sock->send_msg("Fork\n");
    if (_debug)
        std::cout << "Fork" << std::endl;
    _queue.push("Fork");
}

void Ai::eject()
{
    if (_sock)
        _sock->send_msg("Eject\n");
    if (_debug)
        std::cout << "Eject" << std::endl;
    _queue.push("Eject");
}

void Ai::take(std::string obj)
{
    if (_sock)
        _sock->send_msg("Take " + obj + "\n");
    if (_debug)
        std::cout << "Take " << obj << std::endl;
    _queue.push("Take " + obj);
}

void Ai::put(std::string obj)
{
    if (_sock)
        _sock->send_msg("Set " + obj + "\n");
    if (_debug)
        std::cout << "Set " << obj << std::endl;
    _queue.push("Set " + obj);
}

void Ai::incantation()
{
    if (_sock)
        _sock->send_msg("Incantation\n");
    if (_debug)
        std::cout << "Incantation" << std::endl;
    _queue.push("Incantation");
    _isIncant = 0;
    _lastBroad.first.clear();
    _lastBroad.second = 0;
    _broadToFollow.second.clear();
    _broadToFollow.first = 1;
}


// INTERPRETATION //

void Ai::createInterpretationMap()
{
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Forward", &Ai::getForward));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Right", &Ai::getRight));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Left", &Ai::getLeft));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Look", &Ai::getLook));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Inventory", &Ai::getInventory));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Broadcast", &Ai::getBroadcast));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Connect_nbr", &Ai::getConnectNbr));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Fork", &Ai::getForkPlayer));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Eject", &Ai::getEjection));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Take", &Ai::getTake));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Put", &Ai::getPut));
    _behave.insert(std::pair<std::string, void (Ai::*)(std::string param)>("Incantation", &Ai::getIncantation));
}

void Ai::createResourceNeedVect()
{
    std::map<std::string, int> tmp;

    tmp.insert(std::pair<std::string, int>("player", 1));
    tmp.insert(std::pair<std::string, int>("linemate", 1));
    tmp.insert(std::pair<std::string, int>("deraumere", 0));
    tmp.insert(std::pair<std::string, int>("sibur", 0));
    tmp.insert(std::pair<std::string, int>("mendiane", 0));
    tmp.insert(std::pair<std::string, int>("phiras", 0));
    tmp.insert(std::pair<std::string, int>("thystame", 0));
    _elevationNeed.clear();
    _elevationNeed.push_back(tmp);
    tmp["player"] = 2;
    tmp["deraumere"] = 1;
    tmp["sibur"] = 1;
    _elevationNeed.push_back(tmp);
    tmp["linemate"] = 2;
    tmp["phiras"] = 2;
    tmp["deraumere"] = 0;
    _elevationNeed.push_back(tmp);
    tmp["player"] = 4;
    tmp["linemate"] = 1;
    tmp["deraumere"] = 1;
    tmp["sibur"] = 2;
    tmp["phiras"] = 1;
    _elevationNeed.push_back(tmp);
    tmp["deraumere"] = 2;
    tmp["mendiane"] = 3;
    tmp["phiras"] = 0;
    _elevationNeed.push_back(tmp);
    tmp["player"] = 6;
    tmp["sibur"] = 3;
    tmp["mendiane"] = 0;
    tmp["phiras"] = 1;
    _elevationNeed.push_back(tmp);
    tmp["linemate"] = 2;
    tmp["sibur"] = 2;
    tmp["mendiane"] = 2;
    tmp["phiras"] = 2;
    tmp["thystame"] = 1;
    _elevationNeed.push_back(tmp);
}

void Ai::receive(std::string received)
{
    void (Ai::*func_ptr)(std::string param);
    std::vector<std::string> tab;

    if (received.empty())
        return;
    tab = parse(received, '\n');
    for (size_t i = 0; i < tab.size(); i++) {
        if (tab.at(i) == "null")
            continue;
        if (tab.at(i) == "dead") {
            getDeath(tab.at(i));
            return;
        } else if (!std::strncmp(tab.at(i).c_str(), "message ", 8)) {
            getBroadcast(tab.at(i).substr(8));
            continue;
        } for (std::map<std::string, void (Ai::*)(std::string param)>::iterator it = _behave.begin(); it != _behave.end(); it++) {
            if (!_queue.empty() && !std::strncmp(_queue.front().c_str(), it->first.c_str(), it->first.size())) {
                func_ptr = it->second;
                if (_debug)
                    std::cout << "receive: " << it->first << "," << tab.at(i) << std::endl;
                (this->*func_ptr)(tab.at(i));
                _queue.pop();
                break;
            }
        }
    }
}

// Interpretation Functions //

void Ai::getMapSize(std::string param)
{
    _sizeMap->x = std::stoi(param.substr(0, param.find(' ')));
    _sizeMap->y = std::stoi(param.substr(param.find(' ') + 1));
}

void Ai::getLook(std::string param)
{
    bool check = false;
    param = param.substr(1, param.size() - 2);
    std::vector<std::string> tab = parse(param, ',');
    std::vector<std::string> tileCnt;
    std::map<std::string, char> entities = {
        {"Player", 'P'},
        {"Food", 'f'},
        {"Linemate", 'l'},
        {"Deraumere", 'd'},
        {"Sibur", 's'},
        {"Mendiane", 'm'},
        {"Phiras", 'p'},
        {"Thystame", 't'}
    };
    std::string tmp;
    int i = 0;

    while (i < 8) {
        if (((i + 1) * (i + 1)) == tab.size()) {
            check = true;
            break;
        }
        i++;
    }
    if (!check)
        return;
    _vision.clear();
    for (size_t j = 0; j < tab.size(); j++) {
        if (tab.at(j) == "null") {
            _vision.push_back("null");
            continue;
        } tileCnt = parse(tab.at(j), ' ');
        tmp.erase();
        for (size_t k = 0; k < tileCnt.size(); k++) {
            tmp += entities[tileCnt.at(k)];
        }
        _vision.push_back(tmp);
    }
    if (_debug) {
        std::cout << "vision result: ";
        for (size_t j = 0; j < _vision.size(); j++) {
            std::cout << _vision.at(j) << " ";
        } std::cout << std::endl;
    }
}

void Ai::getInventory(std::string param)
{
    param = param.substr(1, param.size() - 2);
    std::vector<std::string> tab = parse(param, ',');

    if (_debug)
        std::cout << "inventory: " << std::endl;
    for (size_t j = 0; j < tab.size(); j++) {
        _resources[tab.at(j).substr(0, tab.at(j).find(' '))] = std::stoi(tab.at(j).substr(tab.at(j).find(' ') + 1));
        if (_debug)
            std::cout << _resources[tab.at(j).substr(0, tab.at(j).find(' '))] << " ";
    } if (_debug)
        std::cout << std::endl;
}

void Ai::getBroadcast(std::string param)
{
    std::vector<std::string> tab = parse(param, ',');
    std::string msg;
    int tile;

    if (param == "ok") {
        // if (_lastBroad.first.empty())
        //     _lastBroad.first = "tmp";
        return;
    }
    msg = tab.at(1).substr(1);
    tile = std::stoi(tab.at(0));
    tab.clear();
    tab = parse(msg, ' ');
    if (_debug)
        std::cout << "BROADCAST: " << msg << ", tile: " << tile << std::endl;
    if (tab.size() == 3 && tab.at(0) == "inc") {
        _broadNb.at(std::stoi(tab.at(2)) - 2)++;
    }
    if (tab.size() == 3 && tab.at(0) == "inc" && std::stoi(tab.at(2)) == _level + 1 && _lastBroad.second < _elevationNeed.at(_level - 1)["player"] && _isIncant == 2) {
        if (_lastBroad.first.empty()) {
            _lastBroad = std::pair<std::string, int>(tab.at(1), 1);
        } else {
            _lastBroad.first = tab.at(1);
            _lastBroad.second++;
        }
        if (tile == 0 && _broadNb.at(_level - 1) % _elevationNeed.at(_level - 1)["player"] == 0 && _queue.front() == "Broadcast") {
            _isIncant = 3;
            _lastBroad.second = 1;
        }
    } else if (!_lastBroad.first.empty() && tab.at(1) == _lastBroad.first) {
        if (tab.at(0) == "here" && _isIncant == 3 && tile == 0)
            _lastBroad.second++;
        else if (tab.at(0) == "go" && _isIncant != 3 && _isIncant != 5 && _isIncant) {
            _isIncant = 4;
            _broadToFollow = std::pair<int, std::string>(tile, "go");
        } else if (tab.at(0) == "launch" && _isIncant == 5) {
            _isIncant = 6;
        }
    }
}

void Ai::getForward(std::string param)
{
    if (_orientation == Orientation::North)
        _pos->y--;
    else if (_orientation == Orientation::East)
        _pos->x++;
    else if (_orientation == Orientation::South)
        _pos->y++;
    else if (_orientation == Orientation::West)
        _pos->x--;
    if (_pos->x < 0)
        _pos->x = _sizeMap->x - 1;
    else if (_pos->x >= _sizeMap->x)
        _pos->x = 0;
    if (_pos->y < 0)
        _pos->y = _sizeMap->y - 1;
    else if (_pos->y >= _sizeMap->y)
        _pos->y = 0;
    _broadToFollow.second = "";
}

void Ai::getLeft(std::string param)
{
    if (_orientation == Orientation::North)
        _orientation = Orientation::West;
    else if (_orientation == Orientation::East)
        _orientation = Orientation::North;
    else if (_orientation == Orientation::South)
        _orientation = Orientation::East;
    else if (_orientation == Orientation::West)
        _orientation = Orientation::South;
}

void Ai::getRight(std::string param)
{
    if (_orientation == Orientation::North)
        _orientation = Orientation::East;
    else if (_orientation == Orientation::East)
        _orientation = Orientation::South;
    else if (_orientation == Orientation::South)
        _orientation = Orientation::West;
    else if (_orientation == Orientation::West)
        _orientation = Orientation::North;
}

void Ai::getForkPlayer(std::string param)
{
    _egg = new Egg(_pos->x, _pos->y);
    // need communication (pipe)
    if (fork() == 0) {
        while (_egg->isAlive()) {
            _egg->run();
        }
        std::exit(0);
    }
}

void Ai::getEjection(std::string param)
{
    int nb = std::stoi(param);

    if (nb == Orientation::North)
        _pos->y--;
    else if (nb == Orientation::East)
        _pos->x++;
    else if (nb == Orientation::South)
        _pos->y++;
    else if (nb == Orientation::West)
        _pos->x--;
    if (_pos->x < 0)
        _pos->x = _sizeMap->x;
    if (_pos->y < 0)
        _pos->y = _sizeMap->y;
}

void Ai::getDeath(std::string param)
{
    _isAlive = false;
    if (_debug)
        std::cout << "**** DEAD ****" << std::endl;
}

void Ai::getTake(std::string resp)
{
    if (resp == "ok") {
        _resources[_queue.front().substr(5)]++;
    }
}

void Ai::getPut(std::string resp)
{
    if (resp == "ok") {
        _resources[_queue.front().substr(4)]--;
    }
}

void Ai::getIncantation(std::string resp)
{
    if (!strncmp("Elevation underway Current level: ", resp.c_str(), 34)) {
        _resources["linemate"] -= _elevationNeed.at(_level - 1)["linemate"];
        _resources["deraumere"] -= _elevationNeed.at(_level - 1)["deraumere"];
        _resources["sibur"] -= _elevationNeed.at(_level - 1)["sibur"];
        _resources["mendiane"] -= _elevationNeed.at(_level - 1)["mendiane"];
        _resources["phiras"] -= _elevationNeed.at(_level - 1)["phiras"];
        _resources["thystame"] -= _elevationNeed.at(_level - 1)["thystame"];
        _level = std::stoi(resp.substr(34));
        if (_debug)
            std::cout << "INCANTATION SUCCESS" << std::endl;
    }
    _isIncant = 0;
    _lastBroad.first = "";
    inventory();
}

void Ai::getConnectNbr(std::string resp)
{
    _clientNum = std::stoi(resp);
}