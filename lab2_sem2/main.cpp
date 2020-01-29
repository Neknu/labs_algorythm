#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct Product {
    string name;
    string group;
    int cost;
};

vector<Product*> collect_data() {
    vector<Product*> products;
    ifstream f;
    f.open("../../data.txt");
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

enum COLOR { RED, BLACK };

class Node {
public:
  Product val;
  int size; // size of subtree including root
  COLOR color;
  Node* left, *right, *parent;

  explicit Node(Product val) {
    this->val = val;
    size = 1;
    parent = left = right = nullptr;
    color = RED;
  }

  Node* uncle() {
    if (parent == nullptr or parent->parent == nullptr)
      return nullptr;

    if (parent->isOnLeft())
      return parent->parent->right;
    else
      return parent->parent->left;
  }

  bool isOnLeft() { return this == parent->left; }

  Node* sibling() {
    if (parent == nullptr)
      return nullptr;

    if (isOnLeft())
      return parent->right;

    return parent->left;
  }

  // moves node down and moves given node in its place
  void moveDown(Node* nParent) {
    if (parent != nullptr) {
      if (isOnLeft()) {
        parent->left = nParent;
      } else {
        parent->right = nParent;
      }
    }
    nParent->parent = parent;
    parent = nParent;
  }

  bool hasRedChild() {
    return (left != nullptr and left->color == RED) or
           (right != nullptr and right->color == RED);
  }
};

class RBTree {
  Node* root;


  void leftRotate(Node* x) {
    Node* nParent = x->right;

    if (x == root)
      root = nParent;

    nParent->size = x->size;
    x->moveDown(nParent);

    x->right = nParent->left;
    if (nParent->left != nullptr)
      nParent->left->parent = x;

    nParent->left = x;
    int left_size = 0;
    int right_size = 0;
    if(x->left != nullptr) {
      left_size = x->left->size;
    }
    if(x->right != nullptr) {
      right_size = x->right->size;
    }
    x->size = right_size + left_size + 1;

  }

  void rightRotate(Node* x) {
    Node* nParent = x->left;

    if (x == root)
      root = nParent;

    nParent->size = x->size;
    x->moveDown(nParent);

    x->left = nParent->right;
    if (nParent->right != nullptr)
      nParent->right->parent = x;

    nParent->right = x;
    int left_size = 0;
    int right_size = 0;
    if(x->left != nullptr) {
        left_size = x->left->size;
    }
    if(x->right != nullptr) {
      right_size = x->right->size;
    }

    x->size = right_size + left_size + 1;
  }

  static void swapColors(Node* x1, Node* x2) {
    COLOR temp;
    temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
  }

  static void swapValues(Node* u, Node* v) {
      Product temp;
      int temp_size;
      temp = u->val;
      temp_size = u->size;
      u->val = v->val;
      u->size = v->size;
      v->val = temp;
      v->size = temp_size;
  }

  void fixRedRed(Node* x) {
    if (x == root) {
      x->color = BLACK;
      return;
    }

    Node* parent = x->parent, *grandparent = parent->parent,
         *uncle = x->uncle();

    if (parent->color != BLACK) {
      if (uncle != nullptr && uncle->color == RED) {
        // uncle red, perform recoloring and recurse
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        fixRedRed(grandparent);
      } else {
        // Else perform LR, LL, RL, RR
        if (parent->isOnLeft()) {
          if (x->isOnLeft()) {
            // for left right
            swapColors(parent, grandparent);
          } else {
            leftRotate(parent);
            swapColors(x, grandparent);
          }
          // for left left and left right
          rightRotate(grandparent);
        } else {
          if (x->isOnLeft()) {
            // for right left
            rightRotate(parent);
            swapColors(x, grandparent);
          } else {
            swapColors(parent, grandparent);
          }

          // for right right and right left
          leftRotate(grandparent);
        }
      }
    }
  }

  void fixSizeInc(Node* x) {
      if(x == root)
          return;
      Node* curr = x->parent;
      while(curr != root) {
          curr->size++;
          curr = curr->parent;
      }
      root->size++;
  }

    void fixSizeDec(Node* x) {
        if(x == root)
            return;
        Node* curr = x->parent;
        while(curr != root) {
            curr->size--;
            curr = curr->parent;
        }
        root->size--;
    }

  // find node that do not have a left child
  // in the subtree of the given node
  static Node* successor(Node* x) {
    Node* temp = x;

    while (temp->left != nullptr)
      temp = temp->left;

    return temp;
  }

  // find node that replaces a deleted node in BST
  static Node* BSTreplace(Node* x) {
    // when node have 2 children
    if (x->left != nullptr and x->right != nullptr)
      return successor(x->right);

    // when leaf
    if (x->left == nullptr and x->right == nullptr)
      return nullptr;

    // when single child
    if (x->left != nullptr)
      return x->left;
    else
      return x->right;
  }

  void deleteNode(Node* v) {
    fixSizeDec(v);
    Node* u = BSTreplace(v);

    bool uvBlack = ((u == nullptr or u->color == BLACK) and (v->color == BLACK));
    Node* parent = v->parent;
    if (u == nullptr) {
      // u is nullptr therefore v is leaf
      if (v == root) {
        // v is root, making root nullptr
        root = nullptr;
      } else {
        if (uvBlack) {
          // u and v both black
          // v is leaf, fix double black at v
          fixDoubleBlack(v);
        } else {
          // u or v is red
          if (v->sibling() != nullptr)
            // sibling is not nullptr, make it red"
            v->sibling()->color = RED;
        }

        // delete v from the tree
        if (v->isOnLeft()) {
          parent->left = nullptr;
        } else {
          parent->right = nullptr;
        }
      }
      delete v;
      return;
    }

    if (v->left == nullptr or v->right == nullptr) {
      // v has 1 child
      if (v == root) {
        // v is root, assign the value of u to v, and delete u
        v->val = u->val;
        v->left = v->right = nullptr;
        delete u;
      } else {
        // Detach v from tree and move u up
        if (v->isOnLeft()) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          // u and v both black, fix double black at u
          fixDoubleBlack(u);
        } else {
          // u or v red, color u black
          u->color = BLACK;
        }
      }
      return;
    }

    // v has 2 children, swap values with successor and recurse
    swapValues(u, v);
    deleteNode(u);
  }

  void fixDoubleBlack(Node* x) {
    if (x == root)
      return;

    Node* sibling = x->sibling(), *parent = x->parent;
    if (sibling == nullptr) {
      // No sibiling, double black pushed up
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling->isOnLeft()) {
          rightRotate(parent);
        } else {
          leftRotate(parent);
        }
        fixDoubleBlack(x);
      } else {
        if (sibling->hasRedChild()) {
          // at least 1 red children
          if (sibling->left != nullptr and sibling->left->color == RED) {
            if (sibling->isOnLeft()) {
              // left left
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rightRotate(parent);
            } else {
              // right left
              sibling->left->color = parent->color;
              rightRotate(sibling);
              leftRotate(parent);
            }
          } else {
            if (sibling->isOnLeft()) {
              // left right
              sibling->right->color = parent->color;
              leftRotate(sibling);
              rightRotate(parent);
            } else {
              // right right
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              leftRotate(parent);
            }
          }
          parent->color = BLACK;
        } else {
          // 2 black children
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
  }

  static void levelOrder(Node* x) {
    if (x == nullptr)
      return;

    queue<Node* > q;
    Node* curr;

    q.push(x);

    while (!q.empty()) {
      curr = q.front();
      q.pop();

      cout << curr->val.name << " ";

      if (curr->left != nullptr)
        q.push(curr->left);
      if (curr->right != nullptr)
        q.push(curr->right);
    }
  }

  static void inorder(Node* x) {
    if (x == nullptr)
      return;
    inorder(x->left);
    cout << x->val.name << " " << x->size << endl;
    inorder(x->right);
  }

public:

  RBTree() { root = nullptr; }

  Node* getRoot() { return root; }

  // searches for given value
  // if found returns the node (used for delete)
  // else returns the last node while traversing (used in insert)
  Node* search(string s) {
    Node* temp = root;
    while (temp != nullptr) {
      if (s < temp->val.name) {
        if (temp->left == nullptr)
          break;
        else
          temp = temp->left;
      } else if (s == temp->val.name) {
        break;
      } else {
        if (temp->right == nullptr)
          break;
        else
          temp = temp->right;
      }
    }

    return temp;
  }


  void insert(Product n) {
    Node* newNode = new Node(n);
    if (root == nullptr) {
      newNode->color = BLACK;
      root = newNode;
    } else {
      Node* temp = search(n.name);

      if (temp->val.name == n.name) {
        return;
      }

      // if value is not found, search returns the node
      // where the value is to be inserted

      // connect new node to correct node
      newNode->parent = temp;

      if (n.name < temp->val.name)
        temp->left = newNode;
      else
        temp->right = newNode;

      // fix size for all newNode's parents
      fixSizeInc(newNode);

      // fix red red voilaton if exists
      fixRedRed(newNode);
    }
  }

  // utility function that deletes the node with given value
  void deleteByVal(Product n) {
    if (root == nullptr)
      // Tree is empty
      return;

    Node* v = search(n.name), *u;

    if (v->val.name != n.name) {
      cout << "No node found to delete with value:" << n.name << endl;
      return;
    }

    deleteNode(v);
  }

  static Node* OSSelectRekurisve(Node* x, int key) {
      int r = x->left->size + 1;
      if(key == r) {
          return x;
      } else if(key < r) {
          return OSSelectRekurisve(x->left, key);
      } else {
          return OSSelectRekurisve(x->right, key - r);
      }
  }

  Node* OSSelect(int key) {
      return OSSelectRekurisve(root, key);
  }

  int OSRank(Node* x) {
      int r = x->left->size + 1;
      Node* y = x;
      while(y != root) {
          if(y == y->parent->right) {
              r += y->parent->left->size + 1;
          }
          y = y->parent;
      }
      return r;
  }

  void printInOrder() {
    cout << "Inorder: " << endl;
    if (root == nullptr)
      cout << "Tree is empty" << endl;
    else
      inorder(root);
    cout << endl;
  }

  void printLevelOrder() {
    cout << "Level order: " << endl;
    if (root == nullptr)
      cout << "Tree is empty" << endl;
    else
      levelOrder(root);
    cout << endl;
  }
};

RBTree create_tree(vector<Product*> products) {
    RBTree tree;
    for(auto product:products) {
        cout << product->name << endl;
        tree.insert(*product);
    }
    return tree;
}

int main() {

    vector<Product*> products;
    products = collect_data();
    print_products(products);

    RBTree tree = create_tree(products);
    tree.printInOrder();
    tree.printLevelOrder();

  return 0;
}