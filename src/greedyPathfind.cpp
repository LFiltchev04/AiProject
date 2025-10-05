#include "greedyPathfind.hpp"

//this is for a hound, fox will need some minor changes
char pathfinder::heuristic(){

    double cDist = linDist(mapInstance->currentPos(),targetCoord);
    
    //if straight ahead is better, go there
    if(cDist > linDist(mapInstance->currentPos()+mapInstance->getHeadingVector(),targetCoord)){
        return 'F';
    }

    //if right is better, rotate, the next turn will sort movement out
    Vec2 tempHeading = mapInstance->getHeadingVector(); 
    Vec2 cPos = mapInstance->currentPos();
    
    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+cPos,targetCoord)){
        if(!mapInstance->isWall(cPos)){
            return 'R';
        }
    }

    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+targetCoord,targetCoord)){
        if(!mapInstance->isWall(cPos)){
            return 'R';
        }
    }

    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+targetCoord,targetCoord)){
        if(!mapInstance->isWall(cPos)){
            return ""
        }    
    }

}


