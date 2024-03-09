#include "Generator.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <random>

void fillVectorFromFile(const std::string& filename, std::vector<std::string>& wordsVector);

void generator(const std::string& filename, int count) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::vector<std::string> N, S, P, c;
    int age, id = 0, sec, N_num, c_num;
    std::string gender;

    fillVectorFromFile("Name.txt", N);
    fillVectorFromFile("Patronymic.txt", P);
    fillVectorFromFile("Surname.txt", S);
    fillVectorFromFile("city.txt", c);

    std::mt19937 rng(static_cast<unsigned int>(std::time(0)));
    std::vector<int> num(c.size(), 0);

    for (int i = 0; i < count; i++) {
        c_num = std::uniform_int_distribution<int>(0, c.size() - 1)(rng);
        N_num = std::uniform_int_distribution<int>(0, N.size() - 1)(rng);

        std::string randomName = N[N_num];
        std::string randomSurname = S[std::uniform_int_distribution<int>(0, S.size() - 1)(rng)];
        std::string randomPatronymic = P[std::uniform_int_distribution<int>(0, P.size() - 1)(rng)];
        std::string city = c[c_num];
        num[c_num]++;
        int randomNumber = num[c_num];

        if (N_num >= N.size() / 2) {
            gender = 'G';
            randomSurname += "a";
            if (randomPatronymic.size() >= 3) {
                randomPatronymic = randomPatronymic.substr(0, randomPatronymic.length() - 3) + "на";
            }
        } else {
            gender = 'M';
        }
        age = std::uniform_int_distribution<int>(18, 60)(rng);
        sec = std::uniform_int_distribution<int>(7800, 21600)(rng);

        outFile << DB::Runner(id++, randomName, randomSurname, randomPatronymic, gender, age, randomNumber, city, sec).print();
    }

    outFile.close();
}

void fillVectorFromFile(const std::string& filename, std::vector<std::string>& wordsVector) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        wordsVector.push_back(word);
    }

    file.close();
}