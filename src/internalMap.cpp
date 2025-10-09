#include "internalMap.hpp"
#include <unordered_map>
#include "utils.hpp"


internalMap::internalMap(){

heading = 'F';
cAbsPos = {0,0};
consistent = true;
}

void internalMap::updateMap(Percepts nVizData){

    // Helper lambda to get char safely
    auto pick = [](const std::string &s)->char{
        return s.empty() ? ' ' : s[0];
    };


    for(int x = 0; x<nVizData.current.size();x++){
        fastAccess[hashCords(cAbsPos.x,cAbsPos.y)]=node(nVizData.current[x][0]);
        checkChange({cAbsPos.x,cAbsPos.y},nVizData.current[x][0]);
    }


    // heading == Forward (0, +1)
    if(heading == 'F'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y + 1 + relDist };
            char ch = pick(nVizData.forward.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.left.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x + 1 + relDist, cAbsPos.y };
            char ch = pick(nVizData.right.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        return;
    }

    // heading == Right (+1, 0)
    if(heading == 'R'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x + 1 + relDist, cAbsPos.y };
            char ch = pick(nVizData.forward.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y + 1 + relDist };
            char ch = pick(nVizData.left.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.right.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        return;
    }

    // heading == Back (0, -1)
    if(heading == 'B'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y - 1 - relDist };
            char ch = pick(nVizData.forward.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x + 1 + relDist, cAbsPos.y };
            char ch = pick(nVizData.left.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.right.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        return;
    }

    // heading == Left (-1, 0)
    if(heading == 'L'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.forward.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y - 1 - relDist };
            char ch = pick(nVizData.left.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y + 1 + relDist };
            char ch = pick(nVizData.right.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        return;
    }
}


Vec2 internalMap::trueDir(char direction){

    if(direction==' '){
        //return {0,0};
    }

    if(heading == 'F'){
        
        if(direction == 'F'){
              return {0,1};
        }

        if(direction == 'R'){
              return {1,0};
        }

        if(direction == 'B'){
            return {0,-1};
        }

        if(direction == 'L'){
            return {-1,0};
        }
    }
    

    if(heading == 'R'){
        
        if(direction == 'F'){
            return {1,0};
        }

        if(direction == 'R'){
            return {0,-1};
        }

        if(direction == 'B'){
            return {-1,0};
        }

        if(direction == 'L'){
            return {0,1};
        }
    }


    if(heading == 'B'){

        if(direction == 'F'){
            return {0,-1};
        }

        if(direction == 'R'){
            return {-1,0};
        }

        if(direction == 'B'){
            return {0,1};
        }

        if(direction == 'L'){
            return {1,0};
        }
    }


    if(heading == 'L'){

        if(direction == 'F'){
            return {-1,0};
        }

        if(direction == 'R'){
            return {0,1};
        }

        if(direction == 'B'){
            return {1,0};
        }

        if(direction == 'L'){
            return {0,-1};
        }
    }

    //return {0,0};
}


void internalMap::iterateState(std::string cmdIn){
    if(cmdIn.empty()) return;
    char c = cmdIn[0];
    // compute absolute target cell for this relative command
    Vec2 rel = trueDir(c);
    Vec2 absTarget = cAbsPos + rel;

    if(c == 'F'){
        // only move forward if the absolute target cell is not a wall
        if(!isWall(absTarget)){
            iterateCpos(rel);
        }
    } else {
        // rotations should update heading immediately (do not gate on a relative coord)
        changeHeading(c);
    }
     
 }

Vec2 internalMap::getHeadingVector(){
    return trueDir(heading);
}

Vec2 internalMap::currentPos(){
    return cAbsPos;
}

bool internalMap::isWall(Vec2 checkPos){
   size_t key = hashCords(checkPos.x, checkPos.y);

    auto it = fastAccess.find(key);
    if (it == fastAccess.end()) return false;        // not present
    return it->second.type == 'w';
};

bool internalMap::priorVisit(Vec2 checkThis){
    if(fastAccess.find(hashCords(checkThis.x,checkThis.y)) == fastAccess.end()){
        return false;
    }

    return true;
}


