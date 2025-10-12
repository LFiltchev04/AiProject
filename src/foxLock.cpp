#include "foxLock.hpp"


foxLock::foxLock(){};

void foxLock::iterate(foxLog log){
    for(foxLog & entry : targetData){
        if(entry.lastKnown == log.lastKnown){
            entry.lastSeen = log.lastSeen;
            entry.manhattanDist = log.manhattanDist;
            return;
        }
    }

}