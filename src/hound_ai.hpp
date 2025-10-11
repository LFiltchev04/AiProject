#pragma once

#include"ai.hpp"
//#include "internalMap.hpp"
//#include "pathfinderClasses.hpp"
//#include "utils.hpp"
//#include "randomizedHeading.hpp"

class HoundAI : public AI {
private:
//pathfinder pFind;
//mode state;
//Vec2 tgt;

//int wallBumps = 0;

//semirandomHeading semiRand;

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
    

    //theese ought to be removed and put in the base class so i can use them for the foxes

    
    //one move per turn to avoid unexpectedly hitting walls and having the cPos becoming inconsistent
    //std::string discoveryMode();

    //allows multiple moves per turn in areas that are seen
    //std::vector<std::string> traverseMode();

    //void setMode();

    //Vec2 randomHead();
    //std::vector<std::string> multiturn();
    //std::string discovery();
    
    //std::string runModel();
    
    
};