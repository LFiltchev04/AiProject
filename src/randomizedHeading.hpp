#include "vec2.hpp"
#include "utils.hpp"
#include <deque>
#include <random>
#include <iostream>


class semirandomHeading{
    int radius;
    Vec2 randomHead;
    std::uniform_int_distribution<int> randInt;
    std::default_random_engine randGen;


    std::deque<Vec2> pastSeven;
    doubleVec averageHeading;

    public:
    Vec2 getFullRandom(Vec2 cPos);
    semirandomHeading();
    Vec2 fortyFiveCcWise(Vec2 cPos);
    Vec2 fortyFiveCwise(Vec2 cPos);
    void iterate(int wallBumps, Vec2 cPos, Vec2 absDir);
    Vec2 getNext();


};