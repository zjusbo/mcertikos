#ifndef _KERN_FS_PARAMS_H_
#define _KERN_FS_PARAMS_H_

#define NOFILE       16  // open files per process
#define NFILE       100  // open files per system
#define NBUF         10  // size of disk block cache
#define NINODE       50  // maximum number of active i-nodes
#define NDEV         10  // maximum major device number
#define ROOTDEV       1  // device number of file system root disk
#define MAXARG       32  // max exec arguments
#define LOGSIZE      10  // max data sectors in on-disk log

#define ROOTINO       1  // root i-number
#define BSIZE       512  // block size

#endif /* !_KERN_FS_PARAMS_H_ */
