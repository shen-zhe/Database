#include <thread>
#include <iostream>
#include "Parts/Server/newServer.hpp"

int main(int argc, char **argv) {
    std::string f;
    FILE* q1;
    int port;

    if (argc != 4) {
        printf("Usage: %s [DBfile] [port] [max_clients]\n", argv[0]);
        return -1;
    }
    if ((q1 = fopen(argv[1], "r")) == nullptr) {
        printf("Can't open file %s\n", argv[1]);
        return -2;
    }
    fclose(q1);
    try {
        std::string s = argv[2];
        port = std::stoi(s);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -3;
    }

    int max_clients;
    try {
        max_clients = std::stoi(argv[3]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -4;
    }

    f = argv[1];
    auto* db = new DB::DB();
    db->loadDB(f);
    bool isRunning = true;

    try {
        tcp_server server(db, port, max_clients);

        std::thread server_thread([&server]() {
            server.run();
        });

        std::cout << "Server started." << std::endl;

        std::string command;
        while (isRunning) {
            std::cin >> command;
            if (command == "stop") {
                isRunning = false;
                server.stop();
            }
        }

        server_thread.join();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    db->unloadDB();
    delete db;
    return 0;
}
