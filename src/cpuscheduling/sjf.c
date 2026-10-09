#include <stdio.h>
#include <stdbool.h>

void sjf(int processes[], int n, int burst[]) {
    int waiting[n], turnaround[n], completed[n];
    for (int i = 0; i < n; i++) completed[i] = 0;

    int time = 0, done = 0;
    printf("\n--- SJF Scheduling ---\n");

    while (done < n) {
        int shortest = -1;
        for (int i = 0; i < n; i++) {
            if (!completed[i] && (shortest == -1 || burst[i] < burst[shortest]))
                shortest = i;
        }
        waiting[shortest] = time;
        time += burst[shortest];
        turnaround[shortest] = time;
        completed[shortest] = 1;
        done++;
        printf("P%d: Waiting=%d, Turnaround=%d\n", processes[shortest], waiting[shortest], turnaround[shortest]);
    }
}
