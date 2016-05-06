#include "DECAF_types.h"
#include "DECAF_main.h"
#include "DECAF_callback.h"
#include "DECAF_callback_common.h"
#include "utils/Output.h"

#define BIT_SET(x, y) ((x) & (1UL << (y)))


static plugin_interface_t my_interface;
static DECAF_Handle vtx_handle = DECAF_NULL_HANDLE;

// exit bitmap: bit 'x' is set if reason (x-1) is enabled
static uint64_t exit_bitmap = 0xFFFFFFFFFFFFFFFF;
// seperate flag for exceptions (exit reason 0)
static uint32_t enable_exceptions = 1; 

static int callback_counts[65];
static int exception_counts[256];

static void vtx_callback(DECAF_Callback_Params* params) {

  uint32_t reason = params->vtx.exit_reason;
  uint32_t vector = params->vtx.vector;
  
  if (reason == VTX_EXIT_EXCP_NMI && enable_exceptions){
    ++callback_counts[VTX_EXIT_EXCP_NMI];
    if (vector < 0x20)
      ++exception_counts[vector];
    return;
  }

  if (BIT_SET(exit_bitmap, reason - 1)){
    ++callback_counts[ (int)reason ];
    return;
  }

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

static int enable_exit(Monitor *mon, const QDict *qdict) {

  int32_t reason = qdict_get_int(qdict,"exit_reason");

  if (reason == -1){
    enable_exceptions = 1;
    exit_bitmap = 0xFFFFFFFFFFFFFFFF;
    return 0;
  }

  if (!(VTX_EXIT_EXCP_NMI <= reason && reason <= VTX_EXIT_XRSTORS)){
    DECAF_printf("Exit reason out of range\n");
    return -1;
  }


  if (reason == VTX_EXIT_EXCP_NMI){
    enable_exceptions = 1;
  } else {
    exit_bitmap |= (1UL << (reason - 1));
  }

  return 0;
}


static int disable_exit(Monitor *mon, const QDict *qdict) {

  int32_t reason = qdict_get_int(qdict,"exit_reason");

  if (reason == -1){
    enable_exceptions = exit_bitmap = 0;
    return 0;
  }

  if (!(VTX_EXIT_EXCP_NMI <= reason && reason <= VTX_EXIT_XRSTORS)){
    DECAF_printf("Exit reason out of range\n");
    return -1;
  }


  if (reason == VTX_EXIT_EXCP_NMI){
    enable_exceptions = 0;
  } else {
    exit_bitmap &= ~(1UL << (reason - 1));
  }

  return 0;
}

static void print_exit_counts(){
    
    int i;
    DECAF_printf("\n{ \"exit_counts\": {");

    for (i=0; i<64; i++){
      if (i == 35 ||
          i == 38 ||
          i == 42 ||
          i == 60 ||
          i == 62) continue;

      DECAF_printf("%d: %d,", i, callback_counts[i]);
    } 
    DECAF_printf("%d: %d", 64, callback_counts[64]);
    DECAF_printf("}}\n");
}

static void print_exception_counts(){
    
    int i;
    DECAF_printf("\n{ \"exception_counts\": {");

    for (i=0; i<0x1F; i++){
      DECAF_printf("%d: %d,", i, exception_counts[i]);
    } 
    DECAF_printf("%d: %d", 0x1F, exception_counts[0x1F]);
    DECAF_printf("}}\n");
}

static void reset_counts(){
      
    memset(callback_counts, 0, sizeof(int) * 65);
    memset(exception_counts, 0, sizeof(int) * 256);

    DECAF_printf("Counts reset\n");
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
  {
    .name           = "enable_exit",
    .args_type      = "exit_reason:i",
    .mhandler.cmd   = enable_exit,
    .params         = "exit_reason",
    .help           = "Enable exit reason <param>, -1 for all"
  },
  {
    .name           = "disable_exit",
    .args_type      = "exit_reason:i",
    .mhandler.cmd   = disable_exit,
    .params         = "exit_reason",
    .help           = "Disable exit reason <param>, -1 for all"
  },
  {
    .name           = "print_exit_counts",
    .args_type      = "",
    .mhandler.cmd   = print_exit_counts,
    .params         = "",
    .help           = "Dump exit counts as json dict"
  },
  {
    .name           = "print_exception_counts",
    .args_type      = "",
    .mhandler.cmd   = print_exception_counts,
    .params         = "",
    .help           = "Dump exception exit counts as json dict"
  },
  {
    .name           = "reset_counts",
    .args_type      = "",
    .mhandler.cmd   = reset_counts,
    .params         = "",
    .help           = "Reset exit & exception counts"
  },
  {NULL, NULL, },
};

plugin_interface_t* init_plugin(void) {
  my_interface.mon_cmds = my_term_cmds;
  my_interface.plugin_cleanup = &my_cleanup;
  
  return (&my_interface);
}
