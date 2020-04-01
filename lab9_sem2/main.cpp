#include <iostream>
#include <vector>
#include <climits>
#include <exception>
#include <set>
#include <string>
#include <iomanip>
#include <cmath>

#include "sqlite3.h"

using std::string;
using std::cout;
using std::cin;
using std::vector;
using std::endl;

using std::set;
using std::istream;
using std::pair;

struct Edge {
    int head;
    long cost;
};

using Graph = vector<vector<Edge>>;
using SingleShortPath = vector<long>;
using AllShortPath = vector<vector<long>>;
const long INF = LONG_MAX;

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





// build logic, that if products cost difference is <= distance - then edge between them exists
// and it weight is this difference
Graph load_graph(vector<Product*> product, int vertices, int distance) {
    Graph g;
    if(vertices > product.size())
        return g;

    g.resize(vertices + 1);
    int count_of_edges = 0;
    // all work with graph starting from 1 (not from 0)
    for(int i = 1; i <= vertices; i++) {
        for(int j = i + 1; j <= vertices; j++) {
            int difference = std::fabs(product[i]->cost - product[j]->cost);
            if(difference < distance ) {
                count_of_edges += 2;
                g[i].push_back({j, difference});
                g[j].push_back({i, difference});
            }
        }
    }

    cout << "Graph has " << vertices << " vertices and " << count_of_edges << " edges." << endl;
    return g;
}

Graph add_zero_edge(Graph g) {
    // add a zero-cost edge from vertex 0 to all other edges
    for (int i = 1; i < g.size(); i++) {
        g[0].push_back({i, 0});
    }

    return g;
}

SingleShortPath bellman_ford(Graph &g, int s) {
    vector<vector<long>> memo(g.size()+2, vector<long>(g.size(), INF));

    // initialise base case
    memo[0][s] = 0;

    for (int i = 1; i < memo.size(); i++) {
        // compute shortest paths from s to all vertices, with max hop-count i
        for (int n = 0; n < g.size(); n++) {
            if (memo[i-1][n] < memo[i][n]) {
                memo[i][n] = memo[i-1][n];
            }
            for (auto& e : g[n]) {
                if (memo[i-1][n] != INF) {
                    if (memo[i-1][n] + e.cost < memo[i][e.head]) {
                        memo[i][e.head] = memo[i-1][n] + e.cost;
                    }
                }
            }
        }
    }

    // check if the last iteration differed from the 2nd-last
    for (int j = 0; j < g.size(); j++) {
        if (memo[g.size()+1][j] != memo[g.size()][j]) {
            throw string{"negative cycle found"};
        }
    }

    return memo[g.size()];
}

SingleShortPath djikstra(const Graph& g, int s) {
    SingleShortPath dist(g.size(), INF);
    set<pair<int,long>> frontier;

    frontier.insert({0,s});

    while (!frontier.empty()) {
        pair<int,long> p = *frontier.begin();
        frontier.erase(frontier.begin());

        int d = p.first;
        int n = p.second;

        // this is our shortest path to n
        dist[n] = d;

        // now look at all edges out from n to update the frontier
        for (auto e : g[n]) {
            // update this node in the frontier if we have a shorter path
            if (dist[n]+e.cost < dist[e.head]) {
                if (dist[e.head] != INF) {
                    // we've seen this node before, so erase it from the set in order to update it
                    frontier.erase(frontier.find({dist[e.head], e.head}));
                }
                frontier.insert({dist[n]+e.cost, e.head});
                dist[e.head] = dist[n]+e.cost;
            }
        }
    }

    return dist;
}

AllShortPath johnson(Graph &g) {
    // build g_prime - added from vertex 0 to all other edges, with cost 0
    Graph g_prime = add_zero_edge(g);

    // now run Bellman-Ford to get single-source shortest paths from s to all other vertices
    SingleShortPath ssp;
    try {
        ssp = bellman_ford(g_prime, 0);
    } catch (string e) {
        cout << "Negative cycles found in graph.  Cannot compute shortest paths." << endl;
        throw e;
    }

    // no re-weight each edge (u,v) in g to be: cost + ssp[u] - ssp[v].
    for (int i = 1; i < g.size(); i++) {
        for (auto &e : g[i]) {
            e.cost = e.cost + ssp[i] - ssp[e.head];
        }
    }

    // now that we've re-weighted our graph, we can invoke N iterations of Djikstra to find
    // all pairs shortest paths
    AllShortPath all_short_path(g.size());
    for (int i = 1; i < g.size(); i++) {
        all_short_path[i] = djikstra(g, i);
    }

    // now re-weight the path costs back to their original weightings
    for (int u = 1; u < g.size(); u++) {
        for (int v = 1; v < g.size(); v++) {
            if (all_short_path[u][v] != INF) {
                all_short_path[u][v] += ssp[v] - ssp[u];
            }
        }
    }

    return all_short_path;
}


// this algorithm also works with negative costs, but it will be hard to prevent negative cycles
int main () {

    vector<Product*> products = collect_data();
    print_products(products);
    int vertices = 20; // count of vertices
    int distance = 30; // max distance between cost of two products that have edge
    Graph g = load_graph(products, vertices, distance);

    AllShortPath all_short_path = johnson(g);

    int space_width = 4;
    cout << "This is array of all short paths from every vertices to other based on their difference in costs" << endl;

    printElement("id", space_width);
    for(int i = 1; i < all_short_path.size(); i++) {
        printElement(i, space_width);
    }
    cout << endl;

    for(int i = 1; i < all_short_path.size(); i++) {
        printElement(std::to_string(i), space_width - 2);
        printElement("|", 2);
        for(int j = 1; j < all_short_path[i].size(); j++) {
            printElement(all_short_path[i][j], space_width);
        }
        cout << endl;
    }

}