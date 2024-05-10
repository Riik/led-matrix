.section .text.reset_point
.global __start

__start:
.option push
.option norelax
    # Load stackpointer
    la      sp, _stack_start
    # Load global pointer
    la      gp, _global_pointer
.option pop
    # Clear the bss segment
    la      a0, _bss
    la      a2, _ebss
    sub     a2, a2, a0
    li      a1, 0
    call    memset
    # Copy the data segment
    la      a0, _data
    la      a1, _flash_data
    la      a2, _edata
    sub     a2, a2, a0
    call    memcpy
    
    call    __libc_init_array
    call    main
_l1:
    j _l1
