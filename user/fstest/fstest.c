#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>
#include <file.h>
#include <gcc.h>

#define exit() return

char buf[8192];
char name[3];
char *echoargv[] = { "echo", "ALL", "TESTS", "PASSED", 0 };

// simple file system tests

void
smallfile(void)
{
  int fd;
  int i;

  printf("=====small file test=====\n");
  fd = open("small", O_CREATE|O_RDWR);
  if(fd >= 0){
    printf("creat small succeeded; ok, fd: %d\n", fd);
  } else {
    printf("error: creat small failed!\n");
    exit();
  }
  for(i = 0; i < 100; i++){
    if(write(fd, "aaaaaaaaaa", 10) != 10){
      printf("error: write aa %d new file failed\n", i);
      exit();
    }
    if(write(fd, "bbbbbbbbbb", 10) != 10){
      printf("error: write bb %d new file failed\n", i);
      exit();
    }
  }
  printf("writes ok\n");
  close(fd);
  fd = open("small", O_RDONLY);
  if(fd >= 0){
    printf("open small succeeded ok\n");
  } else {
    printf("error: open small failed!\n");
    exit();
  }
  i = read(fd, buf, 2000);
  if(i == 2000){
    printf("read succeeded ok\n");
  } else {
    printf("read failed\n");
    exit();
  }
  close(fd);

  if(unlink("small") < 0){
    printf("unlink small failed\n");
    exit();
  }
  printf("=====small file test ok=====\n\n");
}


void
bigfile1(void)
{
  int i, fd, n;

  printf("=====big files test=====\n");

  fd = open("big", O_CREATE|O_RDWR);
  if(fd < 0){
    printf("error: creat big failed!\n");
    exit();
  }

  for(i = 0; i < MAXFILE; i++){
    ((int*)buf)[0] = i;
    if(write(fd, buf, 512) != 512){
      printf("error: write big file failed\n", i);
      exit();
    }
  }

  close(fd);

  fd = open("big", O_RDONLY);
  if(fd < 0){
    printf("error: open big failed!\n");
    exit();
  }

  n = 0;
  for(;;){
    i = read(fd, buf, 512);
    if(i == 0){
      if(n == MAXFILE - 1){
        printf("read only %d blocks from big", n);
        exit();
      }
      break;
    } else if(i != 512){
      printf("read failed %d\n", i);
      exit();
    }
    if(((int*)buf)[0] != n){
      printf("read content of block %d is %d\n",
             n, ((int*)buf)[0]);
      exit();
    }
    n++;
  }
  close(fd);
  if(unlink("big") < 0){
    printf("unlink big failed\n");
    exit();
  }
  printf("=====big files ok=====\n\n");
}

void
createtest(void)
{
  int i, fd;

  printf("=====many creates, followed by unlink test=====\n");

  name[0] = 'a';
  name[2] = '\0';
  for(i = 0; i < 52; i++){
    name[1] = '0' + i;
    fd = open(name, O_CREATE|O_RDWR);
    close(fd);
  }
  name[0] = 'a';
  name[2] = '\0';
  for(i = 0; i < 52; i++){
    name[1] = '0' + i;
    unlink(name);
  }
  printf("=====many creates, followed by unlink; ok=====\n\n");
}

void
rmdot(void)
{
  printf("=====rmdot test=====\n");
  if(mkdir("dots") != 0){
    printf("mkdir dots failed\n");
    exit();
  }
  if(chdir("dots") != 0){
    printf("chdir dots failed\n");
    exit();
  }
  if(unlink(".") == 0){
    printf("rm . worked!\n");
    exit();
  }
  if(unlink("..") == 0){
    printf("rm .. worked!\n");
    exit();
  }
  if(chdir("/") != 0){
    printf("chdir '/' failed\n");
    //    exit();
  }
  if(unlink("dots/.") == 0){
    printf("unlink dots/. worked!\n");
    exit();
  }
  if(unlink("dots/..") == 0){
    printf("unlink dots/.. worked!\n");
    exit();
  }
  if(unlink("dots") != 0){
    printf("unlink dots failed!\n");
    exit();
  }
  printf("=====rmdot ok=====\n\n");
}


