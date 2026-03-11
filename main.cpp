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

bool compArrival(const Process &a, const Process &b) {
    return a.arrival < b.arrival;
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
    cout << "\n[FCFS] Average Waiting Time: ";
    double totalWaiting = 0, totalTurnaround = 0, totalResponse = 0;
    for (auto p : processes) {
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }
    cout << fixed << setprecision(2) << totalWaiting / processes.size();
    cout << ", Turnaround Time: " << totalTurnaround / processes.size();
    cout << ", Response Time: " << totalResponse / processes.size() << endl;
}

void SJF(vector<Process> processes) {
    int currentTime = 0, completed = 0;
    vector<bool> isDone(processes.size(), false);
    while (completed < processes.size()) {
        int idx = -1, minBurst = 1e9;
        for (int i = 0; i < processes.size(); ++i) {
            if (!isDone[i] && processes[i].arrival <= currentTime && processes[i].burst < minBurst) {
                minBurst = processes[i].burst;
                idx = i;
            }
        }
        if (idx == -1) {
            currentTime++;
            continue;
        }
        auto &p = processes[idx];
        p.start = currentTime;
        currentTime += p.burst;
        p.completion = currentTime;
        p.turnaround = p.completion - p.arrival;
        p.waiting = p.turnaround - p.burst;
        p.response = p.start - p.arrival;
        isDone[idx] = true;
        completed++;
    }
    cout << "\n[SJF] Average Waiting Time: ";
    double totalWaiting = 0, totalTurnaround = 0, totalResponse = 0;
    for (auto p : processes) {
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }
    cout << fixed << setprecision(2) << totalWaiting / processes.size();
    cout << ", Turnaround Time: " << totalTurnaround / processes.size();
    cout << ", Response Time: " << totalResponse / processes.size() << endl;
}

void RoundRobin(vector<Process> processes, int quantum) {
    queue<int> q;
    vector<bool> inQueue(processes.size(), false);
    int currentTime = 0, completed = 0;

    for (auto &p : processes) p.remaining = p.burst;

    while (completed < processes.size()) {
        for (int i = 0; i < processes.size(); ++i) {
            if (!inQueue[i] && processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                q.push(i);
                inQueue[i] = true;
            }
        }
        if (q.empty()) {
            currentTime++;
            continue;
        }
        int i = q.front(); q.pop();
        inQueue[i] = false;

        auto &p = processes[i];
        if (p.start == -1) p.start = currentTime;

        int timeUsed = min(quantum, p.remaining);
        currentTime += timeUsed;
        p.remaining -= timeUsed;

        for (int j = 0; j < processes.size(); ++j) {
            if (!inQueue[j] && processes[j].arrival <= currentTime && processes[j].remaining > 0)
                q.push(j), inQueue[j] = true;
        }

        if (p.remaining > 0) q.push(i), inQueue[i] = true;
        else {
            p.completion = currentTime;
            p.turnaround = p.completion - p.arrival;
            p.waiting = p.turnaround - p.burst;
            p.response = p.start - p.arrival;
            completed++;
        }
    }
    cout << "\n[Round Robin] Average Waiting Time: ";
    double totalWaiting = 0, totalTurnaround = 0, totalResponse = 0;
    for (auto p : processes) {
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }
    cout << fixed << setprecision(2) << totalWaiting / processes.size();
    cout << ", Turnaround Time: " << totalTurnaround / processes.size();
    cout << ", Response Time: " << totalResponse / processes.size() << endl;
}

void SRTF(vector<Process> processes) {
    int n = processes.size();
    for (auto &p : processes) p.remaining = p.burst;
    int currentTime = 0, completed = 0, minRem = 1e9, idx = -1;
    bool found = false;

    while (completed != n) {
        minRem = 1e9; found = false;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0 && processes[i].remaining < minRem) {
                minRem = processes[i].remaining;
                idx = i;
                found = true;
            }
        }

        if (!found) {
            currentTime++;
            continue;
        }

        if (processes[idx].start == -1) processes[idx].start = currentTime;

        processes[idx].remaining--;
        currentTime++;

        if (processes[idx].remaining == 0) {
            completed++;
            processes[idx].completion = currentTime;
            processes[idx].turnaround = processes[idx].completion - processes[idx].arrival;
            processes[idx].waiting = processes[idx].turnaround - processes[idx].burst;
            processes[idx].response = processes[idx].start - processes[idx].arrival;
        }
    }

    cout << "\n[SRTF] Average Waiting Time: ";
    double totalWaiting = 0, totalTurnaround = 0, totalResponse = 0;
    for (auto p : processes) {
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }
    cout << fixed << setprecision(2) << totalWaiting / n;
    cout << ", Turnaround Time: " << totalTurnaround / n;
    cout << ", Response Time: " << totalResponse / n << endl;
}

void MLFQ(vector<Process> processes) {
    vector<queue<int>> queues(3); // 3 level queues
    int quantum[3] = {4, 8, 12};
    int currentTime = 0, completed = 0;
    for (auto &p : processes) p.remaining = p.burst;
    vector<bool> added(processes.size(), false);

    while (completed < processes.size()) {
        for (int i = 0; i < processes.size(); ++i) {
            if (!added[i] && processes[i].arrival <= currentTime) {
                queues[0].push(i);
                added[i] = true;
            }
        }

        bool found = false;
        for (int lvl = 0; lvl < 3; ++lvl) {
            if (queues[lvl].empty()) continue;
            int i = queues[lvl].front(); queues[lvl].pop();
            auto &p = processes[i];
            if (p.start == -1) p.start = currentTime;
            int useTime = min(quantum[lvl], p.remaining);
            currentTime += useTime;
            p.remaining -= useTime;

            for (int j = 0; j < processes.size(); ++j) {
                if (!added[j] && processes[j].arrival <= currentTime) {
                    queues[0].push(j);
                    added[j] = true;
                }
            }

            if (p.remaining > 0) {
                if (lvl < 2) queues[lvl+1].push(i);
                else queues[2].push(i);
            } else {
                p.completion = currentTime;
                p.turnaround = p.completion - p.arrival;
                p.waiting = p.turnaround - p.burst;
                p.response = p.start - p.arrival;
                completed++;
            }
            found = true;
            break;
        }

        if (!found) currentTime++;
    }

    cout << "\n[MLFQ] Average Waiting Time: ";
    double totalWaiting = 0, totalTurnaround = 0, totalResponse = 0;
    for (auto p : processes) {
        totalWaiting += p.waiting;
        totalTurnaround += p.turnaround;
        totalResponse += p.response;
    }
    cout << fixed << setprecision(2) << totalWaiting / processes.size();
    cout << ", Turnaround Time: " << totalTurnaround / processes.size();
    cout << ", Response Time: " << totalResponse / processes.size() << endl;
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
