compile:
	g++ -std=c++20 -Iinclude/ -o bin/program src/main.cpp
all: compile
run:
	./bin/program
crun: compile run