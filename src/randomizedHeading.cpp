#include "randomizedHeading.hpp"
#include "utils.hpp"
#include <cmath>


semirandomHeading::semirandomHeading(){
    //random enough
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    randGen.seed(seed);                // <--- seed the RNG

    std::uniform_int_distribution<int> tmp(-5,5);
    randInt = tmp;
    Vec2 temp = getFullRandom({0,0});
    randomHead = temp;
}

Vec2 semirandomHeading::getFullRandom(Vec2 cPos){
    int tempX = randInt(randGen);
    int tempY = randInt(randGen);

    Vec2 temp;
    if(tempX>=0){
        temp.x = static_cast<int>(70.0+30.0*(tempX/5.0));
    }else{
        temp.x = static_cast<int>(-70.0+30.0*(tempX/5.0));
    }

    if(tempY<0){
        temp.y = static_cast<int>(70.0+30.0*(tempY/5.0));
    }else{
        temp.y = static_cast<int>(-70.0+30.0*(tempY/5.0));
    }

    randomHead = temp;

    return temp;
    
}



Vec2 semirandomHeading::fortyFiveCcWise(Vec2 cPos){
    const double c = std::sqrt(2.0) / 2.0;
    const double s = c;

    double nx = randomHead.x * c - randomHead.y * s;
    double ny = randomHead.x * s + randomHead.y * c;

    randomHead.x = static_cast<int>(std::round(nx));
    randomHead.y = static_cast<int>(std::round(ny));

    return cPos + randomHead;
}

Vec2 semirandomHeading::fortyFiveCwise(Vec2 cPos){
    const double c = std::sqrt(2.0) / 2.0;
    const double s = -c;

    double nx = randomHead.x * c - randomHead.y * s;
    double ny = randomHead.x * s + randomHead.y * c;

    randomHead.x = static_cast<int>(std::round(nx));
    randomHead.y = static_cast<int>(std::round(ny));

    return cPos + randomHead;
}


void semirandomHeading::iterate(int wallBumps, Vec2 cPos, Vec2 absDir){
    if(wallBumps>6){
        std::cout<<"wall bump recovery activated";
        //i dont knwo how much to trust the vector, its its been in a series of short term solisions, liek for example if its in a pipe, it will likely go completely wrong
        //i am thinking I should add some multiplier for heading vectors that have been recorded sooner, depends on the map quite a lot, so i dont think i can come up with a perfect scheme for it if i tried

        //its getting wallbumps in first quadrant, so the wall is likely to the right, thus go clockwise
        if(averageHeading.x>0.0 and averageHeading.y>0.0){
            fortyFiveCcWise(cPos);
            return;
        }

        //second quadrant
        if(averageHeading.x<0 and averageHeading.y > 0){
            fortyFiveCwise(cPos);
            return;
        }

        //third quadrant
        if(averageHeading.x<0 and averageHeading.y < 0){
            fortyFiveCcWise(cPos);
            return;
        }

        //fourth
        if(averageHeading.x>0 and averageHeading.y<0){
            fortyFiveCwise(cPos);
            return;
        }

        if(averageHeading.x==0 and averageHeading.y==0){
            randomHead = cPos+getFullRandom(cPos);
            std::cout<<"average direction zeroed out, full random applied"<<std::endl;
        }else{
            randomHead = cPos+getFullRandom(cPos);
            std::cout<< "average was a unit vector, full random applied"<<std::endl;
        }


    }

}


Vec2 semirandomHeading::getNext(){
    return randomHead;
}