#ifndef __SOLAR_SYSTEM_H_
#define __SOLAR_SYSTEM_H_

#ifndef __PLANET_H_
class Planet;
#endif

class SolarSystem{
    public:
        // constructors/destructors
        SolarSystem();
        SolarSystem(const std::string& filename);
        ~SolarSystem();

        // operators
        void update(float dt);

        // setters
        bool initialize(const std::string& filename);

        // getters
        int getNumOfPlanets() const;
        std::vector< std::vector<Vertex> > getAllGeometries();
        Planet * getPlanetPointer( unsigned int index );
        
    private:
        // planets
        std::map<std::string, Planet*> _planets;
        std::vector<std::string> _planetNames;
        // singleton bodies
};

#endif