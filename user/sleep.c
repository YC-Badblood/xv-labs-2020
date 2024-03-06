#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("No arguments provided.\n");
        exit(1);
    }
    // if (argc > 2) {
    //     printf("excceed arguments");
    //     exit(1);
    // }
    int i;
    printf("(nothing happens for a little while)");
    for (i = 1; i < argc; i++){
        int time = atoi(argv[i]);
        sleep(time);        
    }
    exit(0);
}