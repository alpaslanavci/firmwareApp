/*
 * Linker script for STM32L073RZ - Bootloader
 * 
 * Memory Map:
 *   Flash: 192KB @ 0x08000000
 *   RAM:   20KB  @ 0x20000000
 *
 * Bootloader occupies the first 32KB of flash (0x08000000 - 0x08007FFF)
 */

/* Enforce emission of the vector table. */
EXTERN(vector_table)

/* Define the entry point of the output file. */
ENTRY(reset_handler)

/* Define memory regions. */
MEMORY
{
    rom (rx)  : ORIGIN = 0x08000000, LENGTH = 32K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/* Define sections. */
SECTIONS
{
    .text : {
        *(.vectors)     /* Vector table */
        *(.text*)       /* Program code */
        . = ALIGN(4);
        *(.rodata*)     /* Read-only data */
        . = ALIGN(4);
    } >rom

    /* C++ Static constructors/destructors */
    .preinit_array : {
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
    } >rom

    .init_array : {
        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;
    } >rom

    .fini_array : {
        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;
    } >rom

    .ARM.extab : {
        *(.ARM.extab*)
    } >rom

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
    } >rom

    . = ALIGN(4);
    _etext = .;

    /* RAM section, not cleared on reset */
    .noinit (NOLOAD) : {
        *(.noinit*)
    } >ram
    . = ALIGN(4);

    .data : {
        _data = .;
        *(.data*)       /* Read-write initialized data */
        *(.ramtext*)    /* "text" functions to run in RAM */
        . = ALIGN(4);
        _edata = .;
    } >ram AT >rom
    _data_loadaddr = LOADADDR(.data);

    .bss : {
        *(.bss*)        /* Read-write zero initialized data */
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >ram

    /*
     * The .eh_frame section appears to be used for C++ exception handling.
     * You may need to fix this if you're using C++.
     */
    /DISCARD/ : { *(.eh_frame) }

    . = ALIGN(4);
    end = .;
}

PROVIDE(_stack = ORIGIN(ram) + LENGTH(ram));
