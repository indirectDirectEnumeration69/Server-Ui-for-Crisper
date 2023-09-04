#include "Server.h"

std::mutex mtx;
std::condition_variable cv;
std::atomic<bool> ready{false};
std::atomic<bool> stopCheck{false};

enum ServerStatus { IDLE, RUNNING };
enum ThreadStatus { THREAD_IDLE, THREAD_RUNNING, THREAD_STOPPED };

struct ThreadInstance {
    int id;
    ThreadStatus status;
};

struct ServerInstance {
    int id;
    ServerStatus status;
    std::map<int, ThreadInstance> threads;
};

std::map<int, ServerInstance> serverInstances;

void createServerInstance(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    ServerInstance instance = { id, IDLE, {} };
    serverInstances[id] = instance;
    ready = true;
    cv.notify_one();
}

void updateServerInstance(int serverId, int threadId, ThreadStatus threadStatus) {
    std::unique_lock<std::mutex> lock(mtx);
    if (serverInstances.find(serverId) != serverInstances.end()) {
        ThreadInstance instance = { threadId, threadStatus };
        serverInstances[serverId].threads[threadId] = instance;
    }
    ready = true;
    cv.notify_one();
}
void CheckServerInstances() {
    std::unique_lock<std::mutex> lock(mtx);
    while (true) {
        cv.wait(lock, [] { return ready.load() || stopCheck.load(); });
        std::cout << "\033c";
        std::cout << "\033[36m" << "Server List:" << "\033[0m" << std::endl;

        for (auto& pair : serverInstances) {
            auto& instance = pair.second;
            std::cout << "  Server " << instance.id << ": threads " << instance.threads.size() << "\n";
        }
        std::cout << "\n\033[33m" << "Server Status:" << "\033[0m" << std::endl;

        for (auto& pair : serverInstances) {
            auto& instance = pair.second;
            std::cout << "  Server " << instance.id << ": "<< (instance.status == RUNNING ? "\033[32mRunning\033[0m" : "\033[31mIdle\033[0m") << std::endl;
            std::cout << "\033[32mServer " << instance.id << " threads status:\033[0m" << std::endl;
            int totalThreads = instance.threads.size();

            for (auto& threadPair : instance.threads) {
                auto& threadInstance = threadPair.second;
                std::string statusStr;
                std::string colorCode;

                switch (threadInstance.status) {
                case THREAD_IDLE:
                    statusStr = "idle";
                    colorCode = "\033[33m";  
                    break;
                case THREAD_RUNNING:
                    statusStr = "running";
                    colorCode = "\033[32m"; 
                    break;
                case THREAD_STOPPED:
                    statusStr = "stopped";
                    colorCode = "\033[31m"; 
                    break;
                }
                std::cout << "thread " << threadInstance.id << " out of " << totalThreads << ": " 
                   << colorCode << statusStr << "\033[0m" << std::endl;
            }
            std::cout << "\n" << std::endl;
        }

        ready = false;

        if (stopCheck.load()) {
            break;
        }
    }
}
void updateServerStatus(int serverId, ServerStatus status) {
    std::unique_lock<std::mutex> lock(mtx);
    if (serverInstances.find(serverId) != serverInstances.end()) {
        serverInstances[serverId].status = status;
    }
    ready = true;
    cv.notify_one();
}
std::thread checkServerInstancesThread;

void Server() {
    {
        std::unique_lock<std::mutex> lock(mtx);
        checkServerInstancesThread = std::thread(CheckServerInstances);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    createServerInstance(1);
    createServerInstance(2);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    //need to set conditions for server status and instance changes based on thread behaviour.
    updateServerStatus(1, RUNNING);
    updateServerInstance(1, 1, THREAD_RUNNING);
    updateServerInstance(1, 2, THREAD_IDLE);
    updateServerInstance(1, 3, THREAD_STOPPED);

    updateServerStatus(2, IDLE);
    updateServerInstance(2, 1, THREAD_RUNNING);
    updateServerInstance(2, 2, THREAD_IDLE);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::unique_lock<std::mutex> lock(mtx);
        stopCheck = true;
        ready = true;
        cv.notify_one();
    }

    checkServerInstancesThread.join();
}

