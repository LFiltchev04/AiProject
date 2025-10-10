#pragma once

#include"ai.hpp"
#include "internalMap.hpp"
#include "pathfinderClasses.hpp"
#include "utils.hpp"

class HoundAI : public AI {
private:
pathfinder pFind;
mode state;
//tracks the qunatity of wall hits and switches the random heading to another one after a couple of wall hits
int wallBumps;
Vec2 tgt;
//somewhat smartly tries to redirect from a wall 
Vec2 semirandomHeading();

public:
    HoundAI(
        unsigned id,
        unsigned agent_speed,
        std::mt19937_64* rng
    );
    virtual std::vector<std::string> Run(
        Percepts& percepts,
        AgentComm * comms
    );

    //attempts to fix a fox`s position using scent data and comms from other hounds, hounds bark their nearest fox, this is used to attemp a location calculation
    Vec2 trackFox(AgentComm* commData, Vec2 cAbsPos, double scent);
    
    //one move per turn to avoid unexpectedly hitting walls and having the cPos becoming inconsistent
    std::string discoveryMode(int limit);

    //allows multiple moves per turn in areas that are seen
    std::vector<std::string> traverseMode();

    void setMode();
    std::string runModel();
    
    
};