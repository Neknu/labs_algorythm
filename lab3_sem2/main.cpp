#include <iostream>
#include <vector>
#include <string>
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
    int id;
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
    Product* key;
    Node* left, *right;
};

Node* newNode(Product* key)
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
Node* splay(Node* root, string name)
{
    if (root == nullptr || root->key->name == name)
        return root;

    if (root->key->name > name)
    {
        if (root->left == nullptr) return root;

        // Zig-Zig (Left Left)  
        if (root->left->key->name > name)
        {
            // First recursively bring the 
            // key as root of left-left  
            root->left->left = splay(root->left->left, name);

            // Do first rotation for root,  
            // second rotation is done after else  
            root = rightRotate(root);
        }
        else if (root->left->key->name < name) // Zig-Zag (Left Right)
        {
            // First recursively bring 
            // the key as root of left-right  
            root->left->right = splay(root->left->right, name);

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
        if (root->right->key->name > name)
        {
            // Bring the key as root of right-left  
            root->right->left = splay(root->right->left, name);

            // Do first rotation for root->right  
            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }
        else if (root->right->key->name < name)// Zag-Zag (Right Right)
        {
            // Bring the key as root of  
            // right-right and do first rotation  
            root->right->right = splay(root->right->right, name);
            root = leftRotate(root);
        }

        // Do second rotation for root  
        return (root->right == nullptr)? root: leftRotate(root);
    }
}

// this function returns the
// new root of Splay Tree. If key is  
// present in tree then, it is moved to root.  
Node* search(Node* root, string name)
{
    return splay(root, name);
}


Node* insert(Node* root, Product* k)
{
    if (root == nullptr) return newNode(k);

    // Bring the closest leaf node to root  
    root = splay(root, k->name);

    if (root->key->name == k->name) return root;

    Node* newnode = newNode(k);

    // If root's key is greater, make  
    // root as right child of newnode  
    // and copy the left child of root to newnode  
    if (root->key->name > k->name)
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

struct Node* delete_key(struct Node* root, Product* key)
{
    struct Node* temp;
    if (!root)
        return nullptr;

    root = splay(root, key->name);

    if (key->name != root->key->name)
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
        root = splay(root->left, key->name);

        root->right = temp->right;
    }

    free(temp);

    return root;

}

void preOrder(Node* root)
{
    if (root != nullptr)
    {
        print_product(root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

void print_menu() {
    cout << endl;
    cout << "SELECT YOUR CHOICE:" << endl;
    cout << "1 - search for product by name" << endl;
    cout << "2 - add product to the tree by id" << endl;
    cout << endl;
}

int main() {
    vector<Product*> products;
    products = collect_data();
    print_products(products);

    Node* root = nullptr;

    for(int i = 0; i < 10; i++) {
        root = insert(root, products[i]);
    }

    cout << "PREORDER TREE:" << endl;
    preOrder(root);

    while(true) {
        int operation;
        string name;
        int id;
        print_menu();
        cin >> operation;
        switch(operation) {
            case 1:
                cout << "Enter name of product to search" << endl;
                cin >> name;
                root = search(root,name);
                if(root->key->name == name) {
                    print_product(root->key);
                } else {
                    cout << "NO such element in the tree" << endl;
                    break;
                }

                cout << endl << "PREORDER TREE:" << endl;
                preOrder(root);
                break;

            case 2:
                cout << "Enter id of product to insert" << endl;
                cin >> id;
                if(id > 0 && id <= products.size()) {
                    root = insert(root, products[id - 1]);
                } else {
                    cout << "Select correct id!" << endl;
                    break;
                }

                cout << endl << "PREORDER TREE:" << endl;
                preOrder(root);
                break;
            default:
                return 0;
        }
    }

    return 0;
}