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
Vec2 HoundAI::trackFox(AgentComm* commData, Vec2 cAbsPos){
    Vec2 invalidResult = {-9999, -9999}; 
    
    if (!commData || commData->bark.empty() || commData->direction.empty()) {
        return invalidResult;
    }
    
    //The two match? 
    std::vector<Vec2> centers;
    std::vector<double> radii;   
    
    centers.push_back(cAbsPos);
    radii.push_back(static_cast<double>(commData->bark[0])); 
    
    size_t numPeers = std::min(commData->bark.size() - 1, commData->direction.size());
    for(size_t i = 0; i < numPeers; ++i){
        Vec2 relativePos = commData->direction[i];

        
        reorient(pFind.getMap().getHeading(),relativePos);
        
        centers.push_back(relativePos);
        radii.push_back(static_cast<double>(commData->bark[i + 1])); 
    }

    
    std::vector<Vec2> candidates;
    
    for (size_t i = 0; i < centers.size(); ++i) {
        for (size_t j = i + 1; j < centers.size(); ++j) {
            Vec2 c1 = centers[i];
            Vec2 c2 = centers[j];
            double r1 = radii[i];
            double r2 = radii[j];
            
            double d = linDist(c1, c2);
            
            //If they are in the same place 
            if (d <= 1e-12 || d > r1 + r2 + 1e-9 || d < std::fabs(r1 - r2) - 1e-9) {
                //std::cout<<"track impossible from:" << id <<std::endl;
                continue;
            }
            
            double a = (r1*r1 - r2*r2 + d*d) / (2.0 * d);
            double h2 = r1*r1 - a*a;
            
            if(h2 < 0){
              h2 = 0;
            } 

            double h = std::sqrt(h2);
            
            double ux = (c2.x - c1.x) / d;
            double uy = (c2.y - c1.y) / d;
            
            double p2x = c1.x + ux * a;
            double p2y = c1.y + uy * a;
            
            double offx = -uy * h;
            double offy =  ux * h;
            
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
    
    Vec2 bestCandidate = invalidResult;
    double minMaxError = std::numeric_limits<double>::max();
    bool foundValid = false;
    
    for (Vec2 candidate : candidates) {
        double maxError = 1.0;
        bool validForAllCircles = true;
        
        for (size_t i = 0; i < centers.size(); ++i) {
            double distToCenter = linDist(candidate, centers[i]);
            double error = std::fabs(distToCenter - radii[i]);
            
           
            if (error > 2.0) { 
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
        //std::cout<<"valid track: " << bestCandidate.to_string() << " from hound " << id<<"located at "<<pFind.getMap().currentPos().to_string() << std::endl;

    }
    
    return bestCandidate;
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
    
    
    //NOTHING ABOVE THIS
    if (comms != nullptr) {
        for (int i = 0; i < comms->bark.size(); ++i) {
            comms->bark[i] = static_cast<unsigned>(percepts.scent);
        }
    }
    pFind.getMap().updateMap(percepts);







    Vec2 cAbsPos = pFind.getMap().currentPos();

    Vec2 foxCandidate = trackFox(comms, cAbsPos);
    
    if(foxCandidate.x != -9999 && foxCandidate.y != -9999){
        std::cout<<"candidate found" << foxCandidate.to_string()<<std::endl<<std::endl<<std::endl<<std::endl;
        return houindIterate(foxCandidate);
    }


    
   
    return discoveryMode();


    }



    std::vector<std::string> HoundAI::houindIterate(Vec2 foxL){
        
        pFind.newTarget(foxL);
        return runModel(false,3);
    }

