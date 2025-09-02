#include"agent.hpp"

Agent::Agent() {}
Agent::Agent(
    Vec2 loc, Vec2 heading, AgentSight sight, unsigned id, std::mt19937_64* rng,
    unsigned speed, AgentType type, std::string goal, std::string exit,
    std::vector<std::string> & teleporters) 
    : loc(loc), heading(heading), sight(sight), id(id), state(AgentState::Active),
    speed(speed), type(type), points(0)
{
    if (type == AgentType::FOX) {
        ai = std::make_unique<FoxAI>(id, speed, rng, goal, exit, teleporters);
    }
    else if (type == AgentType::HOUND) {
        ai = std::make_unique<HoundAI>(id, speed, rng);
    }
}
unsigned Agent::GetID() const { return id; }
Vec2 Agent::GetLoc() const { return loc; }
Vec2 Agent::GetHeading() const { return heading; }
AgentState Agent::GetAgentState() const { return state; }
unsigned Agent::GetSpeed() const { return speed; }
AgentType Agent::GetType() const { return type;  }
int Agent::GetPoints() const { return points; }
void Agent::SetPoints(int p) { points = p; }
void Agent::SetSpeed(unsigned s) { speed = s; }
void Agent::SetAgentState(AgentState as) { state = as; }
void Agent::SetLoc(Vec2 v) {
    loc = v;
}
void Agent::SetHeading(Vec2 v) {
    heading = v;
}
void Agent::AddPoints(int p) { points += p; }
AgentSight Agent::GetSight() { return sight; }
std::vector<std::string> Agent::RunAI(
    Percepts & percepts,
    AgentComm * comms
) {
    if (state == AgentState::Active) {
        return ai->Run(percepts, comms);
    }
    else {
        return std::vector<std::string>();
    }
}