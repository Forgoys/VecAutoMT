//By PengZhang(NUDT,zhangpeng13a@nudt.edu.cn) 
// 2020.07.27
//
// 
#ifndef HTHREAD_HOST_MACRO_H_
#define HTHREAD_HOST_MACRO_H_

#define HT_STAT_IDLE 1
#define HT_STAT_BUSY 0
#define HT_STAT_FAIL 2

#define HT_SUCCESS 0
#define HT_FAIL_UNKNOWN -1
#define HT_OUT_OF_RANGE -2
#define HT_NOT_INITIALIZED -3
#define HT_DEV_OPEN_FAILED -4
#define HT_PROGRAM_UNLOADED -5
#define HT_TIMEOUT -6
#define HT_LAST_KERNEL_EXCP -7

#define HT_MEM_RO 	1
#define HT_MEM_WO	2
#define HT_MEM_RW 	3
#define HT_MEM_CACHE 	4

typedef unsigned long uint64_t;

#define COUNT_PARMS2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _, ...) _
#define COUNT_PARMS(...)\
    COUNT_PARMS2(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1) 

#define CAT(A, B) CAT2(A, B)
#define CAT2(A, B) A ## B

#define hthread_group_create(...)\
    CAT(hthread_group_create, COUNT_PARMS(__VA_ARGS__))(__VA_ARGS__)

#endif
