# Project Design for CS302 OS Project 2

Our choice is [proj19-process-memory-tracker](https://github.com/oscomp/proj19-process-memory-tracker).

## Project Background

1. What is memory?

In computing, memory is a device or system that is used to store information for immediate use in a computer or related computer hardware and digital electronic devices. The term memory is often synonymous with the term primary storage or main memory. An archaic synonym for memory is store.

Computer memory operates at a high speed compared to storage that is slower but offers higher capacities. If needed, contents of the computer memory can be transferred to storage; a common way of doing this is through a memory management technique called virtual memory.

2. What is memory leak and hazard of that?

In computer science, a memory leak is a type of resource leak that occurs when a computer program incorrectly manages memory allocations in a way that memory which is no longer needed is not released. A memory leak may also happen when an object is stored in memory but cannot be accessed by the running code. A memory leak has symptoms similar to a number of other problems and generally can only be diagnosed by a programmer with access to the programs' source code.

If a program has a memory leak and its memory usage is steadily increasing, there will not usually be an immediate symptom. Every physical system has a finite amount of memory, and if the memory leak is not contained (for example, by restarting the leaking program) it will eventually cause problems.

## Project Description

The project is to show real time statistics of process memory usage and detect memory leak. This project is based on `C/C++`, target on developing a tool on Linux platform, which can display the memory usage of the process you want to observe and the possible memory leakage problems in real time. The specific requirements are as follows:

- Real time statistics of the memory usage of each process and thread in the system

- Detect the memory leakage in a process, including: memory, file handle leakage, real-time display or output the detection to the specified file

## Implementation

The project is based on `C/C++`, which will be test in `Ubuntu`. Since in `Ubuntu`, memory is mapped to `/proc/` directory, thus, we can read memory informations from that.

Reference:

- [A tool for finding memory leaks in Linux](https://github.com/d99kris/heapusage)

## Expected goals

Since the whole project contains many steps, we will split them into several parts:

- Learn about how `Ubuntu` allocates memory (DDL: 5.7)

    For that `Ubuntu` map its memory to `/proc/`, we need to know that stucture of each process and where it place processes' informations.

- Show real time statistics system process and thread memory usage

    Code to achieve the statistics of memory usage information, and sort the memory statistics data and display them in real time

- Detect memory allocation release in a process (DDL: 5.14)

    - Code to detect memory allocation and release in specific process

    - Code to detect the allocation and release of file handle in specific process

- Detect whether there is a memory leak in a process (DDL: 5.21)

    Statistics process memory allocation and release, confirm whether there is a leak, if there is a leak, point out the suspicious code

- Implement a simple GUI

    Using GUI make it clear to show the result of our project

## Division of labor

| SID      | Name        | Task      |
| -------- | ----------- | --------- |
| 11813225 | WANG Yuchen | Designer  |
| 11812804 | DONG Zheng  | Developer |
| 11811305 | CUI Yusong  | Tester    |
