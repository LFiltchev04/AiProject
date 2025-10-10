#include "internalMap.hpp"
#include <unordered_map>
#include "utils.hpp"


internalMap::internalMap(){

heading = 'F';
cAbsPos = {0,0};
consistent = true;
}


//theese overly long functions can probably be made with a loop but the cases are so little i dont want to bother 
//i am pretty sure i need to change the underlying functions to make sure 
void internalMap::updateMap(Percepts nVizData){

    // Helper lambda to get char safely
    auto pick = [](const std::string &s)->char{
        return s.empty() ? ' ' : s[0];
    };


    for(int x = 0; x<nVizData.current.size();x++){
        fastAccess[hashCords(cAbsPos.x,cAbsPos.y)]=node(nVizData.current[x][0]);
        checkChange({cAbsPos.x,cAbsPos.y},nVizData.current[x][0]);
    }


    emptySeenNodes crrMax;
    // heading == Forward (0, +1)
    if(heading == 'F'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y + 1 + relDist };
            char ch = pick(nVizData.forward.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.zeroOne = nVizData.forward.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.left.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.oneZero = nVizData.left.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x + 1 + relDist, cAbsPos.y };
            char ch = pick(nVizData.right.at(relDist));
            
            if(ch=='w'){
                crrMax.zeroOne = nVizData.right.size()-1;
            }
            
            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        writeToSparse(cAbsPos,crrMax);

        return;
    }

    // heading == Right (+1, 0)
    if(heading == 'R'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x + 1 + relDist, cAbsPos.y };
            char ch = pick(nVizData.forward.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.oneZero = nVizData.forward.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y + 1 + relDist };
            char ch = pick(nVizData.left.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.zeroOne = nVizData.left.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.right.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.minusZero = nVizData.right.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        writeToSparse(cAbsPos,crrMax);

        return;
    }

    // heading == Back (0, -1)
    if(heading == 'B'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y - 1 - relDist };
            char ch = pick(nVizData.forward.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.zeroMinus = nVizData.forward.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x + 1 + relDist, cAbsPos.y };
            char ch = pick(nVizData.left.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.oneZero = nVizData.right.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.right.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.minusZero = nVizData.right.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        writeToSparse(cAbsPos,crrMax);

        return;
    }

    // heading == Left (-1, 0)
    if(heading == 'L'){
        for(int relDist = 0; relDist < nVizData.forward.size(); ++relDist){
            Vec2 pos { cAbsPos.x - 1 - relDist, cAbsPos.y };
            char ch = pick(nVizData.forward.at(relDist));
            size_t key = hashCords(pos.x, pos.y);
            checkChange(pos, ch);


            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.minusZero = nVizData.forward.size()-1;
            }

            fastAccess[key] = node(ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.left.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y - 1 - relDist };
            char ch = pick(nVizData.left.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.zeroMinus = cAbsPos.y-nVizData.left.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }

        for(int relDist = 0; relDist < nVizData.right.size(); ++relDist){
            Vec2 pos { cAbsPos.x, cAbsPos.y + 1 + relDist };
            char ch = pick(nVizData.right.at(relDist));

            if(ch == 'o'){
                continue;
            }

            if(ch=='w'){
                crrMax.zeroOne = cAbsPos.y+nVizData.right.size()-1;
            }

            size_t key = hashCords(pos.x, pos.y);
            fastAccess[key] = node(ch);
            checkChange(pos, ch);
            std::cout << "saw a:" << ch << " at: " << pos.x << " <-x|y->" << pos.y << std::endl;
        }
        
        writeToSparse(cAbsPos,crrMax);
        
        return;
    }
}

void internalMap::writeToSparse(Vec2 pos, emptySeenNodes sight){
    auto iterX = xOnlyAccess.find(pos.x);
    auto iterY = yOnlyAccess.find(pos.y);
    
    std::pair<int,int> tempX{sight.zeroOne,sight.zeroMinus};
    if(iterX == xOnlyAccess.end()){        
        xOnlyAccess.emplace(pos.x,std::vector<std::pair<int,int>>{tempX});
    }else{
        iterX->second.push_back(tempX);
    }

    std::pair<int,int> tempY{sight.oneZero,sight.minusZero};
    if(iterY == yOnlyAccess.end()){
        yOnlyAccess.emplace(pos.y,std::vector<std::pair<int,int>>{tempY});
    }else{
        iterY->second.push_back(tempY);
    }

}


Vec2 internalMap::trueDir(char direction){

    if(direction==' '){
        //return {0,0};
    }

    if(heading == 'F'){
        
        if(direction == 'F'){
              return {0,1};
        }

        if(direction == 'R'){
              return {1,0};
        }

        if(direction == 'B'){
            return {0,-1};
        }

        if(direction == 'L'){
            return {-1,0};
        }
    }
    

    if(heading == 'R'){
        
        if(direction == 'F'){
            return {1,0};
        }

        if(direction == 'R'){
            return {0,-1};
        }

        if(direction == 'B'){
            return {-1,0};
        }

        if(direction == 'L'){
            return {0,1};
        }
    }


    if(heading == 'B'){

        if(direction == 'F'){
            return {0,-1};
        }

        if(direction == 'R'){
            return {-1,0};
        }

        if(direction == 'B'){
            return {0,1};
        }

        if(direction == 'L'){
            return {1,0};
        }
    }


    if(heading == 'L'){

        if(direction == 'F'){
            return {-1,0};
        }

        if(direction == 'R'){
            return {0,1};
        }

        if(direction == 'B'){
            return {1,0};
        }

        if(direction == 'L'){
            return {0,-1};
        }
    }

    //return {0,0};
}


