#include "vec2.hpp"
#include "internalMap.hpp"
#include "logger.hpp"
#include "util.h"


class pathfinder {
    Vec2 targetCoord;
    Vec2 startCoord;
    
    int pastVisits;
    internalMap* mapInstance;
    searchNode cNode;

    //used for deriving the optimal path, barebones A*, may need to be changed to an LPA* later, which i am sure will require an overhaul precisely so as to make my current efforts worthless
    std::priority_queue<searchNode> bestGuess;
    std::stack<Vec2> completePath;

    public:
    //should take a reference to the instantiated map, will be done later for convenience
    pathfinder();

    char greedyPathfind();
    void newTarget(Vec2 newTgt);

    void LPApathfind();
    int getNodeScore(Vec2 nodePos, Vec2 head);
    int h(Vec2 head);
    void constrctPath(Vec2 goalNode);

    std::stack<Vec2>* getPath();
    internalMap* getMap();

};

class houndPathfinder: private pathfinder{
    public:
    char nextStep();
};

