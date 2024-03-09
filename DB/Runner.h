//
// структура хранения участников 
//


#ifndef MY_PROJECT_RUNNER_H
#define MY_PROJECT_RUNNER_H

#include <string>
#include <utility>
#include <iostream>
#include <iomanip>



namespace DB {
    class Runner{
    public:
        Runner(int Pass, std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t):
        id(Pass), name(std::move(N)), surname(std::move(S)), patronymic(std::move(P)), gender(std::move(g)), age(a), number(n), city(std::move(c)), sec(t){}

        std::string print() const {
            std::ostringstream str;
            str << id << " | " << surname << " " << name << " " << patronymic << " | " << gender << " " << age << " | " << number<<  " " << city << " | " << std::setfill('0') << std::setw(2)<< sec/3600 << ":"<< std::setfill('0') << std::setw(2)<< (sec%3600)/60 << ":"<< std::setfill('0') << std::setw(2) <<  sec%60 << std::endl;
            return str.str();
        }

        std::string getCSVRepr () const {
            return std::to_string(id) + "," + surname + "," + name + ","  + patronymic + "," + gender + "," + std::to_string(age) + "," + std::to_string(number) + "," + city + "," + std::to_string(sec);
        }

        //std::setfill('0') << std::setw(2)
        int id;
        std::string name;
        std::string surname; 
        std::string patronymic;
        std::string gender;
        int age;
        int number;
        std::string city;
        int sec;

    };
}

#endif //MY_PROJECT_RUNNER_H