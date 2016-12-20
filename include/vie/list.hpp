#ifndef VIE_LIST_HPP_INCLUDED
#define VIE_LIST_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <iterator>

#include <boost/iterator/iterator_facade.hpp>

namespace vie {

namespace detail {
template <typename T> class simple_list_iterator;
}

template <typename T>
class simple_list {
        struct node_base {
                node_base *next;
                node_base *prev;
        };
        struct node : public node_base{
                T     value;

                node(node_base *next, node_base *prev, const T &value) :
                        node_base{ next, prev }, value{ value }
                { }
        };
        node_base head;

        void link(
                node_base *n)
        {
                n->next->prev = n->prev->next = n;
        }
        void pop(
                node *n)
        {
                n->next->prev = n->prev;
                n->prev->next = n->next;
                delete n;
        }
public:
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using iterator = detail::simple_list_iterator<T>;

        friend iterator;
        simple_list() : head{ &head, &head }
        { }
        simple_list(simple_list &&rhs) : head{ rhs.head.next, rhs.head.prev }
        {
                link(&head);
                rhs.head.next = rhs.head.prev = &rhs.head;
        }
        ~simple_list()
        {
                while (!empty())
                        pop_front();
        }
        bool empty()
        {
                return head.next == &head;
        }
        void push_front(const T &value)
        {
                auto n = new node{ head.next, &head, value };
                link(n);
        }
        void pop_front()
        {
                pop(static_cast<node *>(head.next));
        }
        void push_back(const T &value)
        {
                auto n = new node{ &head, head.prev, value };
                link(n);
        }
        void pop_back()
        {
                pop(static_cast<node *>(head.prev));
        }
        iterator begin();
        iterator end();
};

namespace detail {

template <typename T> class simple_list_iterator : public boost::iterator_facade<
                                                                simple_list_iterator<T>,
                                                                typename vie::simple_list<T>::value_type,
                                                                std::bidirectional_iterator_tag,
                                                                typename vie::simple_list<T>::reference,
                                                                typename vie::simple_list<T>::difference_type
                                                                >

{
        using node_base = typename simple_list<T>::node_base;
        using node = typename simple_list<T>::node;
        node_base *curr = nullptr;
        friend simple_list<T>;
        simple_list_iterator(node_base *n) : curr(n)
        { }
public:
        simple_list_iterator() = default;
        simple_list_iterator(const simple_list_iterator &) = default;
        simple_list_iterator &operator=(const simple_list_iterator &) = default;
        ~simple_list_iterator() = default;

        void swap(simple_list_iterator &other)
        {
                using std::swap;
                swap(curr, other.curr);
        }

private:
        friend class boost::iterator_core_access;

        void increment() {
            curr = curr->next;
        }

        void decrement() {
                curr = curr->prev;
        }

        bool equal(const simple_list_iterator &lhs) const {
            return this->curr == lhs.curr;
        }

        const typename vie::simple_list<T>::reference dereference() const {
                return static_cast<node*>(curr)->value;
        }
};

template <typename T>
void swap(simple_list_iterator<T> &l, simple_list_iterator<T> &r)
{
        l.swap(r);
}

}

template <typename T> detail::simple_list_iterator<T> simple_list<T>::begin()
{
        return head.next;
}

template <typename T> detail::simple_list_iterator<T> simple_list<T>::end()
{
        return { &head };
}

}

}

#endif // VIE_LIST_HPP_INCLUDED