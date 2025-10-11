#include "pathfinderClasses.hpp"
#include "utils.hpp"
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
    std::cout<<std::endl<<std::endl<<"RECOMPUTING"<<std::endl<<std::endl<<std::endl;
    // clear data
    while(!bestGuess.empty()){
        bestGuess.pop();
    } 
    while(!completePath.empty()){
        completePath.pop();
    } 

    startCoord = mapInstance.currentPos();
    
    // seed start node
    searchNode startNode;
    startNode.nodePosition = startCoord;
    startNode.parrentCoords = startCoord;
    startNode.global = 0;
    startNode.priority = h(startCoord);
    bestGuess.push(startNode);
    // record the actual start coordinate (avoid recording an uninitialized Vec2)
    recordNode(startNode.nodePosition);

    // fast lookup for best g found per cell
    std::unordered_map<size_t,int> bestG;
    bestG[hashCords(startCoord.x, startCoord.y)] = 0;
    std::unordered_set<size_t> closed;

    // safety guard to avoid pathological infinite loops
    int iterationGuard = 0;
    const int ITERATION_LIMIT = 10000;
    while(!bestGuess.empty()){
        if(++iterationGuard > ITERATION_LIMIT){
            std::cerr << "LPApathfind: iteration limit reached, aborting search\n";
            break;
        }
        searchNode parrent = bestGuess.top();
        bestGuess.pop();

        size_t pKey = hashCords(parrent.nodePosition.x, parrent.nodePosition.y);
        if(closed.count(pKey)){
            continue;
        } 
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

            // skip walls and already-closed nodes
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
            recordNode(neighbour.nodePosition);
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

        
}

std::stack<Vec2>* pathfinder::getPath(){
    return &completePath;
}

internalMap& pathfinder::getMap(){
    return mapInstance;
}

Vec2 pathfinder::getNext(){
    if(completePath.empty()){
        return mapInstance.currentPos();
    } 
    return completePath.top();
}

char pathfinder::pathTranslator(){
    if(completePath.empty()){
        return ' ';
    } 

    Vec2 cPos = mapInstance.currentPos();

    // drop any path nodes equal to current position (safe-check empty after popping)
    while(!completePath.empty() && completePath.top() == cPos){
        completePath.pop();
    }
    if(completePath.empty()){
        return ' ';
    } 

    Vec2 nextTile = completePath.top();

    // convenience: compute absolute positions of the four relative moves
    Vec2 fpos = mapInstance.relativeHead('F') + cPos;
    Vec2 rpos = mapInstance.relativeHead('R') + cPos;
    Vec2 lpos = mapInstance.relativeHead('L') + cPos;
    Vec2 bpos = mapInstance.relativeHead('B') + cPos;

    std::cout<<std::endl;
    std::cout<<"F "<<fpos.x<<" tst "<<fpos.y<<std::endl;
    std::cout<<"R "<<rpos.x<<" tst "<<rpos.y<<std::endl;
    std::cout<<"B "<<bpos.x<<" tst "<<bpos.y<<std::endl;
    std::cout<<"L "<<lpos.x<<" tst "<<lpos.y<<std::endl;

    std::cout<<std::endl<<std::endl<<std::endl<<"LOOK HERE-TURN COUNT:"<<turn<<std::endl;
    std::cout<<fpos.x<<" f-ward "<<fpos.y<<" THE DIR"<<std::endl;
    std::cout<< nextTile.x<<" tl "<<nextTile.y<<" THE GOAL"<<std::endl<<std::endl<<std::endl;
    turn++;

    if(fpos == nextTile){
        std::cout<<"going forward"<<mapInstance.getHeading()<<std::endl;
        return 'F';
    }
    if(rpos == nextTile){
        std::cout<<"going right: "<<mapInstance.getHeading()<<std::endl;
        return 'R';
    }
    if(lpos == nextTile){
        std::cout<<"going left"<<mapInstance.getHeading()<<std::endl;
        return 'L';
    }
    if(bpos == nextTile){
        std::cout<<"should head back?"<<mapInstance.getHeading()<<std::endl;
        return 'B';
    }

    std::cout<<"THE INTERPRETER DEFAULTED"<<std::endl;
    
    //i need to add a handler to make sure theese dont trip something up downstream
    return ' ';
}


void pathfinder::recomputeFrom(){
    //this recomputes from the start point to the end, so it should redo the whole thing, meaning drop the stack and redo from starting point

    //dumps the stack, unfortunatley cant be more efficient
    dumpSearch();
    LPApathfind();


}

bool pathfinder::pathInvalid(){
    return !mapInstance.consistent;
}

void pathfinder::dumpSearch(){
    
    for(Vec2 iter:forCleanup){
        std::cout<<"IT WILL DUMP COORDINATE: "<< "("<<iter.x<<","<<iter.y<<")"<<std::endl;
        // get a reference to the stored node (modify map entry, not copy)
        auto &itr = mapInstance.fastAccess.at(hashCords(iter.x,iter.y));
        searchNode defaultNode;
        itr.pathfindComponent = defaultNode;
    }
    forCleanup.clear();

    while(!completePath.empty()){
        completePath.pop();
    }
}

void pathfinder::recordNode(Vec2 pos){
    forCleanup.push_back(pos);
}

Vec2 pathfinder::followingCoord(){
    if(completePath.empty()){
        // safe fallback — no next coord available
        return mapInstance.currentPos();
    }
    return completePath.top();
}

//not the best solution, but not too bad
bool pathfinder::multiturnSafe(std::stack<Vec2> stkCpy){
    for(int x=0;x<3;x++){
        if(mapInstance.wasSeen(stkCpy.top())){
            stkCpy.pop();
        }else{
            break;
        }
    }

    return true;
}




