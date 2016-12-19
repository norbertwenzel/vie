#ifndef VIE_VECTOR_HPP_INCLUDED
#define VIE_VECTOR_HPP_INCLUDED

#include <cstddef>
#include <utility>
#include <iterator>
#include <type_traits>
#include <memory>

namespace vie {

namespace detail {
template <typename T> class simple_vector_iterator;
}

template <typename T>
class simple_vector {
	using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;
	T *begin_;
	T *end_;
	T *capacity_;
	
	void grow(
		std::size_t n)
	{
		auto cap = capacity();
		if (cap >= n) return;
		if (!cap) cap = 8;
		while (cap < n)
			cap *= 2;
		auto mem = std::make_unique<storage_type []>(cap);
		auto nbegin = reinterpret_cast<T *>(mem.get());
		auto nend = nbegin;
		auto ncapacity = nbegin + cap;
		try {
			for (auto p = begin_; p < end_; ++p) {
				new (nend) T(*p);
				++nend;
				
			}
		} catch (...) {
			for (auto p = nbegin; p < nend; ++p)
				p->~T();
			throw;
		}
		auto obegin = begin_;
		auto oend = end_;
		begin_ = nbegin;
		end_ = nend;
		capacity_ = ncapacity;
		mem.release();
		for (auto p = obegin; p < oend; ++p)
			p->~T();
		delete [] reinterpret_cast<storage_type *>(obegin);
	}

public:
	using value_type = T;
	using size_type = std::size_t;
	using differenze_type = std::ptrdiff_t;
	using reference = value_type &;
	using const_reference = const value_type &;
    using iterator = detail::simple_vector_iterator<T>;

    friend iterator;

	simple_vector() : begin_(nullptr), end_(nullptr), capacity_(nullptr)
	{ }
	~simple_vector()
	{
		if (!begin_) return;
		for (T *p = begin_; p != end_; ++p)
			p->~T();
		delete [] reinterpret_cast<storage_type *>(begin_);
	}
	std::size_t size()
	{
		return end_ - begin_;
	}
	
	std::size_t capacity()
	{
		return capacity_ - begin_;
	}
	
	void push_back(const T &value)
	{
		grow(size() + 1);
		new (end_) T(value);
		++end_;
	}
    T &operator[](size_type n)
    {
            return begin_[n];
    }
    T &at(size_type n)
    {
            if (n >= size())
                throw std::out_of_range("index out of range");
            return begin_[n];
    }
    iterator begin();
    iterator end();
};

namespace detail {
template <typename T> class simple_vector_iterator {
        T *curr = nullptr;
        friend simple_vector<T>;
        simple_vector_iterator(T *c) : curr(c)
        { }
public:
        simple_vector_iterator() = default;
        simple_vector_iterator(const simple_vector_iterator &) = default;
        simple_vector_iterator &operator=(const simple_vector_iterator &) = default;
        ~simple_vector_iterator() = default;
        
        T &operator*()
        {
                return *curr;
        }
        T &operator->()
        {
                return *curr;
        }
        simple_vector_iterator &operator++()
        {
                ++curr;
                return *this;
        }
        simple_vector_iterator operator++(int)
        {
                simple_vector_iterator ret(*this);
                operator++();
                return ret;
        }
        simple_vector_iterator &operator+=(typename simple_vector<T>::differenze_type n)
        {
                curr += n;
        }
        simple_vector_iterator &operator--()
        {
                --curr;
                return *this;
        }
        simple_vector_iterator operator--(int)
        {
                simple_vector_iterator ret(*this);
                operator--();
                return ret;
        }
        simple_vector_iterator &operator-=(typename simple_vector<T>::differenze_type n)
        {
                curr -= n;
        }
        T &operator[](typename simple_vector<T>::differenze_type n)
        {
                return *(curr + n);
        }
        void swap(simple_vector_iterator &other)
        {
                using std::swap;
                swap(curr, other.curr);
        }
        friend bool operator==(
                const simple_vector_iterator &lhs,
                const simple_vector_iterator &rhs)
        {
                return lhs.curr == rhs.curr;
        }
        friend bool operator!=(
                const simple_vector_iterator &lhs,
                const simple_vector_iterator &rhs)
        {
                return !(lhs == rhs);
        }
        friend bool operator<(
                const simple_vector_iterator &lhs,
                const simple_vector_iterator &rhs)
        {
                return lhs.curr < rhs.curr;
        }
        friend bool operator<=(
                const simple_vector_iterator &lhs,
                const simple_vector_iterator &rhs)
        {
                return !(rhs < lhs);
        }
        friend bool operator>=(
                const simple_vector_iterator &lhs,
                const simple_vector_iterator &rhs)
        {
                return !(lhs < rhs);
        }
        friend bool operator>(
                const simple_vector_iterator &lhs,
                const simple_vector_iterator &rhs)
        {
                return rhs < lhs;
        }
        
        friend simple_vector_iterator operator+(
                const simple_vector_iterator &i,
                typename simple_vector<T>::differenze_type n)
        {
                return i.curr + n;
        }
        friend simple_vector_iterator operator-(
                const simple_vector_iterator &i,
                typename simple_vector<T>::differenze_type n)
        {
                return i.curr - n;
        }
};

template <typename T>
void swap(simple_vector_iterator<T> &l, simple_vector_iterator<T> &r)
{
        l.swap(r);
}

}

template <typename T> detail::simple_vector_iterator<T> simple_vector<T>::begin()
{
        return begin_;
}

template <typename T> detail::simple_vector_iterator<T> simple_vector<T>::end()
{
        return end_;
}

}

namespace std {

template <typename T>
struct iterator_traits<typename vie::detail::simple_vector_iterator<T> > {
        using difference_type = typename vie::simple_vector<T>::difference_type;
        using value_type = typename vie::simple_vector<T>::value_type;
        using pointer = typename vie::simple_vector<T>::pointer;
        using reference = typename vie::simple_vector<T>::reference;
        using iterator_category = std::random_access_iterator_tag;
};

}

#endif // VIE_VECTOR_HPP_INCLUDED