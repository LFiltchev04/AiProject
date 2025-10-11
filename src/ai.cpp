#include"ai.hpp"

/***************************************************************
AI CLASS DEFINITION
*/
AI::AI() {}
AI::AI(
    unsigned id, 
    unsigned agent_speed,
    std::mt19937_64* rng
)
    : id(id), agent_speed(agent_speed), rng(rng)
{}

std::vector<std::string> AI::Run(
    Percepts & percepts,
    AgentComm * comms
) {

    return std::vector<std::string>();
}





    
    std::string AI::discoveryMode(){
        semiRand.iterate(wallBumps,pFind.getMap().currentPos(),pFind.getMap().trueDir(pFind.getMap().getHeading()));
        pFind.newTarget(semiRand.getNext());    
        std::string cmd = runModel();

        return cmd;
    }


std::string AI::runModel(){
    if(pFind.getMap().currentPos()==pFind.getTgt()){
        return " ";
    }

        if(pFind.getPath()->empty()){
            //std::cout<<"INITIAL PFIND";
            pFind.LPApathfind();
        }        

        if(pFind.pathInvalid()){
            pFind.recomputeFrom();
        }


        if(pFind.getMap().isWall(pFind.followingCoord())){
            std::cout<< pFind.followingCoord().x<< ","<<pFind.followingCoord().y<<" IS A WALL"<<std::endl;
            
            //the idea of tracking wall bumps is to change direction when in pathfinding mode, the speed is limited as it is
            //there is no need to make moving one block in maybe a good direction take one turn to figure out its wrong, one to rotate and another to move
            wallBumps+=2;    
            pFind.recomputeFrom();
        }else{
            if(wallBumps>0){
                wallBumps -= 1;
            }
        }
    
    
        std::string nxt="";
        char nextStep = pFind.pathTranslator(); 
    
            nxt += nextStep;
        
            pFind.getMap().iterateState(nxt);
            return nxt;

        //std::cout<<"heading now:"<<nextStep<<pFind.getMap().trueDir(nextStep).x<<" "<<pFind.getMap().trueDir(nextStep).y<<std::endl;
}


std::vector<std::string> AI::traverseMode(){
    
}


void AI::setMode(){
    if(pFind.multiturnSafe(*pFind.getPath())){
        state = MULTITURN;
    }else{
        state = DISCOVERY;
    }
}




