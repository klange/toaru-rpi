#include <system.h>
#include <version.h>
#include <list.h>
#include <rpi.h>

void print(char * str) {
	Print(str, strlen(str));
}

uint8_t volatile my_lock = 0;

uint8_t arm__sync_lock_test_and_set(volatile uint8_t * ptr, uint8_t value) {
	register uint8_t result;
	asm(
		"swp %0, %2, [%1]"
		: "=&r" (result)
		: "r"(ptr), "r"(value)
	   );
	return result;

}

int main() {
	UsbInitialise();
	heap_install();

#if 0
	unsigned int ra;

	PUT32(IRQ_DISABLE_BASIC,1);

	ra=GET32(GPFSEL1);
	ra&=~(7<<18);
	ra|=1<<18;
	PUT32(GPFSEL1,ra);
	PUT32(GPSET0,1<<16);

	PUT32(ARM_TIMER_CTL,0x003E0000);
	PUT32(ARM_TIMER_LOD,1000000-1);
	PUT32(ARM_TIMER_RLD,1000000-1);
	PUT32(ARM_TIMER_DIV,0x000000F9);
	PUT32(ARM_TIMER_CLI,0);
	PUT32(ARM_TIMER_CTL,0x003E00A2);

	for(ra=0;ra<5;ra++)
	{
		PUT32(GPCLR0,1<<16);
		while(1) if(GET32(ARM_TIMER_MIS)) break;
		PUT32(ARM_TIMER_CLI,0);
		PUT32(GPSET0,1<<16);
		while(1) if(GET32(ARM_TIMER_MIS)) break;
		PUT32(ARM_TIMER_CLI,0);
	}

	PUT32(ARM_TIMER_LOD,2000000-1);
	PUT32(ARM_TIMER_RLD,2000000-1);
	PUT32(ARM_TIMER_CLI,0);
	PUT32(IRQ_ENABLE_BASIC,1);
	for(ra=0;ra<5;ra++)
	{
		PUT32(GPCLR0,1<<16);
		while(1) if(GET32(IRQ_BASIC)&1) break;
		PUT32(ARM_TIMER_CLI,0);
		PUT32(GPSET0,1<<16);
		while(1) if(GET32(IRQ_BASIC)&1) break;
		PUT32(ARM_TIMER_CLI,0);
	}

	PUT32(ARM_TIMER_LOD,4000000-1);
	PUT32(ARM_TIMER_RLD,4000000-1);
	enable_irq();
#endif

	char version_number[1024];
	sprintf(version_number, __kernel_version_format,
			__kernel_version_major,
			__kernel_version_minor,
			__kernel_version_lower,
			__kernel_version_suffix);
	kprintf("%s %s %s %s %s %s\n",
			__kernel_name,
			version_number,
			__kernel_version_codename,
			__kernel_build_date,
			__kernel_build_time,
			__kernel_arch);

	list_t * l = list_create();

	list_insert(l, "Hello");
	list_insert(l, "world!");
	list_insert(l, "Cake");

	foreach(node, l) {
		char * c = node->value;
		kprintf("node = %s\n", c);
	}

#if 0
	for (int j = 0; j < 10; ++j) {
		for (int i = 0; i < 1000; ++i) {
			SetForeColour(1000 * j + i);
			DrawPixel(j+100,i);
		}
	}
#endif

	kprintf("> ");
	while (1) {
		char buf[256];
		ReadLine(buf, 256);
		kprintf("You typed: %s\n> ", buf);
	}
}

void c_irq_handler() {
	kprintf("Ping\n");
	PUT32(ARM_TIMER_CLI, 0);
}
