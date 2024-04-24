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
         Runner() : id(-1), name(""), surname(""), patronymic(""), gender(""), age(-1), number(-1), city(""), sec(-1) {}

        Runner(int Pass, std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t):
        id(Pass), name(std::move(N)), surname(std::move(S)), patronymic(std::move(P)), gender(std::move(g)), age(a), number(n), city(std::move(c)), sec(t){}

        std::string print() const {
            std::ostringstream str;
            str << id << " | " << surname << " " << name << " " << patronymic << " | " << gender << " " << age << " | " << number<<  " " << city << " | " << std::setfill('0') << std::setw(2)<< sec/3600 << ":"<< std::setfill('0') << std::setw(2)<< (sec%3600)/60 << ":"<< std::setfill('0') << std::setw(2) <<  sec%60 << std::endl;
            return str.str();
        }

        std::string getCSVRepr () const {
            return std::to_string(id) + "," + name  + "," + surname + ","  + patronymic + "," + gender + "," + std::to_string(age) + "," + std::to_string(number) + "," + city + "," + std::to_string(sec);
        }

        bool operator==(const Runner& other) const {
            return (name == other.name &&
                    surname == other.surname &&
                    patronymic == other.patronymic &&
                    gender == other.gender &&
                    age == other.age &&
                    number == other.number &&
                    city == other.city &&
                    sec == other.sec);
        }

        Runner& operator=(const Runner& other) {
            if (this != &other) {
                // Копирование значений из другого объекта
                id = other.id;
                name = other.name;
                surname = other.surname;
                patronymic = other.patronymic;
                gender = other.gender;
                age = other.age;
                number = other.number;
                city = other.city;
                sec = other.sec;
            }
            return *this;
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