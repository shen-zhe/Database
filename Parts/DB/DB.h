#ifndef MY_PROJECT_DB_H
#define MY_PROJECT_DB_H
#include "Runner.h"
#include <set>
#include <vector>


namespace DB {
    class DB {
    private:
        std::set<Runner*> runners;
    public:
        void loadDB (const std::string& filename);
        void saveDB (const std::string& filename);
        std::string printDB ();
        void unloadDB ();
        void add (Runner* runner);

        DB* select (std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t, int mask);
        int insert (std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t);
        DB* deletefromdb (std::string N, std::string S, std::string P, std::string g, int a, int n, std::string c, int t, int mask);
        int getFreeID ();
        
    };
}


#endif //MY_PROJECT_DB_H