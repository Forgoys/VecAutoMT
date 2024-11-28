#ifndef __TIMER__H__
#define __TIMER__H__
#include <bits/multi_core.h>
/***************************************Timer config register***************************************/
    #define TIMER_EMUMGT_SPND  (0x002000000)	//仿真管理与时钟寄存器
    #define TIMER_CNT          (0x002000008) //计数寄存器
    #define TIMER_PRD          (0x002000010) //周期寄存器
    #define TIMER_TCR          (0x002000018) //控制寄存器
    #define TIMER_TGCR         (0x002000020) //全局控制寄存器
    #define TIMER_REL          (0x002000030) //重载寄存器
    #define TIMER_CAP          (0x002000038) //捕获寄存器
    #define TIMER_INTCTLSTAT   (0x002000040) //中断控制与状态寄存器

/***************************************Function definition***************************************/


#define getConfReg(addr) ((unsigned long)*((unsigned long *)(addr)))

#define setConfReg(addr,data) (*((unsigned long *)(addr))=(unsigned long)(data))

//config the timer
#define configTimer(config) setConfReg(TIMER_TCR,config)

//Set the timer's count
#define setTimerCount(cnt) setConfReg(TIMER_CNT,cnt)

//Set the timer's period
#define setTimerPeriod(period) setConfReg(TIMER_PRD,period)

//Set the timer's interrupt
#define setTimerInterrupt(int) setConfReg(TIMER_INTCTLSTAT,int)

//Get the timer's count
#define getTimerCount() getConfReg(TIMER_CNT)

//Get the timer's period
#define getTimerPeriod() getConfReg(TIMER_PRD)

//Get the timer's interrupt
#define getTimerInterrupt() getConfReg(TIMER_INTCTLSTAT)

//Start the timer
#define timerStart() setConfReg(TIMER_TGCR,3)

//Hold the timer
#define timerHold() setConfReg(TIMER_TGCR,0)

#endif
