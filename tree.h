#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <exception>
#include <cmath>
#include <iomanip>

using namespace std;

#define EMPTY 0.0
#define ADD   1.0
#define SUB   2.0
#define MUL   3.0
#define DIV   4.0
#define POW   5.0
#define X 'x'

#define SIN 11.0
#define COS 12.0
#define TAN 13.0
#define CTG 14.0
#define LOG 15.0

#define PI 3,1415926535

enum class ValueType {
    _number,
    _operation,
    _unknown,
    _function,
    _empty
};

class Node {
public:
    explicit Node();
    explicit Node(Node *tree);
    explicit Node(ValueType type, double value);
    explicit Node(ValueType type, double value, Node *left_, Node *right_, bool in);
    ~Node();
private:
    double val;
    ValueType Type;
    Node * left;
    Node * right;
    bool in_word;
    int height;

    friend Node* Read_str_to_tree(istringstream& in, int t, int notation);
    friend void tree_print (Node * tree);

    friend Node* MakeUnarMinus (istringstream &in);
    friend Node* MakeSNode (istringstream &in);
    friend Node* MakeOperNode (istringstream &in, double operation, Node * dig, const int &t);
    friend void tree_destroy (Node * tree);
    friend int tree_height(Node * tree);
    friend bool SearchFunction (istringstream &in, double &func);
    friend Node* MakeFuncNode(istringstream &in, const double& func);
    friend Node* Diff(const Node * tree);
    friend double TreeCount (Node* node, const double& value, int notat);
    friend Node* CopyNode(const Node* node);
    friend void CountVal(Node *node, const double &value, int notat);
};
Node * Read_str_to_tree(istringstream& in, int t, int notation = 10);
void tree_print (Node * tree);
char OperationFind(const double& val);
bool my_isdigit(istringstream &str, double &digit);
Node* MakeUnarMinus (istringstream &in);
Node* MakeSNode (istringstream &in);
Node* MakeOperNode (istringstream &in, double operation, Node * dig, const int &t);
void tree_destroy (Node * tree);
int tree_height(Node * tree);
bool SearchFunction (istringstream &in, double &func);
Node* MakeFuncNode(istringstream &in, const double& func);
char* Makestr(const char& c);
double TreeCount (Node* node, const double& value, int notat = 10);
Node* Diff(const Node * tree);
Node* CopyNode(const Node* node);
void CountVal(Node *node, const double &value, int notat = 10);