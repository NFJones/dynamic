#include "dynamic.hpp"

#define FMT_HEADER_ONLY

#include <fmt/format.h>
#include <cstring>
#include <fstream>
#include <streambuf>

using namespace std;

namespace njones {
    struct dynamic::container {
        union value {
            int                              intVal;
            unsigned int                     uintVal;
            long                             longVal;
            unsigned long                    ulongVal;
            double                           doubleVal;
            bool                             boolVal;
            string *                         stringVal;
            vector<dynamic> *                arrayVal;
            unordered_map<dynamic, dynamic> *mapVal;
        };

        container() {
            t           = dynamic::type::NONE;
            v.stringVal = nullptr;
        }

        container(const container &rhs) {
            set_type(rhs.t);
            switch (t) {
                case dynamic::type::STRING:
                    *(v.stringVal) = *(rhs.v.stringVal);
                    break;
                case dynamic::type::ARRAY:
                    *(v.arrayVal) = *(rhs.v.arrayVal);
                    break;
                case dynamic::type::MAP:
                    *(v.mapVal) = *(rhs.v.mapVal);
                    break;
                default:
                    memcpy(&v, &rhs.v, sizeof(v));
                    break;
            }
        }

        container(container &&rhs) {
            set_type(rhs.t);
            switch (t) {
                case dynamic::type::STRING:
                    *(v.stringVal) = *(rhs.v.stringVal);
                    break;
                case dynamic::type::ARRAY:
                    *(v.arrayVal) = *(rhs.v.arrayVal);
                    break;
                case dynamic::type::MAP:
                    *(v.mapVal) = *(rhs.v.mapVal);
                    break;
                default:
                    memcpy(&v, &rhs.v, sizeof(v));
                    break;
            }
            rhs.reset_type();
        }

        ~container() {
            reset_type();
        }

        void set_type(const dynamic::type t) {
            reset_type();
            this->t = t;
            switch (t) {
                case dynamic::type::STRING:
                    v.stringVal = new string{};
                    break;
                case dynamic::type::ARRAY:
                    v.arrayVal = new vector<dynamic>{};
                    break;
                case dynamic::type::MAP:
                    v.mapVal = new unordered_map<dynamic, dynamic>{};
                    break;
                default:
                    break;
            }
        }

        void reset_type() {
            switch (t) {
                case dynamic::type::STRING:
                    if (v.stringVal != nullptr)
                        delete v.stringVal;
                    v.stringVal = nullptr;
                    break;
                case dynamic::type::ARRAY:
                    if (v.arrayVal != nullptr)
                        delete v.arrayVal;
                    v.arrayVal = nullptr;
                    break;
                case dynamic::type::MAP:
                    if (v.mapVal != nullptr)
                        delete v.mapVal;
                    v.mapVal = nullptr;
                    break;
                default:
                    break;
            }
            memset(&v, 0, sizeof(v));
            t = dynamic::type::NONE;
        }

        value         v;
        dynamic::type t;
    };
}  // namespace njones

using namespace njones;

static string escape_string(const string &str) {
    static const char *const HEX_DIGITS = "0123456789ABCDEF";

    ostringstream s;
    s << '"';
    for (const char c : str) {
        if (' ' <= c && c <= '~' && c != '\\' && c != '"')
            s << c;
        else {
            s << '\\';
            switch (c) {
                case '"':
                    s << '"';
                    break;
                case '\\':
                    s << '\\';
                    break;
                case '\t':
                    s << 't';
                    break;
                case '\r':
                    s << 'r';
                    break;
                case '\n':
                    s << 'n';
                    break;
                default:
                    s << 'x';
                    s << HEX_DIGITS[c >> 4];
                    s << HEX_DIGITS[c & 0xF];
            }
        }
    }
    s << '"';
    return s.str();
}

dynamic_iterator_value::dynamic_iterator_value(const dynamic &key, dynamic *v) : _key(key), v(v) {
}

dynamic dynamic_iterator_value::key() const {
    return _key;
}

dynamic &dynamic_iterator_value::value() const {
    return *v;
}

const_dynamic_iterator_value::const_dynamic_iterator_value(const dynamic &key, const dynamic *v)
    : _key(key), v(v) {
}

dynamic const_dynamic_iterator_value::key() const {
    return _key;
}

const dynamic &const_dynamic_iterator_value::value() const {
    return *v;
}

