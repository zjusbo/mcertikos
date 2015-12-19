#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>
#include <debug.h>
#include <file.h>
#include <gcc.h>
#include <proc.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <proc.h>
#include <syscall.h>
#include <x86.h>

#define BUFLEN 1024
#define ARGUMENT_LEN 128
#define ARG_COUNT 64
#define CMDBUF_SIZE	80
#define WHITESPACE "\t\r\n "
#define MAXARGS 16
static int  runcmd (char *buf);
int shell_ls(int argc, char ** argv);
int shell_pwd(int argc, char **argv);
int shell_cd(int argc, char **argv);
int shell_cp(int argc, char **argv);
int shell_mv(int argc, char **argv);
int shell_rm(int argc, char **argv);
int shell_mkdir(int argc, char **argv);
int shell_cat(int argc, char **argv);
int shell_touch(int argc, char **argv);
int shell_help(int argc, char **argv);
int shell_write(int argc, char **argv);
int shell_append(int argc, char **argv);


int _shell_rm(char * path, int isRecursive);
int rm_file(char * filename);
int ls_dir(char* buf, char* path);
int is_dir_empty(char* dirname);
int is_dir(char * path);
int is_file_exist(char* path);
int _shell_cat(char * path);
int _shell_cp(char * dest_path, char * src_path, int isRecursive);
int extract_filename(char * path, char * filename);
int cp_file(char * dest_filename, char * src_filename);

char shell_buf[BUFLEN];

struct Command
{
	const char* name;
	const char* desc;
	int 
	(*func) (int argc, char** argv);
};

static struct Command commands[] = 
{
	{"ls","list all files and directories under working directory", shell_ls},
	{"pwd","print working directory", shell_pwd},
	{"cd","cd <path> \n\t change directory", shell_cd},
	{"cp", "cp <-r> <src_path> <dest_path> \n\t copy file or directory ",shell_cp},
	{"mv", "mv <src_path> <dest_path> \n\t move file or directory",shell_mv},
	{"rm", "rm <-r> <filename> \n\t remove file or directory",shell_rm},
	{"mkdir", "mkdir <dirname> \n\t create directory",shell_mkdir},
	{"cat", "cat <filename> \n\t print file content",shell_cat},
	{"touch", "touch <fielname> \n\t create new empty file", shell_touch},
       {"write", "write", shell_write},
        {"append", "append", shell_append},
        {"help", "help \n\t print this help message", shell_help}
};

#define NCOMMANDS (sizeof(commands)/sizeof(commands[0]))


int shell_help(int argc, char** argv){
    int i = 0;
    for(i = 0; i < NCOMMANDS; i++){
        printf("%s\n", commands[i].desc);
    }
    return 0;
}

int shell_ls(int argc, char** argv)
{
	if (argc == 1){
	  sys_ls(shell_buf, sizeof(shell_buf));
          printf("%s\n", shell_buf);
        }else if (argc == 2) {
          char path[100];
          sys_pwd(path);
          sys_chdir(argv[1]);
          sys_ls(shell_buf, sizeof(shell_buf));
          printf("%s\n", shell_buf);
          sys_chdir(path);
        }else{
          printf("ls: too many arguments.\n"); 
        }
        return 0;

}

int shell_pwd(int argc, char ** argv)
{
        
	sys_pwd(shell_buf);
        printf("%s\n",shell_buf);
	return 0;	
}

int shell_cd(int argc, char** argv)
{
	char path[1024];
	if (argc == 1){
		strcpy(path, '\0');
		sys_chdir(path);
	}
	else {
		strcpy(path, argv[1]);
	//	printf("%s\n",path);
		sys_chdir(path);	
	}

}
int shell_cp(int argc, char** argv)
{
  char * src_path, * dest_path;
  if(argc < 3){
    printf("cp: too few arguments.\n");
    return 0;
  }
  else if(argc > 4){
    printf("cp: too many arguments.\n");
    return 0;
  }
  if(argc == 3){
   // ordinary cp
    src_path = argv[1];
    dest_path = argv[2];
    _shell_cp(dest_path, src_path, 0);
    return 0;
  }	
  else{// recursive
    if(strcmp(argv[1], "-r")){
      printf("cp: invalid option. try '-r' ?\n");
      return 0;
    }
    // recursive cp
    src_path = argv[2];
    dest_path = argv[3];
    _shell_cp(dest_path, src_path, 1);
    return 0;
  }
}

