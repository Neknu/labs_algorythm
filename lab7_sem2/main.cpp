#include <string>
#include <windows.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <list>

#include "sqlite3.h"

using namespace std;


const char separator = ' ';
const int nameWidth = 35;
const int numWidth = 5;

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



struct Node
{ 
	Product data;
	int degree;
	Node* child, *sibling, *parent;
}; 

Node* newNode(Product key) {
	Node* temp = new Node; 
	temp->data = key; 
	temp->degree = 0; 
	temp->child = temp->parent = temp->sibling = nullptr; 
	return temp; 
} 


Node* mergeBinomialTrees(Node* b1, Node* b2) {
	if (b1->data > b2->data) 
		swap(b1, b2); 
	
	b2->parent = b1; 
	b2->sibling = b1->child; 
	b1->child = b2; 
	b1->degree++; 

	return b1; 
} 

// This function perform union operation on two 
// binomial heap i.e. l1 & l2 
list<Node*> unionBionomialHeap(list<Node*> l1, 
							list<Node*> l2) { 
	// _new to another binomial heap which contain 
	// new heap after merging l1 & l2 
	list<Node*> _new; 
	auto it = l1.begin();
	auto ot = l2.begin();
	while (it!=l1.end() && ot!=l2.end()) {
		if((*it)->degree <= (*ot)->degree) { 
			_new.push_back(*it); 
			it++; 
		}
		else {
			_new.push_back(*ot); 
			ot++; 
		} 
	} 

	while (it != l1.end()) { 
		_new.push_back(*it); 
		it++; 
	} 

	while (ot!=l2.end()) { 
		_new.push_back(*ot); 
		ot++; 
	}

	return _new; 
} 

list<Node*> adjust(list<Node*> _heap) { 
	if (_heap.size() <= 1) 
		return _heap; 
	list<Node*> new_heap; 
	list<Node*>::iterator it1,it2,it3; 
	it1 = it2 = it3 = _heap.begin(); 

	if (_heap.size() == 2) { 
		it2 = it1; 
		it2++; 
		it3 = _heap.end(); 
	} 
	else
	{ 
		it2++; 
		it3=it2; 
		it3++; 
	} 
	while (it1 != _heap.end()) {
		if (it2 == _heap.end()) 
			it1++;

		else if ((*it1)->degree < (*it2)->degree) { 
			it1++; 
			it2++; 
			if(it3!=_heap.end()) 
				it3++; 
		} 

		else if (it3!=_heap.end() && 
				(*it1)->degree == (*it2)->degree && 
				(*it1)->degree == (*it3)->degree) { 
			it1++; 
			it2++; 
			it3++; 
		} 

		else if ((*it1)->degree == (*it2)->degree) {
			*it1 = mergeBinomialTrees(*it1,*it2); 
			it2 = _heap.erase(it2); 
			if(it3 != _heap.end()) 
				it3++; 
		} 
	} 
	return _heap; 
} 

list<Node*> insertATreeInHeap(list<Node*> _heap, Node* tree) {
	list<Node*> temp;

	temp.push_back(tree);
	temp = unionBionomialHeap(_heap,temp);

	return adjust(temp); 
} 

list<Node*> removeMinFromTreeReturnBHeap(Node* tree) { 
	list<Node*> heap; 
	Node* temp = tree->child; 
	Node* lo; 

	while (temp) { 
		lo = temp; 
		temp = temp->sibling; 
		lo->sibling = nullptr; 
		heap.push_front(lo); 
	} 
	return heap; 
} 

list<Node*> insert(list<Node*> _head, Product key) {
	Node* temp = newNode(key); 
	return insertATreeInHeap(_head,temp); 
} 

Node* getMin(list<Node*> _heap) { 
	auto it = _heap.begin(); 
	Node* temp = *it; 
	while (it != _heap.end()) { 
		if ((*it)->data < temp->data) 
			temp = *it; 
		it++; 
	} 
	return temp; 
} 

list<Node*> extractMin(list<Node*> _heap) { 
	list<Node*> new_heap,lo; 
	Node* temp; 

	temp = getMin(_heap); 
	list<Node*>::iterator it; 
	it = _heap.begin(); 
	while (it != _heap.end()) { 
		if (*it != temp) {
			new_heap.push_back(*it); 
		} 
		it++; 
	} 
	lo = removeMinFromTreeReturnBHeap(temp); 
	new_heap = unionBionomialHeap(new_heap,lo); 
	new_heap = adjust(new_heap); 
	return new_heap; 
}

template<typename OStream>
void printTree(OStream& os, const Node* root, std::string indent = "", bool last = true) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    os << indent;
    if (last) {
        os << "Sibling----";
        indent += "     ";
    }
    else {
        os << "Child----";
        indent += "|    ";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    os << root->data.name << '\n';

    if (root->child) {
        printTree(os, root->child, indent, false);
    }
    if (root->sibling) {
        printTree(os, root->sibling, indent, true);
    }
}

void printHeap(list<Node*> _heap) {

    cout << "This is Binomial Heap:" << endl;
	list<Node*> ::iterator it; 
	it = _heap.begin(); 
	while (it != _heap.end()) { 
		printTree(cout, *it);
		it++; 
	} 
}

void print_menu() {
    cout << endl;
    cout << "SELECT YOUR CHOICE:" << endl;
    cout << "1 - add product to the tree by id" << endl;
    cout << "2 - extract min" << endl;
    cout << endl;
}


int main() {
    vector<Product*> products = collect_data();
    print_products(products);

	list<Node*> _heap; 


	for(int i = 0; i < 25; i++) {
	    _heap = insert(_heap, *products[i]);
	}

    while(true) {
        int operation;
        string name;
        int id;
        Node* temp;
        printHeap(_heap);
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

                _heap = insert(_heap, *products[id-1]);
                break;

            case 2:
                 temp = getMin(_heap);
                _heap = extractMin(_heap);

                cout << "This is min of Heap:" << endl;
                print_product(&temp->data);

                break;

            default:
                return 0;
        }
    }
} 
