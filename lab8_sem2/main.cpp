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


struct Product {
    int id;
    string name;
    string group;
    int cost;
};

inline bool operator== (const Product& lhs, const Product& rhs){ return lhs.cost == rhs.cost; }

inline bool operator< (const Product& lhs, const Product& rhs){ return lhs.cost < rhs.cost; }
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

#include <cmath> 
#include <cstdlib> 
#include <iostream>

struct node { 
	node* parent; 
	node* child; 
	node* left; 
	node* right; 
	Product key;
	int degree; 
	char mark; // Black or white mark of the node 
	char c;
}; 

struct node* mini = nullptr; 

// Declare an integer for number of nodes in the heap 
int no_of_nodes = 0; 

// Function to insert a node in heap 
void insertion(Product val) {
	struct node* new_node = new node;
	new_node->key = val; 
	new_node->degree = 0; 
	new_node->mark = 'W'; 
	new_node->c = 'N'; 
	new_node->parent = nullptr; 
	new_node->child = nullptr; 
	new_node->left = new_node; 
	new_node->right = new_node; 
	if (mini != nullptr) { 
		(mini->left)->right = new_node; 
		new_node->right = mini; 
		new_node->left = mini->left; 
		mini->left = new_node; 
		if (new_node->key < mini->key) 
			mini = new_node; 
	} 
	else { 
		mini = new_node; 
	} 
	no_of_nodes++; 
} 
// Linking the heap nodes in parent child relationship 
void Fibonnaci_link(struct node* ptr2, struct node* ptr1) { 
	(ptr2->left)->right = ptr2->right; 
	(ptr2->right)->left = ptr2->left; 
	if (ptr1->right == ptr1) 
		mini = ptr1; 
	ptr2->left = ptr2; 
	ptr2->right = ptr2; 
	ptr2->parent = ptr1; 
	if (ptr1->child == nullptr) 
		ptr1->child = ptr2; 
	ptr2->right = ptr1->child; 
	ptr2->left = (ptr1->child)->left; 
	((ptr1->child)->left)->right = ptr2; 
	(ptr1->child)->left = ptr2; 
	if (ptr2->key < (ptr1->child)->key) 
		ptr1->child = ptr2; 
	ptr1->degree++; 
} 
// Consolidating the heap 
void Consolidate() { 
	int temp1; 
	double temp2 = (log(no_of_nodes)) / (log(2));
	int temp3 = int(temp2);
	struct node* arr[temp3]; 
	for (int i = 0; i <= temp3; i++) 
		arr[i] = nullptr; 
	node* ptr1 = mini; 
	node* ptr2; 
	node* ptr3; 
	node* ptr4 = ptr1; 
	do { 
		ptr4 = ptr4->right; 
		temp1 = ptr1->degree; 
		while (arr[temp1] != nullptr) { 
			ptr2 = arr[temp1]; 
			if (ptr1->key > ptr2->key) { 
				ptr3 = ptr1; 
				ptr1 = ptr2; 
				ptr2 = ptr3; 
			} 
			if (ptr2 == mini) 
				mini = ptr1; 
			Fibonnaci_link(ptr2, ptr1); 
			if (ptr1->right == ptr1) 
				mini = ptr1; 
			arr[temp1] = nullptr; 
			temp1++; 
		} 
		arr[temp1] = ptr1; 
		ptr1 = ptr1->right; 
	} while (ptr1 != mini); 
	mini = nullptr; 
	for (int j = 0; j <= temp3; j++) { 
		if (arr[j] != nullptr) { 
			arr[j]->left = arr[j]; 
			arr[j]->right = arr[j]; 
			if (mini != nullptr) { 
				(mini->left)->right = arr[j]; 
				arr[j]->right = mini; 
				arr[j]->left = mini->left; 
				mini->left = arr[j]; 
				if (arr[j]->key < mini->key) 
					mini = arr[j]; 
			} 
			else { 
				mini = arr[j]; 
			} 
			if (mini == nullptr) 
				mini = arr[j]; 
			else if (arr[j]->key < mini->key) 
				mini = arr[j]; 
		} 
	} 
} 

