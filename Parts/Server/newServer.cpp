#include "newServer.hpp"
#include "../Parser/interpreter.h"
#include <sstream>
#include <iomanip>
#include <iostream>

tcp_connection::tcp_connection(int client_socket, DB::DB* db)
    : client_socket_(client_socket), db_(db) {}

tcp_connection::~tcp_connection() {
    close(client_socket_);
}


void tcp_connection::start() {
    std::cout << "Somebody connected." << std::endl;
    handle_request();
}


void tcp_connection::handle_request() {
    char size_buf[10];
    ssize_t bytes_received = recv(client_socket_, size_buf, 10, 0);
    if (bytes_received <= 0) {
        std::cerr << "Failed to receive size of request." << std::endl;
        return;
    }
    int size;
    std::istringstream(std::string(size_buf, 10)) >> size;
    std::cout << "Got size of request: " << size << std::endl;

    std::string request(size, '\0');
    bytes_received = recv(client_socket_, &request[0], size, 0);
    if (bytes_received <= 0) {
        std::cerr << "Failed to receive request." << std::endl;
        return;
    }

    std::cout << "Got request: " << request << std::endl;
    auto* core = new ParserModule::Interpreter(db_);
    std::cout << "Core created. " << core << std::endl;

    std::istringstream req_stream(request);
    core->switchInputStream(&req_stream);
    std::string answer = core->parse();

    std::stringstream ss;
    ss << std::setw(10) << std::setfill('0') << answer.size();
    std::string size_str = ss.str();

    send(client_socket_, size_str.c_str(), size_str.size(), 0);
    send(client_socket_, answer.c_str(), answer.size(), 0);
}


tcp_server::tcp_server(DB::DB* db, int port, int max_clients)
    : db_(db), port_(port), running_(true), max_clients_(max_clients), active_clients_(0) {
    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in server_addr {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket_, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        close(server_socket_);
        throw std::runtime_error("Failed to bind socket");
    }

    if (listen(server_socket_, SOMAXCONN) == -1) {
        close(server_socket_);
        throw std::runtime_error("Failed to listen on socket");
    }

    std::cout << "Server acceptor started on port " << port << std::endl;

    // Start the command listener thread
    command_thread_ = std::thread(&tcp_server::command_listener, this);

    // Start worker threads
    for (int i = 0; i < max_clients_; ++i) {
        worker_threads_.emplace_back(&tcp_server::worker_thread, this);
    }
}


tcp_server::~tcp_server() {
    stop();
    command_thread_.join();
    for (auto& thread : worker_threads_) {
        thread.join();
    }
    close(server_socket_);
}


void tcp_server::run() {
    while (running_) {
        start_accept();
    }
}


void tcp_server::stop() {
    running_ = false;
    close(server_socket_);
    queue_cv_.notify_all();
}


void tcp_server::start_accept() {
    sockaddr_in client_addr {};
    socklen_t client_addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket_, (struct sockaddr*)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        if (running_) {
            std::cerr << "Failed to accept connection" << std::endl;
        }
        return;
    }

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        client_queue_.push(client_socket);
    }
    queue_cv_.notify_one();
}


void tcp_server::handle_client(int client_socket) {
    tcp_connection connection(client_socket, db_);
    connection.start();
}


void tcp_server::worker_thread() {
    while (running_) {
        int client_socket = -1;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this] { return !client_queue_.empty() || !running_; });
            if (!running_ && client_queue_.empty()) {
                return;
            }
            client_socket = client_queue_.front();
            client_queue_.pop();
        }

        if (client_socket != -1) {
            ++active_clients_;
            handle_client(client_socket);
            --active_clients_;
        }
    }
}


void tcp_server::command_listener() {
    std::string command;
    while (running_) {
        std::cin >> command;
        if (command == "stop") {
            std::cout << "Stopping server..." << std::endl;
            stop();
        }
    }
}