//int ls_dir(char* buf);
//int is_dir_empty(char* dirname);
//int is_dir(char * path);
//int is_file_exist(char* path);

int _shell_cp(char * dest_path, char * src_path, int isRecursive){
  char path[BUFLEN];
  char filename[100];
  char dest_path_buf[BUFLEN];
  char src_path_buf[BUFLEN];
  char * p;
  if(!is_file_exist(src_path)){
    printf("cp: %s does not exist.\n", src_path);
    return 0;
  }
  if(isRecursive == 0){
    // common use
    if(is_dir(src_path)){
      printf("cp: omitting directory '%s'. try '-r' ?\n", src_path);
      return 0;
    }
    // src is a file
    if(is_file_exist(dest_path) && is_dir(dest_path)){
        // dest is a dir
        extract_filename(src_path, filename);
        strcpy(path, dest_path);
        p = path + strlen(path);
        *(p++) = '/';
        strcpy(p, filename);
        _shell_cp(path, src_path, isRecursive); 
    }else{
        // dest is a file or does not exist
        cp_file(dest_path, src_path);
    }
  }else{
    // recursive
    if(is_dir(src_path)){
       // src is a dir
       if(is_file_exist(dest_path)){
          // dest exist
          if(is_dir(dest_path)){
            extract_filename(src_path, filename);
            strcpy(path, dest_path);
            p = path + strlen(path);
            *(p++) = '/';
            strcpy(p, filename);
            _shell_cp(path, src_path, isRecursive);            
          }else{
            // dest is a file
            printf("cp: can not copy a dir to a file '%s'.\n", dest_path);
            return 0;
          }
       }else{
          // dest does not exist, do it recursively
          // preorder traversal
          cp_file(dest_path, src_path);
          int len = ls_dir(path, src_path);
          char* p = path;
          while(p - path < len){
            int dest_len, src_len;
            if(strcmp(p, ".") && strcmp(p, "..")){
              dest_len = strlen(dest_path);
              src_len = strlen(src_path);

              strcpy(dest_path_buf, dest_path);
              strcpy(src_path_buf, src_path);

              dest_path_buf[dest_len] = '/';
              src_path_buf[src_len] = '/';
              strcpy(dest_path_buf+dest_len+1, p);
              strcpy(src_path_buf+src_len+1, p);

              _shell_cp(dest_path_buf, src_path_buf, isRecursive);
            }
            p += strlen(p) + 1; // 
          }
       }
    }else{
      // src is a file
      // do non recursive cp
      _shell_cp(dest_path, src_path, 0);
    }

  }
  return 0;
}


int shell_mv(int argc, char** argv)
{
        if(argc != 3){
           printf("mv: argument invalid.\n");
           return 0;
        }
        char* src = argv[1];
        char* dest = argv[2];
        if(!is_file_exist(src)){
          printf("mv: sorce file %s does not exist.\n", src);
          return 0;
        }
        if (is_dir(src)) {
          if (is_file_exist(dest) &&  !is_dir(dest)) {
            printf("mv: cannot move a dir to a file\n");
            return 0;
          }
          _shell_cp(dest, src, 1);
          _shell_rm(src, 1);
        }else {
          _shell_cp(dest, src, 1);
          _shell_rm(src, 0);
        }
	return 0;
}

int shell_rm(int argc, char** argv)
{
        int fd, isRecursive;
        int pathIdx;
        char * path;
        // TODO support rm
      	if (argc == 1) {
        	printf("Too few arguments.\n");
		return 0;
	}
        if(!strcmp(argv[1], "-r")){
                isRecursive = 1;
                pathIdx = 2;
        }else{
                isRecursive = 0;
                pathIdx = 1;
        }
        if(pathIdx > argc + 1){
                printf("rm: no path argument.\n");
                return 0;
        }
        path = argv[pathIdx];
        if(!is_file_exist(path)){
            printf("rm: can not remove %s: is not a file or directory.\n", path);
            return 0;
        }
        _shell_rm(path, isRecursive);
        return 0;
}
// path is guaranteed to be exist
// DFS, postorder traversal
int _shell_rm(char *path, int isRecursive){
  int errno, len, i;
  char * sub_path;
  char rm_buf[BUFLEN];
  // -r option
  if(isRecursive){
    if(!is_dir(path)) {
      //delete single file or empty directory
      errno = rm_file(path);
      return errno;
    }
    else{
      // path is a non empty directory
      // subdirectory/subfile names are stored in shell_buf, seperated by '\0' 
      // chdir, go to next layer
      sys_chdir(path); 
      len = ls_dir(rm_buf, NULL);
      sub_path = rm_buf;
      while(sub_path - rm_buf < len){
        if(strcmp(sub_path, ".") && strcmp(sub_path, "..")){ 
          _shell_rm(sub_path, isRecursive);
        }
        sub_path += strlen(sub_path) + 1;
      }
      //chdir, go to upper layer
      sys_chdir("..");
      rm_file(path); // rm current dir
    }
    return 0;
  }else{
    if(is_dir(path)){
      printf("rm: can not remove %s: is a directory. try '-r' ?\n", path);
      return -1;
    }else{
      // delete single file
      return rm_file(path);
    }
  }
}

