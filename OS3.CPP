#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int pid;               // Process ID
    vector<int> max;       // Maximum resources needed by the process
    vector<int> allocated; // Resources currently allocated to the process
    vector<int> need;      // Resources still needed by the process
};

// Function to check if the system is in a safe state and find the safe sequence
bool isSafeState(vector<Process>& processes, vector<int>& available, vector<int>& safeSequence, int numProcesses, int numResources) {
    vector<int> work = available;  // Work vector represents available resources
    vector<bool> finish(numProcesses, false); // Finish vector to track finished processes
    int count = 0;  // Count of processes that can be safely executed

    while (count < numProcesses) {
        bool progressMade = false;
        for (int i = 0; i < numProcesses; i++) {
            // Check if the process has not finished and its needs can be satisfied by the current available resources
            if (!finish[i] && all_of(processes[i].need.begin(), processes[i].need.end(),
                [&work](int needed, size_t idx) { return needed <= work[idx]; })) {
                // Allocate resources to this process
                for (int j = 0; j < numResources; j++) {
                    work[j] += processes[i].allocated[j];
                }
                // Add the process to the safe sequence
                safeSequence.push_back(processes[i].pid);
                finish[i] = true; // Mark the process as finished
                count++;  // Increment the count of processes that can be executed safely
                progressMade = true;
                break;
            }
        }
        // If no progress is made in a full pass, the system is in an unsafe state
        if (!progressMade) {
            return false;
        }
    }
    return true;
}

int main() {
    int numProcesses, numResources;

    // Input number of processes and resources
    cout << "Enter the number of processes: ";
    cin >> numProcesses;
    cout << "Enter the number of resources: ";
    cin >> numResources;

    vector<Process> processes(numProcesses);
    vector<int> available(numResources);

    // Input maximum resources needed, allocated resources and available resources
    for (int i = 0; i < numProcesses; i++) {
        processes[i].pid = i;
        cout << "Enter maximum resources needed by process " << i << ": ";
        processes[i].max.resize(numResources);
        for (int j = 0; j < numResources; j++) {
            cin >> processes[i].max[j];
            processes[i].need.push_back(processes[i].max[j]);  // Initially, the need is equal to the max resources
        }
        cout << "Enter allocated resources for process " << i << ": ";
        processes[i].allocated.resize(numResources);
        for (int j = 0; j < numResources; j++) {
            cin >> processes[i].allocated[j];
            processes[i].need[j] -= processes[i].allocated[j];  // Update the need by subtracting allocated resources
        }
    }

    // Input available resources
    cout << "Enter the available resources: ";
    for (int i = 0; i < numResources; i++) {
        cin >> available[i];
    }

    // Try to find a safe sequence
    vector<int> safeSequence;
    if (isSafeState(processes, available, safeSequence, numProcesses, numResources)) {
        cout << "\nThe system is in a safe state." << endl;
        cout << "Safe sequence is: ";
        for (int i = 0; i < safeSequence.size(); i++) {
            cout << safeSequence[i] << " ";
        }
        cout << endl;
    } else {
        cout << "\nThe system is in an unsafe state!" << endl;
    }

    return 0;
}
