#include <iostream>
#include <vector>
#include <string>
#include <sstream>
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

struct Product {
    string name;
    string group;
    int cost;
};

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
            product->name = std::string(reinterpret_cast<const char*>(
                                                sqlite3_column_text(stmt, 1)
                                        ));
            product->group = std::string(reinterpret_cast<const char*>(
                                                sqlite3_column_text(stmt, 3)
                                        ));

            std::stringstream strValue;
            strValue << sqlite3_column_text(stmt, 2);

            int intValue;
            strValue >> intValue;

            product->cost = intValue;
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
//    cout << "name: " << product->name << " -- group: " << product->group << " -- cost: " << product-
    printElement(product->name, nameWidth);
    printElement(product->group, nameWidth);
    printElement(product->cost, numWidth);
    cout << endl;
}

void print_products(const vector<Product*> &products) {
    cout << "WELCOME TO OUR MINIMARKET:" << endl;
    cout << "THIS IS OUR LIST OF PRODUCTS:" << endl;

    printElement("NAME:", nameWidth);
    printElement("GROUP:", nameWidth);
    printElement("COST:", numWidth);
    cout << endl;

    for(auto product:products) {
        print_product(product);
    }
    cout << endl;
}

class Node
{
public:
    int key;
    Node* left, *right;
};

/* Helper function that allocates  
a new Node with the given key and  
    nullptr left and right pointers. */
Node* newNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = node->right = nullptr;
    return (node);
}

// A utility function to right  
// rotate subtree rooted with y  
// See the diagram given above.  
Node* rightRotate(Node* x)
{
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// A utility function to left  
// rotate subtree rooted with x  
// See the diagram given above.  
Node* leftRotate(Node* x)
{
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// This function brings the key at 
// root if key is present in tree.  
// If key is not present, then it 
// brings the last accessed item at  
// root. This function modifies the 
// tree and returns the new root  
Node* splay(Node* root, int key)
{
    // Base cases: root is nullptr or 
    // key is present at root  
    if (root == nullptr || root->key == key)
        return root;

    // Key lies in left subtree  
    if (root->key > key)
    {
        // Key is not in tree, we are done  
        if (root->left == nullptr) return root;

        // Zig-Zig (Left Left)  
        if (root->left->key > key)
        {
            // First recursively bring the 
            // key as root of left-left  
            root->left->left = splay(root->left->left, key);

            // Do first rotation for root,  
            // second rotation is done after else  
            root = rightRotate(root);
        }
        else if (root->left->key < key) // Zig-Zag (Left Right)  
        {
            // First recursively bring 
            // the key as root of left-right  
            root->left->right = splay(root->left->right, key);

            // Do first rotation for root->left  
            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }

        // Do second rotation for root  
        return (root->left == nullptr)? root: rightRotate(root);
    }
    else // Key lies in right subtree  
    {
        // Key is not in tree, we are done  
        if (root->right == nullptr) return root;

        // Zag-Zig (Right Left)  
        if (root->right->key > key)
        {
            // Bring the key as root of right-left  
            root->right->left = splay(root->right->left, key);

            // Do first rotation for root->right  
            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }
        else if (root->right->key < key)// Zag-Zag (Right Right)  
        {
            // Bring the key as root of  
            // right-right and do first rotation  
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        // Do second rotation for root  
        return (root->right == nullptr)? root: leftRotate(root);
    }
}

// The search function for Splay tree.  
// Note that this function returns the  
// new root of Splay Tree. If key is  
// present in tree then, it is moved to root.  
Node* search(Node* root, int key)
{
    return splay(root, key);
}

// A utility function to print  
// preorder traversal of the tree.  
// The function also prints height of every Node  
void preOrder(Node* root)
{
    if (root != nullptr)
    {
        cout<<root->key<<" ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main() {
    vector<Product*> products;
    products = collect_data();
    print_products(products);
    return 0;
}