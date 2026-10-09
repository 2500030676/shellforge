#include <stdio.h>

#define MAX_PAGES 100
#define MAX_FRAMES 20

void run_algorithm(int pages[], int page_count, int frame_count, int choice) {
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES] = {0};
    int frequency[MAX_FRAMES] = {0};
    int loaded_at[MAX_FRAMES] = {0};
    int next_fifo = 0;
    int page_faults = 0;
    int page_hits = 0;

    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;

    printf("\nPage\tFrames\t\tStatus\n");

    for (int i = 0; i < page_count; i++) {
        int page = pages[i];
        int slot = -1;

        // Check whether the page is already in a frame.
        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == page) {
                slot = j;
                break;
            }
        }

        int hit = (slot != -1);

        if (hit) {
            page_hits++;

            if (choice == 2)       // LRU: update last-used time
                last_used[slot] = i;

            if (choice == 3)       // LFU: increase frequency
                frequency[slot]++;
        } else {
            page_faults++;

            // Use an empty frame if one is available.
            for (int j = 0; j < frame_count; j++) {
                if (frames[j] == -1) {
                    slot = j;
                    break;
                }
            }

            // If all frames are full, choose one to replace.
            if (slot == -1) {
                if (choice == 1) {            // FIFO
                    slot = next_fifo;
                } else if (choice == 2) {     // LRU
                    slot = 0;
                    for (int j = 1; j < frame_count; j++) {
                        if (last_used[j] < last_used[slot])
                            slot = j;
                    }
                } else {                      // LFU
                    slot = 0;
                    for (int j = 1; j < frame_count; j++) {
                        if (frequency[j] < frequency[slot] ||
                            (frequency[j] == frequency[slot] &&
                             loaded_at[j] < loaded_at[slot])) {
                            slot = j;
                        }
                    }
                }
            }

            frames[slot] = page;
            last_used[slot] = i;
            frequency[slot] = 1;
            loaded_at[slot] = i;

            if (choice == 1)
                next_fifo = (next_fifo + 1) % frame_count;
        }

        printf("%d\t", page);

        for (int j = 0; j < frame_count; j++) {
            if (frames[j] == -1)
                printf("- ");
            else
                printf("%d ", frames[j]);
        }

        printf("\t%s\n", hit ? "Hit" : "Page Fault");
    }

    printf("\nTotal page hits: %d\n", page_hits);
    printf("Total page faults: %d\n", page_faults);
    printf("Hit ratio: %.2f%%\n", (double)page_hits / page_count * 100);
    printf("Page fault ratio: %.2f%%\n",
           (double)page_faults / page_count * 100);
}

int main(void) {
    int pages[MAX_PAGES];
    int page_count, frame_count, choice;

    printf("Page Replacement Algorithms\n");
    printf("1. FIFO\n");
    printf("2. LRU\n");
    printf("3. LFU\n");
    printf("Choose an algorithm: ");

    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("Invalid choice.\n");
        return 1;
    }

    printf("Enter number of pages (1-%d): ", MAX_PAGES);
    if (scanf("%d", &page_count) != 1 ||
        page_count < 1 || page_count > MAX_PAGES) {
        printf("Invalid number of pages.\n");
        return 1;
    }

    printf("Enter the page reference string: ");
    for (int i = 0; i < page_count; i++) {
        if (scanf("%d", &pages[i]) != 1) {
            printf("Invalid page value.\n");
            return 1;
        }
    }

    printf("Enter number of frames (1-%d): ", MAX_FRAMES);
    if (scanf("%d", &frame_count) != 1 ||
        frame_count < 1 || frame_count > MAX_FRAMES) {
        printf("Invalid number of frames.\n");
        return 1;
    }

    switch (choice) {
        case 1:
            printf("\nRunning FIFO...\n");
            break;
        case 2:
            printf("\nRunning LRU...\n");
            break;
        case 3:
            printf("\nRunning LFU...\n");
            break;
    }

    run_algorithm(pages, page_count, frame_count, choice);
    return 0;
}
