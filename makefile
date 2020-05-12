all: main

main: main.cpp Game.cpp Game.hpp
	g++ -o main main.cpp Game.cpp -std=c++11
