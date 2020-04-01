#pragma once

#include "IterationPolicy.h"

#include <memory>

template<typename T>
class Tree;

template <typename T>
class BplusTree;

template<typename T>
class BplusIterator {
    /**
     *
     * Class for iterators of B+ tree
     *
     * @brief: Allows us to write specific iterators for trees,
     * and define iterating behaviour with IterationPolicy
     * (Mix of Bridge and Strategy Patterns)
     *
     * */

public:
    friend class Tree<T>;
    friend class BplusTree<T>;

    BplusIterator(std::shared_ptr<typename BplusTree<T>::BplusNode> node, std::shared_ptr<IterationPolicy<T>> policy, int ind = 0);

    virtual const T& operator*() const noexcept;

    virtual bool operator!=(const BplusIterator &other) noexcept;

    virtual bool operator==(const BplusIterator &other) noexcept;

    BplusIterator& operator++() noexcept;

    BplusIterator operator+(int n) const noexcept;

protected:
    std::shared_ptr<typename BplusTree<T>::BplusNode> curr_node;
    int curr_ind;
    std::shared_ptr<IterationPolicy<T>> policy;
};


#include "BplusIterator.tpp"