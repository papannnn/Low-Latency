# Encapsulation

## Definition

Encapsulation is one concept in OOP, it is to wrap the data member and function into 1 class.

```
<class>
Member
Method
</class>
```

## Implementation

### Declare member as private

Keep the class member private, so people can't access it directly

### Use getter and setter

To access and modify member, use getter and setter

## Code

```c++
#include <iostream>
#include <string>
using namespace std;

// Class representing a Programmer
class Programmer
{
  private:
    string name; // Private variable

  public:
    // Getter method to access the private data
    string getName()
    {
        return name;
    }

    // Setter method to modify the private data
    void setName(string newName)
    {
        name = newName;
    }
};

int main()
{
    Programmer p;
    p.setName("Geek");                        // Set the name
    cout << "Name=> " << p.getName() << endl; // Get the name
    return 0;
}
```

## Source

https://www.geeksforgeeks.org/cpp/encapsulation-in-cpp/
