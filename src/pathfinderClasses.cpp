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












int pathfinder::h(Vec2 head){
    manhattanDistance(mapInstance->currentPos()+head,targetCoord);
}

int pathfinder::getNodeScore(Vec2 nodePos, Vec2 head){
    return h(head) + cNode.global + 1;
}

void pathfinder::LPApathfind(){
    //THIS IS NOT YET DEFINED, YOU NEED TO DEFINE IT, IF IT BLOWS UP HERE YOU KNOW WHY
    mapInstance->getPrior();

    //i need to look the neighbours up and score the g(n)+h(n), then pick the lowest and move to that location
    
    Vec2 tempVec = mapInstance->getHeadingVector();
    Vec2 cPos = mapInstance->currentPos();
    searchNode parrent = bestGuess.top();
    bestGuess.pop();

    for(int x = 0; x<4; x++){
        if(mapInstance->isWall(cPos+tempVec)){
            ninetyClockwise(tempVec);
            continue;
        }

        if(mapInstance->getPrior(/*cPos+tempVec*/).type == 'G'){
            //construct path function, should go in a stack. 
        }

        bestGuess.emplace(searchNode(cPos+tempVec,parrent,getNodeScore(cPos,tempVec)));
        ninetyClockwise(tempVec);
    }

}


void pathfinder::constrctPath(){
    


    
}