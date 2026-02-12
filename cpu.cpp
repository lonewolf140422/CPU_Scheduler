#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

// Structure to represent a process as used in the simulator project
struct Process
{
      int id;
      int arrivalTime;
      int burstTime;
      int remainingTime;
      int completionTime;
      int turnAroundTime;
      int waitingTime;
};

// Function prototypes
void displayResults(vector<Process> p, string algoName);
void fcfs(vector<Process> p);
void sjf(vector<Process> p);
void roundRobin(vector<Process> p, int quantum);

// Helper to sort processes by arrival time
bool compareArrival(Process a, Process b)
{
      return a.arrivalTime < b.arrivalTime;
}

// Function to display performance metrics: TAT and WT
void displayResults(vector<Process> p, string algoName)
{
      float totalWT = 0, totalTAT = 0;
      int n = p.size();

      cout << "\n--- " << algoName << " Results ---" << endl;
      cout << "PID\tArrival\tBurst\tWaiting\tTurnaround" << endl;

      for (int i = 0; i < n; i++)
      {
            totalWT += p[i].waitingTime;
            totalTAT += p[i].turnAroundTime;
            cout << p[i].id << "\t" << p[i].arrivalTime << "\t" << p[i].burstTime
                 << "\t" << p[i].waitingTime << "\t" << p[i].turnAroundTime << endl;
      }

      cout << fixed << setprecision(2);
      cout << "Average Waiting Time: " << totalWT / n << endl;
      cout << "Average Turnaround Time: " << totalTAT / n << endl;
}

// 1. First-Come-First-Serve (FCFS) Implementation
void fcfs(vector<Process> p)
{
      sort(p.begin(), p.end(), compareArrival);
      int currentTime = 0;
      for (int i = 0; i < p.size(); i++)
      {
            if (currentTime < p[i].arrivalTime)
                  currentTime = p[i].arrivalTime;
            p[i].completionTime = currentTime + p[i].burstTime;
            p[i].turnAroundTime = p[i].completionTime - p[i].arrivalTime;
            p[i].waitingTime = p[i].turnAroundTime - p[i].burstTime;
            currentTime = p[i].completionTime;
      }
      displayResults(p, "FCFS");
}

// 2. Shortest Job First (SJF) Implementation
void sjf(vector<Process> p)
{
      int n = p.size();
      int completed = 0, currentTime = 0;
      vector<bool> isCompleted(n, false);

      while (completed != n)
      {
            int idx = -1;
            int minBurst = 1e9;
            for (int i = 0; i < n; i++)
            {
                  if (p[i].arrivalTime <= currentTime && !isCompleted[i])
                  {
                        if (p[i].burstTime < minBurst)
                        {
                              minBurst = p[i].burstTime;
                              idx = i;
                        }
                  }
            }

            if (idx != -1)
            {
                  p[idx].completionTime = currentTime + p[idx].burstTime;
                  p[idx].turnAroundTime = p[idx].completionTime - p[idx].arrivalTime;
                  p[idx].waitingTime = p[idx].turnAroundTime - p[idx].burstTime;
                  isCompleted[idx] = true;
                  completed++;
                  currentTime = p[idx].completionTime;
            }
            else
            {
                  currentTime++;
            }
      }
      displayResults(p, "SJF");
}

// 3. Round Robin (RR) Implementation with Time Quantum
void roundRobin(vector<Process> p, int quantum)
{
      int n = p.size();
      queue<int> readyQueue;
      int currentTime = 0, completed = 0;
      vector<bool> mark(n, false);

      sort(p.begin(), p.end(), compareArrival);

      readyQueue.push(0);
      mark[0] = true;

      while (completed != n)
      {
            int i = readyQueue.front();
            readyQueue.pop();

            int execute = min(p[i].remainingTime, quantum);
            p[i].remainingTime -= execute;
            currentTime += execute;

            // Check for newly arrived processes
            for (int j = 0; j < n; j++)
            {
                  if (!mark[j] && p[j].arrivalTime <= currentTime)
                  {
                        readyQueue.push(j);
                        mark[j] = true;
                  }
            }

            if (p[i].remainingTime > 0)
            {
                  readyQueue.push(i);
            }
            else
            {
                  completed++;
                  p[i].completionTime = currentTime;
                  p[i].turnAroundTime = p[i].completionTime - p[i].arrivalTime;
                  p[i].waitingTime = p[i].turnAroundTime - p[i].burstTime;
            }

            if (readyQueue.empty() && completed != n)
            {
                  for (int j = 0; j < n; j++)
                  {
                        if (!mark[j])
                        {
                              readyQueue.push(j);
                              mark[j] = true;
                              currentTime = p[j].arrivalTime;
                              break;
                        }
                  }
            }
      }
      displayResults(p, "Round Robin");
}

int main()
{
      int n, quantum;
      cout << "Enter number of processes: ";
      cin >> n;

      vector<Process> p(n);
      for (int i = 0; i < n; i++)
      {
            p[i].id = i + 1;
            cout << "Enter Arrival and Burst Time for P" << p[i].id << ": ";
            cin >> p[i].arrivalTime >> p[i].burstTime;
            p[i].remainingTime = p[i].burstTime;
      }

      cout << "Enter Time Quantum for RR: ";
      cin >> quantum;

      fcfs(p);
      sjf(p);
      roundRobin(p, quantum);

      return 0;
}