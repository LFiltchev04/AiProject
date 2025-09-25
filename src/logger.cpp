#include "logger.hpp"
#include "iostream"
#include <queue>
#include "HTTPRequest.hpp"

sendQ lgr::logQ;

//pops out one of the variety of json objects that the functions log as they happen
void lgr::send(){

    std::string jsn = *logQ.pop();

    try{
       http::Request postData(connString);
       
        
        const auto res = postData.send("POST", jsn, {{"Content-Type","application/json"}});

           std::cout << std::string{res.body.begin(), res.body.end()} << '\n'; // i should remove this.

    }catch(const std::exception& error){
        std::cerr<<"network failure from thread: " << error.what() << std::endl ;
    }
}



logObj lgr::debugFunction(){

    Vec2 testCoords;
    testCoords.x = 20;
    testCoords.y = -20;
    
    logObj test;
    return test;


}


