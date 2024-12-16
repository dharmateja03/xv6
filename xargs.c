#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
    char buf[512];
    char *xargv[MAXARG];
    int xargc = 0;
    
    if(argc < 2) {
        fprintf(2, "usage: xargs command [args...]\n");
        exit(1);
    }
    
    // Copy command and its arguments
    for(int i = 1; i < argc; i++) {
        xargv[xargc++] = argv[i];
    }
    
    char *p = buf;
    while(read(0, p, 1) == 1) {
        if(*p == '\n') {
            *p = 0;  // null terminate the string
            xargv[xargc] = 0;  // null terminate arg list
            
            int pid = fork();
            if(pid < 0) {
                fprintf(2, "fork failed\n");
                exit(1);
            }
            
            if(pid == 0) {  // child
                exec(xargv[0], xargv);
                fprintf(2, "exec failed\n");
                exit(1);
            }
            
            wait(0);  // parent waits for child
            p = buf;  // reset buffer for next line
        } else if(p < buf + sizeof(buf) - 1) {
            p++;
        }
        
        if(p == buf) {  // start of new line
            xargv[xargc] = buf;
        }
    }
    
    exit(0);
}