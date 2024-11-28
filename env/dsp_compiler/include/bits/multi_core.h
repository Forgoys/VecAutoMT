#ifndef __MULTI_CORE__H__
#define __MULTI_CORE__H__


//sync unit 7,lock 7,not blocking
#define LOCK_WRITE_REG      (volatile unsigned long *)0x01701038  //write the lock not blocking
#define LOCK_READ_REG       (volatile unsigned long *)0x01700038  //read the lock not blocking
#define LOCK_FREE_REG       (volatile unsigned long *)0x01704038  //free the lock
#define LOCK_QUERY_REG      (volatile unsigned long *)0x01705038  //query the lock

#define LOCK_WRITE_VALUE    (*LOCK_WRITE_REG)&0x0f
#define LOCK_READ_VALUE     (*LOCK_READ_REG)&0x0f


typedef struct
{
    unsigned int cnt;
    unsigned int owner;
}lock_t;

#define ENTER_LOCK_WRITE    while(1)                                \
                            {                                       \
                                if(0 == LOCK_WRITE_VALUE)    break;  \
                            }                       
#define ENTER_LOCK_READ     while(1)                                \
                            {                                       \
                                if(0 == LOCK_READ_VALUE)   break;  \
                            }
#define EXIT_LOCK           (*LOCK_FREE_REG = 0)


unsigned int getCoreId(void); 
void syn_lock(void);
void syn_unlock(void);
#endif
