#pragma once

#include<algorithm>
#include<string>
#include<random>
#include<cstdlib>
#include<iostream>
#include"percepts.hpp"
#include"comm.hpp"
#include "pathfinderClasses.hpp"
#include "randomizedHeading.hpp"
#include "utils.hpp"
#include <queue>


class AI {
    
    
    mode state;
    Vec2 tgt;
    bool toggleTraverse;

    int wallBumps = 0;

protected:
    // Necessary, do not delete.
    unsigned id;
    unsigned agent_speed;
    std::mt19937_64* rng;
public:
    semirandomHeading semiRand;

    AI();
    AI(
        unsigned id, 
        unsigned agent_speed,
        std::mt19937_64* rng
    );
    virtual std::vector<std::string> Run(
        Percepts & percepts,
        AgentComm * comms
    );
    //i pretty much just went whatever with the class hirearchy, all the code for pathfinding and map update/iteration is accessible solely trought this object, its extremely ugly
    //the pathfinder and the map object are quite coupled and as a result the function calls to get to anything are long and very ugly.
    pathfinder pFind;

     std::vector<std::string> discoveryMode();


    void setMode();
    mode getMode();

    Vec2 randomHead();
    std::vector<std::string> multiturn();
    std::string discovery();
    
    std::vector<std::string> runModel(bool,int);


};



