/* STM32F446RE memory map (NUCLEO-F446RE)

   Flash (code + constants + initial values for .data):  512 KB starting at 0x08000000
   RAM   (stack + .data runtime + .bss):                 128 KB starting at 0x20000000
*/
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{

  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)

    KEEP(*(.init))
    KEEP(*(.fini))

    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)

    . = ALIGN(4);
    _etext = .;
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  . = ALIGN(4);
  end = .;
  _end = .;
}
