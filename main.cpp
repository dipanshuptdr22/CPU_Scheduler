#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int start = -1;
    int completion = 0;
    int turnaround = 0;
    int waiting = 0;
    int response = -1;
};

bool compArrival(const Process& a, const Process& b){
    if (a.arrival == b.arrival)
        return a.pid < b.pid;

    return a.arrival < b.arrival;
}

void printStatistics(const vector<Process>& processes, const string& name){
    double totalWaiting = 0;
    double totalTurnaround = 0;
    double totalResponse = 0;

    for(const auto& p : processes){
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }

    int n = processes.size();

    cout << "\n[" << name << "]\n";
    cout << "Average Waiting Time    : "
         << fixed << setprecision(2)
         << totalWaiting / n << '\n';

    cout << "Average Turnaround Time : "
         << totalTurnaround / n << '\n';

    cout << "Average Response Time   : "
         << totalResponse / n << '\n';
}

void FCFS(vector<Process> processes) {
    sort(processes.begin(), processes.end(), compArrival);
    int currentTime = 0;
    for (auto &p : processes) {
        if (currentTime < p.arrival) currentTime = p.arrival;
        p.start = currentTime;
        currentTime += p.burst;
        p.completion = currentTime;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        p.response = p.start - p.arrival;
    }
    printStatistics(processes, "FCFS");
}

void SJF(vector<Process> processes) {

    sort(processes.begin(), processes.end(), compArrival);

    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    int nextArrival = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq; // {burst, index}

    while (completed < n) {

        while (nextArrival < n && processes[nextArrival].arrival <= currentTime) {
            pq.push({processes[nextArrival].burst, nextArrival});
            nextArrival++;
        }

        if (pq.empty()) {
           if(nextArrival < n)  currentTime = processes[nextArrival].arrival;
            continue;
        }

        int idx = pq.top().second;
        pq.pop();

        Process &p = processes[idx];

        p.start = currentTime;
        currentTime += p.burst;

        p.completion = currentTime;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        p.response = p.start - p.arrival;

        completed++;
    }

   printStatistics(processes, "SJF");
}


void SRTF(vector<Process> processes) {

    sort(processes.begin(), processes.end(), compArrival);

    int n = processes.size();

    for (auto &p : processes)
        p.remaining = p.burst;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq; // {remainingTime, index}

    int currentTime = 0;
    int completed = 0;
    int nextArrival = 0;

    while (completed < n) {

        while (nextArrival < n && processes[nextArrival].arrival <= currentTime) {
            pq.push({processes[nextArrival].remaining, nextArrival});
            nextArrival++;
        }

        if (pq.empty()) {
            currentTime = processes[nextArrival].arrival;
            continue;
        }

        int idx = pq.top().second;
        pq.pop();

        Process &p = processes[idx];

        if (p.start == -1)
            p.start = currentTime;

        // Execute for 1 unit
        p.remaining--;
        currentTime++;

        // Add processes that arrived during this unit
        while (nextArrival < n && processes[nextArrival].arrival <= currentTime) {
            pq.push({processes[nextArrival].remaining, nextArrival});
            nextArrival++;
        }

        if (p.remaining == 0) {

            completed++;

            p.completion = currentTime;
            p.turnaround = p.completion - p.arrival;
            p.waiting = p.turnaround - p.burst;
            p.response = p.start - p.arrival;

        } else {

            pq.push({p.remaining, idx});
        }
    }
    printStatistics(processes, "SRTF");
}


void RoundRobin(vector<Process> processes, int quantum) {
    sort(processes.begin(), processes.end(), compArrival);

    int n = processes.size();

    for (auto &p : processes)
        p.remaining = p.burst;

    queue<int> q;
    int currentTime = 0;
    int completed = 0;
    int nextArrival = 0;

    while (completed < n) {

        // Add all processes that have arrived
        while (nextArrival < n && processes[nextArrival].arrival <= currentTime) {
            q.push(nextArrival);
            nextArrival++;
        }

        // CPU idle
        if (q.empty()) {
            if(nextArrival < n)  currentTime = processes[nextArrival].arrival;
            continue;
        }

        int idx = q.front();
        q.pop();

        Process &p = processes[idx];

        if (p.start == -1)
            p.start = currentTime;

        int timeUsed = min(quantum, p.remaining);

        currentTime += timeUsed;
        p.remaining -= timeUsed;

        // Add processes that arrived during execution
        while (nextArrival < n &&
               processes[nextArrival].arrival <= currentTime) {
            q.push(nextArrival);
            nextArrival++;
        }

        if (p.remaining > 0) {
            q.push(idx);
        } else {
            p.completion = currentTime;
            p.turnaround = p.completion - p.arrival;
            p.waiting = p.turnaround - p.burst;
            p.response = p.start - p.arrival;
            completed++;
        }
    }
    printStatistics(processes, "RoundRobin");
}

void MLFQ(vector<Process> processes){
    sort(processes.begin(), processes.end(), compArrival);

    vector<queue<int>> q(3);
    int quantum[3] = {4, 8, 12};

    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    int nextArrival = 0;

    for (auto &p : processes)
        p.remaining = p.burst;

    while (completed < n){
        // Step 1: Add newly arrived processes
        while (nextArrival < n && processes[nextArrival].arrival <= currentTime){
            q[0].push(nextArrival);
            nextArrival++;
        }

        int lvl = -1;

        // Step 2: pick highest priority non-empty queue
        for (int i = 0; i < 3; i++){
            if (!q[i].empty()){
                lvl = i;
                break;
            }
        }

        // Step 3: if all empty → jump time
        if (lvl == -1){
            currentTime = processes[nextArrival].arrival;
            continue;
        }

        int idx = q[lvl].front();
        q[lvl].pop();

        Process &p = processes[idx];

        if (p.start == -1)
            p.start = currentTime;

        int execTime = min(quantum[lvl], p.remaining);

        currentTime += execTime;
        p.remaining -= execTime;

        // Step 4: add newly arrived processes after execution
        while (nextArrival < n && processes[nextArrival].arrival <= currentTime){
            q[0].push(nextArrival);
            nextArrival++;
        }

        // Step 5: move process
        if (p.remaining > 0){
            if (lvl < 2)
                q[lvl + 1].push(idx);
            else
                q[2].push(idx);
        }
        else{
            p.completion = currentTime;
            p.turnaround = p.completion - p.arrival;
            p.waiting = p.turnaround - p.burst;
            p.response = p.start - p.arrival;
            completed++;
        }
    }

    printStatistics(processes, "MLFQ");
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;
    vector<Process> processes(n);
    cout << "Enter arrival and burst time:\n";
    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cin >> processes[i].arrival >> processes[i].burst;
    }

    FCFS(processes);
    SJF(processes);
    RoundRobin(processes, 2);
    SRTF(processes);
    MLFQ(processes);

    return 0;
}
