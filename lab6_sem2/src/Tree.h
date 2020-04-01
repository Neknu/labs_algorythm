#pragma once

#include <memory>


/**
* @brief Base class for all Trees
* @description Used for making similar interface for all Trees
*/
template<typename T>
class Tree {
public:
    /**
    * @brief Insert element in the tree with Template parameter key
    */
    virtual void insert(const T& key) = 0;

    /**
     * @brief remove element from the tree with Template parameter key
     * @description if there is no element with this key - nothing does
    */
    virtual void remove(const T& key) noexcept = 0;

    /**
     * @brief simple print tree in console
     * @description can be done with Iterators
    */
    virtual void print() const noexcept = 0;

    /**
     * @brief Base Class for all nodes
    */
    class Node{
    public:
        /**
         * @brief get next element ordered by keys
         * @return nullptr if not exist
        */
        virtual std::shared_ptr<typename Tree<T>::Node> next() noexcept = 0;

        /**
         * @brief get previous element ordered by keys
         * @return nullptr if not exist
        */
        virtual std::shared_ptr<typename Tree<T>::Node> previous() noexcept = 0;
    };

    std::shared_ptr<Node> root;
};


/**
* @brief Context Class - for client interface
* @description example of strategy pattern
*/
template<template <typename T> typename TreeType, typename DataType>
class BalancedTreeContext{
public:
    explicit BalancedTreeContext(std::shared_ptr<TreeType<DataType>> tree);
    void insert(const DataType& key);
    void remove(const DataType& key) noexcept;
    void print() const noexcept;

    auto find(const DataType& key);


private:
    std::shared_ptr<TreeType<DataType>> tree;
};



template<template <typename T> typename TreeType, typename DataType>
BalancedTreeContext<TreeType, DataType>::BalancedTreeContext(std::shared_ptr<TreeType<DataType>> tree) : tree(tree){

}



template<template<typename T> typename TreeType, typename DataType>
void BalancedTreeContext<TreeType, DataType>::insert(const DataType &key) {
    tree->insert(key);
}



template<template<typename T> typename TreeType, typename DataType>
void BalancedTreeContext<TreeType, DataType>::remove(const DataType &key) noexcept {
    tree->remove(key);
}

template<template<typename T> typename TreeType, typename DataType>
void BalancedTreeContext<TreeType, DataType>::print() const noexcept {
    tree->print();
}

template<template<typename T> typename TreeType, typename DataType>
auto BalancedTreeContext<TreeType, DataType>::find(const DataType &key) {

    return tree->find(key);
}

