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





std::vector<std::string> AI::traverseMode(int allowedMoves){
    if(pFind.multiturnSafe(*pFind.getPath(),allowedMoves)){
        std::vector<std::string> cmds={};
        std::string nxt="";
       for(int x=0;x<allowedMoves;x++){
            nxt += runModel();
    } else {
        setMode();
        return {discoveryMode()};
    }
}



    
    std::string AI::discoveryMode(){
        Vec2 curPos = pFind.getMap().currentPos();
        Vec2 absDir = pFind.getMap().trueDir(pFind.getMap().getHeading());
    
        semiRand.iterate(curPos, absDir);

    
        Vec2 offset = semiRand.getNext(curPos);
    
    
        pFind.newTarget(curPos+offset);
        std::string cmd = runModel();

        return cmd;
    }


    void AI::setMode(){
        if(pFind.multiturnSafe(*pFind.getPath())){
            state = 'T';
        } else {
            state = 'D';
        }
    }


std::string AI::runModel(){
    if(pFind.getMap().currentPos()==pFind.getTgt()){
        return " ";
    }

        if(pFind.getPath()->empty()){
            //std::cout<<"INITIAL PFIND";
            pFind.aStar();
        }        

        if(pFind.pathInvalid()){
            pFind.recomputeFrom();
        }

        std::stack<Vec2> printPathDebug = *pFind.getPath();

        //std::cout<<"the path is :"<<printPathDebug.size()<<std::endl;

        while(!printPathDebug.empty()){
            std::cout<<printPathDebug.top().to_string()<<" -> ";
            printPathDebug.pop();
        }
        

        std::string nxt="";
        char nextStep = pFind.pathTranslator(); 
    
            nxt += nextStep;
        
            pFind.getMap().iterateState(nxt);
            return nxt;

        //std::cout<<"heading now:"<<nextStep<<pFind.getMap().trueDir(nextStep).x<<" "<<pFind.getMap().trueDir(nextStep).y<<std::endl;
}



