#include "BplusIterator.hpp"
#include "Tree.h"

#include <iostream>

template<typename T>
BplusIterator<T>::BplusIterator(
    std::shared_ptr<typename BplusTree<T>::BplusNode> node,
    std::shared_ptr<IterationPolicy<T>> policy,   
    int ind
)
    : curr_node(node), curr_ind(ind),policy(policy) {}


template<typename T>
BplusIterator<T>& BplusIterator<T>::operator++() noexcept {
    if (curr_node != nullptr) {
        if (policy == std::make_shared<ForwardIteration<T>>()) {
            if (curr_ind < curr_node->data.size() - 1)
                ++curr_ind;
            else {
                curr_node = std::dynamic_pointer_cast<typename BplusTree<T>::BplusNode>(policy->next(curr_node));
                curr_ind = 0;
            }
        }
        else {
            if (curr_ind)
                --curr_ind;
            else {
                curr_node = std::dynamic_pointer_cast<typename BplusTree<T>::BplusNode>(policy->next(curr_node));
                curr_ind = curr_node->data.size()-1;
            }
        }
    }

    return *this;
}

template<typename T>
BplusIterator<T> BplusIterator<T>::operator+(int n) const noexcept {
    auto temp = curr_node;
    auto temp_ind = curr_ind;

    if(policy== std::make_shared<ForwardIteration<T>>())
        while (n) {
            if (temp_ind < temp->data.size() - 1)
                ++temp_ind;
            else {
                if (temp == nullptr) {
                    break;
                }
                temp = std::dynamic_pointer_cast<typename BplusTree<T>::BplusNode>(policy->next(temp));
                temp_ind = 0;
            }
            --n;
        }   
    else 
        while (n) {
            if (temp_ind)
                --temp_ind;
            else {
                if (temp == nullptr) {
                    break;
                }
                temp = std::dynamic_pointer_cast<typename BplusTree<T>::BplusNode>(policy->next(temp));
                temp_ind = temp->data.size()-1;
            }
            --n;
        }

    return BplusIterator(temp, policy,temp_ind);
}

template<typename T>
const T& BplusIterator<T>::operator*() const noexcept {
    return curr_node->data[curr_ind];
}

template<typename T>
bool BplusIterator<T>::operator!=(const BplusIterator &other) noexcept {
    return curr_node != other.curr_node;
}

template<typename T>
bool BplusIterator<T>::operator==(const BplusIterator &other) noexcept {
    return curr_node == other.curr_node;
}


