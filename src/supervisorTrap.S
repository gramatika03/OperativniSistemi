//.extern _ZN5Riscv20handleSupervisorTrapEv
//.extern _ZN5Riscv24handleSoftwareInterrruptEv
//.extern _ZN5Riscv20handleTimerInterruptEv
//.extern _ZN5Riscv22handleConsoleInterruptEv

.global _ZN5Riscv14supervisorTrapEv
.align 4
.type _ZN5Riscv14supervisorTrapEv, @function

_ZN5Riscv14supervisorTrapEv:
    j handleSoftwareInterrruptT
    j handleTimerInterruptT
    addi x0, x0, 0
    addi x0, x0, 0
    addi x0, x0, 0
    addi x0, x0, 0
    addi x0, x0, 0
    addi x0, x0, 0
    addi x0, x0, 0
    j handleConsoleInterruptT


handleSoftwareInterrruptT:
    #push all registers to the stack
    addi sp, sp, -256
    .irp index , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv24handleSoftwareInterrruptEv

    .irp index , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret

handleTimerInterruptT:
    #push all registers to the stack
    addi sp, sp, -256
    .irp index , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv20handleTimerInterruptEv

    .irp index , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret


handleConsoleInterruptT:
    #push all registers to the stack
    addi sp, sp, -256
    .irp index , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv22handleConsoleInterruptEv

    .irp index , 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret
