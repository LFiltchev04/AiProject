#include "internalMap.hpp"
#include <unordered_map>

std::unordered_map<size_t, std::string> fastAccess;


internalMap::internalMap(Percepts initData){

    pastState initial;
    initial.absPos = {0,0};
    initial.heading = {1,0};
    initial.vizData = initData;
    
    inMap.push_back(initial);

    //pushing the initial vizibility to the fast map, has to be redone
    fastAccess.emplace(hashCords(initial.absPos.x,initial.absPos.y),&inMap[0]);
}

void internalMap::updateMap(Vec2 cAbsPos,Vec2 heading,Percepts nVizData){

    pastState toAdd;
    toAdd.absPos = cAbsPos;
    
    //spin the percepts so they align with the internal absolute map
    
    if(heading.x==0 and heading.y==1){
        //aligns with internal map, dump as is into hash map

        for(int relDist = 0; relDist<nVizData.forward.size(); relDist++){
            fastAccess.emplace(hashCords(cAbsPos.x,cAbsPos.y+1+relDist),nVizData.forward[relDist]);
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
    if(heading.x==1 and heading.y==0){

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
    if(heading.x==0 and heading.y==-1){
        
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
    if(heading.x==-1 and heading.y==0){
        
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