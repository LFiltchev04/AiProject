#include "vec2.hpp"
#include "internalMap.hpp"
#include "logger.hpp"
#include "util.h"


class pathfinder {
    Vec2 targetCoord;
    //if a node passes trought nodes that have already been visited too much it means its probably stuck
    //this gets incremented and if it passes a vertain threshold, it changes strategy from greedy optimisation of distance to trying to not go on 
    //previously visited nodes. If that is not possible, it goes along nodes who have been visited least
    int pastVisits;
    internalMap* mapInstance;
    searchNode cNode;
    std::priority_queue<searchNode> bestGuess;
    std::stack<searchNode> completePath;


    //should take a reference to the instantiated map, will be done later for convenience
    pathfinder();

    char heuristic();
    void newTarget(Vec2 newTgt);

    void LPApathfind();
    int getNodeScore(Vec2 nodePos, Vec2 head);
    int h(Vec2 head);
    void constrctPath();


};


struct pathNode{
    int x;
    int y;
    char type;
};


