#include <linux/types.h>
#include <linux/hecaton.h>
#include "hecaton_data.h"
#include <linux/kbuild.h>
#include <linux/bug.h>
#include <linux/kallsyms.h>
#include <asm/ptrace.h>
#include <linux/delay.h>
#include <linux/reboot.h>

// extern uint64_t zydis_advance_pc (uint64_t pc);
int hecaton_global_check = 0;

int binary_search_address(uint64_t addr, uint64_t *beg_addrs,
			  uint64_t *end_addrs, int size)
{
	int l, r, m;
	int index;
	//check the not cold functions:
	index = 0;
	l = 0;
	r = size;
	while (l <= r) {
		m = l + (r - l) / 2;
		if (addr > beg_addrs[m + 1])
			l = m + 1;
		else if (addr < beg_addrs[m - 1])
			r = m - 1;
		else if (addr > beg_addrs[m]) {
			index = m;
			break;
		} else {
			index = m - 1;
			break;
		}
	}
	if (addr < end_addrs[index]) {
		return index;
	} else {
		return -1;
	}
}

int lookup_hecaton_index_aarch64(uint64_t fault_address,
				 struct hecaton_data_aarch64 *data)
{
	int cold_index;
	int index;
	index = binary_search_address(fault_address, hecaton_fnc_begin_addrs,
				      hecaton_fnc_end_addrs,
				      HECATON_ARRAY_SIZE);
	if (index > 0) {
		printk(KERN_ALERT "Hecaton: found in not cold part\n");
		printk(KERN_ALERT
		       "index = %d, supported = %d,  base_addr %llx\n",
		       index, hecaton_supported[index],
		       (long long unsigned int)hecaton_fnc_begin_addrs[index]);
		printk(KERN_ALERT
		       "x19=%d, x20=%d, x21=%d, x22=%d, x23=%d, x24=%d, x25=%d, x26=%d, x27=%d, x28=%d, x29=%d, x30=%d\n",
		       hecaton_x19[index], hecaton_x20[index],
		       hecaton_x21[index], hecaton_x22[index],
		       hecaton_x23[index], hecaton_x24[index],
		       hecaton_x25[index], hecaton_x26[index],
		       hecaton_x27[index], hecaton_x28[index],
		       hecaton_x29[index], hecaton_x30[index]);

		data->supported = hecaton_supported[index];
		data->frame_size = hecaton_frame_size[index];
		data->x19 = hecaton_x19[index];
		data->x20 = hecaton_x20[index];
		data->x21 = hecaton_x21[index];
		data->x22 = hecaton_x22[index];
		data->x23 = hecaton_x23[index];
		data->x24 = hecaton_x24[index];
		data->x25 = hecaton_x25[index];
		data->x26 = hecaton_x26[index];
		data->x27 = hecaton_x27[index];
		data->x28 = hecaton_x28[index];
		data->x29 = hecaton_x29[index]; // fp offset
		data->x30 = hecaton_x30[index]; // lr offset
		return 0;
	} else {
		cold_index = binary_search_address(fault_address,
						   hecaton_cold_begin_addrs,
						   hecaton_cold_end_addrs,
						   HECATON_COLD_ARRAY_SIZE);
		printk(KERN_ALERT "Hecaton: Should be found in cold part\n");
		if (cold_index > 0) {
			printk(KERN_ALERT "Hecaton: found in cold part\n");
			index = hecaton_cold_begin_index[cold_index];
			printk(KERN_ALERT
			       "index = %d, supported = %d,  base_addr %llx\n",
			       index, hecaton_supported[index],
			       (long long unsigned int)
				       hecaton_cold_begin_addrs[cold_index]);
			printk(KERN_ALERT
			       "x19=%d, x20=%d, x21=%d, x22=%d, x23=%d, x24=%d, x25=%d, x26=%d, x27=%d, x28=%d, x29=%d, x30=%d\n",
			       hecaton_x19[index], hecaton_x20[index],
			       hecaton_x21[index], hecaton_x22[index],
			       hecaton_x23[index], hecaton_x24[index],
			       hecaton_x25[index], hecaton_x26[index],
			       hecaton_x27[index], hecaton_x28[index],
			       hecaton_x29[index], hecaton_x30[index]);

			data->supported = hecaton_supported[index];
			data->frame_size = hecaton_frame_size[index];
			data->x19 = hecaton_x19[index];
			data->x20 = hecaton_x20[index];
			data->x21 = hecaton_x21[index];
			data->x22 = hecaton_x22[index];
			data->x23 = hecaton_x23[index];
			data->x24 = hecaton_x24[index];
			data->x25 = hecaton_x25[index];
			data->x26 = hecaton_x26[index];
			data->x27 = hecaton_x27[index];
			data->x28 = hecaton_x28[index];
			data->x29 = hecaton_x29[index]; // fp offset
			data->x30 = hecaton_x30[index]; // lr offset

			return 0;
		} else {
			printk(KERN_ALERT
			       "Hecaton ERROR: Address %llx not found in tables\n",
			       fault_address);
			return -1;
		}
	}
}

