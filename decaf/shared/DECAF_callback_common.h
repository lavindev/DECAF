/*
Copyright (C) <2012> <Syracuse System Security (Sycure) Lab>

DECAF is based on QEMU, a whole-system emulator. You can redistribute
and modify it under the terms of the GNU GPL, version 3 or later,
but it is made available WITHOUT ANY WARRANTY. See the top-level
README file for more details.

For more information about DECAF and other softwares, see our
web site at:
http://sycurelab.ecs.syr.edu/

If you have any questions about DECAF,please post it on
http://code.google.com/p/decaf-platform/
*/
/**
 * @author Lok Yan
 * @date 12 OCT 2012
 */

#ifndef DECAF_CALLBACK_COMMON_H
#define DECAF_CALLBACK_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "cpu.h"
#include "shared/DECAF_types.h"

/*
 * Condition flags for range callbacks. Transitions are only valid for instructions that branch.
 * For all other instructions only 0x00, 0x01, 0x08 are valid.
 */
typedef enum {
	DECAF_ALL					= 0x00,
	DECAF_USER_TO_USER_ONLY     = 0x01,
	DECAF_USER_TO_KERNEL_ONLY   = 0x02,
	DECAF_KERNEL_TO_USER_ONLY   = 0x04,
	DECAF_KERNEL_TO_KERNEL_ONLY = 0x08,
} OpcodeRangeCallbackConditions;

typedef enum {
        DECAF_BLOCK_BEGIN_CB = 0,
        DECAF_BLOCK_END_CB,
        DECAF_INSN_BEGIN_CB,
        DECAF_INSN_END_CB,
        DECAF_MEM_READ_CB,
        DECAF_MEM_WRITE_CB,
        DECAF_EIP_CHECK_CB,
        DECAF_KEYSTROKE_CB, // keystroke event
        DECAF_NIC_REC_CB,
        DECAF_NIC_SEND_CB,
        DECAF_OPCODE_RANGE_CB,
        DECAF_TLB_EXEC_CB,
        DECAF_READ_TAINTMEM_CB,
        DECAF_WRITE_TAINTMEM_CB,
        DECAF_VMCALL_CB,
        DECAF_VTX_CB,
#ifdef CONFIG_TCG_LLVM
	DECAF_BLOCK_TRANS_CB,
#endif /* CONFIG_TCG_LLVM */
        DECAF_LAST_CB, //place holder for the last position, no other uses.
} DECAF_callback_type_t;

// VT-x Exit types

//typedef enum vtx_exit {
#define	VTX_EXIT_NMI 0
#define	VTX_EXIT_EXT_INT 1
#define	VTX_EXIT_TRIPLE_FAULT 2
#define	VTX_EXIT_INIT_SIG 3
#define	VTX_EXIT_SIPI 4
#define	VTX_EXIT_SMI 5
#define	VTX_EXIT_OtherSMI 6
#define	VTX_EXIT_INT_WINDOW 7
#define	VTX_EXIT_NMI_WINDOW 8
#define	VTX_EXIT_TASK_SWITCH 9
#define	VTX_EXIT_CPUID 10
#define	VTX_EXIT_GETSEC 11
#define	VTX_EXIT_HLT 12
#define	VTX_EXIT_INVD 13
#define	VTX_EXIT_INVLPG 14
#define	VTX_EXIT_RDPMC 15
#define	VTX_EXIT_RDTSC 16
#define	VTX_EXIT_RSM 17
#define	VTX_EXIT_VMCALL 18
#define	VTX_EXIT_VMCLEAR 19
#define	VTX_EXIT_VMLAUNCH 20
#define	VTX_EXIT_VMPTRLD 21
#define	VTX_EXIT_VMPTRST 22
#define	VTX_EXIT_VMREAD 23
#define	VTX_EXIT_VMRESUME 24
#define	VTX_EXIT_VMWRITE 25
#define	VTX_EXIT_VMXOFF 26
#define	VTX_EXIT_VMXON 27
#define	VTX_EXIT_CR_ACCESS 28
#define	VTX_EXIT_MOV_DR 29
#define	VTX_EXIT_IO 30
#define	VTX_EXIT_RDMSR 31
#define	VTX_EXIT_WRMSR 32
#define	VTX_EXIT_VM_ENTRY_FAIL_GUEST 33
#define	VTX_EXIT_VM_ENTRY_FAIL_MSR 34
#define	VTX_EXIT_MWAIT 36
#define	VTX_EXIT_MONITOR_TRAP 37
#define	VTX_EXIT_MONITOR 39
#define	VTX_EXIT_PAUSE  40
#define	VTX_EXIT_VM_ENTRY_FAIL_MACHINE_CHECK  41
#define	VTX_EXIT_TPR_THRESHHOLD 43
#define	VTX_EXIT_APIC_ACCESS 44
#define	VTX_EXIT_VIRT_EOI 45
#define	VTX_EXIT_GDTR_IDTR_ACCESS 46
#define	VTX_EXIT_LDTR_TR_ACCESS 47
#define	VTX_EXIT_EPT_VIOLATION 48
#define	VTX_EXIT_EPT_MISCONFIG 49
#define	VTX_EXIT_INVEPT 50
#define	VTX_EXIT_RDTSCP 51
#define	VTX_EXIT_VMX_TIMER_EXP 52
#define	VTX_EXIT_INVVPID 53
#define	VTX_EXIT_WBINVD 54
#define	VTX_EXIT_XSETBV 55
#define	VTX_EXIT_APIC_WRITE 56
#define	VTX_EXIT_RDRAND 57
#define	VTX_EXIT_INVPCID 58
#define	VTX_EXIT_VMFUNC 59
#define	VTX_EXIT_RDSEED 61
#define	VTX_EXIT_XSAVES 63
#define	VTX_EXIT_XRSTORS 64
//} vtx_exit_t;