dynamic_iterator::dynamic_iterator(std::vector<dynamic>::iterator arrayIter)
    : t(dynamic::type::ARRAY), arrayIter(arrayIter) {
}

dynamic_iterator::dynamic_iterator(std::unordered_map<dynamic, dynamic>::iterator mapIter)
    : t(dynamic::type::MAP), mapIter(mapIter) {
}

dynamic_iterator::dynamic_iterator(const dynamic_iterator &other)
    : t(other.t), arrayIter(other.arrayIter), mapIter(other.mapIter) {
}

dynamic_iterator::~dynamic_iterator() {
}

dynamic_iterator &dynamic_iterator::operator++() {
    if (t == dynamic::type::ARRAY)
        arrayIter++;
    else
        mapIter++;
    return *this;
}

dynamic_iterator dynamic_iterator::operator++(int) {
    dynamic_iterator tmp(*this);
    ++(*this);
    return tmp;
}

dynamic_iterator &dynamic_iterator::operator--() {
    if (t == dynamic::type::ARRAY)
        arrayIter--;
    else
        throw domain_error("dynamic value map iterator decrement not implemented");
    return *this;
}

dynamic_iterator dynamic_iterator::operator--(int) {
    dynamic_iterator tmp(*this);
    --(*this);
    return tmp;
}

dynamic::iterator::value dynamic_iterator::operator*() {
    if (t == dynamic::type::ARRAY)
        return dynamic_iterator_value("", &(*arrayIter));
    else
        return dynamic_iterator_value((*mapIter).first, &(mapIter->second));
}

bool dynamic_iterator::operator==(const dynamic_iterator &rhs) {
    if (t == dynamic::type::ARRAY)
        return arrayIter == rhs.arrayIter;
    else
        return mapIter == rhs.mapIter;
}

bool dynamic_iterator::operator!=(const dynamic_iterator &rhs) {
    return !(*this == rhs);
}

reverse_dynamic_iterator::reverse_dynamic_iterator(std::vector<dynamic>::reverse_iterator arrayIter)
    : arrayIter(arrayIter) {
    t = dynamic::type::ARRAY;
}

reverse_dynamic_iterator::reverse_dynamic_iterator(const reverse_dynamic_iterator &other)
    : arrayIter(other.arrayIter) {
    t = other.t;
}

reverse_dynamic_iterator::~reverse_dynamic_iterator() {
}

reverse_dynamic_iterator &reverse_dynamic_iterator::operator++() {
    if (t == dynamic::type::ARRAY)
        arrayIter++;
    else
        throw domain_error("dynamic value map reverse_iterator not implemented");
    return *this;
}

reverse_dynamic_iterator reverse_dynamic_iterator::operator++(int) {
    reverse_dynamic_iterator tmp(*this);
    ++(*this);
    return tmp;
}

reverse_dynamic_iterator &reverse_dynamic_iterator::operator--() {
    if (t == dynamic::type::ARRAY)
        arrayIter--;
    else
        throw domain_error("dynamic value map reverse_iterator not implemented");
    return *this;
}

reverse_dynamic_iterator reverse_dynamic_iterator::operator--(int) {
    reverse_dynamic_iterator tmp(*this);
    --(*this);
    return tmp;
}

dynamic::reverse_iterator::value reverse_dynamic_iterator::operator*() {
    if (t == dynamic::type::ARRAY)
        return dynamic::reverse_iterator::value("", &(*arrayIter));
    else
        throw domain_error("dynamic value map reverse_iterator not implemented");
}

bool reverse_dynamic_iterator::operator==(const reverse_dynamic_iterator &rhs) {
    if (t == dynamic::type::ARRAY)
        return arrayIter == rhs.arrayIter;
    else
        throw domain_error("dynamic value map reverse_iterator not implemented");
}

bool reverse_dynamic_iterator::operator!=(const reverse_dynamic_iterator &rhs) {
    return !(*this == rhs);
}

const_dynamic_iterator::const_dynamic_iterator(std::vector<dynamic>::const_iterator arrayIter)
    : t(dynamic::type::ARRAY), arrayIter(arrayIter) {
}

const_dynamic_iterator::const_dynamic_iterator(
    std::unordered_map<dynamic, dynamic>::const_iterator mapIter)
    : t(dynamic::type::MAP), mapIter(mapIter) {
}

const_dynamic_iterator::const_dynamic_iterator(const const_dynamic_iterator &other)
    : t(other.t), arrayIter(other.arrayIter), mapIter(other.mapIter) {
}

