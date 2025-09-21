#include "logger.hpp"
#include "iostream"
#include <queue>
#include "HTTPRequest.hpp"

//this unpackages the log data objects and sends them to the log server, always runs in worker thread
void lgr::send(logObj data){

    //all the unpackaged data has to be transformed into a json format and crammed into this string, then ill just JSON.parse() on the other end 
    std::string reqBody = "";

    std::string debugString = "{\"this is supposed to test whether\" : \"I can unpackage the JSON\", \"On'\": \'[\"the\",\"other\".\"end\"]\'}";

    try{
        http::Request postData(connString);
        const auto res = postData.send("POST", debugString,{"Content-Type", "application/x-www-form-urlencoded"});

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