#include "tree.h"

using namespace std;
int Notation = 10;

Node::Node() {
    val = EMPTY;
    Type = ValueType ::_empty;
    left = nullptr;
    right = nullptr;
    height = - 1;
    in_word = false;
}

Node::Node(ValueType type, double value) {
    val = value;
    Type = type;
    left = nullptr;
    right = nullptr;
    height = -1;
    in_word = false;
}
Node::Node(ValueType type, double value, Node *left_, Node *right_, bool in) {
    val = value;
    Type = type;
    left = left_;
    right = right_;
    in_word = in;
}
Node::~Node() {
    tree_destroy(this);
}
Node* CopyNode(const Node* node) {
    Node *copy_node = new Node (node->Type, node->val);
    copy_node->in_word = node->in_word;
    if (node->left == nullptr)
        copy_node->left = nullptr;
    else copy_node->left = CopyNode(node->left);

    if (node->right == nullptr)
        copy_node->right = nullptr;
    else copy_node->right = CopyNode(node->right);

    return copy_node;
}
char OperationFind(const double& val) {
    switch (int(val)) {
        case 1 : {
            return '+';
        }
        case 2: {
            return '-';
        }
        case 3 : {
            return '*';
        }
        case 4 : {
            return '/';
        }
        case 5 : {
            return '^';
        }
        default:
            return '$';
    }
}
string FunctionFind (const double& func) {
    switch (int(func)) {
        case 11 : return "sin";
        case 12 : return "cos";
        case 13 : return "tan";
        case 14 : return "ctg";
        case 15 : return "log";
        default:  return "_no";
    }
}
void tree_print ( Node * tree) {
    if (tree == nullptr)
        return;
    if (tree -> left == nullptr && tree -> right == nullptr) {
        if(tree->Type == ValueType::_unknown)
            cout<<"x";
        else if (tree->Type == ValueType::_operation)
            cout <<" "<< OperationFind(tree -> val)<<" ";
        else if (tree->Type == ValueType:: _number)
            cout << tree->val;
        else if (tree->Type == ValueType::_function)
            cout<<FunctionFind(tree->val);
        return;
    }
    if (tree -> left != nullptr) {
        if (tree->left->in_word == true) {
            cout << "(";
            tree_print(tree->left);
            cout << ")";
        } else tree_print(tree->left);
    }

    if (tree->Type == ValueType::_operation)
        cout <<" "<< OperationFind(tree -> val)<<" ";
    else if (tree->Type == ValueType:: _number)
        cout << tree->val;
    else if(tree->Type == ValueType::_unknown)
        cout<<"x";
    else if (tree->Type == ValueType::_function)
        cout<<FunctionFind(tree->val);
    if (tree -> right != nullptr) {
        if (tree->right->in_word == true) {
            cout << "(";
            tree_print(tree->right);
            cout << ")";
        } else tree_print(tree->right);
    }
}

void tree_destroy (Node * tree) {
    if (tree == nullptr)
        return;
    if (tree -> left == nullptr && tree -> right == nullptr ) {
        free(tree);
        return;
    }
    if (tree -> left != nullptr)
        tree_destroy(tree -> left);
    if (tree -> right != nullptr)
        tree_destroy(tree -> right);
    free(tree);
}
int tree_height(Node * tree) {
    if (tree == nullptr)
        return 0;
    if (tree -> left == nullptr && tree -> right == nullptr) {
        tree -> height = 1;
        return 1;
    }
    int left_height = tree_height(tree -> left);
    int right_height = tree_height(tree -> right);
    if (left_height >= right_height)
        tree -> height = left_height + 1;
    else
        tree -> height = right_height + 1;
    return tree -> height;
}

double SearchOperation (istringstream& str) {
    char op = str.peek();

    switch (op) {
        case '+': {;
            return ADD;
        }
        case '-': {
            return SUB;
        }
        case '*': {
            return MUL;
        }
        case '/': {
            return DIV;
        }
        case '^': {
            return POW;
        }
        default:
            return EMPTY;
    }
}

bool my_isdigit(istringstream &str, double &digit) {
    char notat ='0' + Notation;
    char c = str.peek();
    digit = 0.0;
    char digit_str[10];
    int i;
    bool k = false;
    if (c == 'P') {
        str.ignore(1);
        str>> c;
        if (c == 'I') {
            digit = M_PI;
            return true;
        }
        else throw invalid_argument("invalid symbol " + string(Makestr(c)));
    }
    if ((c >= '0' && c < notat) && c != EOF) {
        for (i = 0; c != EOF; c = str.peek()) {
            if (c >= '0' && c < notat) {
                digit_str[i++] = c;
                str.ignore(1);
            } else {
                digit_str[i] = '\0';
                digit = atoi(digit_str);
               // digit = (double)strtol(digit_str, NULL, Notation);
                return true;
            }
        }
        digit_str[i] = '\0';
        digit = atoi(digit_str);
        //digit = (double)strtol(digit_str, NULL, Notation);
        return true;
    }
    else
        return false;
}

