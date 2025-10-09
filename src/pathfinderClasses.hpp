#include "vec2.hpp"
#include "internalMap.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include <stack>


class pathfinder {
    Vec2 targetCoord;
    Vec2 startCoord;
    
    int pastVisits;
    internalMap mapInstance;
    searchNode cNode;

    //used for deriving the optimal path, barebones A*, may need to be changed to an LPA* later, which i am sure will require an overhaul precisely so as to make my current efforts worthless
    std::priority_queue<searchNode> bestGuess;
    std::stack<Vec2> completePath;

    //it should be pointers but it causes problems i dont want to bother with

    public:
        std::vector<Vec2> forCleanup;

    //should take a reference to the instantiated map, will be done later for convenience
    pathfinder(){};

    char greedyPathfind();
    void newTarget(Vec2 newTgt);

    void LPApathfind();
    int getNodeScore(Vec2,int);
    int h(Vec2 head);
    void constrctPath(Vec2 goalNode);

    std::stack<Vec2>* getPath();
    Vec2 getNext();
    internalMap& getMap();
    void dumpSearch();
    void recomputeFrom();

    char pathTranslator();
    bool pathInvalid();
    void updateMap(Percepts p);
    void recordNode(Vec2);

    Vec2 followingCoord();
    
    ~pathfinder(){};
};

class houndPathfinder: private pathfinder{
    public:
    char pathTranslator();
};