node internalMap::getPrior(Vec2 atPlace){
    //i am not sure that this will not blow up if the element is not in the hashmap
    try{
        if(fastAccess.find(hashCords(atPlace.x,atPlace.y))==fastAccess.end()){
            return node();
        } 
        return fastAccess.find(hashCords(atPlace.x,atPlace.y))->second;
    }catch(std::exception err){

        //return error in a way i can interpret
        node ndn;
        return ndn;
    }
    
}


bool internalMap::wasSeen(Vec2 analyzeThis){
    std::vector<emptySeenNodes> &sightData = xOnlyAccess.at(analyzeThis.x);
    
    for(emptySeenNodes iter:sightData){

        //check if above
        if(iter.zeroOne>=analyzeThis.y and analyzeThis.y > iter.visitPoint.x){
            return true;
        }

        //check if ahead
        if(iter.oneZero>=analyzeThis.x and analyzeThis.x > iter.visitPoint.x){
            return true;
        }

        //check if below
        if(iter.zeroMinus<=analyzeThis.y and analyzeThis.y < iter.visitPoint.y){
            return true;
        }

        //check if behind
        if(iter.minusZero<=analyzeThis.x and analyzeThis.x < iter.visitPoint.x){
            return true;
        }
    }

    return false;
}



void internalMap::changeHeading(char in){
    if(in==' '){
        return;
    }

    if(heading == 'F'){
        this->heading = in;

    std::cout<<"heading set to1:"<<this->heading<<std::endl;
        return;
    }

    if(heading=='R'){
        if(in=='F'){
            heading = 'R';
        }

        if(in=='R'){
            heading = 'B';
        }

        if(in=='B'){
            heading = 'L';
        }

        if(in=='L'){
            heading = 'F';
        }

    std::cout<<"heading set to2:"<<this->heading<<std::endl;
        return;
    }


    if(heading=='B'){
        if(in=='F'){
            heading = 'B';
        }

        if(in=='R'){
            heading = 'L';
        }

        if(in=='B'){
            heading = 'F';
        }

        if(in=='L'){
            heading = 'R';
        }

    std::cout<<"heading set to3:"<<this->heading<<std::endl;
        return;
    }

    if(heading=='L'){
        if(in=='F'){
            heading = 'L';
        }

        if(in=='R'){
            heading = 'F';
        }

        if(in=='B'){
            heading = 'R';
        }

        if(in=='L'){
            heading = 'B';
        }

    std::cout<<"heading set to4:"<<this->heading<<std::endl;
        return;
    }

    
}


void internalMap::iterateCpos(Vec2 newPos){
    cAbsPos=cAbsPos+newPos;
}


void internalMap::changeCpos(Vec2 newPos){
    cAbsPos = newPos;
}

char internalMap::getHeading(){
    return heading;
}


void internalMap::checkChange(Vec2 pos, char type){
    
        auto hashmapIter = fastAccess.find(hashCords(pos.x,pos.y));
        if(hashmapIter==fastAccess.end()){
            return;
        }

        if(hashmapIter->second.type == type){
            return;
        }else{
            std::cout<<std::endl<<std::endl<<std::endl<<"MAP NOT CONSISTENT"<<std::endl<<std::endl<<std::endl;
            consistent=false;
        }
    
}


Vec2 internalMap::relativeHead(char in){
    if(heading == 'F'){
        if(in=='F'){
            return {0,1};
        }
        if(in=='R'){
            return {1,0};
        }
        if(in=='B'){
            return {0,-1};
        }
        if(in=='L'){
            return {-1,0};
        }
    }

    if(heading == 'R'){
        if(in=='F'){
            return {1,0};
        }
        if(in=='R'){
            return {0,-1};
        }
        if(in=='B'){
            return {-1,0};
        }
        if(in=='L'){
            return {0,1};
        }
    }

    if(heading == 'B'){
        if(in=='F'){
            return {0,-1};
        }
        if(in=='R'){
            return {-1,0};
        }
        if(in=='B'){
            return {0,1};
        }
        if(in=='L'){
            return {1,0};
        }
    }

    if(heading == 'L'){
        if(in=='F'){
            return {-1,0};
        }
        if(in=='R'){
            return {0,1};
        }
        if(in=='B'){
            return {1,0};
        }
        if(in=='L'){
            return {0,-1};
        }
    }

    return {0,0};
}




internalMap::~internalMap(){

}