void SpaceSkipper (istringstream& str) {
    while (str.peek() == ' ')
        str.ignore(1);
}

int OperationPrioretitet (const double& operation) {
    switch (int(operation)) {
        case 1:  return 2;
        case 2:  return 2;
        case 3:  return 1;
        case 4:  return 1;
        case 5:  return 1;
        default: return -1;
    }
}

char* Makestr(const char& c) {
    char* a = new char[2];
    a[0] = c;
    a[1] = '\0';
    return a;
}
Node* MakeUnarMinus (istringstream &in) {
    double operation;
    in.ignore(1);
    SpaceSkipper(in);
    char next_symbol = in.peek();
    if (next_symbol == '(') {
        in.ignore(1);

        Node *nodev = Read_str_to_tree(in,0);
        SpaceSkipper(in);
        next_symbol = in.peek();
        nodev->in_word = true;
        if (next_symbol == ')') {
            in.ignore(1);
            SpaceSkipper(in);
            next_symbol = in.peek();
        }
        SpaceSkipper(in);
        Node *nodeempt = new Node(ValueType::_empty, EMPTY);
        Node *nodemin = new Node(ValueType::_operation, SUB, nodeempt, nodev, false);

        if ((operation = SearchOperation(in)) != EMPTY) {
            return MakeOperNode (in, operation, nodemin, 0);
        }
        else if (next_symbol == ')') {
            return nodemin;
        }
        else if (next_symbol == EOF) {
            in.ignore(1);
            return nodemin;
        }
        else throw invalid_argument("invalid symbol " + string(Makestr(next_symbol)));
    }
    else throw invalid_argument("You must use \"(\" after unar minus, but nest symbol is " + string(Makestr(next_symbol)));
}

Node* MakeSNode (istringstream &in) {
    double operation;
    in.ignore(1);
    SpaceSkipper(in);
    Node *nodeS = Read_str_to_tree(in, 0);
    nodeS->in_word = true;
    char next_symbol = in.peek();
    if (next_symbol == EOF) {
        return nodeS;
    } else if (next_symbol == ')') {
        in.ignore(1);
        SpaceSkipper(in);
        if ((operation = SearchOperation(in)) != EMPTY) {
            if (OperationPrioretitet(operation) == 1)
                return MakeOperNode(in, operation, nodeS, 1);
            else
                return nodeS;
        } else {
            return nodeS;
        }
    } else throw invalid_argument("invalid symbol " + string(Makestr(next_symbol)));

}

Node* MakeOperNode (istringstream &in, double operation, Node * dig, const int &t) {
    if (OperationPrioretitet(operation) == 2) {
        if (t == 0) {
            in.ignore(1);
            Node *node = new Node(ValueType::_operation, operation, dig, Read_str_to_tree(in, 0), false);
            return node;
        }
        else {
            return dig;
        }
    }
    else {
        in.ignore(1);
        SpaceSkipper(in);
        Node *node_op1 = new Node(ValueType::_operation, operation, dig, Read_str_to_tree(in, 1), false);

        char next_symbol = in.peek();
        if (next_symbol == EOF || next_symbol == ')') {
            return node_op1;
        }
        else if ((operation = SearchOperation(in)) != EMPTY) {
            if (t == 0) {
                in.ignore(1);
                Node *node3 = new Node(ValueType::_operation, operation, node_op1, Read_str_to_tree(in, 0), false);
                return node3;
            }
            else return node_op1;
        }
        throw invalid_argument("invalid symbol " + string(Makestr(next_symbol)));
    }
}

bool SearchFunction (istringstream &in, double &func) {
    char f[3];
    in>>f[0]>>f[1]>>f[2];

    if (f[0] == 's' && f[1] == 'i' && f[2] == 'n')
        func = SIN;
    else if (f[0] == 'c' && f[1] == 'o' && f[2] == 's')
        func = COS;
    else if (f[0] == 't' && f[1] == 'a' && f[2] == 'n')
        func = TAN;
    else if (f[0] == 'c' && f[1] == 't' && f[2] == 'g')
        func = CTG;
    else if (f[0] == 'l' && f[1] == 'o' && f[2] == 'g')
        func = LOG;
    if (func != 0) {
        SpaceSkipper(in);
        if (in.peek() == '(') {
            in.ignore(1);
            return true;
        }
        else throw invalid_argument("You mus use \"(\" for function's argument but next symbol is " + string(Makestr(in.peek())));
    }
    in.seekg(3, in.cur);
    return false;
}

