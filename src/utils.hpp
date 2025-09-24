#include <string>
#include <vector>
#include <sstream>

//transforms a vector of strings into an array representation 
std::string vectorToJson(const std::vector<std::string>& vec) {
    std::ostringstream oss;
    oss << "[";

    for (size_t i = 0; i < vec.size(); ++i) {
        oss << "\"" << vec[i] << "\"";
        if (i + 1 < vec.size()) {
            oss << ",";
        }
    }

    oss << "]";
    return oss.str();
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

//test
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
//i will simply write something to see wich branch i am on
//takes an orientation and turns a relative coordinate into an internal absolute coordinate
Vec2 getAbsolute(Vec2 pos, char heading, Vec2 relCoord){
    reorient(heading,relCoord);

    return relCoord + pos;
}

//computes linear sustance between coordinate pairs
float linDist(Vec2 posOne, Vec2 posTwo){
    return std::abs(std::sqrt((posOne.x-posTwo.x)*(posOne.x-posTwo.x)+(posOne.y-posTwo.y)*(posOne.y-posTwo.y))); 
}
