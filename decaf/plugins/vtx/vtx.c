#include "DECAF_types.h"
#include "DECAF_main.h"
#include "DECAF_callback.h"
#include "DECAF_callback_common.h"
#include "utils/Output.h"

static plugin_interface_t my_interface;
static DECAF_Handle vtx_handle = DECAF_NULL_HANDLE;

static void my_vtx_callback(DECAF_Callback_Params* params) {
  DECAF_printf("Got vtx callback\n");
}

static int start_vtx(void) {
  DECAF_printf("Registering VTX callback handler...\n");

  vtx_handle = DECAF_register_callback(DECAF_VTX_CB, &my_vtx_callback, NULL);
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
