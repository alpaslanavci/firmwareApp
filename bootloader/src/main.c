#include "common-defines.h"
#include <libopencm3/stm32/memorymap.h>
#include <libopencm3/cm3/vector.h>

#define BOOTLOADER_SIZE     (0x8000U)
#define FIRMWARE_ADDRESS    (FLASH_BASE + BOOTLOADER_SIZE)

/* const uint8_t data[0x8000] = {0}; == Compiler will optimize this by discarding the unused data. You can see this line of 
code inside the bootloader.map discarded section. */


static void jump_to_main( void ) {
    vector_table_t* main_vector_entry = (vector_table_t*)(FIRMWARE_ADDRESS);
    main_vector_entry->reset(); 
}

int main( void ) {

    jump_to_main();

    return 0;
}