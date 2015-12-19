#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    printf ("idle\n");

    pid_t ping_pid, pong_pid, shell_pid;
    while(1) ;
/*    if ((ping_pid = spawn (1, 1000)) != -1)
        printf ("ping in process %d.\n", ping_pid);
    else
        printf ("Failed to launch ping.\n");
    
    if ((pong_pid = spawn (2, 1000)) != -1)
        printf ("pong in process %d.\n", pong_pid);
    else
        printf ("Failed to launch pong.\n");*/
    /*if ((shell_pid = spawn(5, 1000)) != -1){
        printf ("shell in process %d.\n", shell_pid);
    }else{
        printf ("failed to launch shell %d.\n", shell_pid);
    } */
    return 0;
}