void
fourteen(void)
{
  int fd;

  // DIRSIZ is 14.
  printf("=====fourteen test=====\n");

  if(mkdir("12345678901234") != 0){
    printf("mkdir 12345678901234 failed\n");
    exit();
  }
  if(mkdir("12345678901234/123456789012345") != 0){
    printf("mkdir 12345678901234/123456789012345 failed\n");
    exit();
  }
  fd = open("123456789012345/123456789012345/123456789012345", O_CREATE);
  if(fd < 0){
    printf("create 123456789012345/123456789012345/123456789012345 failed\n");
    exit();
  }
  close(fd);
  fd = open("12345678901234/12345678901234/12345678901234", 0);
  if(fd < 0){
    printf("open 12345678901234/12345678901234/12345678901234 failed\n");
    exit();
  }
  close(fd);

  if(mkdir("12345678901234/12345678901234") == 0){
    printf("mkdir 12345678901234/12345678901234 succeeded!\n");
    exit();
  }
  if(mkdir("12345678901234/123456789012345") == 0){
    printf("mkdir 12345678901234/123456789012345 succeeded!\n");
    exit();
  }

  printf("=====fourteen ok=====\n\n");
}

void
bigfile2(void)
{
  int fd, i, total, cc;

  printf("=====bigfile test=====\n");

  unlink("bigfile");
  fd = open("bigfile", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("cannot create bigfile");
    exit();
  }
  for(i = 0; i < 20; i++){
    memset(buf, i, 600);
    if(write(fd, buf, 600) != 600){
      printf("write bigfile failed\n");
      exit();
    }
  }
  close(fd);

  fd = open("bigfile", 0);
  if(fd < 0){
    printf("cannot open bigfile\n");
    exit();
  }
  total = 0;
  for(i = 0; ; i++){
    cc = read(fd, buf, 300);
    if(cc < 0){
      printf("read bigfile failed\n");
      exit();
    }
    if(cc == 0)
      break;
    if(cc != 300){
      printf("short read bigfile\n");
      exit();
    }
    if(buf[0] != i/2 || buf[299] != i/2){
      printf("read bigfile wrong data\n");
      exit();
    }
    total += cc;
  }
  close(fd);
  if(total != 20*600){
    printf("read bigfile wrong total\n");
    exit();
  }
  unlink("bigfile");

  printf("=====bigfile test ok=====\n\n");
}


