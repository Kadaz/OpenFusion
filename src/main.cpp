#include "CNLoginServer.hpp"
#include "CNShardServer.hpp"
#include "PlayerManager.hpp"
#include "ChatManager.hpp"

#include "settings.hpp"

#ifdef __MINGW32__
    #include "mingw/mingw.thread.h"
#else 
    #include <thread>
#endif
#include <string>

void startShard(CNShardServer* server) {
    server->start();
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0) {
        std::cerr << "OpenFusion: WSAStartup failed" << std::endl; 
        exit(EXIT_FAILURE); 
    }
#endif
    settings::init();
    std::cout << "[INFO] Intializing Packet Managers..." << std::endl;
    PlayerManager::init();
    ChatManager::init();

    std::cout << "[INFO] Starting Server Threads..." << std::endl;
    CNLoginServer loginServer(settings::LOGINPORT);
    CNShardServer shardServer(settings::SHARDPORT);

    std::thread shardThread(startShard, (CNShardServer*)&shardServer);
    
    loginServer.start();

    shardServer.kill();
    shardThread.join();
    
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}