//Optimized Callback type
typedef enum _OCB_t {
  /**
   * Optimized Callback Condition - Const - The value associated with this flag needs an exact match
   */
  OCB_CONST = 2,
  /**
   * Optimized callback Condition - Page - A match is found as long as the page numbers match
   */
  OCB_PAGE = 4,
  /**
   * Not used yet
   */
  OCB_CONST_NOT = 3,
  /**
   * Not used yet
   */
  OCB_PAGE_NOT = 5,
  /**
   * Optimized Callback Condition - Everything!
   */
  OCB_ALL = -1
} OCB_t;
// HU- for memory read/write callback.Memory be read/written at different grains
//(byte,word,long,quad)
typedef enum{
	DECAF_BYTE=1,
	DECAF_WORD=2,
	DECAF_LONG=4,
	DECAF_QUAD=8,
} DATA_TYPE;

typedef struct _DECAF_Block_Begin_Params
{
  CPUState* env;
  TranslationBlock* tb;
}DECAF_Block_Begin_Params;

typedef struct _DECAF_Tlb_Exec_Params
{
	CPUState *env;
	gva_t vaddr;  //Address loaded to tlb exec cache
} DECAF_Tlb_Exec_Params;

typedef struct _DECAF_Opcode_Range_Params
{
	CPUState *env;
	gva_t eip;
	gva_t next_eip;
	uint16_t op;
} DECAF_Opcode_Range_Params;

typedef struct _DECAF_Block_End_Params
{
  CPUState* env;
  TranslationBlock* tb;
  //THIS IS A PC value - NOT EIP!!!!
  gva_t cur_pc;
  gva_t next_pc;
} DECAF_Block_End_Params;

typedef struct _DECAF_Insn_Begin_Params
{
  CPUState* env;
} DECAF_Insn_Begin_Params;

typedef struct _DECAF_Insn_End_Params
{
  CPUState* env;
} DECAF_Insn_End_Params;
typedef struct _DECAF_Mem_Read_Params
{
	gva_t vaddr;
	gpa_t paddr;
	DATA_TYPE dt;
	unsigned long value;

}DECAF_Mem_Read_Params;
typedef struct _DECAF_Mem_Write_Params
{
	gva_t vaddr;
	gpa_t paddr;
	DATA_TYPE dt;
	unsigned long value;
}DECAF_Mem_Write_Params;
typedef struct _DECAF_EIP_Check_Params
{
	gva_t source_eip;
	gva_t target_eip;
    gva_t target_eip_taint;
}DECAF_EIP_Check_Params;
typedef struct _DECAF_Keystroke_Params
{
	int32_t keycode;
	uint32_t *taint_mark;//mark if this keystroke should be monitored

}DECAF_Keystroke_Params;

typedef struct _DECAF_Nic_Rec_Params
{
	uint8_t *buf;
	int32_t size;
	int32_t cur_pos;
	int32_t start;
	int32_t stop;
}DECAF_Nic_Rec_Params;

typedef struct _DECAF_Nic_Send_Params
{
	uint32_t addr;
	int size;
	uint8_t *buf;
}DECAF_Nic_Send_Params;

typedef struct _DECAF_Read_Taint_Mem
{
	gva_t vaddr;
	gpa_t paddr;
	uint32_t size;
	uint8_t *taint_info;

}DECAF_Read_Taint_Mem;

typedef struct _DECAF_Read_Write_Mem
{
	gva_t vaddr;
	gpa_t paddr;
	uint32_t size;
	uint8_t *taint_info;
}DECAF_Write_Taint_Mem;

#ifdef CONFIG_TCG_LLVM
typedef struct _DECAF_Block_Trans_Params
{
	struct TranslationBlock *tb;
	struct TCGContext *tcg_ctx;
}DECAF_Block_Trans_Params;
#endif /* CONFIG_TCG_LLVM */

typedef struct _DECAF_VTX_Params
{
	uint32_t exit_reason;
}DECAF_VTX_Params;

//LOK: A dummy type
typedef struct _DECAF_Callback_Params
{
	DECAF_Handle cbhandle;
	union{
		DECAF_Block_Begin_Params bb;
		DECAF_Block_End_Params be;
		DECAF_Insn_Begin_Params ib;
		DECAF_Insn_End_Params ie;
		DECAF_Mem_Read_Params mr;
		DECAF_Mem_Write_Params mw;
		DECAF_EIP_Check_Params ec;
		DECAF_Keystroke_Params ks;
		DECAF_Nic_Rec_Params nr;
		DECAF_Nic_Send_Params ns;
		DECAF_Opcode_Range_Params op;
		DECAF_Tlb_Exec_Params tx;
		DECAF_Read_Taint_Mem rt;
		DECAF_Write_Taint_Mem wt;
#ifdef CONFIG_TCG_LLVM
		DECAF_Block_Trans_Params bt;
#endif /* CONFIG_TCG_LLVM */
		DECAF_VTX_Params vtx;
	};
} DECAF_Callback_Params;

typedef void (*DECAF_callback_func_t)(DECAF_Callback_Params*);


#ifdef __cplusplus
}
#endif

#endif//DECAF_CALLBACK_COMMON_H
