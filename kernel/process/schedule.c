#include "kernel.h"

struct PCB *current_pcb = NULL;

/* You interrupt handler (use add_intr_handle to register)
   just need to set "need_sched" to be TRUE,
   schedule() will be called each time before interrupt return. */

boolean need_sched = TRUE;

void
schedule(void) {
	current_pcb=head1;
	head1=head1->next;
	need_sched = FALSE;
}
