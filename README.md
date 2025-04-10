# Multi-Level Page Table Simulation in C

This project implements a simulation of a multi-level (trie-based) page table system for a 64-bit virtual memory model. The goal is to simulate how an operating system translates virtual page numbers (VPN) to physical page numbers (PPN) using a hierarchical structure of page tables.

This assignment was completed as part of the Operating Systems course at the Hebrew University.

## Files

- pt.c — implementation of page table traversal and update logic
- os.c — helper functions simulating physical memory allocation and translation
- os.h — function declarations and constants

## Compilation

Compile the project using:
gcc -O3 -Wall -std=c11 os.c pt.c


