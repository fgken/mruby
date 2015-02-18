#include "mruby.h"
#include "mruby/array.h"
//#include "mruby/compile.h"
#include "mruby/irep.h"
#include "mruby/proc.h"
#include "mruby/string.h"
#include "mruby/dump.h"
#include "mruby/variable.h"

#include "hello-mruby.h"

int main(int argc, char **argv)
{
  mrb_state *mrb = mrb_open();
  int n = -1;
  int i;
  mrb_value ARGV;
  mrbc_context *c;
  mrb_value val;
  mrb_sym zero_sym;

  ARGV = mrb_ary_new_capa(mrb, 3);
  for (i = 0; i < 3; i++) {
    mrb_ary_push(mrb, ARGV, mrb_str_new_cstr(mrb, "test argv"));
  }
  mrb_define_global_const(mrb, "ARGV", ARGV);

  c = mrbc_context_new(mrb);
  //c->dump_result = TRUE;		// verbose mode

  /* Set $0 */
  zero_sym = mrb_intern_lit(mrb, "$0");
  mrbc_filename(mrb, c, "fgkenvm");
  mrb_gv_set(mrb, zero_sym, mrb_str_new_lit(mrb, "fgkenvm"));

  /* Load program */
  {
	  mrb_irep *irep;
	  struct RProc *proc;

	  irep = mrb_read_irep(mrb, hello_fgkenvm);

	  if (!irep) {
		  return -1;
	  }
	  proc = mrb_proc_new(mrb, irep);
	  mrb_irep_decref(mrb, irep);
	  val = mrb_toplevel_run(mrb, proc);
  }

  mrbc_context_free(mrb, c);
  if (mrb->exc) {
    if (!mrb_undef_p(val)) {
      mrb_print_error(mrb);
    }
    n = -1;
  }
  
  return n == 0 ? 0 : -1;
}
