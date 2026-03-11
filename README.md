<h1 align="center">CPU Scheduling Algorithms Simulator (C++)</h1>

<p align="center">
This project implements multiple CPU scheduling algorithms in C++ and compares their performance
using metrics such as Waiting Time, Turnaround Time, and Response Time.
</p>

<hr>

<h2>Overview</h2>
<p>
This program simulates how an operating system schedules processes. 
Users provide arrival time and burst time for each process, and the program calculates 
performance metrics for different scheduling algorithms.
</p>

<hr>

<h2>Implemented Algorithms</h2>

<ul>
<li><b>First Come First Serve (FCFS)</b></li>
<li><b>Shortest Job First (SJF) – Non-Preemptive</b></li>
<li><b>Round Robin (RR)</b></li>
<li><b>Shortest Remaining Time First (SRTF) – Preemptive</b></li>
<li><b>Multi-Level Feedback Queue (MLFQ)</b></li>
</ul>

<hr>

<h2>Performance Metrics</h2>

<ul>
<li><b>Waiting Time (WT)</b> – Time a process waits in the ready queue.</li>
<li><b>Turnaround Time (TAT)</b> – Total time from arrival to completion.</li>
<li><b>Response Time (RT)</b> – Time from arrival until the process first gets CPU.</li>
</ul>

<p>The program calculates the <b>average values</b> of these metrics for each algorithm.</p>

<hr>

<h2>Input Format</h2>

<p>The user must enter:</p>

<ol>
<li>Number of processes</li>
<li>Arrival time and burst time for each process</li>
</ol>

<p><b>Example Input:</b></p>

<pre>
Enter number of processes: 3
Enter arrival and burst time:
0 5
1 3
2 8
</pre>

<hr>

<h2>Example Output</h2>

<pre>
[FCFS] Average Waiting Time: 4.33, Turnaround Time: 8.33, Response Time: 4.33
[SJF] Average Waiting Time: 3.00, Turnaround Time: 7.00, Response Time: 3.00
[Round Robin] Average Waiting Time: 5.33, Turnaround Time: 9.33, Response Time: 2.33
[SRTF] Average Waiting Time: 2.67, Turnaround Time: 6.67, Response Time: 2.00
[MLFQ] Average Waiting Time: 3.33, Turnaround Time: 7.33, Response Time: 2.33
</pre>

<hr>

<h2>How to Compile and Run</h2>

<h3>Step 1: Compile the Program</h3>

<pre>
g++ scheduling.cpp -o scheduling
</pre>

<h3>Step 2: Run the Program</h3>

<pre>
./scheduling
</pre>

<h3>Step 3: Provide Input</h3>

<p>Enter the number of processes and their arrival and burst times.</p>

<hr>

<h2>Features</h2>

<ul>
<li>Implements five major CPU scheduling algorithms</li>
<li>Calculates average performance metrics</li>
<li>Supports both preemptive and non-preemptive scheduling</li>
<li>Simulates Multi-Level Feedback Queue scheduling</li>
<li>Simple command-line interface</li>
</ul>

<hr>

<h2>Data Structures Used</h2>

<ul>
<li>Vectors – storing processes</li>
<li>Queues – Round Robin and MLFQ scheduling</li>
<li>Sorting – scheduling decisions</li>
</ul>

<hr>

<h2>Learning Objectives</h2>

<ul>
<li>Understanding CPU scheduling algorithms</li>
<li>Operating system process management</li>
<li>Performance comparison of scheduling strategies</li>
<li>Implementation of scheduling techniques in C++</li>
</ul>

<hr>

<h2>Author</h2>

<p>
Developed as part of an Operating Systems project to understand and compare CPU scheduling algorithms.
</p>
