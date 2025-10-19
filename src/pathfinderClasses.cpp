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
    if(tgt!=targetCoord){
        while(!completePath.empty()){
            completePath.pop();
        }
        
    }
    targetCoord = tgt;
}

int pathfinder::h(Vec2 head){
    return manhattanDistance(head, targetCoord);
}

int pathfinder::getNodeScore(Vec2 nodePos, int tentativeG){
    return tentativeG + h(/*head will be provided as node position by caller*/ nodePos); // placeholder not used here
}

// this is not actually LPA, its bog standard A*, but it kind of behaves like one when the other parts are considered
void pathfinder::LPApathfind(){
  //  std::cout<<std::endl<<std::endl<<"RECOMPUTING"<<std::endl<<std::endl<<std::endl;

  while(!bestGuess.empty()){
        bestGuess.pop();
    } 
    while(!completePath.empty()){
        completePath.pop();
    } 

    startCoord = mapInstance.currentPos();
    
    mapInstance.consistent = true;
    
    
    if(startCoord == targetCoord){
        
        return;
    }
    
    if(mapInstance.isWall(targetCoord)){
                                                                                                                                                                                                                                                                                                                                                                                                               
        return;
    }
    
    searchNode startNode;
    startNode.nodePosition = startCoord;
    startNode.parrentCoords = startCoord;
    startNode.global = 0;
    startNode.priority = h(startCoord);
    bestGuess.push(startNode);
    //i have to remove this and start using a local hashmap, literally removes all benefit from the sparse map for most cases
    recordNode(startNode.nodePosition);

    std::unordered_map<size_t,int> bestG;                                                                                                                                                           
    bestG[hashCords(startCoord.x, startCoord.y)] = 0;
    std::unordered_set<size_t> closed;

    int iterationGuard = 0;
    const int ITERATION_LIMIT = 10000;
    int nodesExplored = 0;
    
    while(!bestGuess.empty()){
        if(++iterationGuard > ITERATION_LIMIT){
            break;
        }
        
        //this needs to be here otherwise the search space will explode, there is inherintly no limit to the search space as its just throwing guesses at hashmaps, so there has to be an arbitrary upper limit
        //on how far away you can reasonably search, also meaning you map size is bounded, when i set the coordinates to (100,100) it does not give up, i am hoping that the maximum map size is thereabout that
        //if its larger it would need to be given more allowance for searching.
        if(nodesExplored > 10000 && bestGuess.size() > 900){
            
            break;
        }
        searchNode parrent = bestGuess.top();
        bestGuess.pop();

        size_t pKey = hashCords(parrent.nodePosition.x, parrent.nodePosition.y);
        if(closed.count(pKey)){
            continue;
        } 
        closed.insert(pKey);
        nodesExplored++;

        // checks it its goal
        if(parrent.nodePosition == targetCoord){
            constrctPath(parrent.nodePosition);
            return;
        }



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
            
            neighbour.global = tentativeG;
            neighbour.priority = f;
            recordNode(neighbour.nodePosition);
            bestGuess.push(neighbour);

            //this is pretty stupid
            mapInstance.fastAccess[nbKey].pathfindComponent = neighbour;

            ninetyClockwise(dir);
        
        }
        
    }

    
}