// Function to extract minimum node in the heap 
void Extract_min() { 
	if (mini == nullptr) 
		cout << "The heap is empty" << endl; 
	else { 
		node* temp = mini; 
		node* pntr; 
		pntr = temp; 
		node* x = nullptr; 
		if (temp->child != nullptr) { 

			x = temp->child; 
			do { 
				pntr = x->right; 
				(mini->left)->right = x; 
				x->right = mini; 
				x->left = mini->left; 
				mini->left = x; 
				if (x->key < mini->key) 
					mini = x; 
				x->parent = nullptr; 
				x = pntr; 
			} while (pntr != temp->child); 
		} 
		(temp->left)->right = temp->right; 
		(temp->right)->left = temp->left; 
		mini = temp->right; 
		if (temp == temp->right && temp->child == nullptr) 
			mini = nullptr; 
		else { 
			mini = temp->right; 
			Consolidate(); 
		} 
		no_of_nodes--; 
	} 
} 

// Cutting a node in the heap to be placed in the root list 
void Cut(struct node* found, struct node* temp) { 
	if (found == found->right) 
		temp->child = nullptr; 

	(found->left)->right = found->right; 
	(found->right)->left = found->left; 
	if (found == temp->child) 
		temp->child = found->right; 

	temp->degree = temp->degree - 1; 
	found->right = found; 
	found->left = found; 
	(mini->left)->right = found; 
	found->right = mini; 
	found->left = mini->left; 
	mini->left = found; 
	found->parent = nullptr; 
	found->mark = 'B'; 
} 

// Recursive cascade cutting function 
void Cascase_cut(struct node* temp) { 
	node* ptr5 = temp->parent; 
	if (ptr5 != nullptr) { 
		if (temp->mark == 'W') { 
			temp->mark = 'B'; 
		} 
		else { 
			Cut(temp, ptr5); 
			Cascase_cut(ptr5); 
		} 
	} 
} 

// Function to decrease the value of a node in the heap 
void Decrease_key(struct node* found, int val) {
	if (mini == nullptr) 
		cout << "The Heap is Empty" << endl; 

	if (found == nullptr) 
		cout << "Node not found in the Heap" << endl; 

	found->key.cost = val;

	struct node* temp = found->parent; 
	if (temp != nullptr && found->key < temp->key) { 
		Cut(found, temp); 
		Cascase_cut(temp); 
	} 
	if (found->key < mini->key) 
		mini = found; 
} 

// Function to find the given node 
void Find(struct node* mini, Product old_val, int val) {
	struct node* found = nullptr;
	node* temp5 = mini;
	temp5->c = 'Y';
	node* found_ptr = nullptr;
	if (temp5->key == old_val) {
		found_ptr = temp5;
		temp5->c = 'N';
		found = found_ptr;
		Decrease_key(found, val);
	}
	if (found_ptr == nullptr) {
		if (temp5->child != nullptr)
			Find(temp5->child, old_val, val);
		if ((temp5->right)->c != 'Y')
			Find(temp5->right, old_val, val);
	}
	temp5->c = 'N';
	found = found_ptr;
}

// Deleting a node from the heap 
void Deletion(Product val) {
	if (mini == nullptr) 
		cout << "The heap is empty" << endl; 
	else { 

		// Decreasing the value of the node to 0 
		Find(mini, val, 0); 

		// Calling Extract_min function to 
		// delete minimum value node, which is 0 
		Extract_min(); 
		cout << "Key Deleted" << endl; 
	} 
} 

// Function to display the heap 
void display() { 
	node* ptr = mini; 
	if (ptr == nullptr) 
		cout << "The Heap is Empty" << endl; 

	else { 
		cout << "The root nodes of Heap are: " << endl; 
		do { 
			print_product(&ptr->key);
			ptr = ptr->right; 
			if (ptr != mini) { 
				cout << "-->"; 
			} 
		} while (ptr != mini && ptr->right != nullptr); 
		cout << endl 
			<< "The heap has " << no_of_nodes << " nodes" << endl 
			<< endl; 
	} 
} 

int main() {

    vector<Product*> products = collect_data();
    print_products(products);

	cout << "Creating an initial heap" << endl; 
	insertion(*products[5]);
	insertion(*products[8]);
	insertion(*products[7]);

	display(); 

	cout << "Extracting min" << endl;
	Extract_min();
	display();

	cout << "Decrease value of product 8 to 7" << endl;
	Find(mini, *products[8], 7);
	display();

	cout << "Delete the node 7" << endl;
	Deletion(*products[8]);
	display();

	return 0; 
} 
