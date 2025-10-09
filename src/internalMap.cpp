#include "internalMap.hpp"
#include <unordered_map>
#include "utils.hpp"


internalMap::internalMap(Percepts initData){


    //pushing the initial vizibility to the fast map, has to be redone
    //fastAccess.emplace(hashCords(absPos.x, absPos.y),&inMap[0]);
}

void internalMap::updateMap(Percepts nVizData){

    
    //spin the percepts so they align with the internal absolute map
    
    //heading.x==0 and heading.y==1
    if(heading == 'f'){
        //aligns with internal map, dump as is into hash map

        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),nVizData.forward[relDist]);
            
            //mainLogger.push({cAbsPos.x,cAbsPos.y+1+relDist},nVizData.forward[relDist]);
            
        }

        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x-1-relDist, cAbsPos.y), nVizData.left[relDist]);
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x+1+relDist, cAbsPos.y), nVizData.left[relDist]);
        }

        return;
    }


    //rotated relative right
    //heading.x==1 and heading.y==0
    if(heading == 'r'){

        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x+1+relDist,cAbsPos.y),nVizData.forward[relDist]);
        }

        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),nVizData.left[relDist]);
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.y-1-relDist,cAbsPos.y),nVizData.forward[relDist]);
        }

        return;
    }

    //rotated relative back
    //heading.x==0 and heading.y==-1
    if(heading == 'b'){
        
        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y-1-relDist),nVizData.forward[relDist]);
        }

        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x-1-relDist,cAbsPos.y),nVizData.forward[relDist]);
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x+1+relDist,cAbsPos.y),nVizData.forward[relDist]);
        }

        
        
        return;
    }

    //rotated relative left
    if(heading == 'l'){
        
        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x-1-relDist,cAbsPos.y),nVizData.forward[relDist]);
        }
        
        for(int relDist = 0; relDist<nVizData.left.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),nVizData.forward[relDist]);
        }

        for(int relDist = 0; relDist<nVizData.right.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),nVizData.forward[relDist]);
        }

        return;
    }

}

Vec2 internalMap::trueDir(char direction){

    
    if(heading == 'f'){
        
        if(direction == 'f'){
              return {0,1};
        }

        if(direction == 'r'){
              return {1,0};
        }

        if(direction == 'b'){
            return {0,-1};
        }

        if(direction == 'l'){
            return {-1,0};
        }
    }
    

    if(heading == 'r'){
        
        if(direction == 'f'){
            return {1,0};
        }

        if(direction == 'r'){
            return {0,-1};
        }

        if(direction == 'b'){
            return {-1,0};
        }

        if(direction == 'l'){
            return {0,1};
        }
    }


    if(direction == 'b'){

        if(heading == 'f'){
            return {0,-1};
        }

        if(heading == 'r'){
            return {-1,0};
        }

        if(heading == 'b'){
            return {0,1};
        }

        if(heading == 'l'){
            return {1,0};
        }
    }


    if(direction == 'l'){

        if(heading == 'f'){
            return {-1,0};
        }

        if(heading == 'r'){
            return {0,1};
        }

        if(heading == 'b'){
            return {1,0};
        }

        if(heading == 'l'){
            return {0,-1};
        }
    }

    return {0,0};

}



void internalMap::parseCmds(std::vector<std::string> &commandList){

    //feeds the internal map with orientation corrected data, if bad data gets here everything downstream breaks
    for(std::string cmd : commandList){
        cAbsPos + trueDir(cmd.at(0));
    }

    

    fastAccess.emplace(cAbsPos);


}

Vec2 internalMap::getHeadingVector(){
    return trueDir(heading);
}

Vec2 internalMap::currentPos(){
    return cAbsPos;
}

bool internalMap::isWall(Vec2 checkPos){
    if(fastAccess.at(hashCords(checkPos.x,checkPos.y)).type == 'W'){
        return true;
    }

    return false;
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
        fastAccess.at(hashCords(atPlace.x,atPlace.y));
    }catch(std::exception err){
        //return error in a way i can interpret
        //return node();
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
