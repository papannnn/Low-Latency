# Build a Thread-Safe Bounded Queue

## Problem

Implement a bounded blocking queue using condition variables and mutexes.

## Requirements

- Support multiple producers and consumers.
- Avoid deadlocks.
- Avoid missed wakeups.
- Handle spurious wakeups correctly.
