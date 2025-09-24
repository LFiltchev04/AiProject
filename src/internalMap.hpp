#ifndef INTERNALMAP_HPP
#define INTERNALMAP_HPP

#include "vec2.hpp"
#include "logger.hpp"
#include "percepts.hpp"
#include <unordered_map>
#include "utils.hpp"




class internalMap{
    char heading;
    Vec2 cAbsPos;
    std::unordered_map<size_t, std::string> fastAccess;


    public:
    internalMap(Percepts);
    
    virtual void parseCmds(std::vector<std::string>&) = 0;
    
    //used to update the map
    void updateMap(Percepts);

};


class foxMap : private internalMap{
    public:
    foxMap(Percepts p): internalMap(p){};

    //called when a fox it uses a teleporter 
    void wipeMap();

    virtual void parseCmds(std::vector<std::string>);
     
};

//generates an instance per hond for ever internal map, so it has actalized coordinates 
class peerHound{
    //may be optonal
    //unsigned int uid;
    Vec2 absCoords;
    float linDist;
    public:
    
    peerHound(char heading ,Vec2 relCoords);
    Vec2 getNormal();


};

class houndMap : private internalMap {
    //this holds re-embedded IDs that are barked per instance
    //i can cheat here, if the random id is passed to the same arra i can simply lift the indexes and not bother with consistent hashing
    //i am not sre i even really need them to be addressable, but it cant hurt and if it does prove simple wont even matter whether or whether not all that much

    std::vector<peerHound> peerHounds;
    
    public:
    houndMap(Percepts p):internalMap(p){};
    
};























#endif