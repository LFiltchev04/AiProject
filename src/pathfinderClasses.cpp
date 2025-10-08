#include "pathfinderClasses.hpp"
#include <unordered_set>
//this is for a hound, fox will need some minor changes
char pathfinder::greedyPathfind(){

    double cDist = linDist(mapInstance.currentPos(),targetCoord);
    
    //if straight ahead is better, go there
    

    //if right is better, rotate, the next turn will sort movement out
    Vec2 tempHeading = mapInstance.getHeadingVector(); 
    Vec2 cPos = mapInstance.currentPos();
    
    if(!mapInstance.priorVisit(cPos)){
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
        if(!mapInstance.isWall(cPos)){
            return 'R';
        }
    }

    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+cPos,targetCoord)){
        if(!mapInstance.isWall(cPos)){
            return 'R';
        }
    }

    ninetyClockwise(tempHeading);
    if(cDist > linDist(tempHeading+cPos,targetCoord)){
        if(!mapInstance.isWall(cPos)){
            return 'R';
        }    
    }




}










   //=============================
  //                          \ || /
 //                            \  / 
//THE IMPORTANT STUFF IS HERE   \/
                                 

void pathfinder::updateMap(Percepts p){
    mapInstance.updateMap(p);
}

void pathfinder::newTarget(Vec2 tgt){
    targetCoord = tgt;
}

int pathfinder::h(Vec2 head){
    // heuristic must be computed from the node position to target
    return manhattanDistance(head, targetCoord);
}

int pathfinder::getNodeScore(Vec2 nodePos, int tentativeG){
    // f = g + h ; caller must pass tentativeG
    return tentativeG + h(/*head will be provided as node position by caller*/ nodePos); // placeholder not used here
}

// Replace LPApathfind with a correct A*/LPA-style expansion
void pathfinder::LPApathfind(){

    // clear data
    while(!bestGuess.empty()) bestGuess.pop();
    while(!completePath.empty()) completePath.pop();

    startCoord = mapInstance.currentPos();

    // seed start node
    searchNode startNode;
    startNode.nodePosition = startCoord;
    startNode.parrentCoords = startCoord;
    startNode.global = 0;
    startNode.priority = h(startCoord);
    bestGuess.push(startNode);

    // fast lookup for best g found per cell
    std::unordered_map<size_t,int> bestG;
    bestG[hashCords(startCoord.x, startCoord.y)] = 0;
    std::unordered_set<size_t> closed;

    while(!bestGuess.empty()){
        searchNode parrent = bestGuess.top();
        bestGuess.pop();

        size_t pKey = hashCords(parrent.nodePosition.x, parrent.nodePosition.y);
        if(closed.count(pKey)) continue;
        closed.insert(pKey);

        // goal test
        if(parrent.nodePosition.x == targetCoord.x && parrent.nodePosition.y == targetCoord.y){
            constrctPath(parrent.nodePosition);
            return;
        }

        // expand 4-neighbours around parrent.nodePosition
        Vec2 dir = {0,1};
        for(int d=0; d<4; ++d){
            Vec2 nb = parrent.nodePosition + dir;
            size_t nbKey = hashCords(nb.x, nb.y);

            if(mapInstance.isWall(nb) || closed.count(nbKey)){
                ninetyClockwise(dir);
                continue;
            }

            int tentativeG = parrent.global + 1;
            auto it = bestG.find(nbKey);
            if(it != bestG.end() && tentativeG >= it->second){
                ninetyClockwise(dir);
                continue;
            }

            bestG[nbKey] = tentativeG;
            int f = tentativeG + h(nb);

            searchNode neighbour(nb, parrent, f);
            // ensure neighbour.global is correct (constructor sets parrent.global+1)
            neighbour.global = tentativeG;
            neighbour.priority = f;

            bestGuess.push(neighbour);

            // write into the real map entry (getPrior returns by value — use fastAccess directly)
            mapInstance.fastAccess[nbKey].pathfindComponent = neighbour;

            ninetyClockwise(dir);
        
        }
        
    }

    // no path found -> leave completePath empty

}

void pathfinder::constrctPath(Vec2 goalNode){
    while(!completePath.empty()) completePath.pop();

    Vec2 cur = goalNode;
    int safety = 0;
    const int SAFETY_LIMIT = 10000;

    while(safety++ < SAFETY_LIMIT){
        size_t k = hashCords(cur.x, cur.y);
        auto it = mapInstance.fastAccess.find(k);
        if(it == mapInstance.fastAccess.end()) break;
        Vec2 parent = it->second.pathfindComponent.parrentCoords;
        // stop if invalid parent or reached start
        if(parent.x == cur.x && parent.y == cur.y) break;
        completePath.push(parent);
        if(parent.x == startCoord.x && parent.y == startCoord.y) break;
        cur = parent;
    }
    // caller should handle empty stack / no-path case

    //the first one has to be popped always
    completePath.pop();
    
}

std::stack<Vec2>* pathfinder::getPath(){
    return &completePath;
}

internalMap& pathfinder::getMap(){
    return mapInstance;
}

Vec2 pathfinder::getNext(){
    if(completePath.empty()) return mapInstance.currentPos();
    return completePath.top();
}

    char pathfinder::pathTranslator(){
        
        
        // defensive guards
        if(completePath.empty()) return ' ';

        // use the real mapInstance (not a copy) and real heading
        Vec2 cPos = mapInstance.currentPos();
        Vec2 heading = mapInstance.getHeadingVector();

        // drop any already-reached tiles
        while(!completePath.empty() && completePath.top().x == cPos.x && completePath.top().y == cPos.y){
            completePath.pop();
        }
        if(completePath.empty()) return ' ';

        Vec2 nextTile = completePath.top();


        
        // forward
        if(cPos + heading == nextTile){
            mapInstance.iterateCpos(heading);
            completePath.pop();
            return 'F';
        }

        // right
        Vec2 right = heading;
        ninetyClockwise(right);
        if(cPos + right == nextTile){
        
        
            if(mapInstance.trueDir('F')+cPos==nextTile){
                mapInstance.iterateCpos(right);
                return 'F';
            }

        
            //mapInstance.iterateCpos(cPos+right);
            return 'R';
        }

        // left (counter-clockwise = three clockwise)
        Vec2 left = heading;
        ninetyClockwise(left); ninetyClockwise(left); ninetyClockwise(left);
        if(cPos + left == nextTile){

            if(mapInstance.trueDir('F')+cPos==nextTile){
                mapInstance.iterateCpos(left);
                return 'F';
            }


            //mapInstance.iterateCpos(cPos+left);
            return 'L';
        }

        // back
        Vec2 back = heading;
        ninetyClockwise(back); ninetyClockwise(back);
        if(cPos + back == nextTile){


            if(mapInstance.trueDir('F')+cPos==nextTile){
                mapInstance.iterateCpos(back);
                return 'F';
            }


            // return single turn; next call will produce the second turn / movement
            //mapInstance.iterateCpos(cPos+back);
            return 'R';
        }

        // fallback
        return ' ';


        }


void pathfinder::recomputeFrom(){
    //this recomputes from the start point to the end, so it should redo the whole thing, meaning drop the stack and redo from starting point

    //dumps the stack, unfortunatley cant be more efficient
    while(!completePath.empty()){
        completePath.pop();
    }

    LPApathfind();


}

bool pathfinder::pathInvalid(){
    return false;
}