/* vim: tabstop=4 shiftwidth=4 noexpandtab
 */
#ifndef __SYSTEM_H
#define __SYSTEM_H
#define _KERNEL_

#include <types.h>
#include <va_list.h>

#define STR(x) #x
#define STRSTR(x) STR(x)

/* Binary Literals */
#define b(x) ((uint8_t)b_(0 ## x ## uL))
#define b_(x) ((x & 1) | (x >> 2 & 2) | (x >> 4 & 4) | (x >> 6 & 8) | (x >> 8 & 16) | (x >> 10 & 32) | (x >> 12 & 64) | (x >> 14 & 128))

#define asm __asm__
#define volatile __volatile__

extern unsigned int __irq_sem;

#define IRQ_OFF { asm volatile ("mrs r0, cpsr\nbic r0, r0, #(1<<7)\nmsr cpsr_c, r0\nmov pc,lr\n"); }
#define IRQ_RES { asm volatile ("mrs r0, cpsr\norr r0, r0, #(1<<7)\nmsr cpsr_c, r0\nmov pc,lr\n"); }
#define PAUSE   { asm volatile ("wfi"); }
#define IRQS_ON_AND_PAUSE { IRQ_RES; PAUSE; IRQ_OFF; }


#define STOP while (1) { PAUSE; }

#define SYSCALL_VECTOR 0x7F
#define SIGNAL_RETURN 0xFFFFDEAF
#define THREAD_RETURN 0xFFFFB00F

extern void * code;
extern void * end;

extern char * boot_arg; /* Argument to pass to init */
extern char * boot_arg_extra; /* Extra data to pass to init */

extern void *sbrk(uintptr_t increment);

extern void spin_lock(uint8_t volatile * lock);
extern void spin_unlock(uint8_t volatile * lock);

extern int max(int,int);
extern int min(int,int);
extern int abs(int);
extern void swap(int *, int *);
extern void *memcpy(void *restrict dest, const void *restrict src, size_t count);
extern void *memmove(void *restrict dest, const void *restrict src, size_t count);
extern void *memset(void *dest, int val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern uint32_t strlen(const char *str);

/* Memory Management */
extern uintptr_t placement_pointer;
void * sbrk(uintptr_t increment);
void heap_install();


/* klmalloc */
void * __attribute__ ((malloc)) malloc(size_t size);
void * __attribute__ ((malloc)) realloc(void *ptr, size_t size);
void * __attribute__ ((malloc)) calloc(size_t nmemb, size_t size);
void * __attribute__ ((malloc)) valloc(size_t size);
void free(void *ptr);

/* kprintf */
extern size_t vasprintf(char * buf, const char *fmt, va_list args);
extern int    kprintf(const char *fmt, ...);
extern int    sprintf(char *buf, const char *fmt, ...);

struct regs {
    unsigned int _regs[18];
};

#define HALT_AND_CATCH_FIRE(mesg, regs) halt_and_catch_fire(mesg, __FILE__, __LINE__, regs)
#define ASSERT(statement) ((statement) ? (void)0 : assert_failed(__FILE__, __LINE__, #statement))
#define assert(statement) ((statement) ? (void)0 : assert_failed(__FILE__, __LINE__, #statement))
void halt_and_catch_fire(char *error_message, const char *file, int line, struct regs * regs);
void assert_failed(const char *file, uint32_t line, const char *desc);

#endif
