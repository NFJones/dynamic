# dynamic
dynamic is a library which provides a dynamic JSON-like type for C++ which aims to be simple and efficient to use.

## Example
```c++
#include <dynamic.hpp>
#include <iostream>

using namespace std;
using namespace njones;

int main(int argc, char **argv)
{
    try
    {
        dynamic d = 4;
        d = 5.0;
        d = "6";
        d = true;

        d.set_type(dynamic::type::MAP);

        d["key1"]["key2"] = nullptr;
        d.at("key1")["key3"] = "hello";
        d["anArray"].set_type(dynamic::type::ARRAY);
        d.at("anArray").push_back(true);
        d.at("anArray").push_back(4.2);
        d.at("anArray").push_back("a string");

        cout << d.str(true) << endl << endl;

        for(auto iter : d)
            cout << iter.key() << iter.value() << endl << endl;
        cout << endl;

        return 0;
    }
    catch(...)
    {
        return 1;
    }
}
```

## Output
```
{
    "anArray": [true, 4.2, "a string"],
    "key1": {
        "key3": "hello",
        "key2": null
    }
}


"anArray"
[true, 4, "a string"]


"key1"
{"key3": "hello", "key2": null}
```
