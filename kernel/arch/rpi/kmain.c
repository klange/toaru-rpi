#include <system.h>
#include <list.h>

extern void Print(char *, int len);
extern void UsbInitialise();
extern int  ReadLine(char * buf, int len);
extern uint8_t arm_get_lock(uint8_t volatile * lock);

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

	kprintf("Hello world! %d\n", 42);

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
		kprintf("You typed: %s\n> ", buf);
	}
}

