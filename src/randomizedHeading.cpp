#include "randomizedHeading.hpp"
#include "utils.hpp"
#include <cmath>


semirandomHeading::semirandomHeading(){
    //random enough
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randGen.seed(seed);                // <--- seed the RNG

    std::uniform_int_distribution<int> tmp(-5,5);
    randInt = tmp;
    // initialize as a relative offset
    Vec2 temp = getFullRandom();
    randomHead = temp;
}

Vec2 semirandomHeading::getFullRandom(){
    int tempX = randInt(randGen);
    int tempY = randInt(randGen);

    Vec2 temp;
    if(tempX>=0){
        temp.x = static_cast<int>(30.0+30.0*(tempX/5.0));
    }else{
        temp.x = static_cast<int>(-30.0+30.0*(tempX/5.0));
    }

    if(tempY<0){
        temp.y = static_cast<int>(30.0+30.0*(tempY/5.0));
    }else{
        temp.y = static_cast<int>(-30.0+30.0*(tempY/5.0));
    }

    // store as a relative offset
    randomHead = temp;

    return temp;
    
}



Vec2 semirandomHeading::fortyFiveCcWise(){
    const double c = std::sqrt(2.0) / 2.0;
    const double s = c;

    double nx = randomHead.x * c - randomHead.y * s;
    double ny = randomHead.x * s + randomHead.y * c;

    randomHead.x = static_cast<int>(std::round(nx));
    randomHead.y = static_cast<int>(std::round(ny));

    return randomHead;
}

Vec2 semirandomHeading::fortyFiveCwise(){
    const double c = std::sqrt(2.0) / 2.0;
    const double s = -c;

    double nx = randomHead.x * c - randomHead.y * s;
    double ny = randomHead.x * s + randomHead.y * c;

    randomHead.x = static_cast<int>(std::round(nx));
    randomHead.y = static_cast<int>(std::round(ny));

    // return offset (not absolute)
    return randomHead;
}


void semirandomHeading::iterate( Vec2 cPos, Vec2 absDir){
    // Always refresh or adjust the relative offset so discovery produces new targets

    //std::cout<<"Prior "<<prior.to_string()<<std::endl;
    //std::cout<<"PriorPrior "<<priorPrior.to_string()<<std::endl;

    if(prior==cPos and priorPrior==prior){
        //std::cout<<"Prior "<<prior.to_string()<<std::endl;
    //std::cout<<"PriorPrior "<<priorPrior.to_string()<<std::endl;

        randomHead = getFullRandom();
        priorPrior=prior;
        prior=cPos;
    }
    priorPrior=prior;
    prior=cPos;
    return;

        /*
        // use rotated offsets based on averageHeading quadrants
        if(averageHeading.x>=0.0 && averageHeading.y>=0.0){
            fortyFiveCcWise();
            //fortyFiveCcWise();
            return;
        }

        if(averageHeading.x<0.0 && averageHeading.y > 0.0){
            fortyFiveCwise();
            //randomHead = cPos+fortyFiveCwise();
            return;
        }

        if(averageHeading.x<0.0 && averageHeading.y < 0.0){
            fortyFiveCcWise();
            randomHead = cPos+fortyFiveCcWise();
            return;
        }

        if(averageHeading.x>=0.0 && averageHeading.y<=0.0){
            fortyFiveCwise();
            //randomHead = cPos+fortyFiveCwise();
            return;
        }

        // Refresh with a fresh relative random offset (do NOT add cPos here)
        randomHead = getFullRandom();
        std::cout<<"full random applied during wall bump recovery"<<std::endl;
        return;

    // even when not in wall-bump recovery, generate a new candidate occasionally
    // (so the randomizer actually changes targets between discovery calls)

    // update the deque of recent headings (store the absolute direction vector)
    pastSeven.push_back(absDir);
    while(pastSeven.size() > 7) pastSeven.pop_front();

    // compute average of the stored vectors
    double sumx = 0.0, sumy = 0.0;
    for(const Vec2 &v : pastSeven){
        sumx += static_cast<double>(v.x);
        sumy += static_cast<double>(v.y);
    }
    if(!pastSeven.empty()){*/
       // averageHeading.x = sumx / static_cast<double>(pastSeven.size());
      //  averageHeading.y = sumy / static_cast<double>(pastSeven.size());
    //} else {
        //averageHeading.x = 0.0;
      //  averageHeading.y = 0.0;
    //}

    //std::cout<<averageHeading.x<<std::endl;
    //std::cout<<averageHeading.y<<std::endl;
    // avoid zeroing the offset; if you want a neutral target, produce a small random offset
    //return;



    }


Vec2 semirandomHeading::getNext(Vec2 cAbsPos){

    //if i move theese in small increments to the absolute position they wont have to dump a lot of node guesses early
    //also keeps the queues shorter
    
    // Calculate offset (divide by 10 for incremental movement, ensure non-zero)
    int offsetX = randomHead.x / 10;
    int offsetY = randomHead.y / 10;
    
    // Ensure we have at least some movement (avoid zero offset)
    if(offsetX == 0 && offsetY == 0){
        // If both are zero, use the full random head to ensure movement
        offsetX = randomHead.x != 0 ? (randomHead.x > 0 ? 1 : -1) : 1;
        offsetY = randomHead.y != 0 ? (randomHead.y > 0 ? 1 : -1) : 1;
    }
    
    increment.x = offsetX;
    increment.y = offsetY;

    std::cout<<"AI is at:"<<cAbsPos.to_string()<<std::endl;
    std::cout<<"Heading towards offset: "<<increment.to_string()<<std::endl;
    return increment;
}

