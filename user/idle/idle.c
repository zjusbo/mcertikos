#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    printf ("idle\n");

    pid_t ping_pid, pong_pid, ding_pid;

    if ((ping_pid = spawn (1, 1000)) != NUM_IDS)
        printf ("ping in process %d.\n", ping_pid);
    else
        printf ("Failed to launch ping.\n");

    if ((pong_pid = spawn (2, 1000)) != NUM_IDS)
        printf ("pong in process %d.\n", pong_pid);
    else
        printf ("Failed to launch pong.\n");

    if ((ding_pid = spawn (3, 1000)) != NUM_IDS)
        printf ("ding in process %d.\n", ding_pid);
    else
        printf ("Failed to launch ding.\n");

    return 0;
}
