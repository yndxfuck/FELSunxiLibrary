#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#ifndef __YFLOG_H
#define __YFLOG_H

#define LOG_TAG "FEL"

static void _log_func(char *tag, char *str) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    int hrs_s = tv.tv_sec % (60 * 60 * 24);
    int mins_s = hrs_s % (60 * 60);
    int hrs = hrs_s / (60 * 60);
    int mins = mins_s / 60;
    int secs = mins_s % 60;

    FILE *f = fopen("/tmp/.fel_log", "a");
    if (!f)
        return;
    fprintf(f, "[%s] [%02d:%02d:%02d] %s\n", tag, hrs, mins, secs, str);
    fclose(f);
}

static void LOG_clean() {
    unlink("/tmp/.yfio_log");
}

#define LOG(...) \
    do { \
        char mysuperduperpupersuperchar[2048]; \
        sprintf(mysuperduperpupersuperchar, __VA_ARGS__); \
        _log_func(LOG_TAG, mysuperduperpupersuperchar); \
    } while (0)

#endif /* __YFLOG_H */
