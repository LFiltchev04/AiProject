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

 void sendQ::push(logObj logItem) {
    {
        std::lock_guard<std::mutex> lock(lockGuard);
        opQ.push(std::move(logItem));
    }
    cv.notify_one(); // wake the worker if it’s waiting
}


void sendQ::shutdown() {
    {
        std::lock_guard<std::mutex> lock(lockGuard);
        shutdown_ = true;
    }
    cv.notify_all();
}


std::optional<logObj> sendQ::pop() {

    std::unique_lock<std::mutex> lock(lockGuard);
    cv.wait(lock, [this] { return !opQ.empty() || shutdown_; });

    if (shutdown_ && opQ.empty()){
        return std::nullopt; // signal worker to stop
    }
        
    logObj item = std::move(opQ.front());
    opQ.pop();
    return item;

}


void lgr::run(){
        while (true) {
            auto item = logQ.pop();

            //will add persist logic thats toggle-able, but not right now, the node server has to work for that. 
            send(item.value());

            if (!item) break; // queue shutdown + empty, exit thread
        }
}

//this unpackages the log data objects and sends them to the log server, always runs in worker thread
void lgr::send(logObj parse){

    //all the unpackaged data has to be transformed into a json format and crammed into this string, then ill just JSON.parse() on the other end 

    try{

       http::Request postData(connString+"/addLog");
       
        json parsed = parse;
        std::string jString = parsed.dump(-1);
        
        postData.send("POST", jString , {{"Content-Type","application/json"}});

    }catch(const std::exception& error){
        std::cerr<<"network failure from thread: " << error.what() << std::endl ;
    }

}


logObj lgr::buildLogObj(std::string saw, Vec2 pos){
    logObj result;
    entity type;

    if(saw == "o"){
        type = empty;
    }

    if(saw == "w"){
        type = wall;
    }

    if(saw == "f"){
        type = fox;
    }

    if(saw == "h"){
        type = hound;
    }

    if(saw == "{" or saw == "}" or saw == "[" or saw == "]"){
        type = teleporter;
    }

    //i am not so sure about that one to be honest
    if(saw == "e"){
        type == ext;
    }

    //this too? 
    if(saw == "!"){
        type == goal;
    }

    result.absLocation = pos;

    return result;
}