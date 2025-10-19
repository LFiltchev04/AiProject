#include "vec2.hpp"
#include "utils.hpp"
#include <deque>
#include <random>
#include <iostream>


class semirandomHeading{
    int radius;
    Vec2 randomHead;
    Vec2 increment;
    std::uniform_int_distribution<int> randInt;
    std::default_random_engine randGen;
    Vec2 prior;
    Vec2 priorPrior;


    int iters = 0;


    std::deque<Vec2> pastSeven;
    doubleVec averageHeading;

    public:
    Vec2 getFullRandom();
    semirandomHeading();
    Vec2 fortyFiveCcWise();
    Vec2 fortyFiveCwise();
    void iterate(Vec2 cPos, Vec2 absDir);
    Vec2 getNext(Vec2 cAbsPos);

};