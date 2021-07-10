#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include "connect.h"
#include "node.h"

using namespace std;

void play(Connect env, int itermax){
    srand(time(NULL));
    bool continue_game(true);
    bool ask_order(false);
    int move(1);
    string man, computer, order, str;
    int man_turn(2*(rand()%2)-1);
    if(man_turn == 1){ man="first"; computer="second"; }
    else{ man="second"; computer="first"; }

    cout << "You are the " << man << " player, the computer is the " << computer << endl;
    cout << "To play choose the column number between 1 and 7" << endl << endl;
    cout << env.to_string() << endl;

    while(continue_game){
        if(env.getplayerjustmoved()!=man_turn){
            ask_order = true;
            while(ask_order){
                cout << "Enter a column number : ";
                cin >> order;
                if(order=="q"){
                    continue_game = false;
                    ask_order = false;
                }
                else{
                    move = env.convertmove(order);
                    if(move!=-1){
                        env.domove(move);
                        ask_order = false;
                    }
                }
            }
        }
        else{ move = Node::mcts(env, 5000); env.domove(move); }
        if(order=="q"){ cout << "You left the game" << endl; }
        else{
            if(man_turn == env.getplayerjustmoved()){ str = "You"; }
            else{ str = "Computer"; }
            cout << endl << env.show(str, move+1) << endl;
        }
        if(env.getmoves().size()==0){ continue_game = false; }
    }
    if(order!="q"){
        if(env.value()==1){
            if(man_turn == env.getplayerjustmoved()){ str = "You"; }
            else{ str = "Computer"; }
            cout << str + " wins" << endl;
        }
        else{ cout << "Nobody wins" << endl; }
    }
}


int main()
{
    bool continue_main(true);
    bool ask_order(false);
    string order;
    cout << endl << "START CONNECT 4 APPLICATION" << endl << endl;
    while(continue_main){
        Connect env;
        play(env, 5000);
        ask_order = true;
        while(ask_order){
            cout << endl << "You wanna rematch ?" << endl;
            cout << "Choose between [y/n] : ";
            cin >> order;
            if (order=="y" || order=="n"){
                if(order=="n"){ continue_main = false; }
                ask_order = false;
            }
        }
    }
    cout << endl << "EXIT CONNECT 4 APPLICATION" << endl;
    system("PAUSE");
    return 0;
}