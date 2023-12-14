/*
 * (c) Bit Parallel Ltd, 2021
 *
 * note, left out the ENTRY() directive as it's pretty meaningless in simple embedded applications
 */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 128K
}

__ram_start__ = ORIGIN(SRAM);
__ram_size__  = LENGTH(SRAM);
__ram_end__   = __ram_start__ + __ram_size__;
_estack = __ram_end__;

/* used to guarantee that after linking there is a minimum ammount at RAM reserved for use by the heap and stack */
PROVIDE(_min_stack_reserve = 0x4000);
PROVIDE(_min_heap_reserve = 0x4000);

SECTIONS
{
    /* place the vector table at the start of flash */
    .vector_table :
    {
        . = ALIGN(4);
        KEEP (*(.vector_table))
        . = ALIGN(4);
    } >FLASH

    /* the main program code */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH

    /* contains read-only data, constants, strings, stuff that doesn't change */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /* this section contains information for unwinding the stack, used when handling exceptions */
    .ARM.extab :
    {
        . = ALIGN(4);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > FLASH

    /* this section contains information for unwinding the stack, used when handling exceptions */
    .ARM.exidx :
    {
        . = ALIGN(4);
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
        . = ALIGN(4);
    } >FLASH

    /* these sections hold pointers to constructors and destructors that get called before and after main()                          */
    /* preinit_array and init_array reference functions to be called before main() and fini_array references are called after main() */
    /* as this script is for embedded developent, fini_array can be ignored as it will never be needed, left in for completeness...  */
    .preinit_array :
    {
        . = ALIGN(4);
        _spreinit_array = .;
        KEEP (*(.preinit_array))
        KEEP (*(.preinit_array*))
        _epreinit_array = .;
        . = ALIGN(4);
        } >FLASH

    .init_array :
    {
        . = ALIGN(4);
        _sinit_array = .;
        KEEP (*(.init_array))
        KEEP (*(.init_array*))
        _einit_array = .;
        . = ALIGN(4);
    } >FLASH

    .fini_array :
    {
        . = ALIGN(4);
        _sfini_array = .;
        KEEP (*(.fini_array))
        KEEP (*(.fini_array*))
        _efini_array = .;
        . = ALIGN(4);
    } >FLASH

    /* space set aside in RAM for variables etc, _sdata and _edata mark */
    /* the start/end locations for this section                         */
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } >SRAM

    /* this section is similar to the 'data' section but is initialized to 0   */
    /* by the startup code, _sbss and _ebss mark the start/end of this section */
    .bss :
    {
        . = ALIGN(4);
        /*__bss_start__ = .;*/
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        /*__bss_end__ = .;*/
        _ebss = .;
    } >SRAM

    /* note, this must be the last SRAM section!                                                    */
    /*                                                                                              */
    /* this virtual SRAM section, the heap grows up towards __ram_end__ whilst the stack grows down */
    /* referenced by _sbrk() in the libstdc++_nano minimal stubs                                    */
    .heap_stack (NOLOAD) :
    {
        . = ALIGN(4);
        PROVIDE(_heap_end = .);
        *(.heap_stack)
        __heap_stack_limit = .;
        . = ALIGN(4);
        ASSERT((__ram_end__ - _heap_end) > (_min_stack_reserve + _min_heap_reserve), "Error: There is no SRAM left to satisfy the heap and stack minimum values");
    } >SRAM
}
