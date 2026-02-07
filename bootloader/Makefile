# Project name
PROJECT = bootloader

# Toolchain prefix
PREFIX ?= arm-none-eabi-

# Toolchain commands
CC      = $(PREFIX)gcc
LD      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE    = $(PREFIX)size

# Directories
OPENCM3_DIR = ../libopencm3
SRC_DIR     = src
INC_DIR     = inc
BUILD_DIR   = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Linker script
LDSCRIPT = linkerscript.ld

# MCU specific flags
ARCH_FLAGS = -mcpu=cortex-m0plus -mthumb -msoft-float

# Include paths
INCLUDES = -I$(INC_DIR) \
           -I$(OPENCM3_DIR)/include \

# Compiler flags
CFLAGS = $(ARCH_FLAGS) \
         -Os \
         -Wall -Wextra -Wshadow -Wimplicit-function-declaration \
         -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
         -fno-common -ffunction-sections -fdata-sections \
         -std=c99 \
         -ggdb3 \
         -DSTM32L0 \
         $(INCLUDES)

# Linker flags
LDFLAGS = $(ARCH_FLAGS) \
          -T$(LDSCRIPT) \
          -L$(OPENCM3_DIR)/lib \
          -lopencm3_stm32l0 \
          -Wl,--gc-sections \
          -nostartfiles \
          --specs=nano.specs \
          --specs=nosys.specs \
          -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

# Default target
all: $(BUILD_DIR) $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex size

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@printf "  CC      $(<F)\n"
	$(Q)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Link
$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)
	@printf "  LD      $(PROJECT).elf\n"
	$(Q)$(LD) $(OBJS) $(LDFLAGS) -o $@

# Create binary
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@printf "  OBJCOPY $(PROJECT).bin\n"
	$(Q)$(OBJCOPY) -O binary $< $@
	$(Q)python3 padder.py
	

# Create hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@printf "  OBJCOPY $(PROJECT).hex\n"
	$(Q)$(OBJCOPY) -O ihex $< $@

# Print size
size: $(BUILD_DIR)/$(PROJECT).elf
	@printf "  SIZE    $(PROJECT).elf\n"
	$(Q)$(SIZE) $<

# Flash using st-flash
flash: $(BUILD_DIR)/$(PROJECT).bin
	@printf "  FLASH   $(PROJECT).bin\n"
	$(Q)st-flash write $< 0x08000000

# Clean build files
clean:
	@printf "  CLEAN\n"
	$(Q)rm -rf $(BUILD_DIR)

# Disassemble
disasm: $(BUILD_DIR)/$(PROJECT).elf
	$(Q)$(OBJDUMP) -d $< > $(BUILD_DIR)/$(PROJECT).dis

.PHONY: all clean flash size disasm

# Include dependency files
-include $(OBJS:.o=.d)
