#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

#include "numbers.h"

int clock_mtx[5][51] = {0};

static void print_help() {
    printf("tinyclock - small terminal clock\n\n"
           "Options:\n"
           "\t-h, --help         print this message\n"
           "\t-s, --stopwatch    use stopwatch mode, it will count from 0 until specified time\n"
           "\t-t, --timer        use timer mode, it will count from specified time to 0\n\n"
           "Time input:\n"
           "\tin timer and stopwatch modes you can set time in the next format:\n"
           "\th {time in hours} m {time in minutes} s {time in seconds}\n"
           "\texample: tinyclock -t m 1\n"
           "\tthis command will start timer that counts from 1 minute to 0\n"
    );
}

static void setnumber(int number, int start) {
    for (int i = 0; i < 5; i++) {
        for (int j = start; j < start + 6; j++) {
            clock_mtx[i][j] = numbers[number][i][j - start];
        }
    }
}

static void settime(int hours, int minutes, int seconds) {
    setnumber(hours/10, 0);
    setnumber(hours%10, 7);

    setnumber(minutes/10, 19);
    setnumber(minutes%10, 26);

    setnumber(seconds/10, 38);
    setnumber(seconds%10, 45);
}

static void print_clock() {
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    printf("\ec");
    printf("\e[H");

    printf("\e[0m");
    for (int i = 0; i < ((w.ws_row - 5) / 2) + 1; ++i) {
        for (int j = 0; j < w.ws_col; ++j) {
            if (i+1 == ((w.ws_row - 5) / 2) + 1) {
                break;
            }
            printf(" ");
        }
    }

    for (int i = 0; i < 5; ++i) {
        printf("\e[0m");
        for (int k = 0; k < (w.ws_col - 51) / 2 + 1; ++k) {
            printf(" ");
        }

        for (int j = 0; j < 51; ++j) {
            if (clock_mtx[i][j]) {
                printf("\e[42m ");
            } else {
                printf("\e[0m ");
            }
        }
        printf("\e[0m\n");
    }
}

static void clock_mode(int hours, int minutes, int seconds) {
    while (true) {
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

static void timer_mode(int hours, int minutes, int seconds) {
    while (true) {
        if (seconds < 0) {
            minutes--;
            seconds = 59;
        }

        if (minutes < 0) {
            hours--;
            minutes = 59;
        }

        if (hours < 0) {
            break;
        }

        settime(hours, minutes, seconds);

        print_clock();

        sleep(1);
        seconds--;
    }
}

static void stopwatch_mode(int h, int m, int s) {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    while (hours != h || minutes != m || seconds != s || h + m + s == 0) {
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
int main(int argc, char **argv) {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;

    bool stopwatch = false;
    bool timer = false;

    clock_mtx[1][15] = 1;
    clock_mtx[1][16] = 1;
    clock_mtx[3][15] = 1;
    clock_mtx[3][16] = 1;

    clock_mtx[1][34] = 1;
    clock_mtx[1][35] = 1;
    clock_mtx[3][34] = 1;
    clock_mtx[3][35] = 1;

    for (int i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
            print_help();
            return 0;
        } else if ((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--stopwatch") == 0)) {
            stopwatch = true;
        } else if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--timer") == 0)) {
            timer = true;
        } else if ((strcmp(argv[i], "h") == 0) && i + 1 < argc) {
            i++;
            sscanf(argv[i], "%d", &hours);
            if (hours > 99) {
                hours = 99;
            } else if (hours < 0) {
                hours = 0;
            }
        } else if ((strcmp(argv[i], "m") == 0) && i + 1 < argc) {
            i++;
            sscanf(argv[i], "%d", &minutes);
            if (minutes > 59) {
                minutes = 59;
            } else if (minutes < 0) {
                minutes = 0;
            }
        } else if ((strcmp(argv[i], "s") == 0) && i + 1 < argc) {
            i++;
            sscanf(argv[i], "%d", &seconds);
            if (seconds > 59) {
                seconds = 59;
            } else if (seconds < 0) {
                seconds = 0;
            }
        } else {
            print_help();
            return 0;
        }
    }

    if (timer && stopwatch) {
        fprintf(stdout, "Cannot use timer and stopwatch at the same time.");
        return 1;
    }

    if (stopwatch) {
        stopwatch_mode(hours, minutes, seconds);
    } else if (timer) {
        timer_mode(hours, minutes, seconds);
    } else {
        time_t rawtime;
        struct tm *timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        hours = timeinfo->tm_hour;
        minutes = timeinfo->tm_min;
        seconds = timeinfo->tm_sec;

        clock_mode(hours, minutes, seconds);
    }

    return 0;
}