int rm_file(char * filename){
  int errno = sys_unlink(filename);
  if(errno == -1){
    printf("rm: can not remove %s: sys_unlink error.\n", filename);
  }
  return errno;
}
// return the length of buf used.
// buf stores the names of subfile/subdirectory under dirname.
// they are merged as one string while seperated by '\0'
// file_a  \0 file_b \0 dir_a \0 dir_b \0
int ls_dir(char* buf, char * path){
   int len;
   char pwd[BUFLEN];
   // list current dir
   if(path == NULL){
     len  = sys_ls(buf, BUFLEN); 
   }else{
      //list target dir
     sys_pwd(pwd);
     sys_chdir(path);
     len = sys_ls(buf, BUFLEN);
     sys_chdir(pwd); 
   }
   int i = 0;
   while(i < len){
      if(buf[i] == ' '){
          buf[i] = '\0';
      }
      i++;
   }
   return len;
}
int is_dir_empty(char * dirname){
    if(ls_dir(shell_buf, NULL) == 5){
        return 1;
    }else{
        return 0;
    }
}

int is_dir(char * path){
      int fd, isDir;
      if(is_file_exist(path)){
            fd = open(path, O_RDONLY);
      }
      isDir = sys_is_dir(fd);
      close(fd);
      return isDir;
}
// check weather a file/dir exist
int is_file_exist(char* path){
        int fd;
        fd = open(path, O_RDONLY);
	if(fd == -1){
                return 0;
 	}
	close(fd);
        return 1;
}
int shell_mkdir(int argc, char** argv)
{
	int i;
	if (argc == 1)
		printf ("mkdir failed, no path\n");
	
	for (i = 1; i < argc; i++){
		if (sys_mkdir(argv[i]) == 0)
		;	//printf("make dir succeed.\n");
		else
			printf("make dir failed.\n");
	}
	
	return 0;
}

int shell_cat(int argc, char** argv)
{
 	if (argc == 1) {
		printf("touch failed. No Path. \n");
		return;
	}
	int i;
	for (i = 1; i < argc; i++)
	{
                if(_shell_cat(argv[i]) == -1){
                      printf("Path is invalid. \n");
                }
	}

	return 0;
}
int _shell_cat(char * path){
   int fd, size_read;
   char buf[100];
   fd = open(path, O_RDONLY);
   if(fd == -1){
        // file does not exist
        return -1;
   } 
   size_read = read(fd, buf, sizeof(buf) - 1);
   buf[size_read] = '\0';
   printf("%s\n", buf);
   close(fd);
   return 0; // success
}

int shell_touch(int argc, char** argv)
{
	if (argc == 1) {
		printf("touch failed. No Path. \n");
		return;
	}
	int i;
	for (i = 1; i < argc; i++)
	{
                int fd;
                fd = open(argv[i], O_RDONLY);
		if(fd >= 0){
    			printf("%s file exist\n", argv[i]);
                        close(fd);
			continue;
  		} else {
			close(open(argv[i], O_CREATE));
			//printf("%s file create\n", argv[i]);
		}	
	}
	return 0;
}

int shell_write(int argc, char** argv) {
  if (argc == 1) {
    printf("write failed, too few arguments.\n");
    return 0;
  }
  int fd = open(argv[2], O_CREATE|O_RDWR);
  if (fd >= 0) {
    int n = write(fd, argv[1], strlen(argv[1]));
    if (n != strlen(argv[1])) {
      printf("shell write fail\n");
    }
    close(fd);
  }else {
    printf("shell write fail\n");
  }
  return 0;
}

