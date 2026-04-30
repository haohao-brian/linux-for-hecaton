#ifndef _HECATON_HEADER
#define _HECATON_HEADER

#include <linux/types.h>
#include <asm/ptrace.h>

struct hecaton_data_aarch64 {
	int16_t supported;
	int16_t frame_size;
	int16_t x19;
	int16_t x20;
	int16_t x21;
	int16_t x22;
	int16_t x23;
	int16_t x24;
	int16_t x25;
	int16_t x26;
	int16_t x27;
	int16_t x28;
	int16_t x29; // Frame Pointer (fp)
	int16_t x30; // Link Register (lr)
};

int lookup_hecaton_index_aarch64(uint64_t fault_address,
				 struct hecaton_data_aarch64 *);
void hecaton_update_pt_regs_aarch64(struct pt_regs *regs, int badmode_flag);

#endif