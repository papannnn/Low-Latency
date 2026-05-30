# Design a High-Performance Event Dispatcher

## Problem

Implement an event dispatcher where multiple producers publish events and multiple subscribers register callbacks.

## Requirements

- Support lock-free or low-lock event publication.
- Support safe subscription and unsubscription.
- Prevent use-after-free when a subscriber unsubscribes while events are being processed.
- Learn tradeoffs between shared ownership, weak ownership, and intrusive lifetime management.
