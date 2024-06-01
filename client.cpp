#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("%s [host] [port]\n", argv[0]);
        return -1;
    }

    const char* host = argv[1];
    int port = std::stoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -2;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return -3;
    }

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return -4;
    }

    std::string line;
    int size;
    std::stringstream ss;
    char buffer[1024];

    while (std::getline(std::cin, line, '\0') && (line != "stop")) {
        size = line.size();
        ss.str("");
        ss << std::setw(10) << std::setfill('0') << size;
        std::string str = ss.str();

        if (send(sockfd, str.c_str(), str.size(), 0) < 0) {
            perror("send size");
            break;
        }
        std::cout << "Sent size: " << str << std::endl;

        if (send(sockfd, line.c_str(), line.size(), 0) < 0) {
            perror("send request");
            break;
        }
        std::cout << "Sent request: " << line << std::endl;

        if (recv(sockfd, buffer, 10, 0) < 0) {
            perror("recv size");
            break;
        }
        ss.str(std::string(buffer, 10));
        ss >> size;
        std::cout << "Answer size: " << size << std::endl;

        std::string answer;
        answer.resize(size);
        if (recv(sockfd, &answer[0], size, 0) < 0) {
            perror("recv answer");
            break;
        }
        std::cout << answer;
    }

    close(sockfd);
    return 0;
}
