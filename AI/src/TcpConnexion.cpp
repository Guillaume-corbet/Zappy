/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** TcpConnexion
*/

#include "TcpConnexion.hpp"

TcpConnexion::TcpConnexion(std::string machine, int port, std::string teamName, bool debug)
{
    _debug = debug;
    if (!debug && this->init(machine, port, teamName) != 0) {
        destroy("Socket failed to initialize.");
    }
}

TcpConnexion::~TcpConnexion()
{
}

int TcpConnexion::init(std::string machine, int port, std::string teamName)
{
    struct sockaddr_in cli_addr;
    std::string tmp;
    char *str;

    if ((_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return (1);
    str = static_cast<char *>(std::malloc(1024));
    memset(str, 0, 1024);
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = inet_addr(machine.c_str());
    cli_addr.sin_port = htons(port);
    if (connect(_fd, (struct sockaddr *)&cli_addr, sizeof(cli_addr)) != 0) {
        perror(str);
        std::free(str);
        return (1);
    }
    std::free(str);
    tmp = force_receive();
    if (tmp != "WELCOME\n")
        return (1);
    send_msg(teamName + "\n");
    if (_debug || 1)
        std::cout << teamName << std::endl;
    return (0);
}

void TcpConnexion::destroy(std::string msg)
{
    if (_fd > 0 && !_debug)
        close(_fd);
    if (!msg.empty())
        throw Error(msg);
    std::exit(0);
}

void TcpConnexion::send_msg(std::string message)
{
    fd_set val_fdset;
    struct timeval timer;

    if (_debug) {
        std::cout << "SEND: " << message << "." << std::endl;
        return;
    }
    if (message.empty())
        return;
    timer.tv_sec = 0;
    timer.tv_usec = 1000;
    FD_ZERO(&val_fdset);
    FD_SET(_fd, &val_fdset);
    while (!FD_ISSET(_fd, &val_fdset)) {
        FD_ZERO(&val_fdset);
        FD_SET(_fd, &val_fdset);
        if (select(FD_SETSIZE, NULL, &val_fdset, NULL, &timer) == -1) {
            destroy("Socket closed.");
        }
    }
    if (write(_fd, message.c_str(), message.size()) <= 0)
        return;
}

std::string TcpConnexion::force_receive()
{
    std::string msg;

    while ((msg = receive_msg()).empty());
    return (msg);
}

std::string TcpConnexion::receive_msg()
{
    std::string buffer = "";
    fd_set val_fdset;
    struct timeval timer;

    if (_debug) {
        std::cout << "Simulate server:";
        if (std::getline(std::cin, buffer).eof())
            std::exit(0);
        return (buffer);
    }
    FD_ZERO(&val_fdset);
    FD_SET(_fd, &val_fdset);
    timer.tv_sec = 0;
    timer.tv_usec = 1000;
    if (select(FD_SETSIZE, &val_fdset, NULL, NULL, &timer) <= 0)
        return (buffer);
    char *tmp = static_cast<char *>(std::malloc(4096));
    memset(tmp, 0, 4096);
    if (read(_fd, tmp, 4096) <= 0) {
        std::free(tmp);
        destroy("Server closed.");
    }
    buffer += std::string(tmp);
    std::free(tmp);
    return (buffer);
}

int TcpConnexion::getFileDescriptor()const
{
    return (_fd);
}