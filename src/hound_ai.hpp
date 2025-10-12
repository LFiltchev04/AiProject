#pragma once

#include"ai.hpp"
//#include "internalMap.hpp"
//#include "pathfinderClasses.hpp"
//#include "utils.hpp"
//#include "randomizedHeading.hpp"

class HoundAI : public AI {
private:
    // retain chosen target for a few turns to avoid indecision
    Vec2 currentTarget;
    int targetHoldRemaining = 0;
    const int holdDuration = 8; // adjust to keep a target for N turns

public:
    // min-heap of (priority, position) where lower priority is better
    // Use a comparator that only compares the integer priority so Vec2 doesn't
    // need an ordering operator.
    struct FoxTrackCompare {
        bool operator()(const std::pair<int, Vec2>& a, const std::pair<int, Vec2>& b) const {
            return a.first > b.first; // smaller first == higher priority
        }
    };

    std::priority_queue<
        std::pair<int, Vec2>,
        std::vector<std::pair<int, Vec2>>,
        FoxTrackCompare
    > foxTracks;

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