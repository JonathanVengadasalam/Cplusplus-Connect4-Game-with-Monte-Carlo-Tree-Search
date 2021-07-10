#include <iostream>
#include <vector>
#include <string>
#include "connect.h"

using namespace std;

Connect::Connect(){ 
    terminated=false;
    playerjustmoved = -1;
    int i,j;
    for(i=0; i<6; i++){
        vector<int> tmp;
        state.push_back(tmp);
        for (j=0; j<7; j++){ state[i].push_back(0); }
    }
    for (j=0; j<7; j++){ validmoves.push_back(j); }
    for (j=0; j<7; j++){ indexes.push_back(5); }
}

Connect::Connect(bool _termined, int _playerjustmoved, vector<vector<int>> _state, vector<int> _validmoves, vector<int> _indexes){
    terminated = _termined;
    playerjustmoved = _playerjustmoved;
    state = _state;
    validmoves = _validmoves;
    indexes = _indexes;
}

string Connect::convert(int x){
    string res;
    if (x == 1) { res = " X "; }
    if (x ==-1) { res = " O "; }
    if (x == 0) { res = "   "; }
    return res;
}

int Connect::convertmove(string col){
    int res(-1);
    try{
        res = stoi(col)-1;
        if(indexes[res]<0){
            cout << "Impossibme choice" << endl;
            return -1;
        }
    }
    catch(exception const &e){
        cerr << "ERREUR : " << e.what() << endl; 
    }
    return res;
}

int Connect::countneighbors(int ind, int col, int imax, int a, int b){
    int count = 0;
    int item = state[ind][col];
    for (int i=1; i<imax; i++){
        if(item != state[ind + a*i][col + b*i]) { return count; }
        count++;
    }
    return count;
}

void Connect::domove(int col){
    int ind(indexes[col]);
    playerjustmoved = -1*playerjustmoved;
    state[ind][col] = playerjustmoved;
    indexes[col] = ind - 1;
    if(indexes[col] < 0){ removemove(col); }
    if(testalignment(ind,col)) { 
        validmoves.clear();
        terminated = true;
    }
}

int Connect::getplayerjustmoved(){ return playerjustmoved; }

vector<int> Connect::getmoves(){
    vector<int> res(validmoves);
    return res;
}

void Connect::removemove(int col){
    bool _end(false);
    int pos(0);
    while(!_end){
        if(validmoves[pos]==col){
            validmoves.erase(validmoves.begin()+ pos);
            _end=true;
        }
        pos++;
    }
}

string Connect::show(string str, int move){
    return str + " just played the column " + std::to_string(move) + "\n" + this->to_string() + "\n";
}

bool Connect::testalignment(int ind, int col){
    int ind1(ind+1);
    int ind2(6-ind);
    int col1(col+1);
    int col2(7-col);
    if (countneighbors(ind, col, ind2, 1, 0) + countneighbors(ind, col, ind1, -1, 0) > 2) { return true; }
    if (countneighbors(ind, col, col2, 0, 1) + countneighbors(ind, col, col1, 0, -1) > 2) { return true; }
    if (countneighbors(ind, col, min(ind2, col2), 1, 1) + countneighbors(ind, col, min(ind1, col1), -1, -1) > 2) { return true; }
    if (countneighbors(ind, col, min(ind2, col1), 1,-1) + countneighbors(ind, col, min(ind1, col2), -1,  1) > 2) { return true; }
    return false;
}

string Connect::to_string(){
    int i,j;
    string res(convert(state[0][0]));
    string line;
    for (i=0; i<27; i++) { line = line + "-"; }
    for (i=1; i<7;  i++) { res = res + "|" + convert(state[0][i]); }
    for (i=1; i<6;  i++) {
        res = res + "\n" + line + "\n" + convert(state[i][0]);
        for (j=1; j<7;  j++) { 
            res = res + "|" + convert(state[i][j]);
        }
    }
    res = res + "\n 1 ";
    for (i=1; i<7;  i++){ res = res + "  " + std::to_string(i+1) + " "; }
    return res;
}

double Connect::value(){ return 0.5 + 0.5*int(terminated); }