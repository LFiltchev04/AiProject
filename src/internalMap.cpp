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
    //
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

    //thank god i have a swivel chair, would have been harder otherwise
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


    //is supposed to figure out what coordinate to feed the internal map, happens right before the end of the function
    for(std::string cmd : commandList){
        cAbsPos + trueDir(cmd.at(0));
    }

    fastAccess.emplace(cAbsPos);


}


peerHound::peerHound(char heading, Vec2 relCoords){
    reorient(heading,relCoords);

    //this has to be called at the very first move
    Vec2 origin = {0,0};
    absCoords = getAbsolute(relCoords,'f',origin);
    linDist = std::sqrt((absCoords.x)*(absCoords.x)+(absCoords.y)*(absCoords.y));
}

