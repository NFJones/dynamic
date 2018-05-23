#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace njones {
    class dynamic_iterator;
    class const_dynamic_iterator;
    class reverse_dynamic_iterator;
    class const_reverse_dynamic_iterator;

    class dynamic {
       public:
        typedef dynamic_iterator               iterator;
        typedef const_dynamic_iterator         const_iterator;
        typedef reverse_dynamic_iterator       reverse_iterator;
        typedef const_reverse_dynamic_iterator const_reverse_iterator;

        enum class type { NONE = 0, INT, UINT, LONG, ULONG, DOUBLE, BOOL, STRING, ARRAY, MAP };

        dynamic();
        dynamic(const dynamic &rhs);
        dynamic(dynamic &&rhs);
        dynamic(const type t);

        template <class T>
        dynamic(const T &val) : dynamic() {
            *this = val;
        }

        ~dynamic();

        dynamic &operator=(const dynamic &rhs);
        dynamic &operator=(dynamic &&rhs);
        dynamic &operator=(const nullptr_t val);
        dynamic &operator=(const int val);
        dynamic &operator=(const unsigned int val);
        dynamic &operator=(const long val);
        dynamic &operator=(const unsigned long val);
        dynamic &operator=(const double val);
        dynamic &operator=(const bool val);
        dynamic &operator=(const std::string &val);
        dynamic &operator=(const char *val);

        explicit operator int();
        explicit operator unsigned int();
        explicit operator long();
        explicit operator unsigned long();
        explicit operator double();
        explicit operator bool();
        explicit operator std::string();

        bool operator==(const dynamic &rhs) const;
        bool operator!=(const dynamic &rhs) const;
        bool operator<(const dynamic &rhs) const;
        bool operator>(const dynamic &rhs) const;
        bool operator<=(const dynamic &rhs) const;
        bool operator>=(const dynamic &rhs) const;

        type get_type() const;

        void set_type(const type t);

        bool is_null() const;
        bool is_int() const;
        bool is_uint() const;
        bool is_long() const;
        bool is_ulong() const;
        bool is_double() const;
        bool is_bool() const;
        bool is_string() const;
        bool is_array() const;
        bool is_map() const;

        int           as_int() const;
        unsigned int  as_uint() const;
        long          as_long() const;
        unsigned long as_ulong() const;
        double        as_double() const;
        bool          as_bool() const;
        std::string   as_string(const bool pretty = false) const;

        const dynamic &operator[](const dynamic &key) const;
        dynamic &      operator[](const dynamic &key);
        const dynamic &at(const dynamic &key) const;
        dynamic &      at(const dynamic &key);
        dynamic &      front();
        const dynamic &front() const;
        dynamic &      back();
        const dynamic &back() const;

        bool has(const dynamic &key) const;

        dynamic::iterator               begin();
        dynamic::iterator               end();
        dynamic::reverse_iterator       rbegin();
        dynamic::reverse_iterator       rend();
        dynamic::const_iterator         begin() const;
        dynamic::const_iterator         end() const;
        dynamic::const_reverse_iterator rbegin() const;
        dynamic::const_reverse_iterator rend() const;
        dynamic::const_iterator         cbegin() const;
        dynamic::const_iterator         cend() const;
        dynamic::const_reverse_iterator crbegin() const;
        dynamic::const_reverse_iterator crend() const;

        void push_back(const dynamic &val);

        template <class T>
        void push_back(const T &val) {
            dynamic d(val);
            push_back(d);
        }

        size_t size() const;
        size_t max_size() const;
        void   resize(const size_t s);
        size_t capacity() const;
        void   reserve(const size_t s);
        void   shrink_to_fit();
        void   assign(const size_t s, const dynamic &val);

        template <class InputIterator>
        void assign(InputIterator first, InputIterator last) {
            type_check(type::ARRAY);
            clear();
            while (first != last)
                push_back(*first++);
        }

        void pop_back();

        bool empty() const;
        void clear();
        void reset();
        void erase(const dynamic &key);
        void erase(std::vector<dynamic>::const_iterator iter);

        void emplace(std::vector<dynamic>::const_iterator iter, const dynamic &val);

        template <class T>
        void emplace(std::vector<dynamic>::const_iterator iter, const T &val) {
            dynamic d(val);
            emplace(iter, d);
        }

        void emplace_back(const dynamic &val);

        template <class T>
        void emplace_back(const T &val) {
            dynamic d(val);
            emplace_back(d);
        }

        dynamic deep_copy() const;

        std::string str(const bool pretty = false) const;

       private:
        struct container;

        static const std::unordered_map<dynamic::type, std::string> TYPE_NAME;
        std::shared_ptr<dynamic::container>                         v;

        void type_check(const type t) const;

        bool is_type(const type t) const;

        void to_string(const bool pretty, std::ostream &s, const size_t indent) const;

        friend std::ostream &operator<<(std::ostream &stream, const dynamic &d);
    };
}  // namespace njones

