flags = -pedantic-errors -std=c++11

main.o: main.cpp helpers.hpp termcolor.hpp gameflow.hpp
	g++ $(flags) -c $<

gameflow.o: gameflow.cpp gameflow.hpp helpers.hpp termcolor.hpp
	g++ $(flags) -c $<

helpers.o: helpers.cpp helpers.hpp
	g++ $(flags) -c $<

game: main.o gameflow.o helpers.o
	g++ $(flags) $^ -o $@

clean:
	rm -rf *.o game

tar:
	tar -czvf game.tgz *.cpp *.hpp

.PHONY: clean tar