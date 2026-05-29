# Benchmarking False Sharing vs No False Sharing

## Problem

Write a benchmark where two threads increment separate variables placed:

- Adjacent in memory
- Separated by cache-line padding

## Requirements

- Measure the performance difference.
- Explain the reason at the hardware cache-coherency level.
- Discuss the MESI protocol.