void
subdir(void)
{
  int fd, cc;

  printf("=====subdir test=====\n");

  unlink("ff");
  if(mkdir("dd") != 0){
    printf("subdir mkdir dd failed\n");
    exit();
  }

  fd = open("dd/ff", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("create dd/ff failed\n");
    exit();
  }
  write(fd, "ff", 2);
  close(fd);
    
  if(unlink("dd") >= 0){
    printf("unlink dd (non-empty dir) succeeded!\n");
    exit();
  }

  if(mkdir("dd/dd") != 0){
    printf("subdir mkdir dd/dd failed\n");
    exit();
  }

  fd = open("dd/dd/ff", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("create dd/dd/ff failed\n");
    exit();
  }

  write(fd, "FF", 2);
  close(fd);

  fd = open("dd/dd/../ff", 0);
  //fd = open("dd/ff", 0);
  if(fd < 0){
    printf("open dd/dd/../ff failed\n");
    exit();
  }
  cc = read(fd, buf, sizeof(buf));
  if(cc != 2 || buf[0] != 'f'){
    printf("cc = %d, buf[0] = %c\n", cc, buf[0]);
    printf("dd/dd/../ff wrong content\n");
    exit();
  }
  close(fd);

  if(link("dd/dd/ff", "dd/dd/ffff") != 0){
    printf("link dd/dd/ff dd/dd/ffff failed\n");
    exit();
  }

  if(unlink("dd/dd/ff") != 0){
    printf("unlink dd/dd/ff failed\n");
    exit();
  }
  if(open("dd/dd/ff", O_RDONLY) >= 0){
    printf("open (unlinked) dd/dd/ff succeeded\n");
    exit();
  }

  if(chdir("dd") != 0){
    printf("chdir dd failed\n");
    exit();
  }
  if(chdir("dd/../../dd") != 0){
    printf("chdir dd/../../dd failed\n");
    exit();
  }
  if(chdir("dd/../../../dd") != 0){
    printf("chdir dd/../../dd failed\n");
    exit();
  }
  if(chdir("./..") != 0){
    printf("chdir ./.. failed\n");
    exit();
  }

  fd = open("dd/dd/ffff", 0);
  if(fd < 0){
    printf("open dd/dd/ffff failed\n");
    exit();
  }
  if(read(fd, buf, sizeof(buf)) != 2){
    printf("read dd/dd/ffff wrong len\n");
    exit();
  }
  close(fd);

  if(open("dd/dd/ff", O_RDONLY) >= 0){
    printf("open (unlinked) dd/dd/ff succeeded!\n");
    exit();
  }

  if(open("dd/ff/ff", O_CREATE|O_RDWR) >= 0){
    printf("create dd/ff/ff succeeded!\n");
    exit();
  }
  if(open("dd/xx/ff", O_CREATE|O_RDWR) >= 0){
    printf("create dd/xx/ff succeeded!\n");
    exit();
  }
  if(open("dd", O_CREATE) >= 0){
    printf("create dd succeeded!\n");
    exit();
  }
  if(open("dd", O_RDWR) >= 0){
    printf("open dd rdwr succeeded!\n");
    exit();
  }
  if(open("dd", O_WRONLY) >= 0){
    printf("open dd wronly succeeded!\n");
    exit();
  }
  if(link("dd/ff/ff", "dd/dd/xx") == 0){
    printf("link dd/ff/ff dd/dd/xx succeeded!\n");
    exit();
  }
  if(link("dd/xx/ff", "dd/dd/xx") == 0){
    printf("link dd/xx/ff dd/dd/xx succeeded!\n");
    exit();
  }
  if(link("dd/ff", "dd/dd/ffff") == 0){
    printf("link dd/ff dd/dd/ffff succeeded!\n");
    exit();
  }
  if(mkdir("dd/ff/ff") == 0){
    printf("mkdir dd/ff/ff succeeded!\n");
    exit();
  }
  if(mkdir("dd/xx/ff") == 0){
    printf("mkdir dd/xx/ff succeeded!\n");
    exit();
  }
  if(mkdir("dd/dd/ffff") == 0){
    printf("mkdir dd/dd/ffff succeeded!\n");
    exit();
  }
  if(unlink("dd/xx/ff") == 0){
    printf("unlink dd/xx/ff succeeded!\n");
    exit();
  }
  if(unlink("dd/ff/ff") == 0){
    printf("unlink dd/ff/ff succeeded!\n");
    exit();
  }
  if(chdir("dd/ff") == 0){
    printf("chdir dd/ff succeeded!\n");
    exit();
  }
  if(chdir("dd/xx") == 0){
    printf("chdir dd/xx succeeded!\n");
    exit();
  }

  if(unlink("dd/dd/ffff") != 0){
    printf("unlink dd/dd/ff failed\n");
    exit();
  }
  if(unlink("dd/ff") != 0){
    printf("unlink dd/ff failed\n");
    exit();
  }
  if(unlink("dd") == 0){
    printf("unlink non-empty dd succeeded!\n");
    exit();
  }
  if(unlink("dd/dd") < 0){
    printf("unlink dd/dd failed\n");
    exit();
  }
  if(unlink("dd") < 0){
    printf("unlink dd failed\n");
    exit();
  }

  printf("=====subdir ok=====\n\n");
}

void
linktest(void)
{
  int fd;

  printf("=====linktest=====\n");

  unlink("lf1");
  unlink("lf2");

  fd = open("lf1", O_CREATE|O_RDWR);
  if(fd < 0){
    printf("create lf1 failed\n");
    exit();
  }
  if(write(fd, "hello", 5) != 5){
    printf("write lf1 failed\n");
    exit();
  }
  close(fd);

  if(link("lf1", "lf2") < 0){
    printf("link lf1 lf2 failed\n");
    exit();
  }
  unlink("lf1");

  if(open("lf1", 0) >= 0){
    printf("unlinked lf1 but it is still there!\n");
    exit();
  }

  fd = open("lf2", 0);
  if(fd < 0){
    printf("open lf2 failed\n");
    exit();
  }
  if(read(fd, buf, sizeof(buf)) != 5){
    printf("read lf2 failed\n");
    exit();
  }
  close(fd);

  if(link("lf2", "lf2") >= 0){
    printf("link lf2 lf2 succeeded! oops\n");
    exit();
  }

  unlink("lf2");
  if(link("lf2", "lf1") >= 0){
    printf("link non-existant succeeded! oops\n");
    exit();
  }

  if(link(".", "lf1") >= 0){
    printf("link . lf1 succeeded! oops\n");
    exit();
  }

  printf("=====linktest ok=====\n\n");
}

