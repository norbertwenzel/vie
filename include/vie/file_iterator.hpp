#ifndef VIE_FILE_ITERATOR_HPP_INCLUDED
#define VIE_FILE_ITERATOR_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <iterator>
#include <cstdio>

#include <boost/iterator/iterator_facade.hpp>

namespace vie {

class file_iterator : public boost::iterator_facade<file_iterator,
                                                    char,
                                                    std::input_iterator_tag>
{
        FILE *fp = nullptr;
        int curr = EOF;
public:
        explicit file_iterator(FILE *f) : fp(f), curr(fgetc(f)) { }
        file_iterator() = default;
        file_iterator(const file_iterator &) = default;
        file_iterator &operator=(const file_iterator &) = default;
        ~file_iterator() = default;

private:
        friend class boost::iterator_core_access;

        void increment() {
            curr = fgetc(fp);
        }

        bool equal(const file_iterator &lhs) const {
            return this->curr == lhs.curr;
        }

        const int& dereference() const { return curr; }
};

}

#endif // VIE_FILE_ITERATOR_HPP_INCLUDED
