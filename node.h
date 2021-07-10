#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include "connect.h"

class Node{
    public:
        Node();
        Node(int _playerjustmoved, std::vector<int> untriedmoves);
        Node(int _playerjustmoved, int move, Node *parent, std::vector<int> untriedmoved);
        static int mcts(Connect &rootenv, int itermax);
        std::string to_string();
        std::string to_stringtree(int indent);
        Node *uctselect();
        double upperconfidentbouce(int visits);
        Node visitsselect();

    private:
        int _move;
        Node *_parent;
        std::vector<Node> _childs;
        double _wins;
        int _visits;
        std::vector<int> _untriedmoves;
        int _playerjustplayed;
        std::string indentstring(int indent);
        void removeuntriedmove(int move);
        void removemove(int col);
        std::string to_stringmoves();
};

#endif