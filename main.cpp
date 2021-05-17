#include "tree.h"


int main() {
    try {
        int notation_ = 10;
        istringstream str("(x^7)/2");
        Node *expression = Read_str_to_tree(str, 0, notation_);
        tree_print(expression);
        cout <<endl;

        CountVal(expression, 2, notation_);
        cout<<endl;

        Node *derivative = Diff(expression);
        tree_print(derivative);
        cout <<endl;
        CountVal(derivative, 1, notation_);

    } catch (exception &e) {
        cerr << e.what()<<endl;
        exit(1);
    }


    return 0;
}


