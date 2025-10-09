#pragma once

#include"ai.hpp"
#include "internalMap.hpp"
#include "pathfinderClasses.hpp"

class HoundAI : public AI {
private:
pathfinder pFind;
char mode;
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
    char discoveryMode();
    
};