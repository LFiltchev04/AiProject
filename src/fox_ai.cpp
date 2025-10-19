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
            if(pointCount>=6){
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
        //std::cout <<"priority target at "<<var.pos.to_string()<<" of type: "<<var.type<<std::endl;
    }

    if(pointCount>=6){
        if(closeExit()!=pFind.getMap().currentPos()){
            //std::cout<<"heading for exit: "<< pFind.getTgt().to_string() <<std::endl;
            //std::cout<<"i am at: "<<pFind.getMap().currentPos().to_string();
            
            return foxTraverse('?');
        }
    }else{
        if(closeGoal()!=pFind.getMap().currentPos()){
            //std::cout<<"heading for goal: "<< pFind.getTgt().to_string() <<std::endl;
            //            std::cout<<"i am at: "<<pFind.getMap().currentPos().to_string();

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
            return runModel(true,2);
        }else{
            pFind.newTarget(closeGoal());
            return runModel(true,2);
        }



    }