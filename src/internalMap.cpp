#include "internalMap.hpp"
#include <unordered_map>
#include "utils.hpp"


internalMap::internalMap(){

heading = 'F';
cAbsPos = {0,0};
   
}

void internalMap::updateMap(Percepts nVizData){

    
    //spin the percepts so they align with the internal absolute map
    
    //heading.x==0 and heading.y==1
    if(heading == 'F'){
        //aligns with internal map, dump as is into hash map

        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),node(nVizData.forward[relDist][0]));
            std::cout<<"saw a:"<<nVizData.forward[relDist]<<" at: "<< cAbsPos.x << "<-x|y->"<<cAbsPos.y+1+relDist<<std::endl;
            //mainLogger.push({cAbsPos.x,cAbsPos.y+1+relDist},nVizData.forward[relDist]);
            
        }

        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x-1-relDist, cAbsPos.y), node(nVizData.left[relDist][0]));
            std::cout<<"saw a:"<<nVizData.forward[relDist]<<" at: "<< cAbsPos.x-1-relDist << "<-x|y->"<<cAbsPos.y<<std::endl;
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x+1+relDist, cAbsPos.y), node(nVizData.left[relDist][0]));
            std::cout<<"saw a:"<<nVizData.forward[relDist]<<" at: "<< cAbsPos.x+1+relDist << "<-x|y->" <<cAbsPos.y<<std::endl;
        }
        
        return;
    }


    //rotated relative right
    //heading.x==1 and heading.y==0

    if(heading == 'R'){

        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x+1+relDist,cAbsPos.y),node(nVizData.forward[relDist][0]));
            std::cout<<"saw a:"<<nVizData.forward[relDist]<<" at: "<< cAbsPos.x+1+relDist << "<-x|y->" <<cAbsPos.y<<std::endl;

        }

        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),node(nVizData.left[relDist][0]));
            std::cout<<"saw a:"<<nVizData.forward[relDist]<<" at: "<< cAbsPos.x << "<-x|y->"<<cAbsPos.y+1+relDist<<std::endl;

        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.y-1-relDist,cAbsPos.y),node(nVizData.forward[relDist][0]));
        }

        return;
    }

    //rotated relative back
    //heading.x==0 and heading.y==-1
    if(heading == 'B'){
        
        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y-1-relDist),node(nVizData.forward[relDist][0]));
        }

        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x-1-relDist,cAbsPos.y),node(nVizData.forward[relDist][0]));
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x+1+relDist,cAbsPos.y),node(nVizData.forward[relDist][0]));
        }

        
        
        return;
    }

    //rotated relative left
    //
    if(heading == 'L'){
        
        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x-1-relDist,cAbsPos.y),node(nVizData.forward[relDist][0]));
        }
        
        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),node(nVizData.forward[relDist][0]));
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),node(nVizData.forward[relDist][0]));
        }

        return;
    }

}

Vec2 internalMap::trueDir(char direction){

    
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


}



void internalMap::parseCmds(std::vector<std::string> &commandList){

    //feeds the internal map with orientation corrected data, if bad data gets here everything downstream breaks
    for(std::string cmd : commandList){
        cAbsPos + trueDir(cmd.at(0));
    }

    //so there is a node to pathfind trough
    fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y),node('O'));


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
    return it->second.type == 'W';
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