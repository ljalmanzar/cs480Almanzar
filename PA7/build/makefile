# Image Loader
MAGICKLIBS= `Magick++-config --ldflags --libs`
MAGICKFLAGS= `Magick++-config --cxxflags --cppflags`

# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL -lassimp $(MAGICKLIBS)
MODEL= ../bin/planet.obj
TEXTURE= ../bin/capsule0.jpg

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x $(MAGICKFLAGS)

# Program Name
PROGRAMNAME= SolarSystem

all: ../bin/$(PROGRAMNAME)

../bin/$(PROGRAMNAME): ../src/main.cpp ../src/assimpLoader.h ../src/assimpLoader.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/$(PROGRAMNAME) $(LIBS)

clean:
	rm ../bin/$(PROGRAMNAME) -f

run: ../bin/$(PROGRAMNAME)
	../bin/$(PROGRAMNAME) $(MODEL) $(TEXTURE)