const_dynamic_iterator::~const_dynamic_iterator() {
}

const_dynamic_iterator &const_dynamic_iterator::operator++() {
    if (t == dynamic::type::ARRAY)
        arrayIter++;
    else
        mapIter++;
    return *this;
}

const_dynamic_iterator const_dynamic_iterator::operator++(int) {
    const_dynamic_iterator tmp(*this);
    ++(*this);
    return tmp;
}

const_dynamic_iterator &const_dynamic_iterator::operator--() {
    if (t == dynamic::type::ARRAY)
        arrayIter--;
    else
        throw range_error("dynamic value map iterator decrement not implemented");
    return *this;
}

const_dynamic_iterator const_dynamic_iterator::operator--(int) {
    const_dynamic_iterator tmp(*this);
    --(*this);
    return tmp;
}

dynamic::const_iterator::value const_dynamic_iterator::operator*() {
    if (t == dynamic::type::ARRAY)
        return const_dynamic_iterator_value("", &(*arrayIter));
    else
        return const_dynamic_iterator_value((*mapIter).first, &(mapIter->second));
}

bool const_dynamic_iterator::operator==(const const_dynamic_iterator &rhs) {
    if (t == dynamic::type::ARRAY)
        return arrayIter == rhs.arrayIter;
    else
        return mapIter == rhs.mapIter;
}

bool const_dynamic_iterator::operator!=(const const_dynamic_iterator &rhs) {
    return !(*this == rhs);
}

const_reverse_dynamic_iterator::const_reverse_dynamic_iterator(
    std::vector<dynamic>::const_reverse_iterator arrayIter)
    : arrayIter(arrayIter) {
    t = dynamic::type::ARRAY;
}

const_reverse_dynamic_iterator::const_reverse_dynamic_iterator(
    const const_reverse_dynamic_iterator &other)
    : arrayIter(other.arrayIter) {
    t = other.t;
}

const_reverse_dynamic_iterator::~const_reverse_dynamic_iterator() {
}

const_reverse_dynamic_iterator &const_reverse_dynamic_iterator::operator++() {
    if (t == dynamic::type::ARRAY)
        arrayIter++;
    else
        throw domain_error("dynamic value map const_reverse_iterator not implemented");
    return *this;
}

const_reverse_dynamic_iterator const_reverse_dynamic_iterator::operator++(int) {
    const_reverse_dynamic_iterator tmp(*this);
    ++(*this);
    return tmp;
}

const_reverse_dynamic_iterator &const_reverse_dynamic_iterator::operator--() {
    if (t == dynamic::type::ARRAY)
        arrayIter--;
    else
        throw domain_error("dynamic value map const_reverse_iterator not implemented");
    return *this;
}

const_reverse_dynamic_iterator const_reverse_dynamic_iterator::operator--(int) {
    const_reverse_dynamic_iterator tmp(*this);
    --(*this);
    return tmp;
}

dynamic::const_reverse_iterator::value const_reverse_dynamic_iterator::operator*() {
    if (t == dynamic::type::ARRAY)
        return dynamic::const_reverse_iterator::value("", &(*arrayIter));
    else
        throw domain_error("dynamic value map const_reverse_iterator not implemented");
}

bool const_reverse_dynamic_iterator::operator==(const const_reverse_dynamic_iterator &rhs) {
    if (t == dynamic::type::ARRAY)
        return arrayIter == rhs.arrayIter;
    else
        throw domain_error("dynamic value map const_reverse_iterator not implemented");
}

bool const_reverse_dynamic_iterator::operator!=(const const_reverse_dynamic_iterator &rhs) {
    return !(*this == rhs);
}

const unordered_map<dynamic::type, string> njones::dynamic::TYPE_NAME = {
    {dynamic::type::NONE, "null"},   {dynamic::type::INT, "int"},
    {dynamic::type::UINT, "uint"},   {dynamic::type::LONG, "long"},
    {dynamic::type::ULONG, "ulong"}, {dynamic::type::DOUBLE, "double"},
    {dynamic::type::BOOL, "bool"},   {dynamic::type::STRING, "string"},
    {dynamic::type::ARRAY, "array"}, {dynamic::type::MAP, "map"},
};

dynamic::dynamic() : v(make_shared<container>()) {
    set_type(dynamic::type::MAP);
}

dynamic::dynamic(const dynamic::type t) : v(make_shared<container>()) {
    set_type(t);
}

