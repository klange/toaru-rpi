#include <system.h>

char arm__sync_lock_test_and_set(volatile uint8_t * ptr, char value);

void spin_lock(uint8_t volatile * lock) {
	while(arm__sync_lock_test_and_set(lock, 1)) {
#if 0
		switch_task(1);
#endif
	}
}

void spin_unlock(uint8_t volatile * lock) {
	__sync_lock_release(lock);
}

void *memset(void *dest, int val, size_t count) {
	char * d = (char *)dest;
	unsigned int i = 0;
	for ( ; i < count; ++i ) {
		d[i] = val;
	}
	return dest;
}

void * memcpy(void * restrict dest, const void * restrict src, size_t count) {
	char * d = dest;
	char * s = (char *)src;
	unsigned int i = 0;
	for ( ; i < count; ++i) {
		d[i] = s[i];
	}
	return dest;
}

/*
 * memsetw
 * Set `count` shorts to `val`.
 */
unsigned short * memsetw(unsigned short * dest, unsigned short val, int count) {
	int i = 0;
	for ( ; i < count; ++i ) {
		dest[i] = val;
	}
	return dest;
}



int max(int a, int b) {
	return (a > b) ? a : b;
}

int min(int a, int b) {
	return (a > b) ? b : a;
}

int abs(int a) {
	return (a >= 0) ? a : -a;
}

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void * memmove(void * restrict dest, const void * restrict src, size_t count) {
	size_t i;
	unsigned char *a = dest;
	const unsigned char *b = src;
	if (src < dest) {
		for ( i = count; i > 0; --i) {
			a[i-1] = b[i-1];
		}
	} else {
		for ( i = 0; i < count; ++i) {
			a[i] = b[i];
		}
	}
	return dest;
}

int strcmp(const char * a, const char * b) {
	uint32_t i = 0;
	while (1) {
		if (a[i] < b[i]) {
			return -1;
		} else if (a[i] > b[i]) {
			return 1;
		} else {
			if (a[i] == '\0') {
				return 0;
			}
			++i;
		}
	}
}


unsigned int strlen(const char *str) {
	int i = 0;
	while (str[i] != (char)0) {
		++i;
	}
	return i;
}

