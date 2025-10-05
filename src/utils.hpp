#include <string>
#include <vector>
#include <sstream>
#include "json.hpp"
using json = nlohmann::json;



//this encodes the sighted object types, i should probably feed it int representations of characters to match them to the actual char for convenience
enum entity{
    empty,
    wall,
    fox,
    hound,
    teleporter,
    exit,
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
size_t hashCords(int x, int y){
        size_t hash1 = std::hash<int>{}(x);
        size_t hash2 = std::hash<int>{}(y);
        
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
}


//single 90 degree clockwise rotation, chain however many you need
void ninetyClockwise(Vec2 & relCord){
    relCord.SwapXY(); 
    relCord.y *= -1;
}

//rotates the relative position so that it faces forward
void reorient(char heading, Vec2& relLoc){
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
Vec2 getAbsolute(Vec2 pos, char heading, Vec2 relCoord){
    reorient(heading,relCoord);

    return relCoord + pos;
}

//computes linear sustance between coordinate pairs
float linDist(Vec2 posOne, Vec2 posTwo){
    return std::abs(std::sqrt((posOne.x-posTwo.x)*(posOne.x-posTwo.x)+(posOne.y-posTwo.y)*(posOne.y-posTwo.y)));
}

int manhattanDistance(Vec2 posOne, Vec2 posTwo){
    return std::abs(posTwo.x-posOne.x)+std::abs(posTwo.y-posOne.y);
}

std::string logObjParser(logObj input){

}


//gets the degrees difference between two Vec2 vectors
double angleBetween(Vec2 a, Vec2 b) {
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
        searchNode(int computedPriority){
            nodePosition = {0,0};
            parrentCoords = {0,0};
        }
        //this is supposed to iterate the whole thing, lets hope i was not retarded while writing it
        searchNode(Vec2 pos, searchNode parrent,int comptedPriority){
            nodePosition = pos;
            parrentCoords.x = parrent.nodePosition.x;
            parrentCoords.y = parrent.nodePosition.y;
            global = parrent.global++;
            priority = comptedPriority;
        }
        bool operator<(const searchNode &other) const { return priority > other.priority; }
};


//lgr mainLogger("http://127.0.0.1:3005");
