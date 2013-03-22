#include <system.h>

extern void *end;
uintptr_t placement_pointer = (uintptr_t)&end;
uintptr_t heap_end = (uintptr_t)NULL;

void
heap_install() {
	heap_end = (placement_pointer + 0x1000) & ~0xFFF;
}

void *
sbrk(
	uintptr_t increment
    ) {
	ASSERT((increment % 0x1000 == 0) && "Kernel requested to expand heap by a non-page-multiple value");
	ASSERT((heap_end % 0x1000 == 0)  && "Kernel heap is not page-aligned!");
	//ASSERT(heap_end + increment <= KERNEL_HEAP_END && "The kernel has attempted to allocate beyond the end of its heap.");
	uintptr_t address = heap_end;
	heap_end += increment;
	memset((void *)address, 0x0, increment);
	return (void *)address;
}