void pathfinder::constrctPath(Vec2 goalNode){


    if(goalNode.x == 999 and goalNode.y == 999){
        completePath.push(goalNode);
        return;
    }

    while(!completePath.empty()){
        completePath.pop();
    }

    completePath.push(goalNode);
     

    Vec2 cur = goalNode;
    int safety = 0;
    const int safetyLimit = 400;

    while(safety++ < safetyLimit){
        size_t k = hashCords(cur.x, cur.y);
        auto it = mapInstance.fastAccess.find(k);
        if(it == mapInstance.fastAccess.end()) break;
        Vec2 parent = it->second.pathfindComponent.parrentCoords;

        if(parent.x == cur.x and parent.y == cur.y) break;
        completePath.push(parent);
        if(parent.x == startCoord.x && parent.y == startCoord.y) break;
        cur = parent;
    }

    
        
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

    if(completePath.top()==Vec2{999,999}){
       // std::cout<<"no route to path, returning nothing"<<std::endl;
        return ' ';
    }

    Vec2 cPos = mapInstance.currentPos();
   // std::cout<<std::endl<<std::endl<<std::endl<< cPos.to_string()<<"THIS IS THE UPCOMING MOVE< VERY IMPORTANT"<<std::endl<<std::endl<<std::endl;

    while(!completePath.empty() and completePath.top() == cPos){
        completePath.pop();
    }
    if(completePath.empty()){
        return ' ';
    } 

    Vec2 nextTile = completePath.top();

    
    Vec2 fpos = mapInstance.relativeHead('F') + cPos;
    Vec2 rpos = mapInstance.relativeHead('R') + cPos;
    Vec2 lpos = mapInstance.relativeHead('L') + cPos;
    Vec2 bpos = mapInstance.relativeHead('B') + cPos;

    //std::cout<<std::endl;
    //std::cout<<"F "<<fpos.x<<" tst "<<fpos.y<<std::endl;
    //std::cout<<"R "<<rpos.x<<" tst "<<rpos.y<<std::endl;
    //std::cout<<"B "<<bpos.x<<" tst "<<bpos.y<<std::endl;
    //std::cout<<"L "<<lpos.x<<" tst "<<lpos.y<<std::endl;

    //std::cout<<std::endl<<std::endl<<std::endl<<"LOOK HERE-TURN COUNT:"<<turn<<std::endl;
    //std::cout<<fpos.x<<" f-ward "<<fpos.y<<" THE DIR"<<std::endl;
    //std::cout<< nextTile.x<<" tl "<<nextTile.y<<" THE GOAL"<<std::endl<<std::endl<<std::endl;
    turn++;

    if(fpos == nextTile){
      //  std::cout<<"going forward"<<mapInstance.getHeading()<<std::endl;
        return 'F';
    }
    if(rpos == nextTile){
      //  std::cout<<"going right: "<<mapInstance.getHeading()<<std::endl;
        return 'R';
    }
    if(lpos == nextTile){
      //  std::cout<<"going left"<<mapInstance.getHeading()<<std::endl;
        return 'L';
    }
    //has to be like that otherwise the foxes break
    if(bpos == nextTile){
      ///  std::cout<<"should head back?"<<mapInstance.getHeading()<<std::endl;
        return 'R';
    }

   // std::cout<<"THE INTERPRETER DEFAULTED"<<std::endl;
    
    //i need to add a handler to make sure theese dont trip something up downstream
    return ' ';
}


void pathfinder::recomputeFrom(){
    //this recomputes from the start point to the end, so it should redo the whole thing, meaning drop the stack and redo from starting point
    dumpSearch();
    LPApathfind();


}

bool pathfinder::pathInvalid(){
    return !mapInstance.consistent;
}

void pathfinder::dumpSearch(){
    
    for(Vec2 iter:forCleanup){
        auto &itr = mapInstance.fastAccess.at(hashCords(iter.x,iter.y));

        //std::cout<<"IT WILL DUMP COORDINATE: "<< "("<<itr.pathfindComponent.priority<<","<<itr.pathfindComponent.priority<<")"<<std::endl;

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
        return mapInstance.currentPos();
    }
    return completePath.top();
}

//not the best solution, foxes need separate 
bool pathfinder::multiturnSafe(std::stack<Vec2> stkCpy){

    if(stkCpy.size()<3){
        return false;
    }
    for(int x=0;x<3;x++){
        if(mapInstance.wasSeen(stkCpy.top())){
            stkCpy.pop();
        }else{
            break;
        }
    }

    return true;
}

Vec2 pathfinder::getTgt(){
    return targetCoord;
}
