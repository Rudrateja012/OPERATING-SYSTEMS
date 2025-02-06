
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Define a structure to represent each process
struct Process {
    int pid;               // Process ID
    vector<int> max;       // Maximum resources needed by the process
    vector<int> allocated; // Resources currently allocated to the process
    vector<int> need;      // Remaining resources needed by the process
};

// Function to check if system is in a safe state
bool isSafeState(vector<Process>& processes, vector<int>& available, int numResources) {
    vector<int> work = available;  // Work vector represents available resources
    vector<bool> finish(processes.size(), false); // Finish vector to track finished processes
    int completed = 0;

    while (completed < processes.size()) {
        bool progressMade = false;

        for (int i = 0; i < processes.size(); i++) {
            if (!finish[i] && all_of(processes[i].need.begin(), processes[i].need.end(),
                [&work](int needed, size_t idx) { return needed <= work[idx]; })) {
                // If all needed resources are less than or equal to the available resources

                // Add allocated resources to work vector
                for (int j = 0; j < numResources; j++) {
                    work[j] += processes[i].allocated[j];
                }
                finish[i] = true;
                completed++;
                progressMade = true;
                break;
            }
        }
        // If no progress is made, then it means the system is in an unsafe state
        if (!progressMade) {
            return false;
        }
    }
    return true;
}

// Function to allocate resources to the process if it's a safe request
bool requestResources(vector<Process>& processes, vector<int>& available, vector<int>& request, int pid, int numResources) {
    // Check if request is valid: should not exceed need and available resources
    for (int i = 0; i < numResources; i++) {
        if (request[i] > processes[pid].need[i]) {
            cout << "Error: Process " << pid << " is requesting more resources than needed.\n";
            return false;
        }
        if (request[i] > available[i]) {
            cout << "Error: Not enough resources available to satisfy the request.\n";
            return false;
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < numResources; i++) {
        available[i] -= request[i];
        processes[pid].allocated[i] += request[i];
        processes[pid].need[i] -= request[i];
    }

    // Check if the system is still in a safe state
    if (isSafeState(processes, available, numResources)) {
        cout << "Request granted for process " << pid << ".\n";
        return true;
    } else {
        // Rollback the allocation if not safe
        for (int i = 0; i < numResources; i++) {
            available[i] += request[i];
            processes[pid].allocated[i] -= request[i];
            processes[pid].need[i] += request[i];
        }
        cout << "Request denied for process " << pid << " due to safety concerns.\n";
        return false;
    }
}

// Function to release resources held by a process
void releaseResources(vector<Process>& processes, vector<int>& available, int pid, int numResources) {
    for (int i = 0; i < numResources; i++) {
        available[i] += processes[pid].allocated[i];
        processes[pid].allocated[i] = 0;
    }
    cout << "Resources released by process " << pid << ".\n";
}

int main() {
    // Number of processes and resources
    int numProcesses, numResources;
    cout << "Enter the number of processes: ";
    cin >> numProcesses;
    cout << "Enter the number of resources: ";
    cin >> numResources;

    vector<Process> processes(numProcesses);
    vector<int> available(numResources);  // Available resources

    // Input the maximum, allocated resources for each process
    for (int i = 0; i < numProcesses; i++) {
        processes[i].pid = i;
        processes[i].max.resize(numResources);
        processes[i].allocated.resize(numResources);
        processes[i].need.resize(numResources);

        cout << "Enter the maximum resources needed by process " << i << ": ";
        for (int j = 0; j < numResources; j++) {
            cin >> processes[i].max[j];
            processes[i].need[j] = processes[i].max[j];
        }

        cout << "Enter the allocated resources for process " << i << ": ";
        for (int j = 0; j < numResources; j++) {
            cin >> processes[i].allocated[j];
            processes[i].need[j] -= processes[i].allocated[j]; // Update the need
        }
    }

    // Input the available resources
    cout << "Enter the available resources: ";
    for (int i = 0; i < numResources; i++) {
        cin >> available[i];
    }

    // Simulation of resource requests and releases
    int choice;
    while (true) {
        cout << "\n1. Request Resources\n2. Release Resources\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int pid;
            vector<int> request(numResources);
            cout << "Enter process ID for resource request: ";
            cin >> pid;

            cout << "Enter the resource request for process " << pid << ": ";
            for (int i = 0; i < numResources; i++) {
                cin >> request[i];
            }
            // Try to allocate resources to the process
            requestResources(processes, available, request, pid, numResources);

        } else if (choice == 2) {
            int pid;
            cout << "Enter process ID to release resources: ";
            cin >> pid;

            // Release resources held by the process
            releaseResources(processes, available, pid, numResources);

        } else {
            break;
        }
    }

    return 0;
}
