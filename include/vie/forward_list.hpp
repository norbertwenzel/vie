#ifndef VIE_FORWARD_LIST_HPP_INCLUDED
#define VIE_FORWARD_LIST_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <iterator>

namespace vie {

namespace detail {
template <typename T> class simple_forward_list_iterator;
}

template <typename T>
class simple_forward_list {
        struct node {
                node *next;
                T     value;
        };
        node *first;
public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = detail::simple_forward_list_iterator<T>;

        friend iterator;
        simple_forward_list() : first(nullptr)
        { }
        ~simple_forward_list()
        {
                auto c = first;
                while (c) {
                        auto n = c->next;
                        delete c;
                        c = n;
                }
        }
        void push_front(const T &value)
        {
                auto n = new node{ first, value };
                first = n;
        }
        
        iterator begin();
        iterator end();
};

namespace detail {

template <typename T> class simple_forward_list_iterator {
        using node = typename simple_forward_list<T>::node;
        node *curr = nullptr;
        friend simple_forward_list<T>;
        simple_forward_list_iterator(node *n) : curr(n)
        { }
public:
        simple_forward_list_iterator() = default;
        simple_forward_list_iterator(const simple_forward_list_iterator &) = default;
        simple_forward_list_iterator &operator=(const simple_forward_list_iterator &) = default;
        ~simple_forward_list_iterator() = default;
        
        T &operator*()
        {
                return curr->value;
        }
        T &operator->()
        {
                return curr->value;
        }
        simple_forward_list_iterator &operator++()
        {
                curr = curr->next;
                return *this;
        }
        simple_forward_list_iterator operator++(int)
        {
                simple_forward_list_iterator ret(*this);
                operator++();
                return ret;
        }
        void swap(simple_forward_list_iterator &other)
        {
                using std::swap;
                swap(curr, other.curr);
        }
        friend bool operator==(
                const simple_forward_list_iterator &lhs,
                const simple_forward_list_iterator &rhs)
        {
                return lhs.curr == rhs.curr;
        }
        friend bool operator!=(
                const simple_forward_list_iterator &lhs,
                const simple_forward_list_iterator &rhs)
        {
                return !(lhs == rhs);
        }
};

template <typename T>
void swap(simple_forward_list_iterator<T> &l, simple_forward_list_iterator<T> &r)
{
        l.swap(r);
}
        
}

template <typename T> detail::simple_forward_list_iterator<T> simple_forward_list<T>::begin()
{
        return first;
}

template <typename T> detail::simple_forward_list_iterator<T> simple_forward_list<T>::end()
{
        return {};
}

}

namespace std {

template <typename T>
struct iterator_traits<typename vie::detail::simple_forward_list_iterator<T> > {
        using difference_type = typename vie::simple_forward_list<T>::difference_type;
        using value_type = typename vie::simple_forward_list<T>::value_type;
        using pointer = typename vie::simple_forward_list<T>::pointer;
        using reference = typename vie::simple_forward_list<T>::reference;
        using iterator_category = std::forward_iterator_tag;
};

}

#endif // VIE_FORWARD_LIST_HPP_INCLUDED