dynamic::dynamic(const dynamic &rhs) {
    *this = rhs;
}

dynamic::dynamic(dynamic &&rhs) {
    *this = rhs;
}

dynamic::~dynamic() {
}

dynamic &dynamic::operator=(const dynamic &rhs) {
    v = rhs.v;

    return *this;
}

dynamic &dynamic::operator=(dynamic &&rhs) {
    v = move(rhs.v);

    return *this;
}

dynamic &dynamic::operator=(const nullptr_t val) {
    v->set_type(dynamic::type::NONE);

    return *this;
}

dynamic &dynamic::operator=(const int val) {
    v->set_type(dynamic::type::INT);
    v->v.intVal = val;

    return *this;
}

dynamic &dynamic::operator=(const unsigned int val) {
    v->set_type(dynamic::type::UINT);
    v->v.uintVal = val;

    return *this;
}

dynamic &dynamic::operator=(const long val) {
    v->set_type(dynamic::type::LONG);
    v->v.longVal = val;

    return *this;
}

dynamic &dynamic::operator=(const unsigned long val) {
    v->set_type(dynamic::type::ULONG);
    v->v.ulongVal = val;

    return *this;
}

dynamic &dynamic::operator=(const double val) {
    v->set_type(dynamic::type::DOUBLE);
    v->v.doubleVal = val;

    return *this;
}

dynamic &dynamic::operator=(const bool val) {
    v->set_type(dynamic::type::BOOL);
    v->v.boolVal = val;

    return *this;
}

dynamic &dynamic::operator=(const string &val) {
    v->set_type(dynamic::type::STRING);
    *(v->v.stringVal) = val;

    return *this;
}

dynamic &dynamic::operator=(const char *val) {
    v->set_type(dynamic::type::STRING);
    *(v->v.stringVal) = string(val);

    return *this;
}

dynamic::operator int() {
    return as_int();
}

dynamic::operator unsigned int() {
    return as_uint();
}

dynamic::operator long() {
    return as_long();
}

dynamic::operator unsigned long() {
    return as_ulong();
}

dynamic::operator double() {
    return as_double();
}

dynamic::operator bool() {
    return as_bool();
}

dynamic::operator string() {
    return as_string();
}

bool dynamic::operator==(const dynamic &rhs) const {
    return str() == rhs.str();
}

bool dynamic::operator!=(const dynamic &rhs) const {
    return str() != rhs.str();
}

bool dynamic::operator<(const dynamic &rhs) const {
    return str() < rhs.str();
}

bool dynamic::operator>(const dynamic &rhs) const {
    return str() > rhs.str();
}

bool dynamic::operator<=(const dynamic &rhs) const {
    return str() <= rhs.str();
}

bool dynamic::operator>=(const dynamic &rhs) const {
    return str() >= rhs.str();
}

dynamic::type dynamic::get_type() const {
    return v->t;
}

void dynamic::set_type(const dynamic::type t) {
    v->set_type(t);
}

bool dynamic::is_null() const {
    return v->t == dynamic::type::NONE;
}

bool dynamic::is_int() const {
    return v->t == dynamic::type::INT;
}

bool dynamic::is_uint() const {
    return v->t == dynamic::type::UINT;
}

bool dynamic::is_long() const {
    return v->t == dynamic::type::LONG;
}

bool dynamic::is_ulong() const {
    return v->t == dynamic::type::ULONG;
}

bool dynamic::is_double() const {
    return v->t == dynamic::type::DOUBLE;
}

bool dynamic::is_bool() const {
    return v->t == dynamic::type::BOOL;
}

bool dynamic::is_string() const {
    return v->t == dynamic::type::STRING;
}

bool dynamic::is_array() const {
    return v->t == dynamic::type::ARRAY;
}

bool dynamic::is_map() const {
    return v->t == dynamic::type::MAP;
}

int dynamic::as_int() const {
    switch (v->t) {
        case type::NONE:
            return 0;
            break;
        case type::INT:
            return static_cast<decltype(v->v.intVal)>(v->v.intVal);
            break;
        case type::UINT:
            return static_cast<decltype(v->v.intVal)>(v->v.uintVal);
            break;
        case type::LONG:
            return static_cast<decltype(v->v.intVal)>(v->v.longVal);
            break;
        case type::ULONG:
            return static_cast<decltype(v->v.intVal)>(v->v.ulongVal);
            break;
        case type::DOUBLE:
            return static_cast<decltype(v->v.intVal)>(v->v.doubleVal);
            break;
        case type::BOOL:
            return static_cast<decltype(v->v.intVal)>(v->v.boolVal);
            break;
        default:
            throw domain_error(fmt::format("dynamic value type {} is not convertible to {}",
                                           TYPE_NAME.at(v->t), TYPE_NAME.at(type::INT)));
    }
}

