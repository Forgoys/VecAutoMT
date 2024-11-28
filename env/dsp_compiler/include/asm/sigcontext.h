/*
 *  linux/include/asm-c6x/sigcontext.h
 *
 *  Port on Texas Instruments TMS320C6x architecture
 *
 *  Copyright (C) 2004, 2009, 2011 Texas Instruments Incorporated
 *  Author: Aurelien Jacquiot (aurelien.jacquiot@jaluna.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#ifndef __ASM_C6X_SIGCONTEXT_H
#define __ASM_C6X_SIGCONTEXT_H

#if 0
struct sigcontext {
	unsigned long  sc_mask; 	/* old sigmask */
	unsigned long  sc_sp;		/* old user stack pointer */

	unsigned long  sc_a4;
	unsigned long  sc_b4;
	unsigned long  sc_a6;
	unsigned long  sc_b6;
	unsigned long  sc_a8;
	unsigned long  sc_b8;

	unsigned long  sc_a0;
	unsigned long  sc_a1;
	unsigned long  sc_a2;
	unsigned long  sc_a3;
	unsigned long  sc_a5;
	unsigned long  sc_a7;
	unsigned long  sc_a9;

	unsigned long  sc_b0;
	unsigned long  sc_b1;
	unsigned long  sc_b2;
	unsigned long  sc_b3;
	unsigned long  sc_b5;
	unsigned long  sc_b7;
	unsigned long  sc_b9;

#if defined(CONFIG_TMS320C64X) || defined(CONFIG_TMS320C64XPLUS)  || defined(CONFIG_TMS320C66X)
	unsigned long  sc_a16;
	unsigned long  sc_a17;
	unsigned long  sc_a18;
	unsigned long  sc_a19;
	unsigned long  sc_a20;
	unsigned long  sc_a21;
	unsigned long  sc_a22;
	unsigned long  sc_a23;
	unsigned long  sc_a24;
	unsigned long  sc_a25;
	unsigned long  sc_a26;
	unsigned long  sc_a27;
	unsigned long  sc_a28;
	unsigned long  sc_a29;
	unsigned long  sc_a30;
	unsigned long  sc_a31;

	unsigned long  sc_b16;
	unsigned long  sc_b17;
	unsigned long  sc_b18;
	unsigned long  sc_b19;
	unsigned long  sc_b20;
	unsigned long  sc_b21;
	unsigned long  sc_b22;
	unsigned long  sc_b23;
	unsigned long  sc_b24;
	unsigned long  sc_b25;
	unsigned long  sc_b26;
	unsigned long  sc_b27;
	unsigned long  sc_b28;
	unsigned long  sc_b29;
	unsigned long  sc_b30;
	unsigned long  sc_b31;
#endif

	unsigned long  sc_csr;
	unsigned long  sc_pc;
};
#endif

struct sigcontext {
	unsigned long  sc_mask; 	/* old sigmask */
	unsigned long  sc_sp;		/* old user stack pointer */

	unsigned long  sc_r10;
	unsigned long  sc_r12;
	unsigned long  sc_r14;
	unsigned long  sc_r16;
	unsigned long  sc_r18;
	unsigned long  sc_r20;

	unsigned long  sc_r0;
	unsigned long  sc_r1;
	unsigned long  sc_r2;
	unsigned long  sc_r3;
	unsigned long  sc_r4;
	unsigned long  sc_r5;
	unsigned long  sc_r6;
        unsigned long  sc_r7;
	unsigned long  sc_r8;
	unsigned long  sc_r9;

	unsigned long  sc_r11;
	unsigned long  sc_r13;
	unsigned long  sc_r15;
	unsigned long  sc_r17;
        unsigned long  sc_r19;

 	unsigned long  sc_r21;
	unsigned long  sc_r22;
	unsigned long  sc_r23;
	unsigned long  sc_r24;
	unsigned long  sc_r25;
	unsigned long  sc_r26;
	unsigned long  sc_r27;
        unsigned long  sc_r28;
	unsigned long  sc_r29;
	unsigned long  sc_r30;

	unsigned long  sc_r31;
	unsigned long  sc_r32;
	unsigned long  sc_r33;
	unsigned long  sc_r34;
	unsigned long  sc_r35;
	unsigned long  sc_r36;
	unsigned long  sc_r37;
        unsigned long  sc_r38;
	unsigned long  sc_r39;
	unsigned long  sc_r40;

	unsigned long  sc_r41;
	unsigned long  sc_r42;
	unsigned long  sc_r43;
	unsigned long  sc_r44;
	unsigned long  sc_r45;
	unsigned long  sc_r46;
	unsigned long  sc_r47;
        unsigned long  sc_r48;
	unsigned long  sc_r49;
	unsigned long  sc_r50;

	unsigned long  sc_r51;
	unsigned long  sc_r52;
	unsigned long  sc_r53;
	unsigned long  sc_r54;
	unsigned long  sc_r55;
	unsigned long  sc_r56;
	unsigned long  sc_r57;
        unsigned long  sc_r58;
	unsigned long  sc_r59;
	unsigned long  sc_r60;

	unsigned long  sc_r61;
	unsigned long  sc_r62;
	unsigned long  sc_r63;

	unsigned long  sc_ar8;	
	unsigned long  sc_ar11;
	unsigned long  sc_ar12;
	unsigned long  sc_ar13;
	unsigned long  sc_ar14;
	unsigned long  sc_ar15;

	unsigned long  sc_or8;
	unsigned long  sc_or9;
	unsigned long  sc_or10;
	unsigned long  sc_or11;
	unsigned long  sc_or12;
	unsigned long  sc_or13;
	unsigned long  sc_or14;
	unsigned long  sc_or15;

	unsigned long  sc_sst;
	unsigned long  sc_pc;
};

#endif /* __ASM_C6X_SIGCONTEXT_H */

