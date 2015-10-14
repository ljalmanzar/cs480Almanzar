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
        //cout << "I just updated " << _planetNames[i] << endl;
        currentPlanet = _planets[ _planetNames[i] ];
        currentPlanet -> update(dt);
    }
}

bool SolarSystem::initialize( const std::string& filename ){
    //declare variables
    ifstream fin;
    std::string line;
    int counter = 0;

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
        Planet * tempPlanet = new Planet( line, counter++ );
        _planetNames.push_back( line );
        _planets.insert( std::pair<std::string, Planet*>( line, tempPlanet ) );

    }

    fin.close();

    // set all the target pointers
    for( unsigned int i = 1; i < _planetNames.size(); i++ ){
        Planet* currentPlanet;
        currentPlanet = _planets[ _planetNames[i] ];

        currentPlanet->setTarget( _planets[currentPlanet->getTargetKey()]);

    }

    return false;
}

int SolarSystem::getNumOfPlanets() const{
    return _planetNames.size();
}

std::vector< std::vector<Vertex> > SolarSystem::getAllGeometries(){
    Planet* currentPlanet;
    std::vector< std::vector<Vertex> > result( _planetNames.size() );
    for( unsigned int i = 0; i < _planetNames.size(); i++ ){

        currentPlanet = _planets[ _planetNames[i] ];
        result.push_back( currentPlanet->getGeometry() );
    }
    return result;   
}

Planet * SolarSystem::getPlanetPointer( unsigned int index ){
    if( index >= _planetNames.size() ){
        return NULL;
    } else {
        return _planets[ _planetNames[index] ];
    }
}

#endif