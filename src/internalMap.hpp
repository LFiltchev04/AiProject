#ifndef INTERNALMAP_HPP
#define INTERNALMAP_HPP

#include "vec2.hpp"
#include "logger.hpp"
#include "percepts.hpp"
#include <unordered_map>
#include "utils.hpp"

// forward-declare searchNode so node can hold a pointer without depending on include order
struct searchNode;

struct node{
    char type;
    int visits;
    searchNode pathfindComponent;
};

//used to make fast lookups of visited/unvisited possible, needs to be given THE ENTIRE X/Y component, if a visibility boundary 20 ahead is at position (10,10) the oneZero should be 30
struct emptySeenNodes{
//theese are to be read as (x,y) coordinate pairs signalling orientation in absolute space, the value is how many empty coordiantes were recorded.
//THEY HAVE TO BE TOTAL LENGTH, GET THE WHOLE X/Y COORDINATE COMPONENT, THIS SHOULD NOT BE RELATIVE FROM AN OBSERVATION POINT
    int zeroOne;
    int oneZero;
    int zeroMinus;
    int minusZero;
    Vec2 visitPoint;
};

class internalMap{
    char heading;
    Vec2 cAbsPos;
    //this ugly thing is used to check whether the square has been seen at any point
    //the whole idea is to mark with relative coordinated where the maximum sight was, if a coordinate is whitin the range, it is considered explored
    //if its not inside of the 4-way range, its not seen and thus completely unknown
    //its done to prevent every single percept from being stored in memory.
    //i need to change the percept interpretation logic to ensure it works properly however
    std::unordered_map<int,std::vector<emptySeenNodes>> xOnlyAccess;

    std::unordered_map<size_t, node> fastAccess;

    Vec2 trueDir(char direction);
    bool wasSeen(Vec2);
    public:
    internalMap(Percepts);
    virtual void parseCmds(std::vector<std::string>&) = 0;
    
    //used to update the map
    void updateMap(Percepts);
    Vec2 getHeadingVector();
    bool isWall(Vec2 checkPos);
    Vec2 currentPos();
    bool priorVisit(Vec2);
    node getPrior(Vec2);



};


class foxMap : private internalMap{
    public:
    foxMap(Percepts p): internalMap(p){};

    //called when a fox it uses a teleporter 
    void wipeMap();

    virtual void parseCmds(std::vector<std::string>);
     
};























#endif