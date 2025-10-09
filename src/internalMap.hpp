#ifndef INTERNALMAP_HPP
#define INTERNALMAP_HPP

#include "vec2.hpp"
#include "logger.hpp"
#include "percepts.hpp"
#include <unordered_map>
#include "utils.hpp"

// forward-declare searchNode so node can hold a pointer without depending on include order
struct searchNode;

struct node{
    char type;
    int visits;
    searchNode pathfindComponent;
    node(char tpe){
        type = tpe;
        visits = 1;
    }
    node(){
        type = 'o';
        visits = 1;
        
    }
};

//used to make fast lookups of visited/unvisited possible, needs to be given THE ENTIRE X/Y component, if a visibility boundary 20 ahead is at position (10,10) the oneZero should be 30
struct emptySeenNodes{
//theese are to be read as (x,y) coordinate pairs signalling orientation in absolute space, the value is how many empty coordiantes were recorded.
//THEY HAVE TO BE TOTAL LENGTH, GET THE WHOLE X/Y COORDINATE COMPONENT, THIS SHOULD NOT BE RELATIVE FROM AN OBSERVATION POINT
    int zeroOne;
    int oneZero;
    int zeroMinus;
    int minusZero;
    Vec2 visitPoint;
};

class internalMap{
    char heading;
    Vec2 cAbsPos;
    //this ugly thing is used to check whether the square has been seen at any point
    //the whole idea is to mark with relative coordinated where the maximum sight was, if a coordinate is whitin the range, it is considered explored
    //if its not inside of the 4-way range, its not seen and thus completely unknown
    //its done to prevent every single percept from being stored in memory.
    //i need to change the percept interpretation logic to ensure it works properly however
    std::unordered_map<int,std::vector<emptySeenNodes>> xOnlyAccess;



    bool wasSeen(Vec2);

    public:
    bool consistent;
    //turns a relative space character into absolute space vector
    Vec2 trueDir(char direction);

        std::unordered_map<size_t, node> fastAccess;

        //called for every command to maintain heading
    void changeHeading(char);

    internalMap();

    void iterateState(std::string);
    
    //used to update the map every turn
    void updateMap(Percepts);

    //returns the relative direction the AI is pointed towards
    Vec2 getHeadingVector();
    
    //mostly for internal use
    char getHeading();
    
    //determines whether a certain node is traversible
    bool isWall(Vec2 checkPos);
    
    //returns the current position in internal-space
    Vec2 currentPos();
    
    //determines if the node has been seen or visited
    bool priorVisit(Vec2);
    
    //returns a ndoe object if the coordinated have been seen or visited
    node getPrior(Vec2);
    
    
    //changes the internal maps tracker of the AIs location, for use with instructions
    void iterateCpos(Vec2);
    
    //changes the internal maps tracker of the AIs location
    void changeCpos(Vec2);

    //used to determine if a planned path is no longer optimal/possible, checks if a node is inconsistent with a prior observation/plan path
    void checkChange(Vec2 pos, char seenType);

    Vec2 relativeHead(char);
    ~internalMap();
};























#endif