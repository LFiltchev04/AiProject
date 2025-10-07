#include "pathfinderClasses.hpp"

//this is for a hound, fox will need some minor changes
char pathfinder::greedyPathfind(){

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










   //=============================
  //                          \ || /
 //                            \  / 
//THE IMPORTANT STUFF IS HERE   \/
                                 





int pathfinder::h(Vec2 head){
    manhattanDistance(mapInstance->currentPos()+head,targetCoord);
}

int pathfinder::getNodeScore(Vec2 nodePos, Vec2 head){
    //this is a one-step-ahead check of the f(n) 
    return h(head) + cNode.global + 1;
}

void pathfinder::LPApathfind(){

    //i need to look the neighbours up and score the g(n)+h(n), then pick the lowest and move to that location
    
    Vec2 tempVec = mapInstance->getHeadingVector();
    Vec2 cPos = mapInstance->currentPos();
    searchNode parrent = bestGuess.top();
    bestGuess.pop();

    while(parrent.nodePosition != targetCoord){
        //doing a ninety cockwise basically goes to next neighbour, if it is a wall its not inserted for expansion, which i think ought to act the same as ignoring it.
        //it would require a 45 c-wise if it were 8-connected 
        for(int x = 0; x<4; x++){

            if(mapInstance->isWall(cPos+tempVec)){
                ninetyClockwise(tempVec);
                continue;
            }

            

            //lets hoep this pushes it where its supposed to
            searchNode current(cPos+tempVec,parrent,getNodeScore(cPos,tempVec));
            bestGuess.push(current);
            mapInstance->getPrior(cPos+tempVec).pathfindComponent = current;
            ninetyClockwise(tempVec);
        }

        parrent = bestGuess.top();
        bestGuess.pop();
        
    }
    
    constrctPath(parrent.nodePosition);

}

void pathfinder::constrctPath(Vec2 goalNode){
    
    //if i use recursion ill probably run out of memory

    //some if function to toggle logging


    Vec2 next = mapInstance->getPrior(goalNode).pathfindComponent.parrentCoords;
    completePath.push(next);
    while(next != startCoord){

        completePath.push(next);
        next = mapInstance->getPrior(next).pathfindComponent.parrentCoords;
    }

    return;

}

std::stack<Vec2>* pathfinder::getPath(){
    return &completePath;
}

internalMap* pathfinder::getMap(){
    return mapInstance;
}

Vec2 pathfinder::getNext(){
    Vec2 temp =  getPath()->top();
    return temp;
}


char pathfinder::pathTranslator(){
    Vec2 nextTile = getNext();
    Vec2 dirVec(0,1);
    Vec2 cPos = getMap()->currentPos();

    if(cPos+dirVec==nextTile){
        //i should only be popping when the forward move happens and keep rotating it otherwise
        getPath()->pop();
        return 'F';
    }

    ninetyClockwise(dirVec);
    if(cPos+dirVec==nextTile){
        return 'R';
    }

    ninetyClockwise(dirVec);
    if(cPos+dirVec==nextTile){
        return 'L';
    }

    ninetyClockwise(dirVec);
    if(cPos+dirVec==nextTile){
        return 'R';
    }


    
}


void pathfinder::recomputeFrom(){
    //this recomputes from the start point to the end, so it should redo the whole thing, meaning drop the stack and redo from starting point

    //dumps the stack, unfortunatley cant be more efficient
    while(!completePath.empty()){
        completePath.pop();
    }

    LPApathfind();


}