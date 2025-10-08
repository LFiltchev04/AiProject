#include <string>
#include <vector>
#include <sstream>
#include "json.hpp"
#include <climits>
using json = nlohmann::json;

#ifndef UTILS_HPP
#define UTILS_HPP


//this encodes the sighted object types, i should probably feed it int representations of characters to match them to the actual char for convenience
enum entity{
    empty,
    wall,
    fox,
    hound,
    teleporter,
    ext,
    goal
};

//aggregates the reporting data for visualization or persisting on disk, that too should probably go into a utils file
struct logObj {
    entity sightType;
    Vec2 absLocation;
    unsigned id;
};


inline void to_json(json& j, const Vec2& v) {
    j = json{{"x", v.x}, {"y", v.y}};
}

inline void to_json(json& j, const logObj& l) {
    j = json{
        {"sightType", static_cast<int>(l.sightType)},
        {"absLocation", l.absLocation},              
        {"id", l.id}
    };
}




//crams two 32bit integers into a 64 bit one for hashing purpouses
size_t inline hashCords(int x, int y){
        size_t hash1 = std::hash<int>{}(x);
        size_t hash2 = std::hash<int>{}(y);
        
        // Combine the two hash values
        return hash1^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
}


//single 90 degree clockwise rotation, chain however many you need
inline void ninetyClockwise(Vec2 & relCord){
    relCord.SwapXY(); 
    relCord.y *= -1;
}

//rotates the relative position so that it faces forward
inline void reorient(char heading, Vec2& relLoc){
    if(heading == 'f'){
        return;
    }

    if(heading == 'r'){
        ninetyClockwise(relLoc);
        ninetyClockwise(relLoc);
        ninetyClockwise(relLoc);

        return;
    }

    if(heading == 'b'){
        ninetyClockwise(relLoc);
        ninetyClockwise(relLoc);

        return;
    }

    if(heading == 'l'){
        ninetyClockwise(relLoc);

        return;
    }

}

//takes an orientation and turns a relative coordinate into an internal absolute coordinate
Vec2 inline getAbsolute(Vec2 pos, char heading, Vec2 relCoord){
    reorient(heading,relCoord);

    return relCoord + pos;
}

//computes linear sustance between coordinate pairs
float inline linDist(Vec2 posOne, Vec2 posTwo){
    return std::abs(std::sqrt((posOne.x-posTwo.x)*(posOne.x-posTwo.x)+(posOne.y-posTwo.y)*(posOne.y-posTwo.y)));
}

int inline manhattanDistance(Vec2 posOne, Vec2 posTwo){
    return std::abs(posTwo.x-posOne.x)+std::abs(posTwo.y-posOne.y);
}

std::string inline logObjParser(logObj input){

}


//gets the degrees difference between two Vec2 vectors
double inline angleBetween(Vec2 a, Vec2 b) {
    double dot = a.x*b.x + a.y*b.y;
    double cross = a.x*b.y - a.y*b.x;  
    double angle = std::atan2(cross, dot);  
    return angle * 180.0 / M_PI;            
}




struct searchNode {
        Vec2 nodePosition;
        Vec2 parrentCoords;
        int priority;
        int global;
        //i need to call this instructor bedore anything else otherwise the origine node will stay broken, maybe as a part of some init method in the ai class I dont know. 
        searchNode(){
            nodePosition = {0,0};
            parrentCoords = {0,0};
            priority = INT_MAX;
            global = 0;
        }
        //this is supposed to iterate the whole thing, lets hope i was not retarded while writing it
        searchNode(Vec2 pos, const searchNode &parrent,int comptedPriority){
            nodePosition = pos;
            parrentCoords.x = parrent.nodePosition.x;
            parrentCoords.y = parrent.nodePosition.y;
            // do not use ++ on parent's global and ensure correct increment
            global = parrent.global + 1;
            priority = comptedPriority;
        }
        bool operator<(const searchNode &other) const { return priority > other.priority; }
};


//lgr mainLogger("http://127.0.0.1:3005");



#endif