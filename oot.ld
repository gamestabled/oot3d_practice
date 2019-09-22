OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(main_patch)
SECTIONS{
    . = 0x2e3790;
    .patch_load : {
        *(.patch_load*)
    }

	. = 0x310254;
	. = ALIGN(4);
	.patch_main : {
		*(.patch_main*)
	}

	. = 0x4c99a8;
	. = ALIGN(4);
	.text : {
		*(.text*)
	}

	. = 0x4f8348;
	. = ALIGN(4);
	.rodata : {
		*(.rodata*)
	}

	. = 0x5c6610;
	. = ALIGN(4);
	.data : {
		*(.data*)
	}
	.bss : {
		*(.bss*)
	}
}
