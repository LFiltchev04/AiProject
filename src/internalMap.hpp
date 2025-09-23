#ifndef INTERNALMAP_HPP
#define INTERNALMAP_HPP

#include "vec2.hpp"
#include "logger.hpp"
#include "percepts.hpp"


struct pastState {
    Vec2 heading;
    Vec2 absPos;
    Percepts vizData; 
};

class internalMap{
    Vec2 cHeading;
    std::vector<pastState> inMap;

    public:
    internalMap();
    //used to return the visual data of an old state, starting from the initial turn 0
    pastState* operator[](unsigned int);
    //exclusivley for when a fox uses a teleporter 
    void wipeData();



};
























#endif