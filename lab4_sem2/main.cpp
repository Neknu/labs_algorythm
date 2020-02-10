#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

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
    Product* data;
    Node* left;
    Node* right;
};

Node* newNode(Product* product, Node *left, Node* right) {
    auto new_node = new Node();
    new_node->data = product;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

Node* buildTree(const vector<Product*>& products, int root[][MAX_N], int low, int high) {
    if(low > high) {
        return nullptr;
    } else {
        int current_root = root[low][high];
        return newNode(products[current_root], buildTree(products, root, low, current_root - 1), buildTree(products, root, current_root + 1, high));
    }
}


Node* optimalSearchTree(int freq[], int fails[], int n, const vector<Product*>& products)
{
    int cost[n+2][n+1];
    int possibility[n+2][n+1];
    int root[MAX_N][MAX_N];

    for (int i = 1; i <= n+1; i++) {
        cost[i][i-1] = fails[i-1];
        possibility[i][i-1] = fails[i-1];
    }

    for (int L = 1; L <= n + 1; L++) {
        for (int i = 1; i <= n-L+1; i++) {
            int j = i+L-1;
            cost[i][j] = INT_MAX;
            possibility[i][j] = possibility[i][j-1] + freq[j] + fails[j];

            for (int r = i; r <= j; r++) {
                int temp = cost[i][r - 1] + cost[r+1][j] + possibility[i][j];
                if(temp < cost[i][j]) {
                    cost[i][j] = temp;
                    root[i][j] = r;
                }
            }
        }
    }

    cout << "Cost of Optimal BST in COSTS:" << cost[1][n] << endl;

    cout << endl << "COSTS:" << endl;
    for(int i = 1; i < n + 2; i++) {
        for(int j = 0; j < n + 1; j++) {
            if(i <= j+1) {
                printElement(cost[i][j],5);
            } else {
                printElement(0,5);
            };

        }
        cout << endl;
    }

    cout << "POSSIBILITIES:" << endl;
    for(int i = 1; i < n + 2; i++) {
        for(int j = 0; j < n + 1; j++) {
            if(i <= j+1) {
                printElement(possibility[i][j],5);
            } else {
                printElement(0,5);
            };

        }
        cout << endl;
    }

    cout << "ROOTS:" << endl;
    for(int i = 1; i < n + 1; i++) {
        for(int j = 1; j < n + 1; j++) {
            if(i <= j) {
                printElement(root[i][j],5);
            } else {
                printElement(0,5);
            };

        }
        cout << endl;
    }

    return buildTree(products, root, 1, n);
}

void preOrder(Node* root)
{
    if (root != nullptr)
    {
        print_product(root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main() {
    vector<Product*> products = collect_data();
    print_products(products);

    int freq[] = {0, 15, 10, 5, 10, 20}; // in percents
    int fails[] = {5, 10, 5, 5, 5, 10}; // in percents
    int n = 5;
    products.resize(n);
    auto compare = [](const Product* a, const Product* b) {
        return a->name < b->name;};
    std::sort(products.begin(), products.end(),compare);
    Node* root = optimalSearchTree(freq, fails, n, products);
    cout << endl << "This is pre order TREE:" << endl;
    preOrder(root);
    return 0;
}