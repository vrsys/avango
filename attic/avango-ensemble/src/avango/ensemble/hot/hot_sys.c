/* 
 * Contents:  System functionality for HOT (implementation)
 *
 * Author:  Alexey Vaysburd, November 1996
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif

/* Print the specified message to stderr and exit.
 */
void hot_sys_vPanic(char *reason, va_list args) {
    fprintf(stderr, "PANIC:HOT:");
    vfprintf(stderr, reason, args);
    fprintf(stderr, "\n");
    exit(1);
}

/* Print the specified message to stderr.
 */
void hot_sys_vWarning(char *reason, va_list args) {
    fprintf(stderr, "WARNING:HOT:");
    vfprintf(stderr, reason, args);
    fprintf(stderr, "\n");
}

void hot_sys_Panic(char *reason, ...) {
    va_list args;
    va_start(args, reason);
    hot_sys_vPanic(reason, args);
    va_end(args);
    exit(1);
}

void hot_sys_Warning(char *reason, ...) {
    va_list args;
    va_start(args, reason);
    hot_sys_vWarning(reason, args);
    va_end(args);
}
