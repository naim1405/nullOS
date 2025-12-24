# NullOS Development Guide

**Last Updated:** December 2024  
**Status:** GDT implementation complete, codebase reorganized

---

## Overview

NullOS is a simple x86 kernel project demonstrating fundamental OS concepts. This guide documents the project structure, build system, and development practices.

---

## Project Structure

```
nullOS/
├── src/
│   ├── boot/           # Bootloader (Multiboot, stack setup)
│   ├── cpu/            # CPU management (GDT, future: IDT, paging)
│   ├── drivers/        # Hardware drivers (serial port)
│   ├── kernel/         # Core kernel (entry point, initialization)
│   ├── lib/            # Utilities (port I/O operations)
│   ├── include/        # Shared headers (type definitions)
│   └── linker.ld       # Linker script
├── build/              # Compiled artifacts (mirrors src/ structure)
├── docs/               # Documentation
└── Makefile            # Build system
```

### Module Breakdown

| Directory | Purpose | Current Files | Future Additions |
|-----------|---------|---------------|------------------|
| `boot/` | Early initialization | `boot.asm` | - |
| `cpu/` | CPU-specific code | `gdt.c/h`, `gdt_load.asm` | `idt.c/h`, `interrupts.c` |
| `drivers/` | Hardware drivers | `serial.c/h` | `keyboard.c`, `vga.c`, `timer.c` |
| `kernel/` | Core kernel logic | `main.c` | `scheduler.c`, `syscalls.c` |
| `lib/` | Helper functions | `port_io.asm/h` | `string.c`, `memory.c` |
| `include/kernel/` | Shared headers | `types.h` | `multiboot.h`, `stddef.h` |

---

## What We've Built

### 1. **Bootloader** (`src/boot/boot.asm`)
- Multiboot-compliant header for GRUB
- Stack initialization (16KB)
- Jumps to C kernel entry point (`kmain`)

### 2. **GDT Setup** (`src/cpu/`)
- **`gdt.c`**: Configures Global Descriptor Table with 3 entries:
  - Null descriptor (required)
  - Code segment (0x08): kernel code, full 4GB, ring 0
  - Data segment (0x10): kernel data, full 4GB, ring 0
- **`gdt_load.asm`**: Loads GDT pointer into CPU, updates segment registers

**Bug fixed:** GDT pointer structure had fields in wrong order (size before base, not base before size).

### 3. **Serial Driver** (`src/drivers/serial.c`)
- COM1 (0x3F8) initialization
- Character output (`putc`), string output (`printf`)
- Character input (`getc`)
- Used for debugging output

### 4. **Port I/O** (`src/lib/port_io.asm`)
- Assembly wrappers for `inb`/`outb` instructions
- Used by drivers to communicate with hardware

### 5. **Kernel Entry** (`src/kernel/main.c`)
```c
void kmain() {
    setup_gdt();        // Initialize CPU state
    serial_init();      // Enable debugging
    printf("Hello from NullOS!\n");
    // Echo loop
}
```

---

## Recent Reorganization

### Why Reorganize?

**Before:** Flat structure with 11 files in one directory  
**Problem:** Hard to navigate, doesn't scale, unclear dependencies

**After:** Modular structure grouped by function  
**Benefits:**
- ✅ Easy to find code (drivers in `drivers/`, CPU stuff in `cpu/`)
- ✅ Clear separation of concerns
- ✅ Room to grow (add `mm/` for memory, `fs/` for filesystem)
- ✅ Matches industry standards (Linux, FreeBSD structure)

### File Renaming

| Old Name | New Name | Reason |
|----------|----------|--------|
| `kernel.c` | `main.c` | Clarifies it's the entry point |
| `gdt_asm.asm` | `gdt_load.asm` | Describes what it does |
| `io.asm/h` | `port_io.asm/h` | Specifies port I/O operations |

### Include Path Convention

```c
// All includes use module paths from src/
#include "cpu/gdt.h"         // CPU module
#include "drivers/serial.h"  // Driver module
#include "kernel/types.h"    // Shared header
```

**Why:** Makes dependencies explicit and prevents naming collisions.

---

## Build System

### Modern Makefile Features

#### 1. **Auto-Discovery**
```makefile
C_SOURCES   = $(shell find $(SRC) -name '*.c')
ASM_SOURCES = $(shell find $(SRC) -name '*.asm')
```
**Benefit:** Add new `.c` or `.asm` file → automatically compiled. No Makefile edits needed!

#### 2. **Pattern Rules**
```makefile
$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
```
**Benefit:** One rule handles ALL C files. Preserves directory structure in `build/`.

#### 3. **Automatic Dependencies**
```makefile
CFLAGS += -MMD -MP
-include $(DEPS)
```
**Benefit:** Change a `.h` file → all files that include it automatically recompile.

