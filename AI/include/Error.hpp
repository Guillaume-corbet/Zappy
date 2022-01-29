/*
** EPITECH PROJECT, 2020
** delivery
** File description:
** Error
*/

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <string>

class Error {
    public:
        Error(const std::string &message);
        ~Error();
        std::string err;

    protected:
    private:
};

#endif /* !ERROR_HPP_ */