# Tools
AS      = nasm
LD      = ld

# Flags
ASFLAGS = -f elf
LDFLAGS = -T src/linker.ld -m elf_i386

# Paths
SRC     = src
BUILD   = build

# Files
OBJECTS = $(BUILD)/boot.o
KERNEL  = $(BUILD)/kernel.elf

# Default target
all: $(KERNEL)

# Link
$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ -o $@

# Assemble
$(BUILD)/%.o: $(SRC)/%.asm
	@mkdir -p $(BUILD)
	$(AS) $(ASFLAGS) $< -o $@

# Run normally
run: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

# Run with GDB
run-debug: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL) -nographic -s -S

# Clean
clean:
	rm -rf $(BUILD)
