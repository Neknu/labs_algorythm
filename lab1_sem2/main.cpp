#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <fstream>

using std::string;
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

const int MOD = 101;
const int COEF = 57;

int rand_num(int max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(0, max);

    return int(dist(gen));
}


struct Product {
    string name;
    string group;
    int cost;
};

struct Node {
    int m;
    int a;
    int b;
    vector<Product*> products;
    vector<Product*> hash_product;
};

vector<Product*> collect_data() {
    vector<Product*> products;
    ifstream f;
    f.open("data.txt");
    string data;
    while(f >> data) {
        auto product = new Product();
        product->name = data;
        f >> data;
        product->group = data;
        f >> data;
        product->cost = std::stoi(data);
        products.push_back(product);
    }
    f.close();
    return products;
}

int hash_product_name(string name, int count, int a = COEF, int b = 0, int mod = MOD) {
    int hash = b;
    int length = name.length();
    for(int i = 0; i < length; i++) {
        hash = (hash + ((int) name[i])*a + b) % mod;
    }
    return hash % count;
}

void create_inside_hash(Node* h) {
    h->m = h->products.size()*h->products.size();
    h->hash_product.resize(h->m);
    if(h->products.size() == 1) {
        h->a = 0;
        h->b = 0;
        h->hash_product[0] = h->products[0];
        return;
    }
    bool check_collissions = false;
    while(!check_collissions) {
        bool collissions[h->m];
        for(int i = 0; i < h->m; i++) {
            collissions[i] = false;
        }

        h->a = rand_num( MOD - 2) + 1;
        h->b = rand_num( MOD - 1);
        check_collissions = true;
        for(auto product:h->products) {
            int hash = hash_product_name(product->name, h->m, h->a, h->b);
            if(collissions[hash]) {
                check_collissions = false;
                break;
            } else {
                h->hash_product[hash] = product;
                collissions[hash] = true;
            }
        }
    }
    h->products.clear();
}

vector<Node*> create_hash(const vector<Product*> &products) {
    vector<Node*> hash;
    int products_size = products.size();
    for(int i = 0; i < products_size; i++) {
        hash.push_back(new Node());
    }

    for(auto product:products) {
        hash[hash_product_name(product->name, products_size)]->products.push_back(product);
    }

    for(auto h:hash) {
        if(h->products.size() > 0) {
            create_inside_hash(h);
        }
    }

    return hash;
}

Product* getProductByName(vector<Node*> hash, const string &name) {
    Node* h = hash[hash_product_name(name, hash.size())];
    return h->hash_product[hash_product_name(name, h->m, h->a, h->b)];
}

void print_product(Product* product) {
    cout << "name: " << product->name << " -- group: " << product->group << " -- cost: " << product->cost << endl;
}

void print_products(const vector<Product*> &products) {
    cout << "WELCOME TO OUR MINIMARKET HASH:" << endl;
    cout << "You can choose your product name:" << endl;
    for(auto product:products) {
        print_product(product);
    }
    cout << endl;
}

int main() {
    vector<Product*> products;
    products = collect_data();
    print_products(products);
    vector<Node*> hash = create_hash(products);
    cout << "Now please write what product you want: " << endl;
    cout << "(please write only existing product, or error or incomplete data arise)" << endl;
    string product_name;
    cin >> product_name;

    print_product(getProductByName(hash, product_name));

    return 0;
}