unsigned int dynamic::as_uint() const {
    switch (v->t) {
        case type::NONE:
            return 0;
            break;
        case type::INT:
            return static_cast<decltype(v->v.uintVal)>(v->v.intVal);
            break;
        case type::UINT:
            return static_cast<decltype(v->v.uintVal)>(v->v.uintVal);
            break;
        case type::LONG:
            return static_cast<decltype(v->v.uintVal)>(v->v.longVal);
            break;
        case type::ULONG:
            return static_cast<decltype(v->v.uintVal)>(v->v.ulongVal);
            break;
        case type::DOUBLE:
            return static_cast<decltype(v->v.uintVal)>(v->v.doubleVal);
            break;
        case type::BOOL:
            return static_cast<decltype(v->v.uintVal)>(v->v.boolVal);
            break;
        default:
            throw domain_error(fmt::format("dynamic value type {} is not convertible to {}",
                                           TYPE_NAME.at(v->t), TYPE_NAME.at(type::UINT)));
    }
}

long dynamic::as_long() const {
    switch (v->t) {
        case type::NONE:
            return 0;
            break;
        case type::INT:
            return static_cast<decltype(v->v.longVal)>(v->v.intVal);
            break;
        case type::UINT:
            return static_cast<decltype(v->v.longVal)>(v->v.uintVal);
            break;
        case type::LONG:
            return static_cast<decltype(v->v.longVal)>(v->v.longVal);
            break;
        case type::ULONG:
            return static_cast<decltype(v->v.longVal)>(v->v.ulongVal);
            break;
        case type::DOUBLE:
            return static_cast<decltype(v->v.longVal)>(v->v.doubleVal);
            break;
        case type::BOOL:
            return static_cast<decltype(v->v.longVal)>(v->v.boolVal);
            break;
        default:
            throw domain_error(fmt::format("dynamic value type {} is not convertible to {}",
                                           TYPE_NAME.at(v->t), TYPE_NAME.at(type::LONG)));
    }
}

unsigned long dynamic::as_ulong() const {
    switch (v->t) {
        case type::NONE:
            return 0;
            break;
        case type::INT:
            return static_cast<decltype(v->v.ulongVal)>(v->v.intVal);
            break;
        case type::UINT:
            return static_cast<decltype(v->v.ulongVal)>(v->v.uintVal);
            break;
        case type::LONG:
            return static_cast<decltype(v->v.ulongVal)>(v->v.longVal);
            break;
        case type::ULONG:
            return static_cast<decltype(v->v.ulongVal)>(v->v.ulongVal);
            break;
        case type::DOUBLE:
            return static_cast<decltype(v->v.ulongVal)>(v->v.doubleVal);
            break;
        case type::BOOL:
            return static_cast<decltype(v->v.ulongVal)>(v->v.boolVal);
            break;
        default:
            throw domain_error(fmt::format("dynamic value type {} is not convertible to {}",
                                           TYPE_NAME.at(v->t), TYPE_NAME.at(type::ULONG)));
    }
}

double dynamic::as_double() const {
    switch (v->t) {
        case type::NONE:
            return 0.0;
            break;
        case type::INT:
            return static_cast<decltype(v->v.doubleVal)>(v->v.intVal);
            break;
        case type::UINT:
            return static_cast<decltype(v->v.doubleVal)>(v->v.uintVal);
            break;
        case type::LONG:
            return static_cast<decltype(v->v.doubleVal)>(v->v.longVal);
            break;
        case type::ULONG:
            return static_cast<decltype(v->v.doubleVal)>(v->v.ulongVal);
            break;
        case type::DOUBLE:
            return static_cast<decltype(v->v.doubleVal)>(v->v.doubleVal);
            break;
        case type::BOOL:
            return static_cast<decltype(v->v.doubleVal)>(v->v.boolVal);
            break;
        default:
            throw domain_error(fmt::format("dynamic value type {} is not convertible to {}",
                                           TYPE_NAME.at(v->t), TYPE_NAME.at(type::DOUBLE)));
    }
}

