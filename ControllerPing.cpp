/*
 * This accessory.cpp is configurated for light accessory
 */

#include "ControllerPing.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

bool pingClient(const char *address) {
    int pipe_arr[2];
    char buf[1024];
    char temp[1024];
    int packageRecv;
    
    //Create pipe - pipe_arr[0] is "reading end", pipe_arr[1] is "writing end"
    pipe(pipe_arr);
    
    if(fork() == 0) //child
    {
        dup2(pipe_arr[1], STDOUT_FILENO);
        execl("/sbin/ping", "ping", "-q", "-c 1", address, (char*)NULL);
    }
    else //parent
    {
        wait(NULL);
        read(pipe_arr[0], buf, 1024);
    }
    
    sscanf(buf, "%1023[0-9a-zA-Z. ():-\n], %d", temp, &packageRecv);
    
    printf("%s\n", temp);
    printf("%d\n", packageRecv);
    
    
    close(pipe_arr[0]);
    close(pipe_arr[1]);
    
    return packageRecv > 0;
}

int main () {
    //Test
    pingClient("127.1.0.100");
}
