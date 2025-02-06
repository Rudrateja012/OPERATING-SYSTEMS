#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Process {
    int pid;         
    int arrivalTime; 
    int burstTime;   
    int remainingTime; 
    int waitingTime;
    int turnaroundTime;
    int responseTime;   
    bool isFirstResponse; 
};
void roundRobinScheduling(vector<Process> &processes, int timeQuantum) {
    int currentTime = 0;
    queue<int> readyQueue;
    int n = processes.size();
    int completed = 0;
    for (int i = 0; i < n; i++) {
        processes[i].remainingTime = processes[i].burstTime;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
        processes[i].responseTime = -1;
        processes[i].isFirstResponse = true;
    }
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0) {
                bool alreadyInQueue = false;
                for (queue<int> temp = readyQueue; !temp.empty(); temp.pop()) {
                    if (temp.front() == i) {
                        alreadyInQueue = true;
                        break;
                    }
                }
                if (!alreadyInQueue) {
                    readyQueue.push(i);
                }
            }
        }

        if (!readyQueue.empty()) {
            int idx = readyQueue.front();
            readyQueue.pop();

            if (processes[idx].isFirstResponse) {
                processes[idx].responseTime = currentTime - processes[idx].arrivalTime;
                processes[idx].isFirstResponse = false;
            }

            int executeTime = min(timeQuantum, processes[idx].remainingTime);
            processes[idx].remainingTime -= executeTime;
            currentTime += executeTime;

            if (processes[idx].remainingTime == 0) {
                completed++;
                processes[idx].turnaroundTime = currentTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnaroundTime - processes[idx].burstTime;
            } else {
                readyQueue.push(idx);
            }
        } else {
            currentTime++;
        }
    }
}

void calculateAndDisplayMetrics(const vector<Process> &processes) {
    int n = processes.size();
    double totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;

    cout << "\nProcess	Arrival	Burst	Waiting	Turnaround	Response\n";
    for (const auto &p : processes) {
        cout << "P" << p.pid << "\t" << p.arrivalTime << "\t" << p.burstTime << "\t"
             << p.waitingTime << "\t" << p.turnaroundTime << "\t" << p.responseTime << "\n";

        totalWaitingTime += p.waitingTime;
        totalTurnaroundTime += p.turnaroundTime;
        totalResponseTime += p.responseTime;
    }

    cout << "\nAverage Waiting Time: " << totalWaitingTime / n;
    cout << "\nAverage Turnaround Time: " << totalTurnaroundTime / n;
    cout << "\nAverage Response Time: " << totalResponseTime / n << "\n";
}

int main() {
    int n, timeQuantum;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    cout << "Enter the time quantum: ";
    cin >> timeQuantum;

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time and burst time for process P" << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;
    }

    roundRobinScheduling(processes, timeQuantum);
    calculateAndDisplayMetrics(processes);

    return 0;
}
