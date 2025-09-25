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




//aggregates the reporting data for visualization or persisting on disk, that too should probably go into a utils file
struct logObj {
    Percepts vizData;
    Vec2 inCord;
    char mode;
    unsigned id;
};





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

private:
    std::queue<std::string> opQ;
    std::mutex lockGuard;
    std::condition_variable cv;
    bool shutdown_ = false;
};









class lgr {

    std::thread logWorker;

    int foxCount;
    int houndCount;
    static sendQ logQ;
    //the url/port of the remote goes here, hardcoded for now. 
    std::string connString ="http://127.0.0.1:3005/addLog";


    void run(){
        while (true) {
            auto item = logQ.pop();

            //will add persist logic thats toggle-able, but not right now, the node server has to work for that. 
            //send(item.value());

            if (!item) break; // queue shutdown + empty, exit thread
        }
    }

    void fileWrite(logObj);
    //this method should be called during initialization to confirm whether the server is responding to prevent logs from cluttering memory
    void initServer();

    
public:
    // parameterized constructor, also acts as default due to default args
    lgr(): logWorker([this]{this->run();}) {}
    lgr(int x){

    }
    
    void addLogItem(std::string obj){
        logQ.push(obj);
    }

    void send();

    void shutDown(){
        logQ.shutdown();

        if(logWorker.joinable()){
            logWorker.join();
        }
    }

    logObj debugFunction();

    static std::string foxLockJsonify(unsigned int id, Vec2 foxPos);

};









#endif // LOGGER_HPP






