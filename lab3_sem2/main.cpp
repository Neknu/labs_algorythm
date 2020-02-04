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

struct Node {
    int key;
    Node* left, *right;
};

Node* newNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = node->right = nullptr;
    return (node);
}

Node* rightRotate(Node* x)
{
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

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
// root.
Node* splay(Node* root, int key)
{
    if (root == nullptr || root->key == key)
        return root;

    if (root->key > key)
    {
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

// this function returns the
// new root of Splay Tree. If key is  
// present in tree then, it is moved to root.  
Node* search(Node* root, int key)
{
    return splay(root, key);
}


Node* insert(Node* root, int k)
{
    if (root == nullptr) return newNode(k);

    // Bring the closest leaf node to root  
    root = splay(root, k);

    if (root->key == k) return root;

    Node* newnode = newNode(k);

    // If root's key is greater, make  
    // root as right child of newnode  
    // and copy the left child of root to newnode  
    if (root->key > k)
    {
        newnode->right = root;
        newnode->left = root->left;
        root->left = nullptr;
    }

        // If root's key is smaller, make  
        // root as left child of newnode  
        // and copy the right child of root to newnode  
    else
    {
        newnode->left = root;
        newnode->right = root->right;
        root->right = nullptr;
    }

    return newnode;
}

struct Node* delete_key(struct Node* root, int key)
{
    struct Node* temp;
    if (!root)
        return nullptr;

    root = splay(root, key);

    if (key != root->key)
        return root;

    if (!root->left)
    {
        temp = root;
        root = root->right;
    }
    else
    {
        temp = root;

        /*Note: Since key == root->key,
        so after Splay(key, root->lchild),
        the tree we get will have no right child tree
        and maximum node in left subtree will get splayed*/
        // New root
        root = splay(root->left, key);

        root->right = temp->right;
    }

    free(temp);

    return root;

}

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

//    Node* root = newNode(100);
//    root->left = newNode(50);
//    root->right = newNode(200);
//    root->left->left = newNode(40);
//    root->left->left->left = newNode(30);
//    root->left->left->left->left = newNode(20);
//
////    root = search(root, 20);
//    root = insert(root, 25);
//    cout << "Preorder traversal of the modified Splay tree is \n";
//    preOrder(root);

    struct Node* root = newNode(6);
    root->left = newNode(1);
    root->right = newNode(9);
    root->left->right = newNode(4);
    root->left->right->left = newNode(2);
    root->right->left = newNode(7);

    int key = 4;

    root = delete_key(root, key);
    printf("Preorder traversal of the modified Splay tree is \n");
    preOrder(root);

    return 0;
}