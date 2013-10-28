/*
 * Maple (STM32F103RBT6, medium density) linker script for Flash builds.
 */

/*
 * Define memory spaces.
 */
MEMORY
{
  ram (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
  rom (rx)  : ORIGIN = 0x08003000, LENGTH = 56K /*[ROBOTIS][CHANGE]support for cm-900
  
  /*RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K  
  FLASH (rx) : ORIGIN = 0x8003000, LENGTH = 64K
  FLASHB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0*/
  
}

/*
 * Use medium density device vector table
 */
GROUP(libcs3_stm32_med_density.a)

REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_BSS", ram);
REGION_ALIAS("REGION_RODATA", rom);

/*
 * Define the rest of the sections
 */
_FLASH_BUILD = 1;

INCLUDE common.inc
