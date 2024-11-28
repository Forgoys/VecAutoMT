//By PengZhang(NUDT,zhangpeng13a@nudt.edu.cn) 
// 2020.07.22
//
//

#ifndef HTHREAD_DEVICE_H_
#define HTHREAD_DEVICE_H_

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/time.h>

#ifdef __global__
#undef __global__
#endif
#define __global__  __attribute__ ((section (".global")))

#ifdef __share__
#undef __share__
#endif
#define __share__  __attribute__ ((section (".share")))

#ifdef __gsm__ /* added by Jianbin on GSM memory */
#undef __gsm__
#endif
__asm__(".section ._gsm,\"aw\",%nobits");
#define __gsm__  __attribute__ ((section (".gsm")))

#define EXTRACT_VECTOR(vr, i, r) do {           \
        __asm__ __volatile__(                   \
                "VMVCGC     %1, SVR\n"          \
                "snop  1\n"                     \
                "SMVCCG     SVR"#i", %0\n"      \
                "snop  1\n"                     \
                 :"=r"(r)                       \
                 :"v"(vr));                     \
        } while (0)

/*
	G1: Parallelization management
*/

int get_group_size();
unsigned int get_group_cores();
int get_thread_id();
int get_core_id();

void group_barrier(unsigned id);
/*
void threads_barrier(unsigned int tmask);
void core_barrier_wait(unsigned int id, unsigned int num, unsigned int limit_time);
*/
void core_barrier(unsigned int id, unsigned int num); //id = 0-15
void core_barrier_wait(unsigned int id, unsigned int num, unsigned long wait_time); //id = 0-15
//void sn_barrier(unsigned id, unsigned int mask); //id = 0-1

int rwlock_try_rdlock(unsigned int lock_id); //lock_id = 0-63
int rwlock_try_wrlock(unsigned int lock_id);
void rwlock_rdlock(unsigned int lock_id);
void rwlock_wrlock(unsigned int lock_id);
void rwlock_unlock(unsigned int lock_id);

/*
	G2: Array/Scalar Memory On Chip management
*/

void * vector_malloc(unsigned int bytes); //The maxium num of AM bufs = 64
int vector_free(void *ptr);
int vector_load(void *mem, void *buf, unsigned int bytes);
unsigned int vector_load_async(void *mem, void *buf, unsigned int bytes);
int vector_store(void *buf, void *mem, unsigned int bytes);
unsigned int vector_store_async(void *buf, void *mem, unsigned int bytes);
int get_am_free_space ();

void * scalar_malloc(unsigned int bytes); //The maxium num of SM bufs = 32
int scalar_free(void *ptr);
int scalar_load(void *mem, void *buf, unsigned int bytes);
unsigned int scalar_load_async(void *mem, void *buf, unsigned int bytes);
int scalar_store(void *buf, void *mem, unsigned int bytes);
unsigned int scalar_store_async(void *buf, void *mem, unsigned int bytes);
int get_sm_free_space ();

/*
	G4: Raw DMA APIs
*/

unsigned int dma_p2p(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
	     bool row_syn, unsigned int synmask);

unsigned int dma_broadcast(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
	     unsigned core_id, unsigned int barrire_id);

unsigned int dma_segment(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
	     unsigned int c_start, unsigned int c_num, unsigned int c_step, unsigned int barrire_id);

unsigned int dma_sg(void *src_base, void *src_index, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step);

void dma_wait(unsigned int ch);
//Return 1: Idle;
//   	 0: Busy;
int dma_query(unsigned int ch);
void dma_clear();

unsigned int raw_dma_p2p(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
	     bool row_syn, unsigned int synmask, int prir, int chno);
unsigned int raw_dma_broadcast(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
	     unsigned core_id, unsigned int barrire_id, int total_cores, int prir, int chno);
unsigned int raw_dma_segment(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
	     unsigned int c_start, unsigned int c_num, unsigned int c_step, unsigned int barrire_id, int prir, int chno);
unsigned int raw_dma_sg(void *src_base, void *src_index, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
	     void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step, int prir, int chno);
void raw_dma_wait(int tcc);


unsigned int dma_sg_opt(void *src_base, void *src_index, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
             void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step, int ch);

unsigned int dma_p2p_opt(void *src, unsigned long src_row_num, unsigned int src_row_size, int src_row_step,
             void *dst, unsigned long dst_row_num, unsigned int dst_row_size, int dst_row_step,
             bool row_syn, unsigned int p2pmask, int ch);

void dma_wait_p2p(unsigned int ch_no);
void dma_wait_sg(unsigned int ch_no);
void set_prir(unsigned long val);

/*
	G5: Interrupt & Exception APIs
*/
void dsp_abort(unsigned int err_no);
void dsp_halt();
void dsp_sleep(unsigned long usec);

void hthread_printf(const char *fmt, ...);
int hthread_sprintf(char* buffer, const char* format, ...);
int hthread_snprintf(char* buffer, size_t count, const char* format, ...);
int hthread_vsnprintf(char* buffer, size_t count, const char* format, va_list va);

void hthread_gettimeofday(struct timeval *tv);

void * hbm_malloc(unsigned long bytes);
void hbm_free(void *ptr);

void trigger_cpu(unsigned long val);

/*
Performance event ID:
0	---------	CYCLE: 程序运行周期数
1	---------	BRTK: 分支命中次数
2	---------	IACK: 中断有效次数
3	---------	EXEP: 程序执行包个数
4	---------	L1DRWM: L1D读写失效次数
5	---------	L1DRWH: L1D读写命中次数
6	---------	L1PRM: L1P读失效次数
7	---------	L1PRH: L1P读命中次数
8	---------	DPStall: 指令派发部件DP发出停顿的持续周期
9	---------	VMStall: VM发出停顿的持续周期
10	---------	SMCStall: SMC发出停顿的持续周期
11	---------	STALL: 程序停顿周期
12	---------	NOP: 有效的NOP周期
13	---------	NonNOP: 有效的非NOP周期
14	---------	EACK: 异常发生次数
15	---------	标量SIEU指令条数
16	---------	标量SMAC1指令条数
17	---------	标量SMAC2指令条数
18	---------	标量SBR指令条数
19	---------	标量SLD指令条数
20	---------	向量VIEU指令条数
21	---------	向量VMAC1指令条数
22	---------	向量VMAC2指令条数
23	---------	向量VMAC3指令条数
24	---------	向量VLS0指令条数
25	---------	向量VLS1指令条数
*/
void prof_start(int event_id);
unsigned long prof_end(int event_id);
unsigned long prof_read(int event_id);

unsigned long get_clk();

//浮点异常相关接口

//设置计算部件工作模式：val=1 饱和模式、val=0 非饱和模式
void set_sata_mode (int val);

//关闭与开启计算异常
//EID
//TYPE		ID
//OV: 		0
//Inf: 		1
//Inexact: 	2
//DEN: 		3
//SAT: 		4
#define ECR_OV		0x0
#define ECR_INF		0x1
#define ECR_INEXACT	0x2
#define ECR_DEN		0x3
#define ECR_SAT		0x4
void set_ecr (int EID);
void clear_ecr (int EID);

//  Custom Interrupt No for custom is 16-59.
//  Handler Func type: void func()
unsigned long intr_handler_register(void(*func)(int no));
//  User need register interrupt handler in CPU with hthread_register_handler.
void cpu_interrupt(unsigned long val);

#endif
