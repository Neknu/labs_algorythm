// C program to demonstrate delete operation in binary search tree
#include<cstdio>
#include<cstdlib>
#include <string>
#include <windows.h>
#include <conio.h>
#include <iostream>

using std::string;
using std::cout;

struct Node {
    int key;
    struct Node *left, *right;
};

struct Node *newNode(int item) {
    struct Node *temp =  (struct Node *)malloc(sizeof(struct Node));
    temp->key = item;
    temp->left = temp->right = nullptr;
    return temp;
}

void inorder(struct Node *root) {
    if (root != nullptr) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

struct Node* insert(struct Node* Node, int key) {
    if (Node == nullptr) return newNode(key);

    if (key < Node->key)
        Node->left  = insert(Node->left, key);
    else
        Node->right = insert(Node->right, key);

    return Node;
}

/* Given a non-empty binary search tree, return the Node with minimum
   key value found in that tree. */
struct Node * minValueNode(struct Node* Node) {
    struct Node* current = Node;

    while (current && current->left != nullptr)
        current = current->left;

    return current;
}

struct Node* deleteNode(struct Node* root, int key) {
    if (root == nullptr) return root;

    // If the key to be deleted is smaller than the root's key,
    // then it lies in left subtree
    if (key < root->key)
        root->left = deleteNode(root->left, key);

        // If the key to be deleted is greater than the root's key,
        // then it lies in right subtree
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

        // if key is same as root's key, then This is the Node
        // to be deleted
    else
    {
        // Node with only one child or no child
        if (root->left == nullptr) {
            struct Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == nullptr) {
            struct Node *temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest
        // in the right subtree)
        struct Node* temp = minValueNode(root->right);

        // Copy the inorder successor's content to this Node
        root->key = temp->key;

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

template<typename OStream>
void printTree(OStream& os, const Node* root, std::string indent = "", bool last = true) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    os << indent;
    if (last) {
        os << "R----";
        indent += "     ";
    }
    else {
        os << "L----";
        indent += "|    ";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    os << root->key << '\n';

    if (root->left) {
        printTree(os, root->left, indent, false);
    }
    if (root->right) {
        printTree(os, root->right, indent, true);
    }
}

// Driver Program to test above functions
int main() {
    /* Let us create following BST
              50
           /     \
          30      70
         /  \    /  \
       20   40  60   80 */
    struct Node *root = nullptr;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Inorder traversal of the given tree \n");
    printTree(cout, root);

    printf("\nDelete 20\n");
    root = deleteNode(root, 20);
    printf("Inorder traversal of the modified tree \n");
    printTree(cout, root);

    printf("\nDelete 30\n");
    root = deleteNode(root, 30);
    printf("Inorder traversal of the modified tree \n");
    printTree(cout, root);

    printf("\nDelete 50\n");
    root = deleteNode(root, 50);
    printf("Inorder traversal of the modified tree \n");
    printTree(cout, root);

    return 0;
}