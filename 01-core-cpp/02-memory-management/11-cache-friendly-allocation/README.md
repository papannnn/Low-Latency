# Cache-Friendly Allocation

## Spatial Locality

Spatial locality is when a data is accessed is near another data that want to be accessed, like array.

That's why array access from index 0 - n is faster than accessing it randomly

And array access is faster than accessing linked list.

## Contiguous Allocation

Using `struct` also a way to make data contiguous. Because CPU getting data by cache line, which is 64 bytes. That means it will fetch 64 bytes in one time fetching. So make sure your data is near each other.

## Data Oriented Memory Design

SOA (Struct of Array) vs AOS (Array of Struct)

SOA

```c++
struct Data{
    int age[10000];
    int posX[10000];
    int poxY[10000];
} data;
```

AOS

```c++
struct Data {
    int age;
    int posX;
    int posY;
} data[10000];
```

Assuming there's a looping for calculating total age. SOA do better here, because you don't need to put posX and posY inside cache line.

## Source

https://www.geeksforgeeks.org/computer-organization-architecture/difference-between-spatial-locality-and-temporal-locality/