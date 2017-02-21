#include <stdint.h>
#include <stddef.h>

#include <idt.h>

#include <stdc/string.h>

#define ISR_CALL(N) \
    idt_set_gate( N , (uint32_t) isr_##N , 0x08, 0x8E )

#define IRQ_CALL(N) \
    idt_set_gate( 32 + N , (uint32_t) irq_##N , 0x08, 0x8E )

extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
int_handle  interrupts[256];

void register_int_handler(int index, int_handle hdl) {
    interrupts[index] = hdl;
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags | 0x60;
}

void isr_handler(struct interrupt_frame* esp) {
    if (interrupts[esp->int_no])
        (interrupts[esp->int_no])(esp);
}

void irq_handler(struct interrupt_frame* esp) {
    char c_kb = 0;

    if (esp->int_no >= 7) {
         outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);

    if (interrupts[esp->int_no + 32])
        interrupts[esp->int_no + 32](esp);
}

void init_isr() {
    ISR_CALL(0);
    ISR_CALL(1);
    ISR_CALL(2);
    ISR_CALL(3);
    ISR_CALL(4);
    ISR_CALL(5);
    ISR_CALL(6);
    ISR_CALL(7);
    ISR_CALL(8);
    ISR_CALL(9);
    ISR_CALL(10);
    ISR_CALL(11);
    ISR_CALL(12);
    ISR_CALL(13);
    ISR_CALL(14);
    ISR_CALL(15);
    ISR_CALL(16);
    ISR_CALL(17);
    ISR_CALL(18);
    ISR_CALL(19);
    ISR_CALL(20);
    ISR_CALL(21);
    ISR_CALL(22);
    ISR_CALL(23);
    ISR_CALL(24);
    ISR_CALL(25);
    ISR_CALL(26);
    ISR_CALL(27);
    ISR_CALL(28);
    ISR_CALL(29);
    ISR_CALL(30);
    ISR_CALL(31);
}

void init_irq()
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    IRQ_CALL(0);
    IRQ_CALL(1);
    IRQ_CALL(2);
    IRQ_CALL(3);
    IRQ_CALL(4);
    IRQ_CALL(5);
    IRQ_CALL(6);
    IRQ_CALL(7);
    IRQ_CALL(8);
    IRQ_CALL(9);
    IRQ_CALL(10);
    IRQ_CALL(11);
    IRQ_CALL(12);
    IRQ_CALL(13);
    IRQ_CALL(14);
    IRQ_CALL(15);
}

void init_idt()
{
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);

    init_isr();
    init_irq();

    idt_flush((uint32_t)&idt_ptr);

    asm volatile ("sti");
}
