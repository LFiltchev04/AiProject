#include "vec2.hpp"
#include "utils.hpp"


class foxLock{
    std::vector<foxLog> targetData;
    public:
    foxLock();
    //any fox can move a certain number of tiles which i know, so i ca remove some noise from the locations i get fed by placing the estimates whithin the radius
    void trueLocation(Vec2 foxPos);

    //iterates over the feed data and updates the vectors
    void iterate(foxLog log);
    
    //tries to estimate the location of the nearest fox or supplies direct coordinates 
    Vec2 getFoxEstimate();
};