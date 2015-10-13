#include "DECAF_types.h"
#include "DECAF_main.h"
#include "DECAF_callback.h"
#include "DECAF_callback_common.h"
#include "utils/Output.h"



static plugin_interface_t my_interface;
static DECAF_Handle vtx_handle = DECAF_NULL_HANDLE;

static int callback_counts[64];

static void vtx_callback(DECAF_Callback_Params* params) {

  DECAF_printf("Got vtx callback -- ");

  uint32_t reason = params->vtx.exit_reason;
  
  ++callback_counts[ (int)reason ];
  
  switch (reason){
    case VTX_EXIT_NMI: 							DECAF_printf("VTX_EXIT_NMI\n");							break;
    case VTX_EXIT_EXT_INT: 						DECAF_printf("VTX_EXIT_EXT_INT\n");						break;
    case VTX_EXIT_TRIPLE_FAULT: 				DECAF_printf("VTX_EXIT_TRIPLE_FAULT\n");				break;
    case VTX_EXIT_INIT_SIG: 					DECAF_printf("VTX_EXIT_INIT_SIG\n");					break;
    case VTX_EXIT_SIPI: 						DECAF_printf("VTX_EXIT_SIPI\n");						break;
    case VTX_EXIT_SMI: 							DECAF_printf("VTX_EXIT_SMI\n");							break;
    case VTX_EXIT_OtherSMI: 					DECAF_printf("VTX_EXIT_OtherSMI\n");					break;
    case VTX_EXIT_INT_WINDOW: 					DECAF_printf("VTX_EXIT_INT_WINDOW\n");					break;
    case VTX_EXIT_NMI_WINDOW: 					DECAF_printf("VTX_EXIT_NMI_WINDOW\n");					break;
    case VTX_EXIT_TASK_SWITCH: 					DECAF_printf("VTX_EXIT_TASK_SWITCH\n");					break;
    case VTX_EXIT_CPUID: 						DECAF_printf("VTX_EXIT_CPUID\n");						break;
    case VTX_EXIT_GETSEC: 						DECAF_printf("VTX_EXIT_GETSEC\n");						break;
    case VTX_EXIT_HLT: 							DECAF_printf("VTX_EXIT_HLT\n");							break;
    case VTX_EXIT_INVD: 						DECAF_printf("VTX_EXIT_INVD\n");						break;
    case VTX_EXIT_INVLPG: 						DECAF_printf("VTX_EXIT_INVLPG\n");						break;
    case VTX_EXIT_RDPMC: 						DECAF_printf("VTX_EXIT_RDPMC\n");						break;
    case VTX_EXIT_RDTSC: 						DECAF_printf("VTX_EXIT_RDTSC\n");						break;
    case VTX_EXIT_RSM: 							DECAF_printf("VTX_EXIT_RSM\n");							break;
    case VTX_EXIT_VMCALL: 						DECAF_printf("VTX_EXIT_VMCALL\n");						break;
    case VTX_EXIT_VMCLEAR: 						DECAF_printf("VTX_EXIT_VMCLEAR\n");						break;
    case VTX_EXIT_VMLAUNCH: 					DECAF_printf("VTX_EXIT_VMLAUNCH\n");					break;
    case VTX_EXIT_VMPTRLD: 						DECAF_printf("VTX_EXIT_VMPTRLD\n");						break;
    case VTX_EXIT_VMPTRST: 						DECAF_printf("VTX_EXIT_VMPTRST\n");						break;
    case VTX_EXIT_VMREAD: 						DECAF_printf("VTX_EXIT_VMREAD\n");						break;
    case VTX_EXIT_VMRESUME: 					DECAF_printf("VTX_EXIT_VMRESUME\n");					break;
    case VTX_EXIT_VMWRITE: 						DECAF_printf("VTX_EXIT_VMWRITE\n");						break;
    case VTX_EXIT_VMXOFF: 						DECAF_printf("VTX_EXIT_VMXOFF\n");						break;
    case VTX_EXIT_VMXON: 						DECAF_printf("VTX_EXIT_VMXON\n");						break;
    case VTX_EXIT_CR_ACCESS: 					DECAF_printf("VTX_EXIT_CR_ACCESS\n");					break;
    case VTX_EXIT_MOV_DR: 						DECAF_printf("VTX_EXIT_MOV_DR\n");						break;
    case VTX_EXIT_IO: 							DECAF_printf("VTX_EXIT_IO\n");							break;
    case VTX_EXIT_RDMSR: 						DECAF_printf("VTX_EXIT_RDMSR\n");						break;
    case VTX_EXIT_WRMSR: 						DECAF_printf("VTX_EXIT_WRMSR\n");						break;
    case VTX_EXIT_VM_ENTRY_FAIL_GUEST: 			DECAF_printf("VTX_EXIT_VM_ENTRY_FAIL_GUEST\n");			break;
    case VTX_EXIT_VM_ENTRY_FAIL_MSR: 			DECAF_printf("VTX_EXIT_VM_ENTRY_FAIL_MSR\n");			break;
    case VTX_EXIT_MWAIT: 						DECAF_printf("VTX_EXIT_MWAIT\n");						break;
    case VTX_EXIT_MONITOR_TRAP: 				DECAF_printf("VTX_EXIT_MONITOR_TRAP\n");				break;
    case VTX_EXIT_MONITOR: 						DECAF_printf("VTX_EXIT_MONITOR\n");						break;
    case VTX_EXIT_PAUSE: 						DECAF_printf("VTX_EXIT_PAUSE\n");						break;
    case VTX_EXIT_VM_ENTRY_FAIL_MACHINE_CHECK: 	DECAF_printf("VTX_EXIT_VM_ENTRY_FAIL_MACHINE_CHECK\n");	break;
    case VTX_EXIT_TPR_THRESHHOLD: 				DECAF_printf("VTX_EXIT_TPR_THRESHHOLD\n");				break;
    case VTX_EXIT_APIC_ACCESS: 					DECAF_printf("VTX_EXIT_APIC_ACCESS\n");					break;
    case VTX_EXIT_VIRT_EOI: 					DECAF_printf("VTX_EXIT_VIRT_EOI\n");					break;
    case VTX_EXIT_GDTR_IDTR_ACCESS: 			DECAF_printf("VTX_EXIT_GDTR_IDTR_ACCESS\n");			break;
    case VTX_EXIT_LDTR_TR_ACCESS: 				DECAF_printf("VTX_EXIT_LDTR_TR_ACCESS\n");				break;
    case VTX_EXIT_EPT_VIOLATION: 				DECAF_printf("VTX_EXIT_EPT_VIOLATION\n");				break;
    case VTX_EXIT_EPT_MISCONFIG: 				DECAF_printf("VTX_EXIT_EPT_MISCONFIG\n");				break;
    case VTX_EXIT_INVEPT: 						DECAF_printf("VTX_EXIT_INVEPT\n");						break;
    case VTX_EXIT_RDTSCP: 						DECAF_printf("VTX_EXIT_RDTSCP\n");						break;
    case VTX_EXIT_VMX_TIMER_EXP: 				DECAF_printf("VTX_EXIT_VMX_TIMER_EXP\n");				break;
    case VTX_EXIT_INVVPID: 						DECAF_printf("VTX_EXIT_INVVPID\n");						break;
    case VTX_EXIT_WBINVD: 						DECAF_printf("VTX_EXIT_WBINVD\n");						break;
    case VTX_EXIT_XSETBV: 						DECAF_printf("VTX_EXIT_XSETBV\n");						break;
    case VTX_EXIT_APIC_WRITE: 					DECAF_printf("VTX_EXIT_APIC_WRITE\n");					break;
    case VTX_EXIT_RDRAND: 						DECAF_printf("VTX_EXIT_RDRAND\n");						break;
    case VTX_EXIT_INVPCID: 						DECAF_printf("VTX_EXIT_INVPCID\n");						break;
    case VTX_EXIT_VMFUNC: 						DECAF_printf("VTX_EXIT_VMFUNC\n");						break;
    case VTX_EXIT_RDSEED: 						DECAF_printf("VTX_EXIT_RDSEED\n");						break;
    case VTX_EXIT_XSAVES: 						DECAF_printf("VTX_EXIT_XSAVES\n");						break;
    case VTX_EXIT_XRSTORS: 						DECAF_printf("VTX_EXIT_XRSTORS\n");						break;
    default: DECAF_printf("Unknown\n");
  }

  return;

}

