#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAGES 100
#define PAGE_RANGE 10

// 函数声明
void generatePageSequence(int *pages, int n);
int fifo(int *pages, int n, int frames);
int opt(int *pages, int n, int frames);
int lru(int *pages, int n, int frames);
void printResults(const char *algorithm, int pageFaults, int n, int frames);

int main() {
    int n, frames;
    int pages[MAX_PAGES];

    // 输入页面引用串的长度和页面帧数
    printf("请输入页面引用串的长度: ");
    scanf("%d", &n);
    printf("请输入页面帧的数量（3-7）: ");
    scanf("%d", &frames);

    if (frames < 3 || frames > 7) {
        printf("页面帧的数量必须在 3 到 7 之间。\n");
        return 1;
    }

    // 生成随机页面引用串
    generatePageSequence(pages, n);

    printf("生成的页面引用串为:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", pages[i]);
    }
    printf("\n");

    // 执行并显示每种算法的结果
    printResults("FIFO", fifo(pages, n, frames), n, frames);
    printResults("OPT", opt(pages, n, frames), n, frames);
    printResults("LRU", lru(pages, n, frames), n, frames);

    return 0;
}

// 生成随机页面引用串
void generatePageSequence(int *pages, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        pages[i] = rand() % PAGE_RANGE;
    }
}

// FIFO 页面置换算法
int fifo(int *pages, int n, int frames) {
    int frame[frames], front = 0, pageFaults = 0;
    for (int i = 0; i < frames; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("页面 %d 替换了页面 %d\n", pages[i], frame[front]);
            frame[front] = pages[i];
            front = (front + 1) % frames;
            pageFaults++;
        }
    }
    return pageFaults;
}

// OPT 页面置换算法
int opt(int *pages, int n, int frames) {
    int frame[frames], pageFaults = 0;
    for (int i = 0; i < frames; i++) frame[i] = -1;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            int replaceIdx = -1, farthest = i;
            for (int j = 0; j < frames; j++) {
                int k;
                for (k = i + 1; k < n; k++) {
                    if (frame[j] == pages[k]) break;
                }
                if (k > farthest) {
                    farthest = k;
                    replaceIdx = j;
                }
            }
            if (replaceIdx == -1) replaceIdx = 0;
            printf("页面 %d 替换了页面 %d\n", pages[i], frame[replaceIdx]);
            frame[replaceIdx] = pages[i];
            pageFaults++;
        }
    }
    return pageFaults;
}

// LRU 页面置换算法
int lru(int *pages, int n, int frames) {
    int frame[frames], lastUsed[frames], pageFaults = 0;
    for (int i = 0; i < frames; i++) {
        frame[i] = -1;
        lastUsed[i] = -1;
    }

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < frames; j++) {
            if (frame[j] == pages[i]) {
                found = 1;
                lastUsed[j] = i;
                break;
            }
        }
        if (!found) {
            int lruIdx = 0;
            for (int j = 1; j < frames; j++) {
                if (lastUsed[j] < lastUsed[lruIdx]) lruIdx = j;
            }
            printf("页面 %d 替换了页面 %d\n", pages[i], frame[lruIdx]);
            frame[lruIdx] = pages[i];
            lastUsed[lruIdx] = i;
            pageFaults++;
        }
    }
    return pageFaults;
}

// 显示算法结果
void printResults(const char *algorithm, int pageFaults, int n, int frames) {
    double pageFaultRate = (double)pageFaults / n * 100.0;
    printf("\n%s 算法:\n", algorithm);
    printf("缺页次数: %d\n", pageFaults);
    printf("缺页率: %.2f%%\n", pageFaultRate);
}
