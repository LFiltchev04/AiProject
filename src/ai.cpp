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





    
    std::vector<std::string> AI::discoveryMode(){
    //std::cout<<wallBumps<<"W bmp"<<std::endl;
        Vec2 curPos = pFind.getMap().currentPos();
        Vec2 absDir = pFind.getMap().trueDir(pFind.getMap().getHeading());
    
        semiRand.iterate(curPos, absDir);

    
        Vec2 offset = semiRand.getNext(curPos);
    
    
        pFind.newTarget(curPos+offset);
        return runModel();

        
    }


std::vector<std::string> AI::runModel(){

    std::vector<std::string> rslt;

    if(pFind.getMap().currentPos()==pFind.getTgt()){
        rslt.push_back(" ");
        
        return rslt;
    }

        if(pFind.getPath()->empty()){
            //std::cout<<"INITIAL PFIND";
            pFind.LPApathfind();
        }        

            this->setMode();


        if(pFind.pathInvalid()){
            pFind.recomputeFrom();
        }

        std::stack<Vec2> printPathDebug = *pFind.getPath();

        //std::cout<<"the path is :"<<printPathDebug.size()<<std::endl;

        while(!printPathDebug.empty()){
            std::cout<<printPathDebug.top().to_string()<<" -> ";
            printPathDebug.pop();
        }
        
        if(state == SINGELSTEP){
            std::string nxt="";
            char nextStep = pFind.pathTranslator(); 
    
            nxt += nextStep;
        
            pFind.getMap().iterateState(nxt);

            rslt.push_back(nxt);
            return rslt;
        }

        if(state == MULTISTEP){
            for(int x = 0; x<3;x++){
                std::string nxt="";
                char nextStep = pFind.pathTranslator(); 
                nxt += nextStep;
        
                pFind.getMap().iterateState(nxt);
                
                rslt.push_back(nxt);
            }

            return rslt;

        }

       

        //std::cout<<"heading now:"<<nextStep<<pFind.getMap().trueDir(nextStep).x<<" "<<pFind.getMap().trueDir(nextStep).y<<std::endl;
}


std::vector<std::string> AI::traverseMode(){
    
}


void AI::setMode(){

    if(pFind.getPath()->size()==0){
        state = SINGELSTEP;
        return;
    }

    if(pFind.multiturnSafe(*pFind.getPath())){
        std::cout<<"can make 3";
        state = SINGELSTEP;
    }else{
        std::cout<<"cannot make 3";
        state = SINGELSTEP;
    }
}


mode AI::getMode(){
    return state;
}

