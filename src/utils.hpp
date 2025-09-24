#include <string>
#include <vector>
#include <sstream>
#include "json.hpp"
using json = nlohmann::json;




//jsonifies the percepts so i can push them to the logging server
inline std::string agentTypeToString(AgentType t) {
    switch(t) {
        case AgentType::FOX:   return "FOX";
        case AgentType::HOUND: return "HOUND";
        case AgentType::NONE:  return "NONE";
    }
}

void to_json(json& j, const Sighting& s) {
    j = json{
        {"type", agentTypeToString(s.type)},
        {"direction", s.direction},
        {"distance", s.distance}
    };
}

void to_json(json& j, const Percepts& p) {
    j = json{
        {"current", p.current},
        {"forward", p.forward},
        {"backward", p.backward},
        {"left", p.left},
        {"right", p.right},
        {"sightings", p.sightings},
        {"scent", p.scent}
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
