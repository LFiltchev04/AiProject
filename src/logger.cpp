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

    std::string jsn = *logQ.pop();

    try{

       http::Request postData(connString+"/addLog");
       
        
        postData.send("POST", jString , {{"Content-Type","application/json"}});

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