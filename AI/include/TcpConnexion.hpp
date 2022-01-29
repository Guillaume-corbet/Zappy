/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** TCPConnexion
*/

#ifndef TCPCONNEXION_HPP_
#define TCPCONNEXION_HPP_

#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Error.hpp"

class TcpConnexion {
    public:
        TcpConnexion(std::string machine, int port, std::string teamName, bool debug);
        ~TcpConnexion();
        int init(std::string machine, int port, std::string teamName);
        void destroy(std::string msg);
        void send_msg(std::string message);
        std::string force_receive();
        std::string receive_msg();
        int getFileDescriptor()const;

    protected:
        bool _debug;
    private:
        int _fd;
};

#endif /* !TCPCONNEXION_HPP_ */
