#include <stdio.h>
#include <unistd.h>

#include "numbers.h"

int clock[5][55] = {0};

void setnumber(int number, int start) {
    for (int i = 0; i < 5; i++) {
        for (int j = start; j < start + 6; j++) {
            clock[i][j] = numbers[number][i][j - start];
        }
    }
}

void settime(int hours, int minutes, int seconds) {
    setnumber(hours/10, 0);
    setnumber(hours%10, 7);

    setnumber(minutes/10, 20);
    setnumber(minutes%10, 27);

    setnumber(seconds/10, 40);
    setnumber(seconds%10, 47);
}

void print_clock() {
    printf("\033c");
    printf("\033[H");
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 55; ++j) {
            if (clock[i][j]) {
                printf("\033[42m ");
            } else {
                printf("\033[0m ");
            }
        }
        printf("\n");
    }
}

void loop() {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    clock[1][15] = 1;
    clock[1][16] = 1;
    clock[3][15] = 1;
    clock[3][16] = 1;

    clock[1][35] = 1;
    clock[1][36] = 1;
    clock[3][35] = 1;
    clock[3][36] = 1;

    while (1) {
        if (seconds > 59) {
            minutes++;
            seconds = 0;
        }

        if (minutes > 59) {
            hours++;
            minutes = 0;
        }

        if (hours > 24) {
            break;
        }

        settime(hours, minutes, seconds);

        print_clock();

        sleep(1);
        seconds++;
    }
}

int main() {
    loop();

    return 0;
}
