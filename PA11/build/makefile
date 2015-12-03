# Image Loader
MAGICKLIBS= `Magick++-config --ldflags --libs`
MAGICKFLAGS= `Magick++-config --cxxflags --cppflags`

# Bullet
BULLETLIBS= -I/usr/include/bullet -lBulletDynamics -lBulletSoftBody -lBulletCollision -lLinearMath

# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL -lassimp $(MAGICKLIBS) $(BULLETLIBS)

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x $(MAGICKFLAGS)

all: ../bin/Labyrinth

../bin/Labyrinth: ../src/main.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/Labyrinth $(LIBS)

test: ../src/GLD.h ../src/GLD.cpp
	$(CC) $(CXXFLAGS) -c ../src/GLD.cpp 

clean:
	rm ../bin/Labyrinth -f

run: ../bin/Labyrinth
	../bin/Labyrinth