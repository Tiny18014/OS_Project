# Operating Systems Project

This repository contains two multi-threading programs implemented in C:

1. **Dining Philosophers Problem**
2. **Producer-Consumer Problem**

## Dining Philosophers Problem

This problem is a classic synchronization problem. It involves philosophers who do only one of three things: think, eat, or pick up chopsticks. The challenge is to design a protocol such that no philosopher will starve.

### Code Explanation

- **Philosophers and Forks**: We use an array of `N` mutexes to represent forks and an array of `N` threads to represent philosophers.
- **Eating Limit**: Each philosopher can eat up to `EATING_LIMIT` times.
- **Synchronization**: Mutexes are used to handle the critical sections for picking up and putting down forks.
