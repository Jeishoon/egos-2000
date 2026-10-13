/*
 * (C) 2026, Cornell University
 * All rights reserved.
 *
 * Description: starter code for implementing printf()
 */

void terminal_write(const char *str, int len) {
    for (int i = 0; i < len; i++) {
        *(char*)(0x10000000) = str[i];
    }
}

/* Uncomment the code block below when implementing formatted output.
 */

#include <stdlib.h>  // for itoa() and utoa()
#include <string.h>  // for strlen() and strcat()
#include <stdarg.h>  // for va_start(), va_end(), va_arg() and va_copy()

char* ulltoa(unsigned long long int n, char *str, int base) {
    const char digits[] = "0123456789abcdefghijkklmnoopqrstuvwxyz";
    int i, j;
    unsigned remainder;
    char c;
    
    // invalid base
    if ((base < 2) || (base > 36)) {
        str[0] = '\0';
        return NULL;
    }

    i = 0;
    do {
        remainder = n % base;
        str[i] = digits[remainder];
        n = n / base;
        i++;
    } while (n != 0);
    str[i] = '\0';

    for (j = 0, i--; j < i; j++, i--) {
        c = str[j];
        str[j] = str[i];
        str[i] = c;
    }

    return str;
}

size_t format_to_str_len(const char* fmt, va_list args) {
    size_t len = 0;
    for (size_t i = 0; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            len++;
        } else {
            fmt++;
            char buff[24];
            if (*fmt == 's') {
                len += strlen(va_arg(args, char*));
            } else if (*fmt == 'd') {
                itoa(va_arg(args, int), buff, 10);
                len += strlen(buff);
            } else if (*fmt == 'c') {
                len += 1;
            } else if (*fmt == 'x') {
                itoa(va_arg(args, int), buff, 16);
                len += strlen(buff);
            } else if (*fmt == 'u') {
                utoa(va_arg(args, int), buff, 10);
                len += strlen(buff);
            } else if (*fmt == 'p') {
                itoa(va_arg(args, int), buff, 16);
                len += strlen(buff) + 2;
            } else if (*fmt == 'l' && *(fmt+1) == 'l' && *(fmt+2) == 'u') {
                ulltoa(va_arg(args, unsigned long long int), buff, 10);
                len += strlen(buff);
                fmt += 2;
            }
        }
    }
    return len;
}

void format_to_str(char* out, const char* fmt, va_list args) {
    for(out[0] = 0; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            strncat(out, fmt, 1);
        } else {
            fmt++;
            if (*fmt == 's') {
                strcat(out, va_arg(args, char*));
            } else if (*fmt == 'd') {
                itoa(va_arg(args, int), out + strlen(out), 10);
            } else if (*fmt == 'c') {
                int n = strlen(out);
                out[n] = (char) va_arg(args, int);
                out[n+1] = '\0';
            } else if (*fmt == 'x') {
                itoa(va_arg(args, int), out + strlen(out), 16);
            } else if (*fmt == 'u') {
                utoa(va_arg(args, int), out + strlen(out), 10);
            } else if (*fmt == 'p') {
                strcat(out, "0x");
                itoa(va_arg(args, int), out + strlen(out), 16);
            } else if (*fmt == 'l' && *(fmt+1) == 'l' && *(fmt+2) == 'u') {
                ulltoa(va_arg(args, unsigned long long int), out + strlen(out), 10);
                fmt += 2;
            }
        }
    }
}

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    va_list args_copy;
    va_copy(args_copy, args);
    unsigned int len = format_to_str_len(format, args_copy);
    char *buf = malloc(len + 1);
    if (buf == NULL) {
        va_end(args);
        va_end(args_copy);
        return -1;
    }

    format_to_str(buf, format, args);
    va_end(args);
    terminal_write(buf, strlen(buf));

    va_end(args_copy);
    free(buf);

    return 0;
}


/* Uncomment the code block below when implementing dynamic memory allocation.
 */

extern char __heap_start, __heap_end;
static char* brk = &__heap_start;
char* _sbrk(int size) {
    if (brk + size > (char*)&__heap_end) {
        terminal_write("_sbrk: heap grows too large\r\n", 29);
        return NULL;
    }

    char* old_brk = brk;
    brk += size;
    return old_brk;
}


int main() {
    char* msg = "Hello, World!\n\r";
    terminal_write(msg, 15);

    /* Uncomment this line of code when implementing formatted output. */
    printf("%s-%d is awesome!\n\r", "egos", 2000);
    printf("%d is a number\n\r", 1234);
    printf("%s is a string\n\r", "abcd");
    printf("%s-%d is awesome!\n\r", "egos", 2000);
    printf("%c is character $\n\r", '$');
    printf("%c is character 0\n\r", (char)48);
    printf("%x is integer 1234 in hexadecimal\n\r", 1234);
    printf("%u is the maximum of unsigned int\n\r", (unsigned int)0xFFFFFFFF);
    printf("%p is the hexadecimal address of the hello-world string\n\r", msg);
    printf("%llu is the maximum of unsigned long long\n\r", 0xFFFFFFFFFFFFFFFFULL);

    return 0;
}
