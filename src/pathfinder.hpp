#include "vec2.hpp"
#include "comm.hpp"
#include "percepts.hpp"
#include "logger.hpp"
#include "defs.hpp"
#include "internalMap.hpp"

class pathfind {
    //internalMap inMap;
    Vec2 goalCoord;
    Vec2 guideVector;
    

    public:
    pathfind(Percepts initial);
    void setGoal(Vec2);

    std::vector<std::string> nextTurn(); 
    //determines the next char to move solely based on euclidean distance to goal coordinates
    char euclideanHeuristic(Vec2 refCoord);   
};
