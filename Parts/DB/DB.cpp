#include "DB.h"
#include <fstream>
#include <sstream>
#include <algorithm>


void DB::DB::loadDB(const std::string& filename) {
    std::ifstream file(filename);
    for (auto runner: runners) {
        delete runner;
    }
    runners.clear();

    std::string line;
    std::string param;
    std::vector<std::string> data;
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        data.clear();
        while (std::getline(lineStream, param, ',')) {
            data.push_back(param);
        }
        auto* runner = new Runner(std::stoi(data[0]), data[1], data[2], data[3], data[4], std::stoi(data[5]), std::stoi(data[6]), data[7], std::stoi(data[8]));
        runners.insert(runner);
    }
    file.close();
}


void DB::DB::saveDB(const std::string &filename) {
    std::ofstream file(filename);
    file << "id, name, surname, patronymic, gender, age, number, city, sec" <<std::endl;
    for (auto runner: runners) {
        file << runner->getCSVRepr() << std::endl;
    }
    file.close();
}


std::string DB::DB::printDB() {
    std::string str;
    for (auto runner: runners) {
        str += runner->print();
    }
    return str;
}


void DB::DB::unloadDB() {
    for (auto runner: runners) {
        delete runner;
    }
    runners.clear();
}


void DB::DB::add(Runner* runner) {
    runners.insert(runner);
}


int DB::DB::insert(std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t) {
    if (N.empty() || S.empty() || P.empty() || g.empty() || a == 0 || n == 0 || c.empty() || t == 0) {
        return -1;
    }
    auto run = new Runner(this->getFreeID(), N, S, P, g, a, n, c, t);
    this->add(run);
    return 0;
}

int DB::DB::getFreeID() {
    std::vector<int> ids;
    for (auto runner: runners) {
        ids.push_back(runner->id);
    }
    auto it = *std::max_element(std::begin(ids), std::end(ids));
    return it+1;
}


DB::DB* DB::DB::select(std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t, int mask){
        DB* selectDB = new DB();
        bool isRight;

        for (auto runner: runners) {
            isRight = true;
            if ((mask % 2 != 0) && (runner->name != N)) {
                isRight = false;
            }
            if ((mask / 2 % 2 != 0) && (runner->surname != S)) {
                isRight = false;
            }
            if ((mask / 4 % 2 != 0) && (runner->patronymic != P)) {
                isRight = false;
            }
            if ((mask / 8 % 2 != 0) && (runner->gender != g)) {
                isRight = false;
            }
            if ((mask / 16 % 2 != 0) && (runner->age != a)) {
                isRight = false;
            }
            if ((mask / 32 % 2 != 0) && (runner->number != n)) {
                isRight = false;
            }
            if ((mask / 64 % 2 != 0) && (runner->city != c)) {
                isRight = false;
            }
            if ((mask / 128 % 2 != 0) && (runner->sec != t)) {
                isRight = false;
            }
            if (isRight) {
                selectDB->add(runner);
            }
        }       
        return selectDB;
}


DB::DB* DB::DB::deletefromdb(std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t, int mask){
        DB* selectDB = new DB();
        bool isRight;
        auto it = runners.begin();

        for(auto it = runners.begin(); it != runners.end(); ) {
            auto runner = *it;
            isRight = true;
            if ((mask % 2 != 0) && (runner->name != N)) {
                isRight = false;
            }
            if ((mask / 2 % 2 != 0) && (runner->surname != S)) {
                isRight = false;
            }
            if ((mask / 4 % 2 != 0) && (runner->patronymic != P)) {
                isRight = false;
            }
            if ((mask / 8 % 2 != 0) && (runner->gender != g)) {
                isRight = false;
            }
            if ((mask / 16 % 2 != 0) && (runner->age != a)) {
                isRight = false;
            }
            if ((mask / 32 % 2 != 0) && (runner->number != n)) {
                isRight = false;
            }
            if ((mask / 64 % 2 != 0) && (runner->city != c)) {
                isRight = false;
            }
            if ((mask / 128 % 2 != 0) && (runner->sec != t)) {
                isRight = false;
            }
            if (isRight) {
                selectDB->add(runner);
                it = runners.erase(it); 
            } else {
                ++it;
            }
        }       
        return selectDB;
}


// DB::DB* DB::DB::deletefromdb(std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t, int mask){
//         DB* selectDB = new DB();
//         bool isRight;
//         auto it = runners.begin();

//         while (it != runners.end()) {
//             auto runner = *it;
//             isRight = true;
//             if ((mask % 2 != 0) && (runner->name != N)) {
//                 isRight = false;
//             }
//             if ((mask / 2 % 2 != 0) && (runner->surname != S)) {
//                 isRight = false;
//             }
//             if ((mask / 4 % 2 != 0) && (runner->patronymic != P)) {
//                 isRight = false;
//             }
//             if ((mask / 8 % 2 != 0) && (runner->gender != g)) {
//                 isRight = false;
//             }
//             if ((mask / 16 % 2 != 0) && (runner->age != a)) {
//                 isRight = false;
//             }
//             if ((mask / 32 % 2 != 0) && (runner->number != n)) {
//                 isRight = false;
//             }
//             if ((mask / 64 % 2 != 0) && (runner->city != c)) {
//                 isRight = false;
//             }
//             if ((mask / 128 % 2 != 0) && (runner->sec != t)) {
//                 isRight = false;
//             }
//             if (isRight) {
                
//                 selectDB->add(runner);
//                 it = runners.erase(it);
//                 std::cout<<"бан"<< std::endl;
//             } else {
//                 ++it;
//             }
//         }       
//         return selectDB;
// }
