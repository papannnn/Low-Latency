# Emplace vs Push Back

## push_back()

This method is to insert element in a vector at the end of container

```c++
// C++ program to demonstrate the
// push_back() method

#include <iostream>
#include <vector>

using namespace std;

// Class
class GFG {

public:
    int x;

    // Parameterized Constructor
    GFG(int x)
        : x(x)
    {
        cout << "Constructor=>"
             << "x:" << x << endl;
    }

    // Copy Constructor
    GFG(const GFG& GFG)
        : x(GFG.x)
    {
        cout << "Copied=>"
             << "x:" << x << endl;
    }

    ~GFG()
    {
        cout << "destructor=>"
             << "x:" << x << endl;
    }
};

// Driver Code
int main()
{
    // Vector of object of GFG class
    // is created
    vector<GFG> vertices;
    cout << "length of vertices:" << vertices.size()
         << endl;

    // Inserting elements in the object
    // created using push_back() method
    cout << endl;
    // Custom input entries
    vertices.push_back(GFG(1));
    cout << endl;

    vertices.push_back(GFG(11));
    cout << endl;

    vertices.push_back(GFG(21));
    cout << endl;

    return 0;
}
```
```
length of vertices:0

Constructor=>x:1
Copied=>x:1
destructor=>x:1

Constructor=>x:11
Copied=>x:11
Copied=>x:1
destructor=>x:1
destructor=>x:11

Constructor=>x:21
Copied=>x:21
Copied=>x:1
Copied=>x:11
destructor=>x:1
destructor=>x:11
destructor=>x:21

destructor=>x:1
destructor=>x:11
destructor=>x:21
```

## emplace_back()

Instead of doing copy constructor, it's doing inplace construction using parameterized constructor.

```c++
// C++ Program to illustrate the high
// cost of copying the elements in
// vector in STL

#include <iostream>
#include <vector>
using namespace std;

// Class
class GFG {

public:
    float x, y;

    // Parameterized Constructor
    GFG(float x, float y)
        : x(x), y(y)
    {
    }

    // Copy Constructor
    GFG(const GFG& GFG)
        : x(GFG.x), y(GFG.y)
    {
        cout << "Copied" << endl;
    }
};

// Driver Code
int main()
{
    // Create an object of vector
    // class object
    vector<GFG> vertices;

    // Reserve the elements in the
    // vector using reserve() method
    vertices.reserve(3);

    // Add element to vector object
    // using emplace_back() method
    vertices.emplace_back(1, 2);
    cout << endl;

    vertices.emplace_back(4, 5);
    cout << endl;

    vertices.emplace_back(7, 8);
    cout << endl;

    return 0;
}
```

No output
```

```

## Tldr

`push_back` accepts the only object of the type if the constructor accept more than one arguments

`emplace_back` accept arguments of the constructor of the type.

## Source

https://www.geeksforgeeks.org/cpp/push_back-vs-emplace_back-in-cpp-stl-vectors/