static int start_vtx(void) {
  DECAF_printf("Registering VTX callback handler...\n");

  vtx_handle = DECAF_register_callback(DECAF_VTX_CB, &vtx_callback, NULL);
  if (vtx_handle == DECAF_NULL_HANDLE)
  {
    DECAF_printf("Could not register for the VTX Callback\n");  
  }
  return (0);
}

static int stop_vtx(void) {
  DECAF_printf("Stopping VTX callback listening...\n");

  if (vtx_handle != DECAF_NULL_HANDLE) {
    DECAF_unregister_callback(DECAF_VTX_CB, vtx_handle);
    vtx_handle = DECAF_NULL_HANDLE;
  }
  return (0);
}

static void my_cleanup(void) {
  DECAF_printf("Unloading vtx...\n");
  stop_vtx();
} 

static mon_cmd_t my_term_cmds[] = {
  {
    .name           = "start_vtx",
    .args_type      = "",
    .mhandler.cmd   = start_vtx,
    .params         = "",
    .help           = "Start listening for VT-x callbacks"
  },
  {
    .name           = "stop_vtx",
    .args_type      = "",
    .mhandler.cmd   = stop_vtx,
    .params         = "",
    .help           = "Stop listening for VT-x callbacks"
  },
  {NULL, NULL, },
};

plugin_interface_t* init_plugin(void) {
  my_interface.mon_cmds = my_term_cmds;
  my_interface.plugin_cleanup = &my_cleanup;
  
  return (&my_interface);
}
