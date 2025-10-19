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


    /*
    if (percepts.current[0] == goal) cmds.push_back("U");
    else if (percepts.current[0] == exit) cmds.push_back("U");
    else if (std::find(teleporters.begin(), teleporters.end(), percepts.current[0]) != teleporters.end()) {
        cmds.push_back("U");
    }
    std::vector<std::string> arr = { "F", "B", "L", "R" };
    while(cmds.size() < 2) {        
	std::shuffle(arr.begin(), arr.end(), *rng);
        cmds.push_back(arr[0]);
    }
    */




    
    //NOTHING ABOVE THIS LINE
    pFind.getMap().updateMap(percepts);


    

    pFind.getMap().priorityTargets.erase(
        std::remove_if(
            pFind.getMap().priorityTargets.begin(),
            pFind.getMap().priorityTargets.end(),
            [this](const priorityTarget& var) {
                node n = pFind.getMap().getPrior(var.pos);
                return n.type == 'o' || n.type != var.type;
            }
        ),
        pFind.getMap().priorityTargets.end()
    );





    if(percepts.current[0]=="?" or percepts.current[0]=="!"){
          
        std::vector<std::string> res;

        if(percepts.current[0]=="!"){
            pointCount++;
            res = {"U"};
        }
        if(percepts.current[0]=="?"){
            if(pointCount>=2){
                res = {"U"};
            }else{
                return discoveryMode();
            }
            
        }
        

        Vec2 cPos = pFind.getMap().currentPos();
        pFind.getMap().priorityTargets.erase(
            std::remove_if(pFind.getMap().priorityTargets.begin(),pFind.getMap().priorityTargets.end(),[&cPos](const priorityTarget& var) { 
                    return var.pos == cPos; 
                }
            ),
            pFind.getMap().priorityTargets.end()
        );
        
        return res;

    }

    
    std::vector<priorityTarget> temp = pFind.getMap().priorityTargets;
    for(priorityTarget var : temp){
        std::cout <<"priority target at "<<var.pos.to_string()<<" of type: "<<var.type<<std::endl;
    }

    if(pointCount>=2){
        if(closeExit()!=pFind.getMap().currentPos()){
            std::cout<<"heading for exit: "<< pFind.getTgt().to_string() <<std::endl;
            std::cout<<"i am at: "<<pFind.getMap().currentPos().to_string();
            
            return foxTraverse('?');
        }
    }else{
        if(closeGoal()!=pFind.getMap().currentPos()){
            std::cout<<"heading for goal: "<< pFind.getTgt().to_string() <<std::endl;
                        std::cout<<"i am at: "<<pFind.getMap().currentPos().to_string();

            return foxTraverse('!');
        }
    }

    
    return discoveryMode();   
}















Vec2 FoxAI::closeExit(){
    std::vector<priorityTarget> temp = pFind.getMap().priorityTargets;

    if(temp.size()<1){
        return pFind.getMap().currentPos();
    }

    int dist = INT_MAX;
    Vec2 tempV = pFind.getMap().currentPos();
    for(priorityTarget var: temp){
        if(var.type=='?'){
            int d = manhattanDistance(pFind.getMap().currentPos(),var.pos);
            if(d < dist){
                tempV = var.pos;
            }
        }
    }

    std::cout<<"close exit: "<<tempV.to_string()<<std::endl;

    return tempV;
    
}


Vec2 FoxAI::closeGoal(){
    std::vector<priorityTarget> temp = pFind.getMap().priorityTargets;

    if(temp.size()<1){
        return pFind.getMap().currentPos();
    }

    int dist = INT_MAX;
    Vec2 tempV = pFind.getMap().currentPos();
    for(priorityTarget var: temp){
        if(var.type=='!'){
            if(manhattanDistance(pFind.getMap().currentPos(),var.pos)<dist){
                int d = manhattanDistance(pFind.getMap().currentPos(),var.pos);
                tempV = var.pos;
            }
        }
    }

    std::cout<<"close goal: "<<tempV.to_string()<<std::endl;
    return tempV;
}



 std::vector<std::string> FoxAI::foxTraverse(char type){
        
        if(type=='?'){
            pFind.newTarget(closeExit());
            
            //the meaningless boolean passed to it is to enable the model to run in multiturn mode whenever its safe
            return runModel(true);
        }else{
            pFind.newTarget(closeGoal());
            return runModel(true);
        }



    }