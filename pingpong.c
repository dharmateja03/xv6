#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2];  // parent to child
    int p2[2];  // child to parent
    char buf[1];
    
    pipe(p1);
    pipe(p2);
    
    int pid = fork();
    if(pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    
    if(pid == 0) {  // child
        close(p1[1]);
        close(p2[0]);
        
        if(read(p1[0], buf, 1) != 1) {
            fprintf(2, "child read failed\n");
            exit(1);
        }
        
        printf("%d: received ping\n", getpid());
        
        if(write(p2[1], "x", 1) != 1) {
            fprintf(2, "child write failed\n");
            exit(1);
        }
        
        exit(0);
    } else {  // parent
        close(p1[0]);
        close(p2[1]);
        
        if(write(p1[1], "x", 1) != 1) {
            fprintf(2, "parent write failed\n");
            exit(1);
        }
        
        if(read(p2[0], buf, 1) != 1) {
            fprintf(2, "parent read failed\n");
            exit(1);
        }
        
        printf("%d: received pong\n", getpid());
        
        exit(0);
    }
}