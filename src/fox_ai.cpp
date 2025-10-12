#include"fox_ai.hpp"

/***************************************************************
FOX AI CLASS DEFINITION
*/

FoxAI::FoxAI(
    unsigned id,
    unsigned agent_speed,
    std::mt19937_64* rng,
    std::string goal,
    std::string exit,
    std::vector<std::string> teleporters)
    :AI(id, agent_speed, rng), goal(goal), exit(exit), teleporters(teleporters)
{
}

std::vector<std::string> FoxAI::Run(
    Percepts& percepts,
    AgentComm * comms
) {

    // DO NOT LEAVE THIS CODE (COMMENTED OUT OR IN) IN
    // YOUR SUBMISSION

    /*std::cout << "\n=========================\n";
    std::cout << "FOX ID: " << id << std::endl;
    std::cout << "CURRENT: ";
    for (size_t i = 0; i < percepts.current.size(); i++) {
        std::cout << percepts.current[i] << " ";
    }
    std::cout << "\nFORWARD: ";
    for (size_t i = 0; i < percepts.forward.size(); i++) {
        std::cout << percepts.forward[i] << " ";
    }
    std::cout << "\nBACKWARD: ";
    for (size_t i = 0; i < percepts.backward.size(); i++) {
        std::cout << percepts.backward[i] << " ";
    }
    std::cout << "\nLEFT: ";
    for (size_t i = 0; i < percepts.left.size(); i++) {
        std::cout << percepts.left[i] << " ";
    }
    std::cout << "\nRIGHT: ";
    for (size_t i = 0; i < percepts.right.size(); i++) {
        std::cout << percepts.right[i] << " ";
    }
    std::cout << "\nSIGHTINGS:\n";
    for (size_t i = 0; i < percepts.sightings.size(); i++) {
        if (percepts.sightings[i].type == AgentType::FOX) std::cout << "   FOX ";
        else std::cout << "  HOUND ";
        std::cout << "Dir " << percepts.sightings[i].direction << " ";
        std::cout << "Dis " << percepts.sightings[i].distance << "\n";
    }
    std::cout << "SCENT: " << percepts.scent << std::endl;
    std::cout << "COMMS:\n";
    if (comms == nullptr) std::cout << "No communication.\n";
    else {
        for (size_t i = 0; i < comms->size(); i++) {
            std::cout << "HOUND " << i << " a: " << (*comms)[i].a
                << " b: " << (*comms)[i].b
                << " c: " << (*comms)[i].c
                << std::endl;
        }
    }*/


    /***************************************************************
    YOUR FOX AI CODE GOES HERE.
    Notes:
    - you may add other member functions to this class.
    - you may add member data to this class.
    - you may develop new classes to help or supplement this one.
    - you may NOT alter the function definition here or where
        it is called by the simulation.
    - you may not alter ANY simulation code other than this AI file.
    */


    // The following is a purely random solution to give you an idea
    // how percepts and commands work. You should not include the
    // following in your solution.

    std::vector<std::string> cmds;

    pFind.updateMap(percepts);

    for (const std::string& cell : percepts.current) {
        if (cell == goal && pointCount < 2) {
            cmds.push_back("U");
            return cmds;
        }
        if (cell == exit && pointCount >= 2) {
            std::cout << "STANDING ON EXIT! Using it." << std::endl;
            cmds.push_back("U");
            return cmds;
        }
        if (std::find(teleporters.begin(), teleporters.end(), cell) != teleporters.end()) {
            std::cout << "STANDING ON TELEPORTER! Using it." << std::endl;
            cmds.push_back("U");
            return cmds;
        }
    }
    
    std::cout<<"PRIORTY TARGETS SIZE: "<<pFind.getMap().priorityTargets.size()<<std::endl;

for(priorityTarget pt : pFind.getMap().priorityTargets){
        std::cout<<"THEESE ARE THE PRIORITY TARGETS"<<pt.type<<" at "<<pt.pos.to_string()<<std::endl;
    }




    if(pFind.getMap().priorityTargets.size() > 0){
        
        //if it spotted a goal
        if(pointCount < 2){
            Vec2 goalTest = closeGoal();
            Vec2 currentTarget = pFind.getTgt();

            // Only change target if we don't have one or if the new goal is closer
            if(goalTest != pFind.getMap().currentPos()){
                // Check if we already have this as our target
                if(currentTarget == goalTest){
                    std::cout << "Already heading to goal at: " << goalTest.to_string() << std::endl;
                    std::vector<std::string> result;
                    std::string tst = runModel();
                    result.push_back(tst);
                    return result;
                }
                
                std::cout << "SETTING GOAL TARGET: " << goalTest.to_string() << std::endl;
                pFind.newTarget(goalTest);
    
                std::string tst = runModel();
                std::vector<std::string> result;
                result.push_back(tst);

                //= traverseMode(2);
                std::cout << "TRAVERSE MODE RETURNED: ";
                for(const auto& cmd : result) {
                    std::cout << cmd << " ";
                }
                std::cout << std::endl;
                return result;
            }
        }

        //if it spotted an exit
        if(pointCount >= 2){
            Vec2 exitTest = closeExit();
            Vec2 currentTarget = pFind.getTgt();

            // Only change target if we don't have one or if the new exit is closer
            if(exitTest != pFind.getMap().currentPos()){
                // Check if we already have this as our target
                if(currentTarget == exitTest){
                    std::cout << "Already heading to exit at: " << exitTest.to_string() << std::endl;

                    std::string tst = runModel();
                    std::vector<std::string> result;
                    result.push_back(tst);
                    return result;
                }
                
                std::cout << "SETTING EXIT TARGET: " << exitTest.to_string() << std::endl;
                pFind.newTarget(exitTest);
                    std::vector<std::string> result;

                std::string tst = runModel();
                result.push_back(tst);

                std::cout << "TRAVERSE MODE RETURNED: ";
                for(const auto& cmd : result) {
                    std::cout << cmd << " ";
                }
                std::cout << std::endl;
                return result;
            }
        }
    }
    

    
    
    Vec2 currentTarget = pFind.getTgt();
    Vec2 currentPos = pFind.getMap().currentPos();
    
    if(currentTarget != Vec2{0,0} && currentTarget != currentPos){
        std::cout << "Continuing toward existing target: " << currentTarget.to_string() << std::endl;
        std::vector<std::string> result = traverseMode(2);
        return result;
    }

    // Only use discovery mode if we have no target at all
    std::string nextS = discoveryMode();
    cmds.push_back(nextS);

    return cmds;
}





























