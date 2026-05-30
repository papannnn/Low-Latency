# Implement a Concurrent Read-Mostly Data Structure

## Problem

Design a data structure optimized for heavy reads and rare writes, such as a configuration map or routing table.

Readers must not block each other. Writers must update safely without corrupting data.

## Possible Approaches

- Copy-on-write strategy
- RCU-style pattern
- Atomic pointer swaps

## Requirements

- Explain memory ordering requirements.
- Explain how to prevent use-after-free during updates.
