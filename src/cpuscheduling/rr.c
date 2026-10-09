#include <stdio.h>

void rr(int processes[], int n, int burst[], int quantum) {
    int remaining[n], time = 0;
    for (int i = 0; i < n; i++) remaining[i] = burst[i];

    printf("\n--- Round Robin Scheduling (quantum=%d) ---\n", quantum);

    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0) {
                done = 0;
                if (remaining[i] > quantum) {
                    time += quantum;
                    remaining[i] -= quantum;
                    printf("P%d ran for %d, remaining=%d\n", processes[i], quantum, remaining[i]);
                } else {
                    time += remaining[i];
                    printf("P%d finished at time %d\n", processes[i], time);
                    remaining[i] = 0;
                }
            }
        }
        if (done) break;
    }
}
