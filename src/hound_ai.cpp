#include "hound_ai.hpp"
#include "utils.hpp"


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

    auto brks = commData->bark.begin();     
    auto dir = commData->direction.begin();

    //i am assuming that they will always be of equal length and that the two things align, so rel location vector 9 is the same as the distance vector
    while(brks != commData->bark.end() and dir != commData->direction.end()){

        //as of right now the unsigned does not embed anything besides the distance.

        double distBarker = linDist(cAbsPos,*dir);
        
        if((std::abs(distBarker) + std::abs(static_cast<int>(*brks)) > scent) && (scent + std::abs(distBarker) > std::abs(static_cast<int>(*brks))) && (std::abs(static_cast<int>(*brks) + scent > std::abs(distBarker)))){
            return 
        }



        
        

        brks++;
        dir++;

    }

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

    std::vector<std::string> cmds;

    std::vector<std::string> arr = { "F", "L", "R" };

    mapInstace.updateMap(percepts);

    
    Vec2 resultOfTrack;
    //resultOfTrack = trackFox();
    pFind.newTarget(resultOfTrack);

    
    if(pFind.getPath()->empty()){

        if(pFind.pathInvalid()){
            pFind.recomputeFrom();
        }
        pFind.LPApathfind();
    }
    

    for(int x = 0; x<3; x++){
        std::string nxt = "";
        nxt += pFind.pathTranslator();
        cmds.push_back(nxt);
    }
    







    return cmds;
}



