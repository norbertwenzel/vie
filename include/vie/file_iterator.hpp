#ifndef VIE_FILE_ITERATOR_HPP_INCLUDED
#define VIE_FILE_ITERATOR_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <iterator>
#include <cstdio>

namespace vie {

class file_iterator {
        FILE *fp = nullptr;
        int curr = EOF;
public:
        explicit file_iterator(FILE *f) : fp(f), curr(fgetc(f)) { }
        file_iterator() = default;
        file_iterator(const file_iterator &) = default;
        file_iterator &operator=(const file_iterator &) = default;
        ~file_iterator() = default;
        
        char operator *() { return curr; }
        char operator ->() { return curr; }
        file_iterator &operator++()
        {
                curr = fgetc(fp);
                return *this;
        }
        file_iterator operator++(int)
        {
                file_iterator ret(*this);
                operator++();
                return ret;
        }
        friend bool operator==(
                const file_iterator &lhs,
                const file_iterator &rhs)
        {
                return lhs.curr == rhs.curr;
        }
        friend bool operator!=(
                const file_iterator &lhs,
                const file_iterator &rhs)
        {
                return !(lhs == rhs);
        }
};

}

namespace std {

template <>
struct iterator_traits<vie::file_iterator> {
        using difference_type = std::ptrdiff_t;
        using value_type = char;
        using pointer = char *;
        using reference = char &;
        using iterator_category = std::input_iterator_tag;
};

}


#endif // VIE_FILE_ITERATOR_HPP_INCLUDED