### Common Commands

```bash
make              # Build kernel
make clean        # Remove build artifacts
make run          # Run in QEMU with serial output
make run-debug    # Run with GDB server
make show-files   # List discovered source files
```

### Adding New Files

**No Makefile changes needed!** Just add files:

```bash
# Add new driver
touch src/drivers/keyboard.c
touch src/drivers/keyboard.h
make  # Automatically compiles keyboard.c
```

---

## Development Workflow

### 1. **Adding a Feature**
```bash
# Example: Add keyboard driver
vim src/drivers/keyboard.c
vim src/drivers/keyboard.h
# Update main.c to call keyboard_init()
vim src/kernel/main.c
make && make run
```

### 2. **Debugging**
```bash
# Terminal 1: Start QEMU with GDB server
make run-debug

# Terminal 2: Connect GDB
gdb build/kernel.elf
(gdb) target remote :1234
(gdb) break kmain
(gdb) continue
```

### 3. **Incremental Testing**
```bash
make              # Build
make run          # Test in QEMU
# Change one file
make              # Rebuilds only changed files + dependencies
make run          # Test again
```

---

## Key Concepts Implemented

### Global Descriptor Table (GDT)
- **What:** Defines memory segments (code, data, stack)
- **Why:** Required for protected mode on x86
- **How:** `gdt.c` creates table, `gdt_load.asm` loads it into CPU

### Serial Port Communication
- **What:** COM1 UART driver for text output
- **Why:** Kernel debugging without screen driver
- **How:** Configure baud rate, check transmit status, use `outb` to send data

### Multiboot Compliance
- **What:** Standard bootloader interface
- **Why:** Allows GRUB to load our kernel
- **How:** Magic number (0x1BADB002) in boot.asm header

---

## Design Principles

### 1. **Separation of Concerns**
Each module has one responsibility:
- `boot/` → initialize and jump to kernel
- `cpu/` → manage CPU state
- `drivers/` → talk to hardware
- `kernel/` → coordinate everything

### 2. **Scalability**
Structure supports future growth:
- Memory manager → `src/mm/`
- Filesystem → `src/fs/`
- Networking → `src/net/`
- User programs → `src/user/`

### 3. **Explicit Dependencies**
Include paths show what depends on what:
```c
#include "cpu/gdt.h"        // Uses CPU module
#include "drivers/serial.h" // Uses driver
```

### 4. **Automatic Build**
Makefile handles:
- Finding all source files
- Tracking header changes
- Creating build directories
- Linking in correct order

---

## Technical Details

### Memory Layout
```
0x00100000 (1MB)  ← Kernel loaded here by GRUB
    ↓
[.text section]   ← Code
[.rodata section] ← Constants
[.data section]   ← Initialized data
[.bss section]    ← Uninitialized data
    ↓
[Stack]           ← 16KB stack
```

### Segment Configuration
```
Segment   Selector  Base       Limit      Access
Code      0x08      0x00000000 0xFFFFFFFF Read/Execute, Ring 0
Data      0x10      0x00000000 0xFFFFFFFF Read/Write, Ring 0
```

### Compiler Flags
```makefile
CFLAGS = -m32                   # 32-bit code
         -ffreestanding          # No standard library
         -nostdinc              # No standard headers
         -fno-builtin           # No builtin functions
         -fno-stack-protector   # No stack canaries
         -Isrc -Isrc/include    # Include paths
         -MMD -MP               # Generate dependencies
```

---


## Common Issues & Fixes

### No Serial Output
**Cause:** Triple fault before serial initialization  
**Check:**
- GDT pointer structure (size before base)
- GDT entry flags (shift by 4, not 0x0F)
- Stack alignment in boot.asm

### Build Errors
**Cause:** Missing dependencies  
**Fix:** 
```bash
make clean && make  # Full rebuild
```

### QEMU Won't Start
**Cause:** Kernel not Multiboot-compliant  
**Check:**
```bash
grub-file --is-x86-multiboot build/kernel.elf
echo $?  # Should be 0
```

---

### Tools
- **QEMU** - x86 emulator for testing
- **GDB** - Debugger
- **NASM** - x86 assembler
- **GCC** - C compiler (use cross-compiler for production)

---

## Summary

**Current State:**
- ✅ Boots via GRUB/Multiboot
- ✅ 32-bit protected mode with GDT
- ✅ Serial port debugging
- ✅ Modular, scalable structure
- ✅ Automatic build system

**Next Steps:**
1. Implement IDT and interrupt handlers
2. Add keyboard and timer drivers
3. Set up physical memory management
4. Implement paging for virtual memory

**Build & Run:**
```bash
make && make run
# Should see: "Hello from NullOS!" in terminal
```

---

**Development Philosophy:** Keep it simple, understand every line, build incrementally.
