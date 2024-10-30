##
## EPITECH PROJECT, 2021
## OOP [WSL: Ubuntu]
## File description:
## Makefile
##

SRC =	src/Parser.cpp			\
		src/Shell.cpp			\
		src/Circuit.cpp			\
		src/AComponent.cpp		\
		main.cpp

CXX = g++
CXXFLAGS = -W -I./include -std=gnu++17
LDFLAGS =
NAME = nanotekspice

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	@./$(NAME)

.PHONY: all clean fclean re test

debug: CXXFLAGS += -ggdb
debug: all