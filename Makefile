##
## EPITECH PROJECT, 2020
## Makefile
## File description:
## zappy
##

MAKEFLAGS	+=	--no-print-directory

all:	clean
	(cd AI && make move)
	(cd server && make move)
zappy_server:	clean
	(cd server && make move)
zappy_ai:	clean
	(cd AI && make move)

proto:
	(cd server && make proto)

.PHONY: clean
clean:
	(cd AI && make clean)
	(cd server && make clean)

fclean: clean
	(cd AI && make fclean)
	(cd server && make fclean)
	rm -rf .vscode

re:	fclean all

auteur:
	echo $(USER) > auteur