namespace std {
    template <>
    struct hash<njones::dynamic> {
        std::size_t operator()(const njones::dynamic &d) const {
            return hash<string>()(d.str());
        }
    };
}  // namespace std

namespace njones {
    class dynamic_iterator_value {
       public:
        dynamic_iterator_value(const dynamic &key, dynamic *v);

        dynamic key() const;

        dynamic &value() const;

       private:
        dynamic  _key;
        dynamic *v;
    };

    class const_dynamic_iterator_value {
       public:
        const_dynamic_iterator_value(const dynamic &key, const dynamic *v);
        dynamic        key() const;
        const dynamic &value() const;

       private:
        dynamic        _key;
        const dynamic *v;
    };

    class dynamic_iterator
        : public std::iterator<std::bidirectional_iterator_tag, dynamic_iterator_value> {
       public:
        typedef dynamic_iterator_value value;

        dynamic_iterator(std::vector<dynamic>::iterator arrayIter);
        dynamic_iterator(std::unordered_map<dynamic, dynamic>::iterator mapIter);
        dynamic_iterator(const dynamic_iterator &other);
        ~dynamic_iterator();

        dynamic_iterator &operator++();
        dynamic_iterator  operator++(int);
        dynamic_iterator &operator--();
        dynamic_iterator  operator--(int);
        bool              operator==(const dynamic_iterator &rhs);
        bool              operator!=(const dynamic_iterator &rhs);

        dynamic::iterator::value operator*();

       protected:
        dynamic::type                                  t;
        std::vector<dynamic>::iterator                 arrayIter;
        std::unordered_map<dynamic, dynamic>::iterator mapIter;
    };

    class const_dynamic_iterator
        : public std::iterator<std::bidirectional_iterator_tag, dynamic_iterator_value> {
       public:
        typedef const_dynamic_iterator_value value;

        const_dynamic_iterator(std::vector<dynamic>::const_iterator arrayIter);
        const_dynamic_iterator(std::unordered_map<dynamic, dynamic>::const_iterator mapIter);
        const_dynamic_iterator(const const_dynamic_iterator &other);
        ~const_dynamic_iterator();

        const_dynamic_iterator &       operator++();
        const_dynamic_iterator         operator++(int);
        const_dynamic_iterator &       operator--();
        const_dynamic_iterator         operator--(int);
        bool                           operator==(const const_dynamic_iterator &rhs);
        bool                           operator!=(const const_dynamic_iterator &rhs);
        dynamic::const_iterator::value operator*();

       protected:
        dynamic::type                                        t;
        std::vector<dynamic>::const_iterator                 arrayIter;
        std::unordered_map<dynamic, dynamic>::const_iterator mapIter;
    };

    class reverse_dynamic_iterator {
       public:
        typedef dynamic_iterator_value value;

        reverse_dynamic_iterator(std::vector<dynamic>::reverse_iterator arrayIter);
        reverse_dynamic_iterator(const reverse_dynamic_iterator &other);
        ~reverse_dynamic_iterator();

        reverse_dynamic_iterator &operator++();
        reverse_dynamic_iterator  operator++(int);
        reverse_dynamic_iterator &operator--();
        reverse_dynamic_iterator  operator--(int);
        bool                      operator==(const reverse_dynamic_iterator &rhs);
        bool                      operator!=(const reverse_dynamic_iterator &rhs);

        dynamic::reverse_iterator::value operator*();

       protected:
        dynamic::type                          t;
        std::vector<dynamic>::reverse_iterator arrayIter;
    };

    class const_reverse_dynamic_iterator {
       public:
        typedef const_dynamic_iterator_value value;

        const_reverse_dynamic_iterator(std::vector<dynamic>::const_reverse_iterator arrayIter);
        const_reverse_dynamic_iterator(const const_reverse_dynamic_iterator &other);
        ~const_reverse_dynamic_iterator();

        const_reverse_dynamic_iterator &operator++();
        const_reverse_dynamic_iterator  operator++(int);
        const_reverse_dynamic_iterator &operator--();
        const_reverse_dynamic_iterator  operator--(int);
        bool                            operator==(const const_reverse_dynamic_iterator &rhs);
        bool                            operator!=(const const_reverse_dynamic_iterator &rhs);

        dynamic::const_reverse_iterator::value operator*();

       protected:
        dynamic::type                                t;
        std::vector<dynamic>::const_reverse_iterator arrayIter;
    };

    std::ostream &operator<<(std::ostream &stream, const dynamic &d);
}  // namespace njones
