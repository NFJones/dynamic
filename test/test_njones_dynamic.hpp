#include <cxxtest/TestSuite.h>
#include <cmath>
#include <limits>
#include <iostream>

#include "dynamic.hpp"

using namespace std;

class dynamic_test_suite : public CxxTest::TestSuite {
   public:
    void test_creation() {
        njones::dynamic d;
        TS_ASSERT(true);
    }

    void test_copy_construction() {
        njones::dynamic original = numeric_limits<int>::max();
        njones::dynamic d(original);
        TS_ASSERT(original.as_int() == numeric_limits<int>::max());
        TS_ASSERT(original.get_type() == njones::dynamic::type::INT);
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::INT);
    }

    void test_move_construction() {
        njones::dynamic original = numeric_limits<int>::max();
        njones::dynamic d(move(original));
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::INT);
    }

    void test_null_construction() {
        njones::dynamic d(nullptr);
        TS_ASSERT(d.get_type() == njones::dynamic::type::NONE);
    }

    void test_int_construction() {
        njones::dynamic d(numeric_limits<int>::max());
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::INT);
    }

    void test_uint_construction() {
        njones::dynamic d(numeric_limits<unsigned int>::max());
        TS_ASSERT(d.as_uint() == numeric_limits<unsigned int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::UINT);
    }

    void test_long_construction() {
        njones::dynamic d(numeric_limits<long>::max());
        TS_ASSERT(d.as_long() == numeric_limits<long>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::LONG);
    }

    void test_ulong_construction() {
        njones::dynamic d(numeric_limits<unsigned long>::max());
        TS_ASSERT(d.as_ulong() == numeric_limits<unsigned long>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::ULONG);
    }

    void test_double_construction() {
        njones::dynamic d(numeric_limits<double>::max());
        TS_ASSERT(d.as_double() == numeric_limits<double>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::DOUBLE);
    }

    void test_bool_construction() {
        njones::dynamic d(true);
        TS_ASSERT(d.as_bool() == true);
        TS_ASSERT(d.get_type() == njones::dynamic::type::BOOL);
    }

    void test_string_construction() {
        njones::dynamic d;
        d = "A string"s;
        TS_ASSERT(d.as_string() == "A string");
        TS_ASSERT(d.get_type() == njones::dynamic::type::STRING);
    }

    void test_cstring_construction() {
        njones::dynamic d("A string");
        TS_ASSERT(d.as_string() == "A string");
        TS_ASSERT(d.get_type() == njones::dynamic::type::STRING);
    }

    void test_copy_assignment() {
        njones::dynamic original = numeric_limits<int>::max();
        njones::dynamic d;
        d = original;
        TS_ASSERT(original.as_int() == numeric_limits<int>::max());
        TS_ASSERT(original.get_type() == njones::dynamic::type::INT);
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::INT);
    }

    void test_move_assignment() {
        njones::dynamic original = numeric_limits<int>::max();
        njones::dynamic d;
        d = move(original);
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::INT);
    }

    void test_null_assignment() {
        njones::dynamic d;
        d = nullptr;
        TS_ASSERT(d.get_type() == njones::dynamic::type::NONE);
    }

    void test_int_assignment() {
        njones::dynamic d;
        d = numeric_limits<int>::max();
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::INT);
    }

    void test_uint_assignment() {
        njones::dynamic d;
        d = numeric_limits<unsigned int>::max();
        TS_ASSERT(d.as_uint() == numeric_limits<unsigned int>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::UINT);
    }

    void test_long_assignment() {
        njones::dynamic d;
        d = numeric_limits<long>::max();
        TS_ASSERT(d.as_long() == numeric_limits<long>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::LONG);
    }

    void test_ulong_assignment() {
        njones::dynamic d;
        d = numeric_limits<unsigned long>::max();
        TS_ASSERT(d.as_ulong() == numeric_limits<unsigned long>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::ULONG);
    }

    void test_double_assignment() {
        njones::dynamic d;
        d = numeric_limits<double>::max();
        TS_ASSERT(d.as_double() == numeric_limits<double>::max());
        TS_ASSERT(d.get_type() == njones::dynamic::type::DOUBLE);
    }

    void test_bool_assignment() {
        njones::dynamic d;
        d = true;
        TS_ASSERT(d.as_bool() == true);
        TS_ASSERT(d.get_type() == njones::dynamic::type::BOOL);
    }

    void test_string_assignment() {
        njones::dynamic d;
        d = "A string"s;
        TS_ASSERT(d.as_string() == "A string");
        TS_ASSERT(d.get_type() == njones::dynamic::type::STRING);
    }

    void test_cstring_assignment() {
        njones::dynamic d;
        d = "A string";
        TS_ASSERT(d.as_string() == "A string");
        TS_ASSERT(d.get_type() == njones::dynamic::type::STRING);
    }

    void test_int_cast() {
        njones::dynamic d   = numeric_limits<int>::max();
        int             val = static_cast<int>(d);
        TS_ASSERT(val == numeric_limits<int>::max());
    }

    void test_uint_cast() {
        njones::dynamic d   = numeric_limits<unsigned int>::max();
        unsigned int    val = static_cast<unsigned int>(d);
        TS_ASSERT(val == numeric_limits<unsigned int>::max());
    }

    void test_long_cast() {
        njones::dynamic d   = numeric_limits<long>::max();
        long            val = static_cast<long>(d);
        TS_ASSERT(val == numeric_limits<long>::max());
    }

    void test_ulong_cast() {
        njones::dynamic d   = numeric_limits<unsigned long>::max();
        unsigned long   val = static_cast<unsigned long>(d);
        TS_ASSERT(val == numeric_limits<unsigned long>::max());
    }

    void test_double_cast() {
        njones::dynamic d   = numeric_limits<double>::max();
        double          val = static_cast<double>(d);
        TS_ASSERT(val == numeric_limits<double>::max());
    }

    void test_bool_cast() {
        njones::dynamic d   = true;
        bool            val = static_cast<bool>(d);
        TS_ASSERT(val == true);
    }

    void test_string_cast() {
        njones::dynamic d   = "A string";
        string          val = static_cast<string>(d);
        TS_ASSERT(val == "A string");
    }

    void test_null_eq() {
        njones::dynamic a(nullptr);
        njones::dynamic b(nullptr);
        TS_ASSERT(a == b);
    }

    void test_int_eq() {
        njones::dynamic a(numeric_limits<int>::max());
        njones::dynamic b(numeric_limits<int>::max());
        TS_ASSERT(a == b);
    }

    void test_uint_eq() {
        njones::dynamic a(numeric_limits<unsigned int>::max());
        njones::dynamic b(numeric_limits<unsigned int>::max());
        TS_ASSERT(a == b);
    }

    void test_long_eq() {
        njones::dynamic a(numeric_limits<long>::max());
        njones::dynamic b(numeric_limits<long>::max());
        TS_ASSERT(a == b);
    }

    void test_ulong_eq() {
        njones::dynamic a(numeric_limits<unsigned long>::max());
        njones::dynamic b(numeric_limits<unsigned long>::max());
        TS_ASSERT(a == b);
    }

    void test_double_eq() {
        njones::dynamic a(numeric_limits<double>::max());
        njones::dynamic b(numeric_limits<double>::max());
        TS_ASSERT(a == b);
    }

    void test_bool_eq() {
        njones::dynamic a(true);
        njones::dynamic b(true);
        TS_ASSERT(a == b);
    }

    void test_string_eq() {
        njones::dynamic a("A string");
        njones::dynamic b("A string");
        TS_ASSERT(a == b);
    }

    void test_array_eq() {
        njones::dynamic a(njones::dynamic::type::ARRAY);
        njones::dynamic b(njones::dynamic::type::ARRAY);
        a.push_back(0);
        a.push_back(1.25);
        a.push_back("value");
        b.push_back(0);
        b.push_back(1.25);
        b.push_back("value");
        TS_ASSERT(a == b);
    }

    void test_map_eq() {
        njones::dynamic a(njones::dynamic::type::MAP);
        njones::dynamic b(njones::dynamic::type::MAP);
        a["key"] = "value";
        a[1]     = 1.25;
        a[true]  = false;
        b["key"] = "value";
        b[1]     = 1.25;
        b[true]  = false;
        TS_ASSERT(a == b);
    }

    void test_int_neq() {
        njones::dynamic a(numeric_limits<int>::max());
        njones::dynamic b(numeric_limits<int>::min());
        TS_ASSERT(a != b);
    }

    void test_uint_neq() {
        njones::dynamic a(numeric_limits<unsigned int>::max());
        njones::dynamic b(numeric_limits<unsigned int>::min());
        TS_ASSERT(a != b);
    }

    void test_long_neq() {
        njones::dynamic a(numeric_limits<long>::max());
        njones::dynamic b(numeric_limits<long>::min());
        TS_ASSERT(a != b);
    }

    void test_ulong_neq() {
        njones::dynamic a(numeric_limits<unsigned long>::max());
        njones::dynamic b(numeric_limits<unsigned long>::min());
        TS_ASSERT(a != b);
    }

    void test_double_neq() {
        njones::dynamic a(numeric_limits<double>::max());
        njones::dynamic b(numeric_limits<double>::min());
        TS_ASSERT(a != b);
    }

    void test_bool_neq() {
        njones::dynamic a(true);
        njones::dynamic b(false);
        TS_ASSERT(a != b);
    }

    void test_string_neq() {
        njones::dynamic a("A string");
        njones::dynamic b("A different string");
        TS_ASSERT(a != b);
    }

    void test_array_neq() {
        njones::dynamic a(njones::dynamic::type::ARRAY);
        njones::dynamic b(njones::dynamic::type::ARRAY);
        a.push_back("value");
        a.push_back(1.25);
        a.push_back(0);
        b.push_back(0);
        b.push_back(1.25);
        b.push_back("value");
        TS_ASSERT(a != b);
    }

    void test_map_neq() {
        njones::dynamic a(njones::dynamic::type::MAP);
        njones::dynamic b(njones::dynamic::type::MAP);
        a["key"] = "value";
        a[1]     = 1.25;
        a[true]  = false;
        b["key"] = "value";
        b[1]     = 1.25;
        b[true]  = true;
        TS_ASSERT(a != b);
    }

    void test_int_lt() {
        njones::dynamic a(numeric_limits<int>::min());
        njones::dynamic b(numeric_limits<int>::max());
        TS_ASSERT(a < b);
    }

    void test_uint_lt() {
        njones::dynamic a(numeric_limits<unsigned int>::min());
        njones::dynamic b(numeric_limits<unsigned int>::max());
        TS_ASSERT(a < b);
    }

    void test_long_lt() {
        njones::dynamic a(numeric_limits<long>::min());
        njones::dynamic b(numeric_limits<long>::max());
        TS_ASSERT(a < b);
    }

    void test_ulong_lt() {
        njones::dynamic a(numeric_limits<unsigned long>::min());
        njones::dynamic b(numeric_limits<unsigned long>::max());
        TS_ASSERT(a < b);
    }

    void test_double_lt() {
        njones::dynamic a(0.0);
        njones::dynamic b(1.123456);
        TS_ASSERT(a < b);
    }

    void test_bool_lt() {
        njones::dynamic a(false);
        njones::dynamic b(true);
        TS_ASSERT(a < b);
    }

    void test_string_lt() {
        njones::dynamic a("A lesser string");
        njones::dynamic b("A string");
        TS_ASSERT(a < b);
    }

    void test_array_lt() {
        njones::dynamic a(njones::dynamic::type::ARRAY);
        njones::dynamic b(njones::dynamic::type::ARRAY);
        a.push_back(0);
        a.push_back(1.25);
        a.push_back("value");
        b.push_back(0);
        b.push_back(1.25);
        TS_ASSERT(a < b);
    }

    void test_map_lt() {
        njones::dynamic a(njones::dynamic::type::MAP);
        njones::dynamic b(njones::dynamic::type::MAP);
        a["key"] = "value";
        a[1]     = 1.25;
        a[true]  = false;
        b[1]     = 1.25;
        b[true]  = false;
        TS_ASSERT(a < b);
    }

    void test_int_gt() {
        njones::dynamic a(numeric_limits<int>::max());
        njones::dynamic b(numeric_limits<int>::min());
        TS_ASSERT(a > b);
    }

    void test_uint_gt() {
        njones::dynamic a(numeric_limits<unsigned int>::max());
        njones::dynamic b(numeric_limits<unsigned int>::min());
        TS_ASSERT(a > b);
    }

    void test_long_gt() {
        njones::dynamic a(numeric_limits<long>::max());
        njones::dynamic b(numeric_limits<long>::min());
        TS_ASSERT(a > b);
    }

    void test_ulong_gt() {
        njones::dynamic a(numeric_limits<unsigned long>::max());
        njones::dynamic b(numeric_limits<unsigned long>::min());
        TS_ASSERT(a > b);
    }

    void test_double_gt() {
        njones::dynamic a(1.123456);
        njones::dynamic b(0.0);
        TS_ASSERT(a > b);
    }

    void test_bool_gt() {
        njones::dynamic a(true);
        njones::dynamic b(false);
        TS_ASSERT(a > b);
    }

    void test_string_gt() {
        njones::dynamic a("A string");
        njones::dynamic b("A lesser string");
        TS_ASSERT(a > b);
    }

    void test_array_gt() {
        njones::dynamic a(njones::dynamic::type::ARRAY);
        njones::dynamic b(njones::dynamic::type::ARRAY);
        a.push_back(0);
        a.push_back(1.25);
        b.push_back(0);
        b.push_back(1.25);
        b.push_back("value");
        TS_ASSERT(a > b);
    }

    void test_map_gt() {
        njones::dynamic a(njones::dynamic::type::MAP);
        njones::dynamic b(njones::dynamic::type::MAP);
        a[1]     = 1.25;
        a[true]  = false;
        b[1]     = 1.25;
        b[true]  = false;
        b["key"] = "value";
        TS_ASSERT(a > b);
    }

    void test_get_type() {
        njones::dynamic d(njones::dynamic::type::LONG);
        TS_ASSERT(d.get_type() == njones::dynamic::type::LONG);
    }

    void test_set_type() {
        njones::dynamic d;
        d.set_type(njones::dynamic::type::BOOL);
        TS_ASSERT(d.get_type() == njones::dynamic::type::BOOL);
    }

    void test_null_check() {
        const njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.is_null());
    }

    void test_int_check() {
        const njones::dynamic d(njones::dynamic::type::INT);
        TS_ASSERT(d.is_int());
    }

    void test_uint_check() {
        const njones::dynamic d(njones::dynamic::type::UINT);
        TS_ASSERT(d.is_uint());
    }

    void test_long_check() {
        const njones::dynamic d(njones::dynamic::type::LONG);
        TS_ASSERT(d.is_long());
    }

    void test_ulong_check() {
        const njones::dynamic d(njones::dynamic::type::ULONG);
        TS_ASSERT(d.is_ulong());
    }

    void test_double_check() {
        const njones::dynamic d(njones::dynamic::type::DOUBLE);
        TS_ASSERT(d.is_double());
    }

    void test_bool_check() {
        const njones::dynamic d(njones::dynamic::type::BOOL);
        TS_ASSERT(d.is_bool());
    }

    void test_string_check() {
        const njones::dynamic d(njones::dynamic::type::STRING);
        TS_ASSERT(d.is_string());
    }

    void test_array_check() {
        const njones::dynamic d(njones::dynamic::type::ARRAY);
        TS_ASSERT(d.is_array());
    }

    void test_map_check() {
        const njones::dynamic d(njones::dynamic::type::MAP);
        TS_ASSERT(d.is_map());
    }

    void test_int_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_int() == 0);
        d = numeric_limits<int>::max();
        TS_ASSERT(d.as_int() == numeric_limits<int>::max());
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_int() == static_cast<int>(1234));
        d = static_cast<long>(1234);
        TS_ASSERT(d.as_int() == static_cast<int>(1234));
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_int() == static_cast<int>(1234));
        d = static_cast<double>(1.25);
        TS_ASSERT(d.as_int() == static_cast<int>(1));
        d = true;
        TS_ASSERT(d.as_int() == static_cast<int>(1));
        try {
            d.set_type(njones::dynamic::type::STRING);
            d.as_int();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::ARRAY);
            d.as_int();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::MAP);
            d.as_int();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
    }

    void test_uint_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_uint() == 0);
        d = numeric_limits<unsigned int>::max();
        TS_ASSERT(d.as_uint() == numeric_limits<unsigned int>::max());
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_uint() == static_cast<unsigned int>(1234));
        d = static_cast<long>(1234);
        TS_ASSERT(d.as_uint() == static_cast<unsigned int>(1234));
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_uint() == static_cast<unsigned int>(1234));
        d = static_cast<double>(1.25);
        TS_ASSERT(d.as_uint() == static_cast<unsigned int>(1));
        d = true;
        TS_ASSERT(d.as_uint() == static_cast<unsigned int>(1));
        try {
            d.set_type(njones::dynamic::type::STRING);
            d.as_uint();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::ARRAY);
            d.as_uint();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::MAP);
            d.as_uint();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
    }

    void test_long_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_long() == 0);
        d = numeric_limits<int>::max();
        TS_ASSERT(d.as_long() == numeric_limits<int>::max());
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_long() == static_cast<long>(1234));
        d = static_cast<long>(1234);
        TS_ASSERT(d.as_long() == static_cast<long>(1234));
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_long() == static_cast<long>(1234));
        d = static_cast<double>(1.25);
        TS_ASSERT(d.as_long() == static_cast<long>(1));
        d = true;
        TS_ASSERT(d.as_long() == static_cast<long>(1));
        try {
            d.set_type(njones::dynamic::type::STRING);
            d.as_long();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::ARRAY);
            d.as_long();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::MAP);
            d.as_long();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
    }

    void test_ulong_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_ulong() == 0);
        d = numeric_limits<int>::max();
        TS_ASSERT(d.as_ulong() == numeric_limits<int>::max());
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_ulong() == static_cast<unsigned long>(1234));
        d = static_cast<long>(1234);
        TS_ASSERT(d.as_ulong() == static_cast<unsigned long>(1234));
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_ulong() == static_cast<unsigned long>(1234));
        d = static_cast<double>(1.25);
        TS_ASSERT(d.as_ulong() == static_cast<unsigned long>(1));
        d = true;
        TS_ASSERT(d.as_ulong() == static_cast<unsigned long>(1));
        try {
            d.set_type(njones::dynamic::type::STRING);
            d.as_ulong();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::ARRAY);
            d.as_ulong();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::MAP);
            d.as_ulong();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
    }

    void test_double_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_double() == 0.0);
        d = static_cast<int>(1234);
        TS_ASSERT(d.as_double() == static_cast<double>(1234));
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_double() == static_cast<double>(1234));
        d = static_cast<long>(1234);
        TS_ASSERT(d.as_double() == static_cast<double>(1234));
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_double() == static_cast<double>(1234));
        d = static_cast<double>(1.25);
        TS_ASSERT(d.as_double() == static_cast<double>(1.25));
        d = true;
        TS_ASSERT(d.as_double() == static_cast<double>(1));
        try {
            d.set_type(njones::dynamic::type::STRING);
            d.as_double();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::ARRAY);
            d.as_double();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
        try {
            d.set_type(njones::dynamic::type::MAP);
            d.as_double();
            TS_ASSERT(false);
        } catch (const domain_error &e) {
            TS_ASSERT(true);
        }
    }

    void test_bool_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_bool() == false);
        d = static_cast<int>(0);
        TS_ASSERT(d.as_bool() == false);
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_bool() == true);
        d = static_cast<long>(0);
        TS_ASSERT(d.as_bool() == false);
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_bool() == true);
        d = static_cast<double>(0.0);
        TS_ASSERT(d.as_bool() == false);
        d = true;
        TS_ASSERT(d.as_bool() == true);
        d = "A string";
        TS_ASSERT(d.as_bool() == true);
        d.set_type(njones::dynamic::type::ARRAY);
        TS_ASSERT(d.as_bool() == false);
        d.set_type(njones::dynamic::type::MAP);
        d["key"] = "value";
        TS_ASSERT(d.as_bool() == true);
    }

    void test_string_view() {
        njones::dynamic d(njones::dynamic::type::NONE);
        TS_ASSERT(d.as_string() == "null");
        d = static_cast<int>(0);
        TS_ASSERT(d.as_string() == "0");
        d = static_cast<unsigned int>(1234);
        TS_ASSERT(d.as_string() == "1234");
        d = static_cast<long>(0);
        TS_ASSERT(d.as_string() == "0");
        d = static_cast<unsigned long>(1234);
        TS_ASSERT(d.as_string() == "1234");
        d = static_cast<double>(1.25);
        TS_ASSERT(d.as_string() == "1.25");
        d = true;
        TS_ASSERT(d.as_string() == "true");
        d = "A string";
        TS_ASSERT(d.as_string() == "A string");
        d.set_type(njones::dynamic::type::ARRAY);
        TS_ASSERT(d.as_string() == "[]");
        d.set_type(njones::dynamic::type::MAP);
        d["key"] = "value";
        TS_ASSERT(d.as_string() == "{\"key\": \"value\"} ");
    }
};
