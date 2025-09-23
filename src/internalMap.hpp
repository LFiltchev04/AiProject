#ifndef INTERNALMAP_HPP
#define INTERNALMAP_HPP

#include "vec2.hpp"
#include "logger.hpp"
#include "percepts.hpp"
#include <unordered_map>
#include "utils.hpp"

std::unordered_map<size_t, std::string> fastAccess;

struct pastState {
    Vec2 heading;
    Vec2 absPos;
    Percepts vizData; 
};

class internalMap{
    Vec2 cHeading;
    std::vector<pastState> inMap;

    public:
    internalMap(Percepts);
    
    //used to return the visual data of an old state, starting from the initial turn 0
    pastState* operator[](unsigned int);
    
    //exclusivley for when a fox uses a teleporter 
    void wipeData();
    
    //used to update the map
    void updateMap(Vec2 ,Vec2, Percepts);





};
























#endif