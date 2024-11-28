#include <unistd.h>
#define __TI_data_synch_INV(x, y)
#define __TI_data_synch_WBINV(x, y)
#define __TI_resource_lock(x)   
#define __TI_resource_unlock(x) 
#define _CODE_ACCESS __attribute__((noinline)) 
#define _DATA_ACCESS

#define _DECL extern

//#ifndef _OFF_T
//#define _OFF_T
//typedef int off_t;
//#endif /* _OFF_T */
#if 0
_DECL _CODE_ACCESS int   _open(const char *path, unsigned flags, int mode);
_DECL _CODE_ACCESS int   read(int fildes, char *bufptr, unsigned cnt);
_DECL _CODE_ACCESS int   write(int fildes, const char *bufptr, unsigned cnt);
_DECL _CODE_ACCESS off_t lseek(int fildes, off_t offset, int origin);
_DECL _CODE_ACCESS int   close(int fildes);
_DECL _CODE_ACCESS int   unlink(const char *path);
_DECL _CODE_ACCESS int   rename(const char *old_name, const char *new_name);
#endif
extern long __CIO_SIZE;
extern int open (const char *__file, int __oflag, ...);
extern int close (int __fd);
//extern ssize_t write (int __fd, __const void *__buf, size_t __n) ;
//extern ssize_t read (int __fd, void *__buf, size_t __nbytes) ;
extern __off_t lseek (int __fd, __off_t __offset, int __whence);
extern int unlink (__const char *__name);
extern int rename (__const char *__old, __const char *__new);






_DECL _CODE_ACCESS int add_device(
    char     *name,			           
    unsigned  flags,
    int      (*dopen)(const char *path, unsigned flags, int llv_fd),
    int      (*dclose)(int dev_fd),
    int      (*dread)(int dev_fd, char *buf, unsigned count),
    int      (*dwrite)(int dev_fd, const char *buf, unsigned count),
    off_t    (*dlseek)(int dev_fd, off_t offset, int origin),
    int      (*dunlink)(const char *path),
    int      (*drename)(const char *old_name, const char *new_name));
_DECL _CODE_ACCESS int remove_device(char *name);
/*---------------------------------------------------------------------------*/
/* _NSTREAM defines the max number of files you can have open with fopen().  */
/* Since the standard streams(stdin/stdout/stderr) use 3 of them by default, */
/* (_NSTREAM - 3) will be available to users.                                */
/*---------------------------------------------------------------------------*/
#define _NSTREAM         20                   

#define _NDEVICE         3                   /* Size of device table        */

#define _SSA      (0x0000)             /* Single Stream allowed       */
#define _BUSY     (0x0001)             /* Device busy                 */
#define _MSA      (0x0002)             /* Multiple Streams Allowed    */

#undef _DECL

/****************************************************************************/
/* TYPES THAT ANSI REQUIRES TO BE DEFINED                                   */
/****************************************************************************/
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif
/****************************************************************************/
/* MACROS THAT ANSI REQUIRES TO BE DEFINED                                  */
/****************************************************************************/
//#define BUFSIZ          8192 

#ifndef NULL
#define NULL 0
#endif

/*---------------------------------------------------------------------------*/
/* constants for the data transfer functions for the predefined driver       */
/*---------------------------------------------------------------------------*/
#define _DTOPEN    (0xF0)
#define _DTCLOSE   (0xF1)
#define _DTREAD    (0xF2)
#define _DTWRITE   (0xF3)
#define _DTLSEEK   (0xF4)
#define _DTUNLINK  (0xF5)
#define _DTGETENV  (0xF6)
#define _DTRENAME  (0xF7)
#define _DTGETTIME (0xF8)
#define _DTGETCLK  (0xF9)
#define _DTSYNC    (0xFF)

//#define CIOBUFSIZ ((BUFSIZ)+32)


/*---------------------------------------------------------------------------*/
/* THESE MACROS PACK AND UNPACK SHORTS AND LONGS INTO CHARACTER ARRAYS       */
/*                                                                           */
/* THIS IS DONE BYTE REVERSED TO MAKE THE PC'S JOB EASIER AND SINCE THE      */
/*     DEVICE COULD BE BIG OR LITTLE ENDIAN                                  */
/*---------------------------------------------------------------------------*/
#define LOADSHORT(x,y,z)  { x[(z)]   = (unsigned short) (y); \
                            x[(z)+1] = (unsigned short) (y) >> 8;  }

#define UNLOADSHORT(x,z) ((short) ( (short) x[(z)] +             \
				   ((short) x[(z)+1] << 8)))

#define LOAD32(x,y,z) { x[(z)]   = (unsigned int) (y); \
                          x[(z)+1] = (unsigned int) (y) >> 8; \
                          x[(z)+2] = (unsigned int) (y) >> 16;  \
                          x[(z)+3] = (unsigned int) (y) >> 24;   }

#define UNLOAD32(x,z) ( (int) x[(z)]           +  \
		       ((int) x[(z)+1] << 8)   +  \
		       ((int) x[(z)+2] << 16)  +  \
		       ((int) x[(z)+3] << 24))

/*---------------------------------------------------------------------------*/
/* THESE MACROS CAN BE USED TO PACK AND UNPACK CHARACTER STREAMS INTO TARGET */
/* WORDS FOR MORE EFFICIENT TRANSFER.                                        */
/*---------------------------------------------------------------------------*/
#define PACKCHAR(val, base, byte) ( (base)[(byte)] = (val) )

#define UNPACKCHAR(base, byte)    ( (base)[byte] )


