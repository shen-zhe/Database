#ifndef MY_PROJECT_AST_H
#define MY_PROJECT_AST_H
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "../DB/DB.h"

namespace AST {
    class Param {
    public:
        std::string name;
        std::string value;
        Param() = default;
        Param(std::string n, std::string val) : name(std::move(n)), value(std::move(val)) {}
    };

    class Operation {
    public:
        std::string name;
        std::string surname; 
        std::string patronymic;
        std::string gender;
        int age;
        int number;
        std::string city;
        int sec;
        int mask = 0;
        void addParam (const Param& param) {
            if (param.name == "name") {
                name = param.value;
                mask += 1;
            }
            else if (param.name == "surname") {
                surname = param.value;
                mask += 2;
            }
            else if (param.name == "patronymic") {
                patronymic = param.value;
                mask += 4;
            }
            else if (param.name == "gender") {
                gender = param.value;
                mask += 8;
            }
            else if (param.name == "age") {
                age = std::stoi(param.value);
                mask += 16;
            }
            else if (param.name == "number") {
                number = std::stoi(param.value);
                mask += 32;
            }
            else if (param.name == "city") {
                city = param.value;
                mask += 64;
            }
            else if (param.name == "time") {
                sec = std::stoi(param.value);
                mask += 128;
            }
            else {
                std::cout << "Unknown Parameter: " << param.name << std::endl;
            }
        }
        virtual std::string execute (DB::DB* db) { return ""; }
    };

    class OperationSelect : public Operation {
        std::string execute(DB::DB* db) override {
            DB::DB* res = db->select(name, surname, patronymic, gender, age, number, city, sec, mask);
            std::string data = res->printDB();
            data += "\n";
            delete res;
            return data;
        }
    };

    class OperationInsert : public Operation {
        std::string execute(DB::DB* db) override {
            int res = db->insert(name, surname, patronymic, gender, age, number, city, sec);
            if (res == 0) {
                return "OK\n";
            } else {
                return "NOT OK\n";
            }
        }
    };

    class OperationDelete : public Operation {
        std::string execute(DB::DB* db) override {
            db->deletefromdb(name, surname, patronymic, gender, age, number, city, sec, mask);
            return "OK\n";
        }
    };
}





#endif //MY_PROJECT_AST_H