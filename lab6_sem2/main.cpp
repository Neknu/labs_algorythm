#include "src/BplusTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "src/sqlite3.h"

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

inline bool operator==(const Product& lhs, const Product& rhs) {return lhs.name == rhs.name; }
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

void print_menu() {
    cout << endl;
    cout << "SELECT YOUR CHOICE:" << endl;
    cout << "1 - add product to the tree by id" << endl;
    cout << "2 - delete product from the tree by id" << endl;
    cout << endl;
}

int main () {
    auto products = collect_data();
    print_products(products);
    BalancedTreeContext<BplusTree, Product> context(std::make_shared<BplusTree<Product>>(3));

    for(int i = 0; i < 10; i++) {
        context.insert(*products[i]);
    }

    while(true) {
        int operation;
        string name;
        int id;
        context.print();
        print_menu();
        cin >> operation;
        switch(operation) {
            case 1:
                cout << "Enter id of product to insert" << endl;
                cin >> id;
                if(id < 0 || id >= products.size()) {
                    cout << "select correct id!" << endl;
                    continue;
                }

                context.insert(*products[id-1]);
                break;

            case 2:
                cout << "Enter id of product to delete" << endl;
                cin >> id;
                if(id < 0 || id >= products.size()) {
                    cout << "select correct id!" << endl;
                    continue;
                }

                context.remove(*products[id-1]);
                break;
            default:
                return 0;
        }
    }


    return 0;
}