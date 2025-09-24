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
        // Hash the first element
        size_t hash1 = std::hash<int>{}(x);
        // Hash the second element
        size_t hash2 = std::hash<int>{}(y);
        // Combine the two hash values
        return hash1
               ^ (hash2 + 0x9e3779b9 + (hash1 << 6)
                  + (hash1 >> 2));
}

//takes an orientation and turns a relative coordinate into an internal absolute coordinate
Vec2 getAbsolute(Vec2 pos, char heading, Vec2 relCoord){
    
}


//single 90 degree clockwise rotation, chain however many you need
Vec2 ninetyClockwise(Vec2 relCord){

}