void hecaton_update_pt_regs_aarch64(struct pt_regs *regs, int badmode_flag)
{
	unsigned long shadow_regs[31];
	uint64_t current_pc = regs->pc;
	uint64_t current_sp = regs->sp;
	uint64_t current_fp = regs->regs[29];

	int depth = 0;
	int first = 1;
	int index = 0;
	struct hecaton_data_aarch64 hec_data;
	char symb[512];

	int i;
	for (i = 19; i <= 30; i++) {
		shadow_regs[i] = regs->regs[i];
	}

	if (badmode_flag == 1) {
		printk(KERN_ALERT
		       "Hecaton: It is a badmode bug not implemented yet\n");
	}
	sprint_symbol(symb, instruction_pointer(regs));
	printk(KERN_ALERT
	       "BUG: hecaton recovered bug PC is at: %s (addr: %llx)\n",
	       symb, current_pc);
	dump_stack();

	while (1) {
		depth++;
		if (depth > 7) {
			printk(KERN_ALERT
			       "Hecaton: Max unwind depth reached. Aborting.\n");
			msleep(2000);
			emergency_restart();
		}

		// Lookup Metadata for current PC
		printk(KERN_ALERT "Hecaton: looking up for the pc = %llx\n",
		       (unsigned long long)current_pc);
		index = lookup_hecaton_index_aarch64(current_pc, &hec_data);
		printk(KERN_ALERT
		       "Hecaton: inside %s,,,, hecaton_index = %d, supported = %d\n",
		       __func__, index, hec_data.supported);
		if (index != 0) {
			printk(KERN_ALERT
			       "Hecaton: PC %llx not found in tables. Unwind failed.\n",
			       current_pc);
			break;
		}

		// Check if the pc resides in a bowknot supported function
		if (hec_data.supported == 1) {
			printk(KERN_ALERT
			       "Hecaton: Found recovery point. Committing registers.\n");

			for (i = 19; i <= 28; i++) {
				regs->regs[i] = shadow_regs[i];
			}

			regs->regs[29] = current_fp;
			regs->regs[30] = shadow_regs[30];

			regs->sp = current_sp;

			if (first == 1) {
				regs->pc = current_pc + 4;
			} else {
				regs->pc = current_pc;
			}

			current->hf = 1;
			break;
		}

		first = 0;

		// Recursive Unwind (Prepare for next frame)
		printk(KERN_ALERT
		       "Hecaton: Unwinding stack frame at %llx (depth %d)\n",
		       current_fp, depth);

		// A. Restore Callee-Saved Registers
		printk(KERN_ALERT
		       "Hecaton: offsets are: x19=%d, x20=%d, x21=%d, x22=%d, x23=%d, x24=%d, x25=%d, x26=%d, x27=%d, x28=%d, x29=%d, x30=%d\n",
		       hec_data.x19, hec_data.x20, hec_data.x21, hec_data.x22,
		       hec_data.x23, hec_data.x24, hec_data.x25, hec_data.x26,
		       hec_data.x27, hec_data.x28, hec_data.x29, hec_data.x30);

		if (hec_data.x19 != 100)
			shadow_regs[19] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x19));
		if (hec_data.x20 != 100)
			shadow_regs[20] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x20));
		if (hec_data.x21 != 100)
			shadow_regs[21] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x21));
		if (hec_data.x22 != 100)
			shadow_regs[22] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x22));
		if (hec_data.x23 != 100)
			shadow_regs[23] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x23));
		if (hec_data.x24 != 100)
			shadow_regs[24] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x24));
		if (hec_data.x25 != 100)
			shadow_regs[25] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x25));
		if (hec_data.x26 != 100)
			shadow_regs[26] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x26));
		if (hec_data.x27 != 100)
			shadow_regs[27] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x27));
		if (hec_data.x28 != 100)
			shadow_regs[28] = READ_ONCE_NOCHECK(
				*(unsigned long *)(current_fp + hec_data.x28));

		// B. Calculate Next Frame Coordinates
		// Default offsets: saved FP at [FP], saved LR at [FP+8]
		int offset_fp = (hec_data.x29 != 100) ? hec_data.x29 : 0;
		int offset_lr = (hec_data.x30 != 100) ? hec_data.x30 : 8;

		uint64_t old_fp = READ_ONCE_NOCHECK(
			*(unsigned long *)(current_fp + offset_fp));
		uint64_t old_pc = READ_ONCE_NOCHECK(
			*(unsigned long *)(current_fp + offset_lr));

		// C. Restore Stack Pointer (SP)
		if (hec_data.frame_size > 0) {
			current_sp = current_fp + hec_data.frame_size;
		} else {
			current_sp = current_fp + 16;
		}

		// D. Update Regs for the Next Iteration
		shadow_regs[29] =
			old_fp; // Update Shadow FP (for consistency if needed)
		shadow_regs[30] = old_pc; // Update Shadow LR

		current_fp = old_fp; // Advance tracking variable
		current_pc = old_pc; // Advance tracking variable
	}
}