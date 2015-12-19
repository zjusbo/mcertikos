#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    printf("[ping] ping started.\n");
    char msg[] = "Hello pong, this is ping.";
    unsigned int i;
    //ping 4, pong 5, ding 6
    printf("[ping] sending msg...\n");
    sys_sync_send(8, msg, sizeof(msg));
    printf("[ping] msg sent.\n");
    printf("[ping] recving msg...\n");
    sys_sync_receive(8, msg, sizeof(msg));
    printf("[ping] msg from proc %d: %s\n", 5, msg);
    while(1) ;
    return 0;

}
