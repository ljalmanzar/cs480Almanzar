# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL -lassimp `Magick++-config --ldflags --libs`
MODEL= ../bin/capsule.obj

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x `Magick++-config --cxxflags --cppflags`

all: ../bin/Assimp1

../bin/Assimp1: ../src/main.cpp ../src/assimpLoader.h ../src/assimpLoader.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/Assimp1 $(LIBS)

clean:
	rm ../bin/Assimp1 -f

run: ../bin/Assimp1
	../bin/Assimp1 $(MODEL)
