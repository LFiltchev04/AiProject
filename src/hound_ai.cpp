#include "hound_ai.hpp"
#include "utils.hpp"
#include <cmath>
#include "utils.hpp"
#include <iostream>
#include <string>


/***************************************************************
HOUND AI CLASS DEFINITION
*/

HoundAI::HoundAI(
    unsigned id,
    unsigned agent_speed,
    std::mt19937_64* rng
)
    : AI(id, agent_speed, rng)
{
}



//the absolute position has to be passed from the map class, the rotations are handled there 
Vec2 HoundAI::trackFox(AgentComm* commData, Vec2 cAbsPos, double scent){

    if (commData == nullptr){
        return cAbsPos;
    } 

    auto brks = commData->bark.begin();     
    auto dir = commData->direction.begin();

    while(brks != commData->bark.end() && dir != commData->direction.end()){

        Vec2 bPos = *dir;                 // reporter absolute position
        double r1 = scent;                // radius from this hound (scent)
        double r2 = static_cast<double>(*brks); // radius from the barker

        // distance between the two circle centers
        double d = linDist(cAbsPos, bPos);

        // skip degenerate / impossible cases
        if (d <= 1e-12){
            // same center or invalid
            ++brks; ++dir; 
            continue; 
        }           
        if (d > r1 + r2 + 1e-9){
            // circles too far apart
            ++brks; 
            ++dir;
            continue; 
        }   
        if (d < std::fabs(r1 - r2) - 1e-9){
            // one inside the other
            ++brks;
            ++dir;
            continue; 
        } 

        // compute circle intersection (standard geometry)
        double a = (r1*r1 - r2*r2 + d*d) / (2.0 * d);
        double h2 = r1*r1 - a*a;
        if(h2 < 0){
            h2 = 0;
        } 
        double h = std::sqrt(h2);

        // unit vector from this hound to the barker
        double ux = (bPos.x - cAbsPos.x) / d;
        double uy = (bPos.y - cAbsPos.y) / d;

        // point P2 which is the point along the line between centers at distance a from this hound
        Vec2 P2;
        P2.x = cAbsPos.x + ux * a;
        P2.y = cAbsPos.y + uy * a;

        // perpendicular offset to get the two intersection points
        Vec2 off;
        off.x = -uy * h;
        off.y =  ux * h;

        Vec2 inter1, inter2;
        inter1.x = P2.x + off.x;
        inter1.y = P2.y + off.y;
        inter2.x = P2.x - off.x;
        inter2.y = P2.y - off.y;

        // choose one intersection to return — pick the one closer to the barker reporter
        double d1 = linDist(inter1, bPos);
        double d2 = linDist(inter2, bPos);
        return (std::fabs(d1 - r2) < std::fabs(d2 - r2)) ? inter1 : inter2;
    }

    // no valid intersection found, return current position as fallback
    return cAbsPos;
}













