bool dynamic::as_bool() const {
    switch (v->t) {
        case type::NONE:
            return false;
            break;
        case type::INT:
            return static_cast<decltype(v->v.boolVal)>(v->v.intVal);
            break;
        case type::UINT:
            return static_cast<decltype(v->v.boolVal)>(v->v.uintVal);
            break;
        case type::LONG:
            return static_cast<decltype(v->v.boolVal)>(v->v.longVal);
            break;
        case type::ULONG:
            return static_cast<decltype(v->v.boolVal)>(v->v.ulongVal);
            break;
        case type::DOUBLE:
            return static_cast<decltype(v->v.boolVal)>(v->v.doubleVal);
            break;
        case type::BOOL:
            return static_cast<decltype(v->v.boolVal)>(v->v.boolVal);
            break;
        default:
            return !empty();
    }
}

string dynamic::as_string(const bool pretty) const {
    switch (v->t) {
        case type::STRING:
            return *(v->v.stringVal);
            break;
        default:
            return str(pretty);
    }
}

const dynamic &dynamic::operator[](const dynamic &key) const {
    if (v->t == type::MAP) {
        if (!has(key))
            throw range_error(fmt::format("dynamic value has no member: {}", key.str()));
        return (*(v->v.mapVal)).at(key);
    } else if (v->t == type::ARRAY) {
        if (key.as_ulong() >= size())
            throw range_error(fmt::format("dynamic value index out of range {} > {}",
                                          key.as_ulong(), size() - 1));
        return (*(v->v.arrayVal)).at(key.as_ulong());
    } else
        throw domain_error("dynamic value is not an array or map");
}

dynamic &dynamic::operator[](const dynamic &key) {
    if (v->t == type::MAP) {
        if (!has(key))
            (*(v->v.mapVal))[key] = dynamic(dynamic::type::MAP);
        return (*(v->v.mapVal)).at(key);
    } else if (v->t == type::ARRAY) {
        if (key.as_ulong() >= size())
            throw range_error(fmt::format("dynamic value index out of range {} > {}",
                                          key.as_ulong(), size() - 1));
        return (*(v->v.arrayVal)).at(key.as_ulong());
    } else
        throw domain_error("dynamic value is not an array or map");
}

const dynamic &dynamic::at(const dynamic &key) const {
    return (*this)[key];
}

dynamic &dynamic::at(const dynamic &key) {
    if (v->t == type::MAP) {
        if (!has(key))
            throw range_error(fmt::format("dynamic value has no member: {}", key.str()));
        return (*(v->v.mapVal)).at(key);
    } else if (v->t == type::ARRAY) {
        return (*this)[key];
    } else
        throw domain_error("dynamic value is not an array or map");
}

dynamic &dynamic::front() {
    if (v->t == type::ARRAY) {
        return at(0);
    } else
        throw domain_error("dynamic value is not an array or string");
}

const dynamic &dynamic::front() const {
    if (v->t == type::ARRAY) {
        return at(0);
    } else
        throw domain_error("dynamic value is not an array or string");
}

dynamic &dynamic::back() {
    if (v->t == type::ARRAY) {
        return at(size() - 1);
    } else
        throw domain_error("dynamic value is not an array or string");
}

const dynamic &dynamic::back() const {
    if (v->t == type::ARRAY) {
        return at(size() - 1);
    } else
        throw domain_error("dynamic value is not an array or string");
}

