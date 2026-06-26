# RAII Patterns for Thread Ownership

## Thread Ownership

If you running a thread, thread must be `join()`-ed or `detach()`-ed before thread object got destroyed. If you fail to do this, you will get `std::terminate`. Making your program shutdown.

## First solution

You can create a RAII Thread class wrapper to prevent you forgetting to `join` the thread.

```c++
class threadRAII {
private:
    std::thread& th;
public:
    explicit threadRAII(std::thread& t ) : th(t) {

    }

    ~threadRAII() {
        if(th.joinable()) {
            th.join(); // join() in destructor
            std::cout<<"\n"<<__PRETTY_FUNCTION__<<" Destroyed Object & Automatic JOIN"<<std::endl;
        }
        std::cout<<"\n"<<__PRETTY_FUNCTION__<<" Destruction accomplished"<<std::endl;
    }

    //delete the copy constructor as thread cannot be copied or assigned
    //only can it be moved
    threadRAII(const threadRAII &)= delete;

    //delete the copy assignment operator
    threadRAII& operator=(const threadRAII &)= delete ;
};
```

This will automatically `join` the thread when the object got destroyed.

## `std::jthread`

In C++20, you don't need to create a wrapper class. There's `std::jthread` that do that.

## Source

https://stackoverflow.com/questions/57066847/why-must-one-call-join-or-detach-before-thread-destruction

https://gist.github.com/anubhavrohatgi/74137561707121a7ac604002d6ef240f

https://www.youtube.com/watch?v=_X9kY3FdUUo