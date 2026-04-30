.data
.pushsection .hyp.reloc, "a"
.global __hyp_section_.hyp.idmap.text
.word 0
.reloc 0, R_AARCH64_PREL32, __hyp_section_.hyp.idmap.text + 0xab0
.word 0
.reloc 4, R_AARCH64_PREL32, __hyp_section_.hyp.idmap.text + 0xab8
.global __hyp_section_.hyp.text
.word 0
.reloc 8, R_AARCH64_PREL32, __hyp_section_.hyp.text + 0x2fe8
.word 0
.reloc 12, R_AARCH64_PREL32, __hyp_section_.hyp.text + 0x2ff0
.global __hyp_section_.hyp.rodata
.word 0
.reloc 16, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x220
.word 0
.reloc 20, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x228
.word 0
.reloc 24, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x230
.word 0
.reloc 28, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x238
.word 0
.reloc 32, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x240
.word 0
.reloc 36, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x248
.word 0
.reloc 40, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x250
.word 0
.reloc 44, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x258
.word 0
.reloc 48, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x260
.word 0
.reloc 52, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x268
.word 0
.reloc 56, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x270
.word 0
.reloc 60, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x278
.word 0
.reloc 64, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x280
.word 0
.reloc 68, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x288
.word 0
.reloc 72, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x290
.word 0
.reloc 76, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x298
.word 0
.reloc 80, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x2a0
.word 0
.reloc 84, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x2a8
.word 0
.reloc 88, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x2b0
.word 0
.reloc 92, R_AARCH64_PREL32, __hyp_section_.hyp.rodata + 0x2b8
.popsection