int shell_append(int argc, char** argv) {
  if (argc == 1) {
    printf("append failed, too few arguments.\n");
    return 0;
  } 
  int fd = open(argv[2], O_RDONLY);
  if (fd >= 0) {
    char buf[1000];
    int n = read(fd, buf, 1000);
    close(fd);
    if (n == 0) {
      buf[0] = 0;
    }
    fd = open(argv[2], O_CREATE|O_RDWR);
    if (fd < 0) {
      printf("create: append failed!\n");
      return 0;
    }
    strncpy(buf + n, argv[1], strlen(argv[1]));
    write(fd, buf, strlen(buf));
    close(fd);
  }else {
    printf("open append failed!\n");
  }
  return 0;
}

void shell_test() {
  printf("start testing shell\n");
  const char* cmds[28] = {"ls", "pwd", "mkdir dir1", "mkdir dir2", "mkdir dir3", "ls",
                      "cd dir1", "mkdir dir1_1", "touch file1", "touch file2",
                      "pwd", "cd ..", "cp -r dir1 dir4", "cd dir4", "ls", "cd ..",
                      "write hello f1", "cp f1 f2", "ls", "cat f1", "append world f1",
                      "cat f1", "mv f1 dir2", "ls dir2", "rm f2", "ls", "rm -r dir4", "ls"};
  int i = 0;
  for (i = 0; i < 28; ++i) {
    printf("running test case %d :%s\n", i + 1, cmds[i]);
    char buf[100];
    strncpy(buf, cmds[i], strlen(cmds[i]) + 1);
    runcmd(buf);
    printf("test case pass!\n");
  }
  printf("shell test finished!\n");
}

int extract_filename(char* path, char* filename) {
  int n = strlen(path);
  if (n == 0) return 0;
  int pos = n - 1;
  while (pos >= 0) {
    if (path[pos] == '/') {
      break;
    }
    pos--;
  }
  strncpy(filename, path + pos + 1, n - (pos + 1));
  return n - (pos + 1);
}

int cp_file(char* dest_filename, char* src_filename) {
  if (is_dir(src_filename)) {
    sys_mkdir(dest_filename);
    return 0;
  }
  int fd = open(src_filename, O_RDONLY);
  char buf[1000];
  read(fd, buf, 1000);
  close(fd);
  fd = open(dest_filename, O_CREATE|O_RDWR);
  write(fd, buf, strlen(buf)); 
  close(fd);
  return 0;
}



void shell_readline(char* buf) {
  sys_readline(buf);
}

static int 
runcmd (char *buf)
{
	int argc;
	char *argv[MAXARGS];
	int i;

	argc = 0;
	argv[argc] = 0;
	
	while(1)
	{
		while (*buf && strchr(WHITESPACE, *buf))
			*buf++ = 0;
		if (*buf == 0)
			break;

		if (argc == MAXARGS - 1)
		{
			printf("Too many arguments (max %d)\n", MAXARGS);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE, *buf))
			buf++;
	}
	argv[argc] = 0;	
	if (argc == 0)
		return 0;
	for (i = 0; i < NCOMMANDS; i++)
	{
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv);
	}
	printf("Unknown command '%s'\n", argv[0]);
        printf("try 'help' to see all supported commands.\n");
	return 0;


}
int ipc_test(){
    printf ("ipc test begin\n");

    pid_t ping_pid, pong_pid;
    if ((ping_pid = spawn (1, 1000)) != -1)
        printf ("ping in process %d.\n", ping_pid);
    else
        printf ("Failed to launch ping.\n");
    
    if ((pong_pid = spawn (2, 1000)) != -1)
        printf ("pong in process %d.\n", pong_pid);
    else
        printf ("Failed to launch pong.\n");
    int i = 0;
    for(i = 0; i < 100000; i++) ;
    printf("ipc test pass!!\n");
    return 0;
}
int main (int argc, char** argv)
{
	char buf[1024];
	printf("\n********Welcome to left-mid-right shell*********\n");
        printf("********This is the final project for CPSC 422/522 Operating Systems in Yale********\n");
        printf("********Author: Bo Song, Haoliang Zhang********\n");
        printf("********Date: 12/18/2015 ********\n");
	close(open("usertests.ran", O_CREATE));
       
        shell_test();
        ipc_test();
	while(1)
	{
		shell_readline(buf);
		if (buf != NULL){
			if (runcmd (buf) < 0)
				break;
		}
	}
}
