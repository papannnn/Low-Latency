# Rule of 3 / 5 / 0

## Rule of 3

If you declare one of these manually:
- Destructor
- Copy Constructor
- Copy Assignment operator

Then you need to explicitly define all of those.

### Why we need to do it?

Default constructor and Assignment operator only do shallow copy, to do deep copy, you need to define your own Copy Constructor and Copy Assignment.

Also, shallow copy data is pointed to it's own original address of the object. If you using default destructor, there's a chance you delete a data more than once.

### Example Code

```c++
// In the below C++ code, we have created
// a destructor, but no copy constructor
// and no copy assignment operator.
class Array
{
private:
    int size;
    int* vals;  
 
public:
    ~Array();
    Array( int s, int* v );
};
 
Array::~Array()
{
   delete vals;
   vals = NULL;
}
 
Array::Array( int s, int* v )
{
    size = s;
    vals = new int[ size ];
    std::copy( v, v + size, vals );
}
 
int main()
{
   int vals[ 4 ] = { 11, 22, 33, 44 }; 
   Array a1( 4, vals );

   // This line causes problems.
   Array a2( a1 );

   return 0;
}
```

In this example, we don't set any Copy constructor, that means `a1` and `a2` will point to same `vals`, when both `a1` and `a2` got destroyed, the destructor will destroy `vals` 2 times.

## Rule of 5

If you declare one of these manually:
- Destructor
- Copy Constructor
- Copy Assignment operator
- Move Constructor
- Move Assignment operator

You need to manually create all of those.

## Rule of 0

If all of your class member don't need to manage memory manually because they already RAII class. Don't need to create manually of

- Destructor
- Copy Constructor
- Copy Assignment operator
- Move Constructor
- Move Assignment operator

## Source

https://www.geeksforgeeks.org/cpp/rule-of-three-in-cpp/

https://www.geeksforgeeks.org/cpp/rule-of-five-in-cpp/

https://en.cppreference.com/cpp/language/rule_of_three