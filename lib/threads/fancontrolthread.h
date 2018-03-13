/*
 * fancontrolthread.h
 *
 *  Created on: Mar 8, 2018
 *      Author: dev03
 */

#ifndef LIB_THREADS_FANCONTROLTHREAD_H_
#define LIB_THREADS_FANCONTROLTHREAD_H_


#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <utility>
#include <queue>

namespace Dsr {

class HardwareManager;
class ThermalDetectionTask;

class FanControlThread {

public:
    explicit FanControlThread(HardwareManager* hdManager);
    void enqueue(::std::pair<int, char*> data);
    bool isQueueEmpty();
    ~FanControlThread();

private:
    HardwareManager* piManager;
    ::std::thread _thread;
    ::std::condition_variable condVar;
    ::std::mutex mutex;
    bool stop;

    ::std::queue<::std::pair<int, char*>> dataQueue;

    int process(int temp, char* id);
public:
    void run();
    void abort();

};

}


#endif /* LIB_THREADS_FANCONTROLTHREAD_H_ */
