#ifndef __SOLAR_SYSTEM_CPP_
#define __SOLAR_SYSTEM_CPP_

#include <fstream>

#include "planet.h"
#include "solarSystem.h"

SolarSystem::SolarSystem(){

}

SolarSystem::SolarSystem( const std::string& filename ){
    initialize( filename );
}

SolarSystem::~SolarSystem(){
    Planet* currentPlanet;
    //deallocate all the memory
    for( unsigned int i = 0; i < _planetNames.size(); i++ ){
        currentPlanet = _planets[ _planetNames[i] ];
        delete currentPlanet;
    }
}

void SolarSystem::update( float dt ){
    Planet* currentPlanet;
    for( unsigned int i = 0; i < _planetNames.size(); i++ ){
        currentPlanet = _planets[ _planetNames[i] ];
        currentPlanet -> update(dt);
    }
}

bool SolarSystem::initialize( const std::string& filename ){
    //declare variables
    ifstream fin;
    std::string line;

    //attempt to open file 
    fin.clear();
    fin.open( filename.c_str() );

    if( !fin.good() ){
        std::cerr << "Error: Cannot find file: "
                  << filename << endl;
        return false;
    }

    while( fin.good() ){
        std::getline( fin, line );
        if( line == "end" ){
            break;
        }

        //save the things
        Planet * tempPlanet = new Planet( line );
        _planetNames.push_back( line );
        _planets.insert( std::pair<std::string, Planet*>( line, tempPlanet ) );

    }

    fin.close();

    return false;
}

#endif