#include <stdio.h>
#include <unistd.h>

int main(void) {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    printf("Hello World from host: %s\n", hostname);
}