Vec2 FoxAI::closeGoal(){
    Vec2 cPos = pFind.getMap().currentPos();
    int minManhattan = std::numeric_limits<int>::max();

    for(priorityTarget pt : pFind.getMap().priorityTargets){
        if(pt.type == '!'){
            int manh = manhattanDistance(cPos, pt.pos);
            if(manh < minManhattan){
                minManhattan = manh;
            }
        }
    }

    if(minManhattan == std::numeric_limits<int>::max()){
        return cPos;
    }

    for(priorityTarget pt : pFind.getMap().priorityTargets){
        if(pt.type == '!' && manhattanDistance(cPos, pt.pos) == minManhattan){
            return pt.pos;
        }
    }
    
    return cPos;
}

//theese are not so good
Vec2 FoxAI::closeExit(){
    Vec2 cPos = pFind.getMap().currentPos();
    int minManhattan = std::numeric_limits<int>::max();

    for(priorityTarget pt : pFind.getMap().priorityTargets){
        if(pt.type == '?'){
            int manh = manhattanDistance(cPos, pt.pos);
            if(manh < minManhattan){
                minManhattan = manh;
            }
        }
    }
    
    if(minManhattan == std::numeric_limits<int>::max()){
        return cPos;
    }

    for(priorityTarget pt : pFind.getMap().priorityTargets){
        if(pt.type == '?' && manhattanDistance(cPos, pt.pos) == minManhattan){
            return pt.pos;
        }
    }
    
    // If we get here, no exit was found
    return cPos;
}

bool FoxAI::atGoal(){
    // This should be called with percepts to check current cell
    // For now, just check if we're at a goal priority target position
    Vec2 cPos = pFind.getMap().currentPos();
    for(priorityTarget pt : pFind.getMap().priorityTargets){
        if(pt.type == '!'){
            if(cPos == pt.pos){
                return true;
            }
        }
    }
    return false;
}

bool FoxAI::atExit(){
    Vec2 cPos = pFind.getMap().currentPos();
    for(priorityTarget pt : pFind.getMap().priorityTargets){
        if(pt.type == '?'){
            if(cPos == pt.pos){
                return true;
            }
        }
    }
    return false;
}


