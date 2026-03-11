CPU Scheduling Algorithms Simulator (C++)
Overview

This project implements multiple CPU scheduling algorithms in C++. It simulates how an operating system schedules processes and calculates performance metrics such as Waiting Time, Turnaround Time, and Response Time.

The program takes process arrival times and burst times as input and evaluates different scheduling strategies.

Implemented Algorithms

The following scheduling algorithms are implemented:

First Come First Serve (FCFS)

Shortest Job First (SJF) – Non-Preemptive

Round Robin (RR)

Shortest Remaining Time First (SRTF) – Preemptive

Multi-Level Feedback Queue (MLFQ)

Performance Metrics

The program calculates the following metrics for each algorithm:

Waiting Time (WT)
Time a process waits in the ready queue.

Turnaround Time (TAT)
Total time from arrival to completion.

Response Time (RT)
Time from arrival until the process first gets CPU.

Average values for these metrics are displayed for comparison.
