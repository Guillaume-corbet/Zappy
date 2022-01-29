##
## EPITECH PROJECT, 2020
## Makefile
## File description:
## ai zappy
##

SRC			=	src/*.cpp

INCLUDE		=	-Iinclude/

BIN_NAME	=	zappy_ai

LIBS		=	-luuid

OPT			=	-o

VALGRIND	=	-g3

CFLAGS		=	-Werror -Wextra -W -Wall

PROTO_PATH	=	~/out/

GMETHOD		=	g++

all:	clean
	$(GMETHOD) $(OPT) $(BIN_NAME) $(INCLUDE) $(SRC) $(LIBS)

move:	clean
	$(GMETHOD) $(VALGRIND) $(OPT) $(BIN_NAME) $(INCLUDE) $(SRC) $(LIBS)
	(mv $(BIN_NAME) ../)

valgrind:	clean
	$(GMETHOD) $(VALGRIND) $(OPT) $(BIN_NAME) $(INCLUDE) $(SRC) $(LIBS)

proto:
	($(PROTO_PATH)proto src include/Ai.hpp -e)

.PHONY: clean
clean:
	rm -f ../$(BIN_NAME)

fclean: clean
	rm -f $(BIN_NAME)
	rm -rf .vscode

re:	fclean all

auteur:
	echo $(USER) > auteur
