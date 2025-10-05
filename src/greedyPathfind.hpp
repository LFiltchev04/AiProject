#include "vec2.hpp"
#include "internalMap.hpp"
#include "logger.hpp"
#include "utils.hpp"


class pathfinder {
    Vec2 targetCoord;
    
    internalMap* mapInstance;


    //should take a reference to the instantiated map, will be done later for convenience
    pathfinder();

    char heuristic();
    void newTarget(Vec2 newTgt);
    void newMovePattern();
    void dumpMovePattern();

    void wallRedirect();




};