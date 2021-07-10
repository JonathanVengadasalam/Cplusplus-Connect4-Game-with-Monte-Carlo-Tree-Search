#include "node.h"
#include <vector>
#include <string>
#include <cmath>
#include <time.h>

using namespace std;

Node::Node(int playerjustmoved, vector<int> untriedmoved){
    _playerjustplayed = playerjustmoved;
    _untriedmoves = untriedmoved;
    _wins = 0;
    _visits = 0;
    _parent = 0;
}

Node::Node(int playerjustmoved, int move, Node *parent, vector<int> untriedmoved){
    _playerjustplayed = playerjustmoved;
    _untriedmoves = untriedmoved;
    _move = move;
    _parent = parent;
    _wins = 0;
    _visits = 0;
}

string Node::indentstring(int indent){
    string res("\n");
    for(int i=0; i<indent; i++){ res += "| "; }
    return res;
}

int Node::mcts(Connect &rootenv, int itermax){
    Node rootnode = Node(rootenv.getplayerjustmoved(), rootenv.getmoves());
    srand(time(NULL));
    
    
    for(int i=0; i<itermax; i++){
        Node *node(&rootnode);
        Connect env(rootenv);
        vector<int> moves;
        int move, pjm;
        double value;

        //select
        while(node->_untriedmoves.size()==0 && node->_childs.size()!=0){
            node = node->uctselect();
            env.domove(node->_move);
        }

        //expand
        if(node->_untriedmoves.size()!=0){
            move = node->_untriedmoves[rand()%node->_untriedmoves.size()];
            env.domove(move);
            node->removemove(move);
            node->_childs.push_back(Node(env.getplayerjustmoved(), move, node, env.getmoves()));
            node = &node->_childs.back();
        }

        //rollout
        moves = env.getmoves();
        while(moves.size() != 0){
            move = moves[rand()%moves.size()];
            env.domove(move);
            moves = env.getmoves();
        }

        //bockpropagate
        value = env.value();
        pjm = env.getplayerjustmoved();
        while(node != 0){
            node->_wins += 0.5 + (value - 0.5)*pjm*node->_playerjustplayed;
            node->_visits++;
            node = node->_parent;
        }
    }
    return rootnode.visitsselect()._move;
}

void Node::removeuntriedmove(int move){
    for(auto it=_untriedmoves.begin(); it!=_untriedmoves.end(); ++it){
        if(*it==move){ _untriedmoves.erase(it); }
    }
}

void Node::removemove(int col){
    bool _end(false);
    int pos(0);
    while(!_end){
        if(_untriedmoves[pos]==col){
            _untriedmoves.erase(_untriedmoves.begin()+ pos);
            _end=true;
        }
        pos++;
    }
}

string Node::to_string(){
    return "[M:" + std::to_string(_move) 
        + " W/V:" + std::to_string(int(_wins*100)/100) 
        + "/" + std::to_string(_visits) 
        + " U:" + to_stringmoves() + "]";
}

string Node::to_stringmoves(){
    string res("[");
    auto it=_untriedmoves.begin();
    if(it != _untriedmoves.end()){ res += std::to_string(*it); ++it; }
    for(it; it!=_untriedmoves.end(); ++it){
        res += "," + std::to_string(*it);
    }
    return res + "]";
}

string Node::to_stringtree(int indent){
    string res(indentstring(indent) + to_string());
    for(auto it=_childs.begin(); it!=_childs.end(); ++it){
        if(0 < it->_visits){ res += it->to_stringtree(indent + 1); }
    }
    return res;
}

Node *Node::uctselect(){
    bool _end(false);
    double max(-1), score(0);
    Node *best, *tmp;
    for(int i=0; i<_childs.size(); i++){
        tmp = &_childs[i];
        score = tmp->upperconfidentbouce(this->_visits);
        if (max < score){ max = score; best = tmp; }
    }
    return best;
}

double Node::upperconfidentbouce(int visits){
    return _wins/_visits + 0.5*sqrt(visits)/_visits;
}

Node Node::visitsselect(){
    double max(-1), score;
    Node *best, *tmp;
    for(int i=0; i<_childs.size(); i++){
        tmp = &_childs[i];
        score = tmp->_visits;
        if (max < score){ max = score; best = tmp; }
    }
    return *best;
}