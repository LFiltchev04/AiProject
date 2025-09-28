#include "pathfinder.hpp"



pathfind::pathfind(Percepts initial){
    //inMap = internalMap(initial);
}

void pathfind::setGoal(Vec2 newGoal){
    goalCoord = newGoal;
}

char pathfind::euclideanHeuristic(Vec2 refCoord){

    Vec2 tempGuideVector = goalCoord - refCoord;


    //the scheme for movement i have chosen is to try to move on a "mini manhattan", basically a vector would be taken as a ratio of moves per single move in another direction
    //example: the direction vector (7,13) would be reduced to (1,1) * 7, so for every step in x you do 1 in y and whatever the remainder is you straight line to it. 
    //since no movement on the diagonals is allowed euclidean or manhattan movement makes no difference

    int divisor = std::min({tempGuideVector.x,tempGuideVector.y});


    guideVector.x = tempGuideVector.x / divisor;
    guideVector.y = tempGuideVector.y / divisor;

    



}

