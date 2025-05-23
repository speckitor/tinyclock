#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "numbers.h"

int clock[5][51] = {0};

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

    setnumber(minutes/10, 19);
    setnumber(minutes%10, 26);

    setnumber(seconds/10, 38);
    setnumber(seconds%10, 45);
}

void print_clock() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    printf("\033c");
    printf("\033[H");

    printf("\033[0m");
    for (int i = 0; i < ((w.ws_row - 5) / 2) + 1; ++i) {
        for (int j = 0; j < w.ws_col; ++j) {
            if (i+1 == ((w.ws_row - 5) / 2) + 1) {
                break;
            }
            printf(" ");
        }
    }

    for (int i = 0; i < 5; ++i) {
        printf("\033[0m");
        for (int k = 0; k < (w.ws_col - 51) / 2 + 1; ++k) {
            printf(" ");
        }

        for (int j = 0; j < 51; ++j) {
            if (clock[i][j]) {
                printf("\033[42m ");
            } else {
                printf("\033[0m ");
            }
        }
        printf("\033[0m\n");
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

    clock[1][34] = 1;
    clock[1][35] = 1;
    clock[3][34] = 1;
    clock[3][35] = 1;

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
