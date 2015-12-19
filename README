List here the name of your partner, answers to the questions in the "Question" section of the lab, the brief description of what you have implemented, and anything else you would like us to know.
bs744, hz322

We evenly contributed to this project, pair programming.

In part1, we implemented a IPC scheme. Details are:
kern/lib/ipc.c
kern/lib/ipc.h
ker/trap/TSyscall/TSyscall.c

When a msg is sent to process B from process A, process A stores the recv_pid, length and start address of message in msgBlock. Wakeup all threads sleeping on its send channel and sleep itself on its own recv channel.
When a msg is recv by process B sent from process A, process B sleeps on process A's send channel and then wakeup all threads sleeping on process A's recv channel. Process B then copy msg from the address space of process A into kernel space and then copy it out from kernel space to user space of process B.



In part2, we implemeted a shell that support the basic operations like cp(-r), mv, rm(-r), etc.
In this part, we create a user process called shell. We launch shell when kernel starts. Most of the operations are implemented in user level. for example:
1)cp/cp -r
2)mv
3)rm/rm -r
4)cd


Other operations are implemented in kernel level, because it will use some inode related system calls. For example:
1)ls
2)pwd
3)mkdir
4)touch
5)write
6)read
7)append

In this part, we find it much easy to implement some operations in user level rather than kernel level, because it will leverage a lot of apis that we implemented in the previous lab. This is more easy to debug. For example, we tried to implement cp/cp-r in kernel mode, however, we find it rather tricky to deal with inode locks. Sometimes it will get stuck. We then reimplemented all these operations in user level, and this makes things much easier to do.


We created 28 test cases for this project. Our program passed all of them.
Test cases are written in user/shell/shell.c
Please set the value of  variable 'mode' to 1,2,0 in order to enter different test mode.
You need to make the project again to change test mode.