dynamic::iterator dynamic::begin() {
    if (v->t == dynamic::type::MAP)
        return dynamic::iterator(v->v.mapVal->begin());
    if (v->t == dynamic::type::ARRAY)
        return dynamic::iterator(v->v.arrayVal->begin());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::iterator dynamic::end() {
    if (v->t == dynamic::type::MAP)
        return dynamic::iterator(v->v.mapVal->end());
    if (v->t == dynamic::type::ARRAY)
        return dynamic::iterator(v->v.arrayVal->end());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::const_iterator dynamic::begin() const {
    if (v->t == dynamic::type::MAP)
        return dynamic::const_iterator(v->v.mapVal->begin());
    if (v->t == dynamic::type::ARRAY)
        return dynamic::const_iterator(v->v.arrayVal->begin());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::const_iterator dynamic::end() const {
    if (v->t == dynamic::type::MAP)
        return dynamic::const_iterator(v->v.mapVal->end());
    if (v->t == dynamic::type::ARRAY)
        return dynamic::const_iterator(v->v.arrayVal->end());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::reverse_iterator dynamic::rbegin() {
    if (v->t == dynamic::type::MAP)
        throw domain_error("dynamic value map rbegin not implemented");
    if (v->t == dynamic::type::ARRAY)
        return dynamic::reverse_iterator(v->v.arrayVal->rbegin());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::reverse_iterator dynamic::rend() {
    if (v->t == dynamic::type::MAP)
        throw domain_error("dynamic value map rend not implemented");
    if (v->t == dynamic::type::ARRAY)
        return dynamic::reverse_iterator(v->v.arrayVal->rend());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::const_reverse_iterator dynamic::rbegin() const {
    if (v->t == dynamic::type::MAP)
        throw domain_error("dynamic value map rbegin not implemented");
    if (v->t == dynamic::type::ARRAY)
        return dynamic::const_reverse_iterator(v->v.arrayVal->rbegin());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::const_reverse_iterator dynamic::rend() const {
    if (v->t == dynamic::type::MAP)
        throw domain_error("dynamic value map rend not implemented");
    if (v->t == dynamic::type::ARRAY)
        return dynamic::const_reverse_iterator(v->v.arrayVal->rend());
    else
        throw domain_error("dynamic value is not an array or a map");
}

dynamic::const_iterator dynamic::cbegin() const {
    return begin();
}

dynamic::const_iterator dynamic::cend() const {
    return end();
}

dynamic::const_reverse_iterator dynamic::crbegin() const {
    return rbegin();
}

dynamic::const_reverse_iterator dynamic::crend() const {
    return rend();
}

void dynamic::push_back(const dynamic &val) {
    type_check(dynamic::type::ARRAY);
    (*(v->v.arrayVal)).push_back(val);
}

bool dynamic::has(const dynamic &key) const {
    type_check(dynamic::type::MAP);
    return (*(v->v.mapVal)).find(key) != (*(v->v.mapVal)).end();
}

size_t dynamic::size() const {
    if (v->t == dynamic::type::ARRAY)
        return (*(v->v.arrayVal)).size();
    else if (v->t == dynamic::type::MAP)
        return (*(v->v.mapVal)).size();
    else if (v->t == dynamic::type::STRING)
        return (*(v->v.stringVal)).size();
    else
        throw domain_error("dynamic value type must be string, array, or map to have a size");
}

size_t dynamic::max_size() const {
    if (v->t == dynamic::type::ARRAY)
        return (*(v->v.arrayVal)).max_size();
    else if (v->t == dynamic::type::MAP)
        return (*(v->v.mapVal)).max_size();
    else if (v->t == dynamic::type::STRING)
        return (*(v->v.stringVal)).max_size();
    else
        throw domain_error("dynamic value type must be string, array, or map to have a max size");
}

void dynamic::resize(const size_t s) {
    if (v->t == dynamic::type::ARRAY)
        (*(v->v.arrayVal)).resize(s);
    else if (v->t == dynamic::type::STRING)
        (*(v->v.stringVal)).resize(s);
    else
        throw domain_error("dynamic value type must be string or array to resize");
}

size_t dynamic::capacity() const {
    if (v->t == dynamic::type::ARRAY)
        return (*(v->v.arrayVal)).capacity();
    else if (v->t == dynamic::type::STRING)
        return (*(v->v.stringVal)).capacity();
    else
        throw domain_error("dynamic value type must be string or array to have a capacity");
}

void dynamic::reserve(const size_t s) {
    if (v->t == dynamic::type::ARRAY)
        (*(v->v.arrayVal)).reserve(s);
    else if (v->t == dynamic::type::STRING)
        (*(v->v.stringVal)).reserve(s);
    else
        throw domain_error("dynamic value type must be string or array to reserve");
}

void dynamic::shrink_to_fit() {
    if (v->t == dynamic::type::ARRAY)
        (*(v->v.arrayVal)).shrink_to_fit();
    else if (v->t == dynamic::type::STRING)
        (*(v->v.stringVal)).shrink_to_fit();
    else
        throw domain_error("dynamic value type must be string or array to shrink");
}

void dynamic::assign(const size_t s, const dynamic &val) {
    if (v->t == type::ARRAY)
        (*(v->v.arrayVal)).assign(s, val);
    else
        throw domain_error("dynamic value type must be array to assign");
}

void dynamic::pop_back() {
    type_check(type::ARRAY);
    (*(v->v.arrayVal)).pop_back();
}

void dynamic::clear() {
    if (v->t == dynamic::type::MAP)
        (*(v->v.mapVal)).clear();
    else if (v->t == dynamic::type::ARRAY)
        (*(v->v.arrayVal)).clear();
    else
        throw domain_error("dynamic value is not an array or a map");
}

bool dynamic::empty() const {
    return size() == 0;
}

void dynamic::reset() {
    set_type(v->t);
}

void dynamic::erase(const dynamic &key) {
    type_check(dynamic::type::MAP);
    (*(v->v.mapVal)).erase(key);
}

void dynamic::erase(vector<dynamic>::const_iterator iter) {
    type_check(dynamic::type::ARRAY);
    (*(v->v.arrayVal)).erase(iter);
}

void dynamic::emplace(std::vector<dynamic>::const_iterator iter, const dynamic &val) {
    type_check(dynamic::type::ARRAY);
    (*(v->v.arrayVal)).emplace(iter, val);
}

void dynamic::emplace_back(const dynamic &val) {
    type_check(dynamic::type::ARRAY);
    (*(v->v.arrayVal)).emplace_back(val);
}

dynamic dynamic::deep_copy() const {
    dynamic ret;

    switch (v->t) {
        case dynamic::type::MAP:
            ret.set_type(dynamic::type::MAP);
            for (auto item : *this)
                ret[item.key()] = item.value().deep_copy();
            break;
        case dynamic::type::ARRAY:
            ret.set_type(dynamic::type::ARRAY);
            for (auto item : *this) {
                dynamic tmp(item.value().deep_copy());
                ret.push_back(tmp);
            }
            break;
        case dynamic::type::INT:
            ret = as_int();
            break;
        case dynamic::type::UINT:
            ret = as_uint();
            break;
        case dynamic::type::LONG:
            ret = as_long();
            break;
        case dynamic::type::ULONG:
            ret = as_ulong();
            break;
        case dynamic::type::DOUBLE:
            ret = as_double();
            break;
        case dynamic::type::BOOL:
            ret = as_bool();
            break;
        case dynamic::type::STRING:
            ret = as_string();
            break;
        default:
            break;
    }

    return ret;
}

void dynamic::type_check(const dynamic::type t) const {
    if (v->t != t)
        throw domain_error(
            fmt::format("dynamic value expected {}, got {}", TYPE_NAME.at(v->t), TYPE_NAME.at(t)));
}

bool dynamic::is_type(const dynamic::type t) const {
    return v->t == t;
}

string dynamic::str(const bool pretty) const {
    ostringstream output;
    to_string(pretty, output, 0);
    return output.str();
}

void dynamic::to_string(const bool pretty, std::ostream &s, const size_t indent) const {
    switch (v->t) {
        case type::NONE:
            s << "null";
            break;
        case type::INT:
            s << as_int();
            break;
        case type::UINT:
            s << as_uint();
            break;
        case type::LONG:
            s << as_long();
            break;
        case type::ULONG:
            s << as_ulong();
            break;
        case type::DOUBLE:
            s << as_double();
            break;
        case type::BOOL:
            if (as_bool())
                s << "true";
            else
                s << "false";
            break;
        case type::STRING:
            s << escape_string(as_string());
            break;
        case type::ARRAY:
            s << "[";
            if (size() > 0) {
                for (const dynamic &d : *(v->v.arrayVal)) {
                    d.to_string(pretty, s, indent);
                    s << ", ";
                }
                s.seekp(s.tellp() - (streamoff)2);
            }
            s << "]";
            break;
        case type::MAP:
            s << "{";
            if (size() > 0) {
                if (pretty)
                    s << '\n';
                for (const auto &p : *(v->v.mapVal)) {
                    if (pretty)
                        for (size_t i = 0; i <= indent; i++)
                            s << "    ";
                    p.first.to_string(pretty, s, indent);
                    s << ": ";
                    p.second.to_string(pretty, s, indent + 1);
                    s << ", ";
                    if (pretty)
                        s << '\n';
                }
                if (pretty)
                    s.seekp(s.tellp() - (streamoff)3);
                else
                    s.seekp(s.tellp() - (streamoff)2);
                if (pretty) {
                    s << '\n';
                    for (size_t i = 0; i < indent; i++)
                        s << "    ";
                }
            }
            s << "}";
            break;
        default:
            break;
    }
}

ostream &njones::operator<<(ostream &stream, const dynamic &d) {
    stream << d.str() << endl;
    return stream;
}
