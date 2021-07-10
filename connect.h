#ifndef CONNECT_H
#define CONNECT_H

#include <vector>
#include <string>

class Connect{
    public:
        Connect();
        Connect(bool _termined, int playerjustmoved, std::vector<std::vector<int>> _state, std::vector<int> _validmoves, std::vector<int> _indexes);
        int convertmove(std::string col);
        void domove(int col);
        std::vector<int> getmoves();
        int getplayerjustmoved();
        std::string show(std::string str, int move);
        std::string to_string();
        double value();

    private:
        bool terminated;
        int playerjustmoved;
        std::vector<std::vector<int>> state;
        std::vector<int> validmoves;
        std::vector<int> indexes;
        void removemove(int col);
        int countneighbors(int ind, int col, int imax, int a, int b);
        bool testalignment(int ind, int col);
        std::string convert(int x);
};

#endif