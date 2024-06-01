#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Parts/DB/DB.h"
#include "Parts/Parser/interpreter.h"

std::string urlDecode(const std::string& str) {
    std::stringstream decoded;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            decoded << ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            int value;
            std::istringstream(str.substr(i + 1, 2)) >> std::hex >> value;
            decoded << static_cast<char>(value);
            i += 2;
        } else {
            decoded << str[i];
        }
    }
    return decoded.str();
}

std::string extractQueryFromRequest(const std::string& request) {
    size_t pos = request.find("GET /?");
    if (pos == std::string::npos) {
        pos = request.find("POST /?");
    }
    if (pos == std::string::npos) {
        return "";
    }

    size_t start = pos + 5;
    size_t end = request.find(" ", start);
    return request.substr(start, end - start);
}

void processRequest(int clientSocket, ParserModule::Interpreter& interpreter) {
    char buf[4096];
    memset(buf, 0, sizeof(buf));
    std::stringstream response;

    int recvLen = recv(clientSocket, buf, sizeof(buf) - 1, 0);
    if (recvLen == -1) {
        perror("recv");
        close(clientSocket);
        return;
    }

    std::string request(buf, recvLen);
    std::cout << "Received request: " << request << std::endl;

    std::string query = extractQueryFromRequest(request);
    if (query.empty()) {
        response << "HTTP/1.1 400 Bad Request\r\n";
        response << "Content-Length: 0\r\n";
        response << "\r\n";
        send(clientSocket, response.str().c_str(), response.str().length(), 0);
        close(clientSocket);
        return;
    }

    query = urlDecode(query);

    std::cout << "Decoded query: " << query << std::endl;

    // Парсим запрос с помощью интерпретатора
    interpreter.switchInputStream(&std::istringstream(query));
    std::string result = interpreter.execute();

    // Формируем ответ
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Length: " << result.length() << "\r\n";
    response << "\r\n";
    response << result;

    // Отправляем ответ клиенту
    if (send(clientSocket, response.str().c_str(), response.str().length(), 0) == -1) {
        perror("send");
    }

    close(clientSocket);
}


int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) == -1) {
        perror("listen");
        close(serverSocket);
        return 1;
    }

    std::cout << "Waiting for connections...\n";

    // Создаем экземпляр базы данных и интерпретатора
    auto* db = new DB::DB();
    db->loadDB("runners.csv");
    ParserModule::Interpreter interpreter(db);

    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            perror("accept");
            continue;
        }

        std::cout << "Got a connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        // Обрабатываем запрос
        processRequest(clientSocket, interpreter);
    }

    close(serverSocket);

    return 0;
}
