compile:
	g++ -std=c++20 -Iinclude/ -o bin/program src/main.cpp
all: compile
run:
	./bin/program
crun: compile run
release:
	g++ -std=c++20 -Iinclude/ -o ../RVM/Emake src/main.cpp -O3 -funroll-loops -fomit-frame-pointer -Ofast -std=c++2a -finline-functions -funswitch-loops -ffast-math -fforce-addr -pipe