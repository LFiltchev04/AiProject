#include "logger.hpp"
#include "iostream"
#include <queue>
#include "HTTPRequest.hpp"

sendQ lgr::logQ;


bool lgr::initServer(std::string connString){
    
    try{
        http::Request checkServer(connString+"/checkIsOn");
        checkServer.send("GET");
    }catch(std::exception error){
        std::cerr<<"failed to find server instance at: "<< connString <<std::endl<<"by error: "<< error.what();
        return false;
    }


}

//this unpackages the log data objects and sends them to the log server, always runs in worker thread
void lgr::send(){

    //all the unpackaged data has to be transformed into a json format and crammed into this string, then ill just JSON.parse() on the other end 
    std::string reqBody = "";

    std::string debugString = R"({"thisisatest":"On", "wheteher":["this","will","work"]})";

    try{
       http::Request postData(connString);
       
        //http::HeaderFields hdr("Content-Type","application/json");
        
        const auto res = postData.send("POST", debugString, {{"Content-Type","application/json"}});

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