// can I unlink a file and still read it?
void
unlinkread(void)
{
  int fd, fd1;

  printf("=====unlinkread test=====\n");
  fd = open("unlinkread", O_CREATE | O_RDWR);
  if(fd < 0){
    printf("create unlinkread failed\n");
    exit();
  }
  write(fd, "hello", 5);
  close(fd);

  fd = open("unlinkread", O_RDWR);
  if(fd < 0){
    printf("open unlinkread failed\n");
    exit();
  }
  if(unlink("unlinkread") != 0){
    printf("unlink unlinkread failed\n");
    exit();
  }

  fd1 = open("unlinkread", O_CREATE | O_RDWR);
  write(fd1, "yyy", 3);
  close(fd1);

  if(read(fd, buf, sizeof(buf)) != 5){
    printf("unlinkread read failed");
    exit();
  }
  if(buf[0] != 'h'){
    printf("unlinkread wrong data\n");
    exit();
  }
  if(write(fd, buf, 10) != 10){
    printf("unlinkread write failed\n");
    exit();
  }
  close(fd);
  unlink("unlinkread");
  printf("=====unlinkread ok=====\n\n");
}

void
dirfile(void)
{
  int fd;

  printf("=====dir vs file=====\n");

  fd = open("dirfile", O_CREATE);
  if(fd < 0){
    printf("create dirfile failed\n");
    exit();
  }
  close(fd);
  if(chdir("dirfile") == 0){
    printf("chdir dirfile succeeded!\n");
    exit();
  }
  fd = open("dirfile/xx", 0);
  if(fd >= 0){
    printf("create dirfile/xx succeeded!\n");
    exit();
  }
  fd = open("dirfile/xx", O_CREATE);
  if(fd >= 0){
    printf("create dirfile/xx succeeded!\n");
    exit();
  }
  if(mkdir("dirfile/xx") == 0){
    printf("mkdir dirfile/xx succeeded!\n");
    exit();
  }
  if(unlink("dirfile/xx") == 0){
    printf("unlink dirfile/xx succeeded!\n");
    exit();
  }
  if(link("README", "dirfile/xx") == 0){
    printf("link to dirfile/xx succeeded!\n");
    exit();
  }
  if(unlink("dirfile") != 0){
    printf("unlink dirfile failed!\n");
    exit();
  }

  fd = open(".", O_RDWR);
  if(fd >= 0){
    printf("open . for writing succeeded!\n");
    exit();
  }
  fd = open(".", 0);
  if(write(fd, "x", 1) > 0){
    printf("write . succeeded!\n");
    exit();
  }
  close(fd);

  printf("=====dir vs file OK=====\n\n");
}

// test that iput() is called at the end of _namei()
void
iref(void)
{
  int i, fd;

  printf("=====empty file name=====\n");

  // the 50 is NINODE
  for(i = 0; i < 50 + 1; i++){
    if(mkdir("irefd") != 0){
      printf("mkdir irefd failed\n");
      exit();
    }
    if(chdir("irefd") != 0){
      printf("chdir irefd failed\n");
      exit();
    }

    mkdir("");
    link("README", "");
    fd = open("", O_CREATE);
    if(fd >= 0)
      close(fd);
    fd = open("xx", O_CREATE);
    if(fd >= 0)
      close(fd);
    unlink("xx");
  }

  chdir("/");
  printf("=====empty file name OK=====\n\n");
}

  char namel[10];

// directory that uses indirect blocks
void
bigdir(void)
{
  int i, fd;

  printf("=====bigdir test=====\n");
  unlink("bd");

  fd = open("bd", O_CREATE);
  if(fd < 0){
    printf("bigdir create failed\n");
    exit();
  }
  close(fd);

  for(i = 0; i < 500; i++){
    namel[0] = 'x';
    namel[1] = '0' + (i / 64);
    namel[2] = '0' + (i % 64);
    namel[3] = '\0';
    if(link("bd", namel) != 0){
      printf("bigdir link failed\n");
      exit();
    }
  }

  unlink("bd");
  for(i = 0; i < 500; i++){
    namel[0] = 'x';
    namel[1] = '0' + (i / 64);
    namel[2] = '0' + (i % 64);
    namel[3] = '\0';
    if(unlink(namel) != 0){
      printf("bigdir unlink failed");
      exit();
    }
  }

  printf("=====bigdir ok=====\n\n");
}


int
main(int argc, char *argv[])
{
  printf("*******usertests starting*******\n\n");

  printf("=====test file usertests.ran does not exists=====\n");

  if(open("usertests.ran", O_RDONLY) >= 0){
    printf("already ran user tests (file usertests.ran exists) -- recreate certikos_disk.img\n");
    exit();
  }
  printf("=====test file usertests.ran does not exists: ok\n\n");
  close(open("usertests.ran", O_CREATE));

  smallfile();
  bigfile1();
  createtest();

  rmdot();
  fourteen();
  bigfile2();
  subdir();
  linktest();
  unlinkread();
  dirfile();
  iref();
  bigdir(); // slow
  printf("*******end of tests*******\n");
}
