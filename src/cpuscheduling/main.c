#include <stdio.h>

// function declarations
void fcfs(int[], int, int[]);
void sjf(int[], int, int[]);
void rr(int[], int, int[], int);
void cfs(int[], int, int[]);

int main() {
    int processes[] = {1, 2, 3};
    int burst[] = {5, 9, 6};
    int n = 3;

    fcfs(processes, n, burst);
    sjf(processes, n, burst);
    rr(processes, n, burst, 3);
    cfs(processes, n, burst);

    return 0;
}
