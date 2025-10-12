#pragma once

#include"ai.hpp"

class FoxAI : public AI {
private:
    std::string goal;
    std::string exit;
    std::vector<std::string> teleporters;
    int pointCount = 0;
public:
    FoxAI(
        unsigned id,
        unsigned agent_speed,
        std::mt19937_64* rng,
        std::string goal,
        std::string exit,
        std::vector<std::string> teleporters);
    virtual std::vector<std::string> Run(
        Percepts& percepts,
        AgentComm * comms
    );
};