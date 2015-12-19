#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main (int argc, char **argv)
{
    char msg_buf[100];
    char msg[] = "Hello ping, this is pong.";
    printf("[pong] started.\n");
    printf("[pong] recving msg...\n");
    sys_sync_receive(7, msg_buf, sizeof(msg_buf));
    printf("[pong] msg from proc %d: %s\n", 4, msg_buf);
    printf("[pong] sending msg...\n");
    sys_sync_send(7, msg, sizeof(msg));
    printf("[pong] msg sent.");
    while(1) ; 
    return 0;
}
