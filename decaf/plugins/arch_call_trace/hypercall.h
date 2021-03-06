/*
 * hypercall.h
 *
 *  Created on: Oct 5, 2011
 *      Author: Cuong Pham
 */

#ifndef HYPERCALL_H_
#define HYPERCALL_H_

/* Command list*/
#define START_TRACE 0
#define STOP_TRACE 1

/* This instruction is vmcall.  On non-VT architectures, it will generate a
 * trap that we will then rewrite to the appropriate instruction.
 */
#define KVM_HYPERCALL ".byte 0x0f,0x01,0xc1"

/* For KVM hypercalls, a three-byte sequence of either the vmrun or the vmmrun
 * instruction.  The hypervisor may replace it with something else but only the
 * instructions are guaranteed to be supported.
 *
 * Up to four arguments may be passed in rbx, rcx, rdx, and rsi respectively.
 * The hypercall number should be placed in rax and the return value will be
 * placed in rax.  No other registers will be clobbered unless explicited
 * noted by the particular hypercall.
 */

#define HYPERCALL0(nr)						\
			asm volatile(KVM_HYPERCALL		\
		     : /* no output */				\
		     : "a"(nr)						\
		     : "memory")					\

#define HYPERCALL1(nr, p1)					\
			asm volatile(KVM_HYPERCALL		\
		     : /* no output */				\
		     : "a"(nr), "b"(p1)				\
		     : "memory")					\

#define HYPERCALL2(nr, p1, p2)				\
			asm volatile(KVM_HYPERCALL		\
		     : /* no output */				\
		     : "a"(nr), "b"(p1), "c"(p2)	\
		     : "memory")					\

#define HYPERCALL3(nr, p1, p2, p3)					\
			asm volatile(KVM_HYPERCALL				\
		     : /* no output */						\
		     : "a"(nr), "b"(p1), "c"(p2), "d"(p3)	\
		     : "memory")							\

#define HYPERCALL4(nr, p1, p2, p3, p4)						\
			asm volatile(KVM_HYPERCALL						\
		     : /* no output */								\
		     : "a"(nr), "b"(p1), "c"(p2), "d"(p3), "e"(p4)	\
		     : "memory")									\

#endif /* HYPERCALL_H_ */
