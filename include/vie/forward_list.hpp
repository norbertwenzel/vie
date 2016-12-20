#ifndef VIE_FORWARD_LIST_HPP_INCLUDED
#define VIE_FORWARD_LIST_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>

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

template <typename T> class simple_forward_list_iterator : public boost::iterator_facade<
                                                                        simple_forward_list_iterator<T>,
                                                                        typename vie::simple_forward_list<T>::value_type,
                                                                        std::forward_iterator_tag,
                                                                        typename vie::simple_forward_list<T>::reference,
                                                                        typename vie::simple_forward_list<T>::difference_type
                                                                        >
{
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

        void swap(simple_forward_list_iterator &other)
        {
                using std::swap;
                swap(curr, other.curr);
        }

private:
        friend class boost::iterator_core_access;

        void increment() {
            curr = curr->next;
        }

        bool equal(const simple_forward_list_iterator &lhs) const {
            return this->curr == lhs.curr;
        }

        const typename vie::simple_forward_list<T>::reference dereference() const {
                return curr.value;
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

#endif // VIE_FORWARD_LIST_HPP_INCLUDED