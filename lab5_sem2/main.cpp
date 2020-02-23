// C program to demonstrate delete operation in binary search tree
#include<cstdio>
#include<cstdlib>
#include <string>
#include <windows.h>
#include <iostream>
#include <vector>
#include <iomanip>

#include "sqlite3.h"

using std::string;
using std::cout;
using std::cin;
using std::vector;
using std::endl;

const char separator = ' ';
const int nameWidth = 35;
const int numWidth = 5;

const int MAX_N = 100;

struct Product {
    int id;
    string name;
    string group;
    int cost;
};

inline bool operator< (const Product& lhs, const Product& rhs){ return lhs.name < rhs.name; }
inline bool operator> (const Product& lhs, const Product& rhs){ return rhs < lhs; }
inline bool operator<=(const Product& lhs, const Product& rhs){ return !(lhs > rhs); }
inline bool operator>=(const Product& lhs, const Product& rhs){ return !(lhs < rhs); }

vector<Product*> collect_data() {
    sqlite3 *db;
    sqlite3_stmt * stmt;

    vector<Product*> products;

    if (sqlite3_open("../../data.db", &db) == SQLITE_OK)
    {
        sqlite3_prepare( db, "SELECT * from product;", -1, &stmt, nullptr );//preparing the statement
        sqlite3_step( stmt );
        while( sqlite3_column_text( stmt, 0 ) )
        {
            auto product = new Product();
            std::stringstream strValueID;
            strValueID << sqlite3_column_text(stmt, 0);

            int intValueID;
            strValueID >> intValueID;

            product->id = intValueID;

            product->name = std::string(reinterpret_cast<const char*>(
                                                sqlite3_column_text(stmt, 1)
                                        ));
            product->group = std::string(reinterpret_cast<const char*>(
                                                 sqlite3_column_text(stmt, 3)
                                         ));

            std::stringstream strValueCost;
            strValueCost << sqlite3_column_text(stmt, 2);

            int intValueCost;
            strValueCost >> intValueCost;

            product->cost = intValueCost;
            products.push_back(product);
            sqlite3_step( stmt );
        }
    }
    else
    {
        cout << "Failed to open db\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return products;
}

template<typename T> void printElement(T t, const int& width) {
    cout << std::left << std::setw(width) << std::setfill(separator) << t;
}

void print_product(Product* product) {
    printElement(product->id, numWidth);
    printElement(product->name, nameWidth);
    printElement(product->group, nameWidth);
    printElement(product->cost, numWidth);
    cout << endl;
}

void print_products(const vector<Product*> &products) {
    cout << "WELCOME TO OUR MINIMARKET:" << endl;
    cout << "THIS IS OUR LIST OF PRODUCTS:" << endl;

    printElement("ID:", numWidth);
    printElement("NAME:", nameWidth);
    printElement("GROUP:", nameWidth);
    printElement("COST:", numWidth);
    cout << endl;

    int length = products.size();
    for(int i = 0; i < length; i++) {
        print_product(products[i]);
    }
    cout << endl;
}

struct Node {
    Product key;
    struct Node *left, *right;
};

struct Node *newNode(Product item) {
    struct Node *temp =  (struct Node *)malloc(sizeof(struct Node));
    temp->key = item;
    temp->left = temp->right = nullptr;
    return temp;
}

struct Node* insert(struct Node* Node, Product key) {
    if (Node == nullptr) return newNode(key);

    if (key < Node->key)
        Node->left  = insert(Node->left, key);
    else
        Node->right = insert(Node->right, key);

    return Node;
}

struct Node* insertPersistent(struct Node* root, Product key) {
    if (root == nullptr) return newNode(key);

    Node* new_root = newNode(root->key);
    if (key < root->key) {
        new_root->right = root->right;
        new_root->left = insertPersistent(root->left, key);
    }
    else {
        new_root->left = root->left;
        new_root->right = insertPersistent(root->right, key);
    }

    return new_root;
}



/* Given a non-empty binary search tree, return the Node with minimum
   key value found in that tree. */
struct Node * minValueNode(struct Node* Node) {
    struct Node* current = Node;

    while (current && current->left != nullptr)
        current = current->left;

    return current;
}

struct Node* deleteNode(struct Node* root, Product key) {
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

struct Node* deleteNodePersistent(struct Node* root, Product key) {
    if (root == nullptr) return root;

    Node* new_root = newNode(root->key);
    if (key < root->key) {
        new_root->right = root->right;
        new_root->left = deleteNodePersistent(root->left, key);
    }
    else if (key > root->key) {
        new_root->left = root->left;
        new_root->right = deleteNodePersistent(root->right, key);
    }
    else
    {
        if (root->left == nullptr) {
            struct Node *temp = root->right;
            free(new_root);
            return temp;
        }
        else if (root->right == nullptr) {
            struct Node *temp = root->left;
            free(new_root);
            return temp;
        }

        struct Node* temp = minValueNode(root->right);

        new_root->key = temp->key;

        new_root->right = deleteNodePersistent(root->right, temp->key);
        new_root->left = root->left;
    }
    return new_root;
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
    os << root->key.name << '\n';

    if (root->left) {
        printTree(os, root->left, indent, false);
    }
    if (root->right) {
        printTree(os, root->right, indent, true);
    }
}


int main() {

    vector<Product*> products = collect_data();
    print_products(products);

    vector<Node*> roots;

    struct Node *root = nullptr;
    for(int i = 0; i < 10; i++)
        insert(root, *products[i]);

//    printf("Inorder traversal of the given tree \n");
//    printTree(cout, root);
//
//    struct Node* new_root;
//    new_root = insertPersistent(root, 100);
//
//    printf("\nVersion 1 \n");
//    printTree(cout, root);
//
//    printf("\nVersion 2 \n");
//    printTree(cout, new_root);
//
//    printf("\nDelete 20\n");
//    new_root = deleteNodePersistent(root, 20);
//
//    printf("\nVersion 1 \n");
//    printTree(cout, root);
//
//    printf("\nVersion 2 \n");
//    printTree(cout, new_root);
//
//    printf("\nDelete 30\n");
//    new_root = deleteNodePersistent(root, 30);
//
//    printf("\nVersion 1 \n");
//    printTree(cout, root);
//
//    printf("\nVersion 2 \n");
//    printTree(cout, new_root);
//
//    printf("\nDelete 50\n");
//    new_root = deleteNodePersistent(root, 50);
//
//    printf("\nVersion 1 \n");
//    printTree(cout, root);
//
//    printf("\nVersion 2 \n");
//    printTree(cout, new_root);


    return 0;
}