void internalMap::iterateState(std::string cmdIn){
    if(cmdIn.empty()) return;
    char c = cmdIn[0];
    // compute absolute target cell for this relative command
    Vec2 rel = trueDir(c);
    Vec2 absTarget = cAbsPos + rel;

    if(c == 'F'){
        // only move forward if the absolute target cell is not a wall
        if(!isWall(absTarget)){
            iterateCpos(rel);
        }
    } else {
        // rotations should update heading immediately (do not gate on a relative coord)
        changeHeading(c);
    }
     
 }

Vec2 internalMap::getHeadingVector(){
    return trueDir(heading);
}

Vec2 internalMap::currentPos(){
    return cAbsPos;
}

bool internalMap::isWall(Vec2 checkPos){
   size_t key = hashCords(checkPos.x, checkPos.y);

    auto it = fastAccess.find(key);
    if (it == fastAccess.end()){
        return false;
    }         
    return it->second.type == 'w';
};

bool internalMap::priorVisit(Vec2 checkThis){

    if(wasSeen(checkThis)){
        return true;
    }
    
    if(fastAccess.find(hashCords(checkThis.x,checkThis.y)) == fastAccess.end()){
        return false;
    }

    return false;
}


node internalMap::getPrior(Vec2 atPlace){
    //i am not sure that this will not blow up if the element is not in the hashmap
    try{
        if(fastAccess.find(hashCords(atPlace.x,atPlace.y))==fastAccess.end()){
            
            if(wasSeen(atPlace)){
                return node('o');
            }
            return node();
        } 
        return fastAccess.find(hashCords(atPlace.x,atPlace.y))->second;
    }catch(std::exception err){

        //return error in a way i can interpret it as such
        node ndn;
        return ndn;
    }
    
}


bool internalMap::wasSeen(Vec2 analyzeThis){
    
    
    auto xIter = xOnlyAccess.find(analyzeThis.x);
    auto yIter = yOnlyAccess.find(analyzeThis.y);
    
    if(xIter==xOnlyAccess.end() and yIter==yOnlyAccess.end()){
        return false;
    }

    for(std::pair<int,int> var:xIter->second){
        if(analyzeThis.x>=var.first and analyzeThis.x<=var.second){
            return true;
        }
    }

    for(std::pair<int,int> var:yIter->second){
        if(analyzeThis.y>=var.first and analyzeThis.y<=var.second){
            return true;
        }
    }

}



void internalMap::changeHeading(char in){
    if(in==' '){
        return;
    }

    if(heading == 'F'){
        this->heading = in;

    std::cout<<"heading set to1:"<<this->heading<<std::endl;
        return;
    }

    if(heading=='R'){
        if(in=='F'){
            heading = 'R';
        }

        if(in=='R'){
            heading = 'B';
        }

        if(in=='B'){
            heading = 'L';
        }

        if(in=='L'){
            heading = 'F';
        }

    std::cout<<"heading set to2:"<<this->heading<<std::endl;
        return;
    }


    if(heading=='B'){
        if(in=='F'){
            heading = 'B';
        }

        if(in=='R'){
            heading = 'L';
        }

        if(in=='B'){
            heading = 'F';
        }

        if(in=='L'){
            heading = 'R';
        }

    std::cout<<"heading set to3:"<<this->heading<<std::endl;
        return;
    }

    if(heading=='L'){
        if(in=='F'){
            heading = 'L';
        }

        if(in=='R'){
            heading = 'F';
        }

        if(in=='B'){
            heading = 'R';
        }

        if(in=='L'){
            heading = 'B';
        }

    std::cout<<"heading set to4:"<<this->heading<<std::endl;
        return;
    }

    
}


void internalMap::iterateCpos(Vec2 newPos){
    cAbsPos=cAbsPos+newPos;
}


void internalMap::changeCpos(Vec2 newPos){
    cAbsPos = newPos;
}

char internalMap::getHeading(){
    return heading;
}


void internalMap::checkChange(Vec2 pos, char type){
    
        auto hashmapIter = fastAccess.find(hashCords(pos.x,pos.y));
        if(hashmapIter==fastAccess.end()){
            std::cout<<"CHECK FOUND NO MATCH AT "<<pos.to_string()<<std::endl;
            return;
        }

        if(hashmapIter->second.type == type){
            return;
        }else{
            std::cout<<std::endl<<std::endl<<std::endl<<"MAP NOT CONSISTENT"<<std::endl<<std::endl<<std::endl;
            consistent=false;
            return;
        }
    
}


Vec2 internalMap::relativeHead(char in){
    if(heading == 'F'){
        if(in=='F'){
            return {0,1};
        }
        if(in=='R'){
            return {1,0};
        }
        if(in=='B'){
            return {0,-1};
        }
        if(in=='L'){
            return {-1,0};
        }
    }

    if(heading == 'R'){
        if(in=='F'){
            return {1,0};
        }
        if(in=='R'){
            return {0,-1};
        }
        if(in=='B'){
            return {-1,0};
        }
        if(in=='L'){
            return {0,1};
        }
    }

    if(heading == 'B'){
        if(in=='F'){
            return {0,-1};
        }
        if(in=='R'){
            return {-1,0};
        }
        if(in=='B'){
            return {0,1};
        }
        if(in=='L'){
            return {1,0};
        }
    }

    if(heading == 'L'){
        if(in=='F'){
            return {-1,0};
        }
        if(in=='R'){
            return {0,1};
        }
        if(in=='B'){
            return {1,0};
        }
        if(in=='L'){
            return {0,-1};
        }
    }

    return {0,0};
}




internalMap::~internalMap(){

}