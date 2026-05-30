# Build a Custom Shared Library Loader

## Problem

Write a wrapper around dynamic loading mechanisms, such as `dlopen` and `dlsym` on Linux.

## Requirements

- Design RAII-based resource management.
- Support safe loading.
- Support safe unloading of shared libraries.
