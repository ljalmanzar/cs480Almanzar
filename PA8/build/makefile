# Image Loader
MAGICKLIBS= `Magick++-config --ldflags --libs`
MAGICKFLAGS= `Magick++-config --cxxflags --cppflags`

# Linux
CC=g++
LIBS= -lglut -lGLEW -lGL -lassimp $(MAGICKLIBS)
MODEL= ../bin/box.obj
TEXTURE= ../bin/metal.jpg

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x $(MAGICKFLAGS)

all: ../bin/Assimp2

../bin/Assimp2: ../src/main.cpp ../src/assimpLoader.h ../src/assimpLoader.cpp
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/Assimp2 $(LIBS)

clean:
	rm ../bin/Assimp2 -f

run: ../bin/Assimp2
	../bin/Assimp2 $(MODEL) $(TEXTURE)
