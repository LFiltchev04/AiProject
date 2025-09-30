#include "pathfinderClasses.hpp"

//this is for a hound, fox will need some minor changes
char pathfinder::heuristic(){

    double cDist = linDist(mapInstance->currentPos(),targetCoord);
    
    //if straight ahead is better, go there
    

    //if right is better, rotate, the next turn will sort movement out
    Vec2 tempHeading = mapInstance->getHeadingVector(); 
    Vec2 cPos = mapInstance->currentPos();
    
    if(!mapInstance->priorVisit(cPos)){
        pastVisits++;
    }


    if(cDist > linDist(cPos+tempHeading,targetCoord)){
        return 'F';
    }

    if(pastVisits == 15){
        //enter unstuck mode, will be a different function, has to determine when to xero out the trigger variable
        //return unstuck();
    }
    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+cPos,targetCoord)){
        if(!mapInstance->isWall(cPos)){
            return 'R';
        }
    }

    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+cPos,targetCoord)){
        if(!mapInstance->isWall(cPos)){
            return 'R';
        }
    }

    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+cPos,targetCoord)){
        if(!mapInstance->isWall(cPos)){
            return 'R';
        }    
    }




}


char pathfinder::unstuck(){
    
}



void pathfinder::LPApathfind(){
    
}