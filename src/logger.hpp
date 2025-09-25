#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <thread>
#include <fstream>
#include <queue>
#include <mutex>
#include <iostream>
#include <optional>
#include <condition_variable>
#include "percepts.hpp"
#include "json.hpp"
#include "vec2.hpp"
#include "utils.hpp"




//this will be used to push the data to the logger where it will be processed independently by the worker thread
class sendQ {
public:
    void push(std::string logItem) {
        {
            std::lock_guard<std::mutex> lock(lockGuard);
            opQ.push(std::move(logItem));
        }
        cv.notify_one(); // wake the worker if it’s waiting
    }

    // pop waits until an item is available, returns std::nullopt if shutdown
    std::optional<std::string> pop() {
        std::unique_lock<std::mutex> lock(lockGuard);
        cv.wait(lock, [this] { return !opQ.empty() || shutdown_; });

        if (shutdown_ && opQ.empty())
            return std::nullopt; // signal worker to stop

        std::string item = std::move(opQ.front());
        opQ.pop();
        return item;
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(lockGuard);
            shutdown_ = true;
        }
        cv.notify_all();
    }
    void push(logObj logItem);

    // pop waits until an item is available, returns std::nullopt if shutdown
    std::optional<logObj> pop();
    //called to eliminate the thread if the logger cannot be initialized
    void shutdown();

private:
    std::queue<std::string> opQ;
    std::mutex lockGuard;
    std::condition_variable cv;
    bool shutdown_ = false;
};









class lgr {

    std::thread logWorker;
    bool inited = false;
    int foxCount;
    int houndCount;
    static sendQ logQ;
    //the url/port of the remote goes here, hardcoded for now. 
    std::string connString ="http://127.0.0.1:3005/addLog";


    void run();

    void fileWrite(logObj);
    //this method should be called during initialization to confirm whether the server is responding to prevent logs from cluttering memory
    bool initServer(std::string connStr);

    
public:
//refactor this bullshit in the cpps later
    lgr(std:: string connString): logWorker([this]{this->run();}) {
        
        if(connString.empty()){
            logQ.shutdown();
            return;
        }

        if(!initServer(connString)){
            logQ.shutdown();
            return;
        }

        std::cout << "logger succesfully initialized"<<std::endl;
    }
    
    void addLogItem(logObj obj, Vec2 pos){
        
        logQ.push(obj);
    }

    void send(logObj parse);


    void shutDown(){
        logQ.shutdown();

        if(logWorker.joinable()){
            logWorker.join();
        }
    }

    //add item to visualizer is absoloute position only
    logObj buildLogObj(std::string saw, Vec2 pos);
};


 
//the string should look like this: http://(address):(port number), no trailing slash
//initializes the logger globally here:





#endif // LOGGER_HPP

