#include <stdio.h>

void fcfs(int processes[], int n, int burst[]) {
    int waiting[n], turnaround[n];
    waiting[0] = 0;

    for (int i = 1; i < n; i++)
        waiting[i] = waiting[i-1] + burst[i-1];

    for (int i = 0; i < n; i++)
        turnaround[i] = waiting[i] + burst[i];

    printf("\n--- FCFS Scheduling ---\n");
    for (int i = 0; i < n; i++)
        printf("P%d: Waiting=%d, Turnaround=%d\n", processes[i], waiting[i], turnaround[i]);
}
