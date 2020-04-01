#pragma once

#include <memory>

template<typename T>
class Tree;

template<typename T>
class IterationPolicy{
    /**
     *
     * Base class for iteration policies
     * @brief: Defines behaviour of iterator using IterationPolicy::next(node)
     *
     * */
public:
    virtual std::shared_ptr<typename Tree<T>::Node> next(std::shared_ptr<typename Tree<T>::Node> node) const noexcept = 0;
};



template<typename T>
class ForwardIteration : public IterationPolicy<T>{
    /**
     *
     * Extends IterationPolicy
     * @brief: Allows us to iterate on tree in forward order
     *
     * */
public:
    std::shared_ptr<typename Tree<T>::Node> next(std::shared_ptr<typename Tree<T>::Node> node) const noexcept override;
};



template<typename T>
class ReverseIteration : public IterationPolicy<T>{
    /**
     *
     * Extends IterationPolicy
     * @brief: Allows us to iterate on tree in reverse order
     *
     * */
public:
    std::shared_ptr<typename Tree<T>::Node> next(std::shared_ptr<typename Tree<T>::Node> node) const noexcept override;
};

template<typename T>
std::shared_ptr<typename Tree<T>::Node> ForwardIteration<T>::next(std::shared_ptr<typename Tree<T>::Node> node) const noexcept{
    return node->next();
}


template<typename T>
std::shared_ptr<typename Tree<T>::Node> ReverseIteration<T>::next(std::shared_ptr<typename Tree<T>::Node> node) const noexcept{
    return node->previous();
}
