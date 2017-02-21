/* Multiboot garbage. */

.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Stack. */
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 // 16 KiB
stack_top:

.set KERNEL_VBASE, 0xC0000000
.set KERNEL_PNUM,  (KERNEL_VBASE >> 22)

.section .data
.align 0x1000
boot_pgdir:
    .int 0x83
    .fill (KERNEL_PNUM - 1), 4, 0
    .int 0x83
    .fill (1024 - KERNEL_PNUM - 1), 4, 0

/* Our actual entrypoint. */
.section .text
.global _start
.type _start, @function
_start:
    // Map our kernel at KERNEL_VBASE and enable paging
    mov    $(boot_pgdir - KERNEL_VBASE), %ecx
    mov    %ecx,%cr3

    mov    %cr4,%ecx
    or     $0x10,%ecx
    mov    %ecx,%cr4

    mov    %cr0,%ecx
    or     $0x80000000,%ecx
    mov    %ecx,%cr0

    lea    .init, %ecx
    jmp    *%ecx

.init:
    // Unmap page 0 (it's not needed now)
    movl   $0, boot_pgdir
    invlpg 0

    mov $stack_top, %esp

    call i686_init

    call kernel_main

    cli
.halt:
    hlt
    jmp .halt

.size _start, . - _start

.global gdt_flush
.type gdt_flush, @function
gdt_flush:
    mov    0x4(%esp),%eax
    lgdt  (%eax)
    mov    $0x10,%ax
    mov    %eax,%ds
    mov    %eax,%es
    mov    %eax,%fs
    mov    %eax,%gs
    mov    %eax,%ss
    lea    .flush, %eax
    jmp *%eax
    .flush:
        ret

.size gdt_flush, . - gdt_flush

.macro ISR index
    .global isr_\index
    .type isr_\index , @function
    isr_\index :
        push $0
        push $\index
        jmp isr_stub
.endm

.macro ISR_E index
    .global isr_\index
    .type isr_\index , @function
     isr_\index :
         push $\index
         jmp isr_stub
.endm

.macro IRQ index
    .global irq_\index
    .type irq_\index , @function
     irq_\index :
         push $0
         push $\index
         jmp irq_stub
.endm

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR_E 8
ISR 9
ISR_E 10
ISR_E 11
ISR_E 12
ISR_E 13
ISR_E 14
ISR 15
ISR 16
ISR_E 17
ISR 18
ISR 19
ISR 20
ISR 21
ISR 22
ISR 23
ISR 24
ISR 25
ISR 26
ISR 27
ISR 28
ISR 29
ISR_E 30
ISR 31

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15

isr_stub:
        pusha

        mov %ds, %ax
        push %eax

        mov    $0x10,%ax
        mov    %ax,%ds
        mov    %ax,%es
        mov    %ax,%fs
        mov    %ax,%gs

        push %esp

        cld
        call isr_handler

        pop    %eax

        pop    %eax
        mov    %ax,%ds
        mov    %ax,%es
        mov    %ax,%fs
        mov    %ax,%gs

        popa
        add $8, %esp
        iret


irq_stub:
        pusha

        mov %ds, %ax
        push %eax

        mov    $0x10,%ax
        mov    %ax,%ds
        mov    %ax,%es
        mov    %ax,%fs
        mov    %ax,%gs

        push %esp

        cld
        call irq_handler

        pop    %ebx

        pop    %ebx
        mov    %bx,%ds
        mov    %bx,%es
        mov    %bx,%fs
        mov    %bx,%gs

        popa
        add $8, %esp
	sti
        iret

.global idt_flush
.type idt_flush, @function
idt_flush:
    mov    0x4(%esp),%eax
    lidt  (%eax)
    ret
