#include <system.h>
#include <list.h>

extern void Print(char *, int len);
extern void UsbInitialise();
extern int  ReadLine(char * buf, int len);
extern uint8_t arm_get_lock(uint8_t volatile * lock);

void print(char * str) {
	Print(str, strlen(str));
}

static uint8_t volatile lock __attribute__((aligned (4))) = 0;

uint8_t arm__sync_lock_test_and_set(volatile uint8_t * ptr, uint8_t value) {
	register char result;
	asm (
		"1: ldrexb   %0,  [%1]\n\t"
			"strexb   r4,   %2, [%1]\n\t"
			"cmp     r4,   #0\n\t"
			"bne     1b	\n\t "
			: "=&r" (result)
	: "r"(ptr), "r"(value)
	: "r4"
	);
	return result;

}

int main() {
	UsbInitialise();

	kprintf("Hello world! %d\n", 42);

	{
		char buf[256];

		kprintf("I am going to execute the arm_get_lock function and tell you the result. Press enter to continue.\n");
		ReadLine(buf, 256);
		uint8_t result = arm__sync_lock_test_and_set(&lock, 1);

		kprintf("The result was 0x%x\n", (uint32_t)result);

		kprintf("I am going to obtain and release a lock. Press enter to continue.\n");
		ReadLine(buf, 256);

		spin_lock(&lock);

		kprintf("I haz lock.\n");
		ReadLine(buf, 256);

		spin_unlock(&lock);

		kprintf("I no haz lock.\n");
		ReadLine(buf, 256);
	}

	{
		char buf[256];
		kprintf("I'm going to try to allocate some space. Press enter to continue.\n");
		kprintf("(End of kernel = 0x%x)\n", &end);
		ReadLine(buf, 256);

		char * x = malloc(630);

		kprintf("I'm still here.");
		ReadLine(buf, 256);

		x[0] = 'H';
		x[1] = 'e';
		x[2] = 'l';
		x[3] = 'l';
		x[4] = 'o';
		x[5] = '\0';

		kprintf("x = %s\n", x);
		ReadLine(buf, 256);
	}



	list_t * l = list_create();

	list_insert(l, "Hello");
	list_insert(l, "world!");
	list_insert(l, "Cake");

	foreach(node, l) {
		char * c = node->value;
		kprintf("node = %s\n", c);
	}

	while (1) {
		char buf[256];
		ReadLine(buf, 256);
		kprintf("You typed: %s\n", buf);
	}
}