Node* MakeFuncNode(istringstream &in, const double& func) {
    SpaceSkipper(in);
    Node *nodeempt = new Node(ValueType::_empty, 0);
    Node *funcnode = new Node(ValueType::_function, func, nodeempt, Read_str_to_tree(in, 0), true);
    funcnode->right->in_word = true;

    char next_symbol = in.peek();
    double operation;
    if(next_symbol == EOF)
        return funcnode;
    else if (next_symbol == ')') {
        in.ignore(1);
        SpaceSkipper(in);
        if ((operation = SearchOperation(in)) != EMPTY) {
            return MakeOperNode(in,operation, funcnode, 0);
        }
        else {
            return funcnode;
        }
    }
    throw invalid_argument("invalid symbol " + string(Makestr(next_symbol)));
}
Node* Read_str_to_tree(istringstream &in, int t, int notation) {
    if (notation != 10) {
        Notation = notation;
    }
    double operation, digit, function = 0;
    SpaceSkipper(in);
    char next_symbol = in.peek();
    if (my_isdigit(in,digit) == true) {
        Node *dig = new Node(ValueType::_number, digit);
        SpaceSkipper(in);
        next_symbol = in.peek();
        if ((operation = SearchOperation(in)) != EMPTY) {
            Node *nodeon = MakeOperNode(in, operation, dig, t);
            return nodeon;
        }
        else if (next_symbol == ')') {
            return dig;
        }
        else if (next_symbol == EOF) {
            in.ignore(1);
            return dig;
        }
        else
            throw invalid_argument("invalid symbol " + string(Makestr(next_symbol)));

    }
    else if (next_symbol == '(') {
        Node *nodesn = MakeSNode(in);
        return nodesn;
    }
    else if (next_symbol == '-') {
        Node *nodeunar = MakeUnarMinus(in);
        return nodeunar;
    }
    else if(next_symbol == X) {
        Node *nodex = new Node(ValueType::_unknown, 0);
        in.ignore(1);
        SpaceSkipper(in);
        next_symbol = in.peek();
        if ((operation = SearchOperation(in)) != EMPTY) {
            Node *nodeon = MakeOperNode(in, operation, nodex, t);
            return nodeon;
        }
        else if (next_symbol == ')') {
            return nodex;
        }
        else if (next_symbol == EOF) {
            in.ignore(1);
            return nodex;
        }
    }
    else if (SearchFunction(in, function)) {
        Node* nodefunc = new Node;
        nodefunc = MakeFuncNode(in, function);
        return nodefunc;
    }
    else {
        throw invalid_argument("invalid symbol " + string(Makestr(next_symbol)));
    }

    return nullptr;
}
void CountVal(Node *node, const double &value, int notat) {
    double ans = TreeCount(node, value, notat);
    if (abs(ans) < 0.000001)
        cout << 0;
    if (notat == 8)
        cout<<oct<<ans;
    else
        cout<<ans;
}
double TreeCount (Node* node, const double& value, int notat) {
    switch (node->Type) {
        case (ValueType::_operation) : {

            switch (int(node->val)) {
                case (int(ADD)) :
                    return TreeCount(node->left, value) + TreeCount(node->right, value);
                    break;
                case (int(SUB)) :
                    return TreeCount(node->left, value) - TreeCount(node->right, value);
                    break;
                case (int(MUL)) :
                    return TreeCount(node->left, value) * TreeCount(node->right, value);
                    break;
                case (int(DIV)) :
                    return TreeCount(node->left, value) / TreeCount(node->right, value);
                    break;
                case (int(POW)) :
                    return pow(TreeCount(node->left, value), TreeCount(node->right, value));
                    break;
            }
        }
        case (ValueType::_number) : { return node->val; break; }
        case (ValueType::_empty)  : { return 0; break;}
        case (ValueType::_function) : {
            switch(int(node->val)) {
                case (int(SIN)) : return sin(TreeCount(node->right, value));
                    break;
                case (int(COS)) : return cos(TreeCount(node->right, value));
                    break;
                case (int(TAN)) : return tan(TreeCount(node->right, value));
                    break;
                case (int(CTG)) : return 1 / (tan(TreeCount(node->right, value)));
                    break;
                case (int(LOG)) : return log(TreeCount(node->right, value));
                    break;
            }
        }
            break;
        case (ValueType::_unknown) : return value;
    }
    return value;
}
Node* Diff (const Node *tree) {
    switch (tree->Type) {
        case (ValueType::_operation): {
            switch(int(tree->val)) {
                case (int(ADD)) : {
                    Node *diff_tree = new Node(ValueType::_operation, ADD, Diff(tree->left), Diff(tree->right),tree->in_word);
                    return diff_tree;
                }
                case (int(SUB)) : {
                    Node *diff_tree = new Node(ValueType::_operation, SUB);
                    diff_tree->in_word = tree->in_word;
                    if (tree->left->Type != ValueType::_empty)
                        diff_tree->left = Diff(tree->left);
                    else
                        diff_tree->left = new Node(ValueType::_empty, EMPTY);
                    diff_tree->right = Diff(tree->right);
                    return diff_tree;
                }
                case (int(MUL)) : {
                    Node *left_diff = new Node(ValueType::_operation, MUL,Diff(tree->left),CopyNode(tree->right), false);
                    Node *right_diff = new Node(ValueType::_operation, MUL,CopyNode(tree->left),Diff(tree->right), false);
                    Node *diff_tree = new Node(ValueType::_operation, ADD, left_diff, right_diff, true);

                    return diff_tree;
                }
                case (int(DIV)) : {
                    Node *l = new Node(ValueType::_operation, MUL, Diff(tree->left), CopyNode(tree->right), false);
                    Node *r = new Node(ValueType::_operation, MUL, CopyNode(tree->left), Diff(tree->right), false);
                    Node *left_diff = new Node(ValueType::_operation, SUB, l,r, true);

                    Node *node2 = new Node(ValueType::_number, 2);
                    node2->in_word = true;
                    Node *right_diff = new Node(ValueType::_operation, POW ,CopyNode(tree->right) , node2, true);
                    right_diff->left->in_word = true;
                    Node *diff_tree = new Node(ValueType::_operation, DIV,left_diff,right_diff, false);

                    return diff_tree;
                }
                case (int(POW)) : {

                    Node *node1 = new Node(ValueType::_number,1);
                    Node *rlr = new Node(ValueType::_operation, SUB, CopyNode(tree->right), node1, true);

                    Node *nodex = new Node(ValueType::_unknown, 0);
                    Node *rl = new Node(ValueType::_operation, POW, nodex, rlr, true);

                    Node *right_diff = new Node(ValueType::_operation, MUL, rl, Diff(tree->left), false);

                    Node *diff_tree = new Node(ValueType::_operation, MUL,CopyNode(tree->right), right_diff, false);

                    return diff_tree;
                }
            }
        }
        case (ValueType::_unknown) : {
            Node *diff_tree = new Node(ValueType::_number, 1);
            return diff_tree;
        }
        case (ValueType::_number) : {
            Node *diff_tree = new Node(ValueType::_number, 0);
            return diff_tree;
        }
        case (ValueType ::_empty) : { throw  runtime_error("Diff() cant diff type: empty"); }

        case (ValueType::_function) : {
            switch (int(tree->val)) {
                case (int(SIN)) : {
                    Node *empty_node = new Node(ValueType::_empty, EMPTY);

                    Node *left_diff = new Node(ValueType::_function, COS, empty_node, tree->right, false);

                    Node *diff_tree = new Node(ValueType::_operation, MUL, left_diff, Diff(tree->right), false);

                    return diff_tree;
                }
                case (int(COS)) : {
                    Node *empty_node = new Node(ValueType::_empty, EMPTY);
                    Node *empty_node1 = new Node(ValueType::_empty, EMPTY);

                    Node *left_diff = new Node(ValueType::_function, SIN, empty_node, CopyNode(tree->right), false);

                    Node *diff_tree = new Node(ValueType::_operation, MUL, left_diff, Diff(tree->right), true);

                    Node *nodeu = new Node(ValueType ::_operation, SUB, empty_node1, diff_tree,true);

                    return nodeu;
                }
                case (int(TAN)) : {
                    Node *node2 = new Node(ValueType::_number, 2);
                    Node *empty_node = new Node(ValueType::_empty, EMPTY);

                    Node *nodecos = new Node(ValueType::_function, COS, empty_node, CopyNode(tree->right), false);


                    Node *right_diff = new Node(ValueType::_operation, POW, nodecos, node2, true);

                    Node *diff_tree = new Node(ValueType::_operation, DIV, Diff(tree->right), right_diff, false);
                    diff_tree->left->in_word = true;

                    return diff_tree;
                }
                case (int(CTG)) : {
                    Node *node2 = new Node(ValueType::_number, 2);
                    Node *empty_node = new Node(ValueType::_empty, EMPTY);

                    Node *nodesin = new Node(ValueType::_function, SIN, empty_node, CopyNode(tree->right), false);

                    Node *right_diff = new Node(ValueType::_operation, POW, nodesin, node2, true);

                    Node *left_diff = new Node(ValueType::_operation, SUB, CopyNode(empty_node), Diff(tree->right), false);

                    Node *diff_tree = new Node(ValueType::_operation, DIV, left_diff, right_diff, false);

                    return diff_tree;
                }
                case (int(LOG)) : {
                    Node *diff_tree = new Node(ValueType::_operation, DIV, Diff(tree->right), CopyNode(tree->right), tree->in_word);
                    diff_tree->left->in_word = true;
                    return diff_tree;
                }
            }
        }
    }
    return nullptr;
}