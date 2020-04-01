#pragma once

#include "Tree.h"
#include "BplusIterator.hpp"

#include <vector>
#include <algorithm>
using std::vector;
using std::shared_ptr;

/**
* @brief Class for implementing Bplus Tree
*/
template<typename T>
class BplusTree : public Tree<T> {
public:
    class BplusNode;
    typedef shared_ptr<BplusNode> NodePtr;
public:
    explicit BplusTree(const int m);

    void insert(const T& key) override;
    void remove(const T& key) noexcept override;
    void print() const noexcept override;
    
    BplusIterator<T> find(const T& key) const noexcept;
    BplusIterator<T> begin() const noexcept;
    BplusIterator<T> end() const noexcept;
    BplusIterator<T> rbegin() const noexcept;
    BplusIterator<T> rend() const noexcept;

public:
   class BplusNode : public Tree<T>::Node {
    public:
        explicit BplusNode();        
       /* ~BplusNode();*/

        [[nodiscard]] bool is_leaf() const noexcept;

        std::shared_ptr<typename Tree<T>::Node> next() noexcept override;
        std::shared_ptr<typename Tree<T>::Node> previous() noexcept override;

        NodePtr left() const noexcept;
        NodePtr right() const noexcept;

        template<typename OStream>
        void print(OStream& os, int level = 1) const noexcept;
    public:       
        NodePtr parent;
        NodePtr nxt;
        NodePtr prv;
        vector<NodePtr> children;
        vector<int> indexes;
        vector<T> data;        
    };

private:
    const int M, MININD;
    NodePtr minNode;
private:
    ///Utility functions for insert and remove
    int _min(NodePtr start) const noexcept;
    void split(NodePtr ptr) noexcept;
    void borrowLeft(NodePtr ptr) noexcept;
    void borrowRight(NodePtr ptr) noexcept;
    NodePtr merge(NodePtr ptr) const noexcept;
    void shrink() noexcept;
};

#include "BplusTree.tpp"

