#include <iostream>
using namespace std;
//The below code represents a node in a Binary Tree.
struct Node {
    int value;  // The value stored in the node.
    Node* left; //Pointer to the left child node.
    Node* right; // Pointer to the right child node.
    //Constructor for Node.
    //value to assign to the node.
    Node(int val) {
        value = val;
        left = nullptr;
        right = nullptr;
    }
};
//Calculates the height of a binary tree.
int Height(Node* root) {
    if (!root) 
    {
        return -1; // if tree is empty it return -1.
    }
    else
    {
        return max(Height(root->left), Height(root->right)) + 1; // it returns maximum height of left and right subtrees.
    }
}

int main() {
    Node* root = new Node(10);
    root->left = new Node(15);
    root->right = new Node(10); // Creating root node with value 10.
    cout << "Height: " << Height(root) << endl; // Output the height of the given tree.
}