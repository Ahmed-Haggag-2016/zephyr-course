set(SUPPORTED_EMU_PLATFORMS qemu)

set(QEMU_CPU_TYPE_${ARCH} cortex-m3)
set(QEMU_FLAGS_${ARCH}
  -cpu ${QEMU_CPU_TYPE_${ARCH}}
  -machine lm3s6965evb
  -nographic
  -vga none
  -no-reboot 
  )
board_set_debugger_ifnset(qemu)

# board_set_flasher_ifnmatch(qemu)
# board_set_debugger_ifnmatch(qemu)

# Instruct QEMU to execute using the lm3s6965evb core model matrix
list(APPEND QEMU_EXTRA_FLAGS "-machine" "lm3s6965evb" "-no-reboot")
