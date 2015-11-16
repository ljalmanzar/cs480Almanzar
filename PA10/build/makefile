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

all: ../bin/LightExercise

../bin/AirHockey: ../src/main.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/LightExercise $(LIBS)

test: ../src/GLD.h ../src/GLD.cpp
	$(CC) $(CXXFLAGS) -c ../src/GLD.cpp 

clean:
	rm ../bin/LightExercise -f

run: ../bin/AirHockey
	../bin/LightExercise
