#include <system.h>

extern void *end;
uintptr_t placement_pointer = (uintptr_t)&end;

#define heap_end placement_pointer

void * sbrk(uintptr_t increment) {
	uintptr_t address = heap_end;
	heap_end += increment;
	memset((void *)address, 0x0, increment);
	return (void *)address;
}
