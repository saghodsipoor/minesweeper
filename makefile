all: main

main: main.cpp Game.cpp Game.hpp
	g++ -std=c++11 -o main main.cpp Game.cpp graphics.cpp -lsfml-window \
	-lsfml-graphics -lsfml-system
