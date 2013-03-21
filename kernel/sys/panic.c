#include <system.h>

void kernel_halt() {
	while(1) {
		/* DO NOTHING */
	}
}

void halt_and_catch_fire(char * error_message, const char * file, int line, struct regs * regs) {
	kprintf("HACF: %s\n", error_message);
#if 0
	kprintf("Proc: %d\n", getpid());
#endif
	kprintf("File: %s\n", file);
	kprintf("Line: %d\n", line);
}

void assert_failed(const char *file, uint32_t line, const char *desc) {
	IRQ_OFF;
	kprintf("Kernel Assertion Failed: %s\n", desc);
	kprintf("File: %s\n", file);
	kprintf("Line: %d\n", line);
	kernel_halt();
}