std::vector<std::string> HoundAI::Run(
    Percepts& percepts,
    AgentComm * comms
) {

    // DO NOT LEAVE THIS CODE (COMMENTED OUT OR IN) IN
    // YOUR SUBMISSION

    /*std::cout << "\n=========================\n";
    std::cout << "HOUND ID: " << id << std::endl;
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
    std::cout << "SCENT: " << percepts.scent << std::endl;*/
    std::cout << "COMMS:\n";
    if (comms == nullptr) std::cout << "No communication.\n";
    else {
        for (size_t i = 0; i < comms->bark.size(); i++) {
            std::cout << "HOUND " << i << " Bark: " << comms->bark[i]
                << " Dir: " << comms->direction[i].to_string()
                << std::endl;
        }
    }

    /***************************************************************
    YOUR HOUND AI CODE GOES HERE.
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

    // If the AI's id is 0, it sets its message to 9.
    // This demonstrates how you can pass messages.
    if (id == 0) {
        comms->bark[0] = 9;
    }

    std::vector<std::string> cmds={};

    std::vector<std::string> arr = {};

    
    pFind.updateMap(percepts);

   
    
    Vec2 resultOfTrack={-9,1};
    resultOfTrack = trackFox(comms,pFind.getMap().currentPos(),percepts.scent);
    pFind.newTarget(resultOfTrack);

    std::cout<< resultOfTrack.x<<" "<<resultOfTrack.y<<" fox track "<<id<<std::endl;


            std::cout<<"cPos: "<<pFind.getMap().currentPos().x << " <x|y> " << pFind.getMap().currentPos().y<<std::endl;

    if(pFind.getMap().currentPos()==resultOfTrack){
        return cmds;
    }


        if(pFind.getPath()->empty()){
            //std::cout<<"INITIAL PFIND";
            pFind.LPApathfind();

        }

        if(pFind.pathInvalid()){
            pFind.recomputeFrom();
        }

    std::stack<Vec2> tst = *pFind.getPath();
    //std::cout<<"projected path:"<<std::endl;
    //while(!tst.empty()){
    //    std::cout<<tst.top().x<<":"<<tst.top().y<<std::endl;
    //    tst.pop();
    //}

    if(pFind.getMap().isWall(pFind.followingCoord())){
        //std::cout<< pFind.followingCoord().x<< ","<<pFind.followingCoord().y<<" IS A WALL"<<std::endl;
        pFind.recomputeFrom();
    }


    //std::cout<<pFind.getMap().getHeading()<<std::endl;

    //pFind.getMap().changeHeading('R');

    //std::cout<<pFind.getMap().getHeading()<<std::endl;


    //return cmds;

    
    std::string nxt="";
   


   //std::cout<<pFind.getMap().trueDir('F').x<<" | "<<pFind.getMap().trueDir('F').y<<std::endl;

    //return cmds;

    //for(int x = 0; x<3;x++){
        char nextStep = pFind.pathTranslator(); 
    //std::cout<<"        ISSUED COMMAND: "<< nextStep<<std::endl;
    if(nextStep != ' '){
        nxt += nextStep;
        
        pFind.getMap().iterateState(nxt);
        cmds.push_back(nxt);
        


        //std::cout<<"heading now:"<<nextStep<<pFind.getMap().trueDir(nextStep).x<<" "<<pFind.getMap().trueDir(nextStep).y<<std::endl;
    //}
    
    }
    
    std::cout<<std::endl<<std::endl<<std::endl<<std::endl<<std::endl;
    



/*

    std::string nxt = "";

    if(pFind.getMap().currentPos()==tgt){
    
    }

    if(pFind.getPath()->empty()){
    //std::cout<<"INITIAL PFIND";
        pFind.LPApathfind();
    }

    if(pFind.pathInvalid()){
        pFind.recomputeFrom();
    } 

    pFind.getPath()->top();
    if(pFind.getMap().isWall(pFind.getPath()->top())){
        pFind.recomputeFrom();
    }


    //should have used dequeue to beign with for iterators, but i guess it must all happen the hard way
    std::stack<Vec2> analysisCopy = *pFind.getPath();
    for(int x=0;x<3;x++){
        if(pFind.getMap().isWall(analysisCopy.top())){
            
        }
    }













    char nextStep = pFind.pathTranslator();
    nxt+=nextStep;
    pFind.getMap().iterateState(nxt);
    



    













*/




    return cmds;



    }

/*

std::string HoundAI::discoveryMode(int limit){
    //should return only one command per turn to avoid it blowing up the relative coordinates if it unwittingly hits a wall
    std::string nxt = "";

    if(pFind.getMap().currentPos()==tgt){
        return "b";
    }

    if(pFind.getPath()->empty()){
    //std::cout<<"INITIAL PFIND";
        pFind.LPApathfind();
    }

    if(pFind.pathInvalid()){
        pFind.recomputeFrom();
    } 

    pFind.getPath()->top();
    if(pFind.getMap().isWall(pFind.getPath()->top())){
        pFind.recomputeFrom();
    }

    char nextStep = pFind.pathTranslator();
    nxt+=nextStep;
    pFind.getMap().iterateState(nxt);
    

    return nxt;
}


std::vector<std::string> HoundAI::traverseMode(){
    
}*/