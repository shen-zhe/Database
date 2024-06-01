#ifndef NEWSERVER_HPP
#define NEWSERVER_HPP

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../DB/DB.h"
#include <thread>
#include <atomic>
#include <queue>
#include <vector>
#include <condition_variable>
#include <mutex>

class tcp_connection {
public:
    explicit tcp_connection(int client_socket, DB::DB* db);
    ~tcp_connection();
    void start();

private:
    void handle_request();
    int client_socket_;
    DB::DB* db_;
};

class tcp_server {
public:
    explicit tcp_server(DB::DB* db, int port, int max_clients);
    ~tcp_server();
    void run();
    void stop();

private:
    void start_accept();
    void handle_client(int client_socket);
    void worker_thread();

    int server_socket_;
    int port_;
    DB::DB* db_;
    std::atomic<bool> running_;
    std::thread command_thread_;
    std::vector<std::thread> worker_threads_;
    std::queue<int> client_queue_;
    std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    int max_clients_;
    std::atomic<int> active_clients_;

    void command_listener();
};

#endif // NEWSERVER_HPP
