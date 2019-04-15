#include <iostream> // IO is used for debugging and testing.
#include <cstdlib>  // rand() is for generating a random data set to fill the tree.
#include <fstream>  // For saving tree to file for visualiztion.

using namespace std;

// Defines to control debugging behavior.
#define __PRINT_NULL_NODE__ 0 
#define __PRINT_ORDERED_LIST__ 0
#define __CREATE_DATA_FILE__ 1
#define __TREE_SIZE__ 50
#define __RANDOM_CLAMP__ 500

// Defines to control properties of graph.
#define __ALLOW_DUPLICATE_KEYS__ 1

struct node{
    node(int val) : left(nullptr), right(nullptr) {this->data=val;}
    int data;
    struct node* left;
    struct node* right;

    void addNode(node* N){
        if(nullptr == this->right && N->data > this->data){
            this->right = N;
        }else if(nullptr != this->right && N->data > this->data){
            this->right->addNode(N);
        }else if(nullptr == this->left && N->data < this->data){
            this->left = N;
        }else if(nullptr != this->left && N->data < this->data){
            this->left->addNode(N);
        }else if(N->data == this->data && __ALLOW_DUPLICATE_KEYS__){
            N->left = this->left;
            this->left = N;
        }
    }

    int visit(){
        if(nullptr != this->left){
            this->left->visit();
        }
        // This line will print an ordered list of the data in the tree.
        if(__PRINT_ORDERED_LIST__){
            cout << this->data << endl;
        }
        if(nullptr != this->right){
            this->right->visit();
        }
        return this->data;
    }

    node* search(int val){
        node* result = nullptr;
        if(val == this->data){
            result = this;
        }else if(nullptr != this->left && val < this->data){
            result = this->left->search(val);
        }else if(nullptr != this->right && val > this->data){
            result = this->right->search(val);
        }
        return result;
    }

    void visualize(ofstream* dotfile){
        static int nullcount = 0;

        if(nullptr != this->left){
            *dotfile << "\t" << this->data << " -> " << this->left->data << endl;
            this->left->visualize(dotfile);
        }else{
            print_null_node(nullcount++, dotfile);
        }

        if(nullptr != this->right){
            *dotfile << "\t" << this->data << " -> " << this->right->data << endl;
            this->right->visualize(dotfile);
        }else{
            print_null_node(nullcount++, dotfile);
        }
    }

    void print_null_node(int nullcount, ofstream* dotfile){
        // This code will print the leaf nodes if you want to see them.
        if(__PRINT_NULL_NODE__){
            *dotfile << "\tnull" << nullcount << "[shape=point];" << endl;
            *dotfile << "\t" << this->data << " -> null" << nullcount << endl;
        }
    }
};

struct tree{
    tree() : root(nullptr) {}
    node* root;

    void addValue(int val){
        node* N = new node(val);
        if(nullptr == this->root){
            this->root = N;
        }else{
            this->root->addNode(N);
        }
    }

    void traverse(){
        if(nullptr == this->root){
            cout << "Null" << endl;
        }
        else{
            this->root->visit();
        }
    }

    node* search(int val){
        if(nullptr != this->root){
            return this->root->search(val);
        }else {
            return nullptr;
        }
    }

    // Tree creates a text file for use in graphiz for visualization.
    void visualize(ofstream* dotfile){
        if(dotfile->is_open()){
            cout << "Ready to edit file." << endl;
            *dotfile << "digraph BST {" << endl;
            *dotfile << "\tnode [fontname=\"Arial\"];" << endl;

            if(nullptr == this->root){
                *dotfile << endl;
            }else {
                this->root->visualize(dotfile);
            }
            *dotfile << "}" << endl;
        }else{
            cout << "Bad file." << endl;
        }
    }
};

// Toy program to test data structure.
int main(){

    tree data;

    for( int i = 0; i < __TREE_SIZE__; i++){
        int newValue = (rand() % __RANDOM_CLAMP__)+1; 
        data.addValue(newValue);
    }

    data.traverse();
    node* result = data.search(12);
    if(nullptr != result){
        cout << "Found: " << result->data << endl;
    }

    if(__CREATE_DATA_FILE__){
        ofstream dotfile;
        dotfile.open("TestOutput/binaryTree.dot");
        data.visualize(&dotfile);
        dotfile.close();

        //Call the system to generate the png file with grapviz dot and display with eog.
        system("dot -Tpng TestOutput/binaryTree.dot -o TestOutput/binaryTree.png && eog TestOutput/binaryTree.png");
    }
    return 0;
}
