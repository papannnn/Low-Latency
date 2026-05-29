# Event-Driven and High-Performance Timers

## Topics

- Timer wheels or priority queues for managing multiple timers
- Using `std::condition_variable` for scheduling
- High-resolution timers
- Avoiding busy-waiting with efficiency

## Practice Ideas

- Build a timer manager for scheduling 1000+ tasks.
- Build a rate limiter using timers, such as one API call every X ms.
- Implement retry/backoff logic using timers plus exponential delays.
