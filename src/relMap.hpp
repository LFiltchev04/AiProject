#pragma once
#include "percepts.hpp"

struct sightVector {
    int x;
    int y;
};
typedef std::vector<std::string> sightData;
struct posVector{
    int x;
    int y;
};
//i should move the command typedefs to a file upstream. 
class relMap {
    

    public:

        //is called exactly once, initializes the map data.
        void update(Percepts);

        //changes the current location by decoding the commands passed to the array
        void iterate(posVector);

        //returns which direction it is facing relative to the starting point
        sightVector getOrientation();




    };


    class sightRecord{
        posVector position;
        //all of theese are relative to the starting position, which is itsel North->forward, East->right
        sightData North;
        sightData East;
        sightData South;
        sightData West;

        public:
        sightRecord();
        //The default array organization is always [North, East, South, West], clockwise starting North. 
        void addPercepts(posVector,sightData[4]);

    };