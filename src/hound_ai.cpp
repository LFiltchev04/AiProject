#include "hound_ai.hpp"
#include "utils.hpp"
#include <cmath>
#include "utils.hpp"
#include <iostream>
#include <string>
#include <limits>


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
std::vector<Vec2> HoundAI::trackFox(AgentComm* commData, Vec2 cAbsPos){
    std::vector<Vec2> results;
    
    if (!commData || commData->bark.empty() || commData->direction.empty()) {
        return results;
    }
    
    // Collect all circle centers and radii correctly
    std::vector<Vec2> centers;
    std::vector<double> radii;   
    
    centers.push_back(cAbsPos);
    radii.push_back(static_cast<double>(commData->bark[0])); // Assuming bark[0] is current hound's scent
    
    // Add peer hounds (make sure we have matching bark and direction data)
    size_t numPeers = std::min(commData->bark.size() - 1, commData->direction.size());
    for(size_t i = 0; i < numPeers; ++i){
        centers.push_back(commData->direction[i]);
        radii.push_back(static_cast<double>(commData->bark[i + 1])); // Skip bark[0] since it's current hound
    }
    
    // Need at least 3 circles
    if (centers.size() < 3) {
        return results;
    }
    
    // Find all candidate points from pairs of circles
    std::vector<Vec2> candidates;
    
    for (size_t i = 0; i < centers.size(); ++i) {
        for (size_t j = i + 1; j < centers.size(); ++j) {
            Vec2 c1 = centers[i];
            Vec2 c2 = centers[j];
            double r1 = radii[i];
            double r2 = radii[j];
            
            // distance between the two circle centers
            double d = linDist(c1, c2);
            
            // skip degenerate / impossible cases
            if (d <= 1e-12 || d > r1 + r2 + 1e-9 || d < std::fabs(r1 - r2) - 1e-9) {
                std::cout<<"track impossible from:" << id <<std::endl;
                continue;
            }
            
            // compute circle intersection 
            double a = (r1*r1 - r2*r2 + d*d) / (2.0 * d);
            double h2 = r1*r1 - a*a;
            if(h2 < 0) h2 = 0;
            double h = std::sqrt(h2);
            
            double ux = (c2.x - c1.x) / d;
            double uy = (c2.y - c1.y) / d;
            
            double p2x = c1.x + ux * a;
            double p2y = c1.y + uy * a;
            
            double offx = -uy * h;
            double offy =  ux * h;
            
            // compute intersection points
            Vec2 inter1, inter2;
            inter1.x = static_cast<int>(std::round(p2x + offx));
            inter1.y = static_cast<int>(std::round(p2y + offy));
            inter2.x = static_cast<int>(std::round(p2x - offx));
            inter2.y = static_cast<int>(std::round(p2y - offy));
            
            candidates.push_back(inter1);
            if (!(inter1.x == inter2.x && inter1.y == inter2.y)) {
                candidates.push_back(inter2);
            }
        }
    }
    
    // Find the candidate that is closest to all circles (trilateration)
    Vec2 bestCandidate;
    double minMaxError = std::numeric_limits<double>::max();
    bool foundValid = false;
    
    for (Vec2 candidate : candidates) {
        double maxError = 0.0;
        bool validForAllCircles = true;
        
        // Check how well this candidate fits all circles
        for (size_t i = 0; i < centers.size(); ++i) {
            double distToCenter = linDist(candidate, centers[i]);
            double error = std::fabs(distToCenter - radii[i]);
            
            // If error is too large, this candidate is invalid
            if (error > 3.0) { // tolerance for grid discretization
                validForAllCircles = false;
                break;
            }
            
            maxError = std::max(maxError, error);
        }
        
        if (validForAllCircles && maxError < minMaxError) {
            minMaxError = maxError;
            bestCandidate = candidate;
            foundValid = true;
        }
    }
    
    if (foundValid) {
        std::cout<<"valid track:"<<foundValid<<std::endl;
        results.push_back(bestCandidate);
    }else{
        results = candidates;
    }
    
    return results;
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
    
    comms->bark[id]=percepts.scent;
    
    
    if (comms != nullptr) {
        for (size_t i = 0; i < comms->bark.size(); ++i) {
            comms->bark[i] = static_cast<unsigned>(pFind.getMap().currentPos().Distance(comms->direction[i]));
        }
    }

    Vec2 cAbsPos = pFind.getMap().currentPos();

    std::vector<std::string> cmds={};
    std::vector<std::string> arr = {};

    
    pFind.updateMap(percepts);
    
std::vector<Vec2> targs;

     targs = trackFox(comms, pFind.getMap().currentPos());

    /*
    if(targs.size()>0){
        for (const Vec2 &targ : targs) {
        // ignore no-op matches to current pos
        
        if(targ != pFind.getMap().currentPos()){
            int prio = manhattanDistance(pFind.getMap().currentPos(), targ);
            foxTracks.emplace(prio, targ);
            continue;    
        }
        std::cout<<"no fox detected"<<std::endl;
    }
    } else {
        std::cout<<std::endl<<"no fox detected"<<std::endl;
    }
    

*/


    cmds = discoveryMode();
    //std::cout<<std::endl<<"the head:"<<semiRand.getNext().to_string()<<std::endl;
    std::cout<<std::endl<<"the head:"<<pFind.getTgt().to_string()<<std::endl;

        return cmds;


    }



