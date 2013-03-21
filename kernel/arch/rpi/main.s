.section .init
.globl _start
_start:
	b _rpi_main

.section .text

_rpi_main:
	mov sp,#0x8000

	mov r0,#1024
	mov r1,#768
	mov r2,#16
	bl InitialiseFrameBuffer
	fbInfoAddr .req r4
	mov fbInfoAddr,r0
	bl SetGraphicsAddress

	/* Jump to C main */
	b main

