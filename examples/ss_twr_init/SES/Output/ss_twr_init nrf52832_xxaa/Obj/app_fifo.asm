	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"app_fifo.c"
	.text
.Ltext0:
	.section	.text.fifo_length,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	fifo_length, %function
fifo_length:
.LFB147:
	.file 1 "C:\\Users\\ptath\\Downloads\\dwm1001-examples-master\\dwm1001-examples-master\\nRF5_SDK_14.2.0\\components\\libraries\\fifo\\app_fifo.c"
	.loc 1 45 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI0:
	str	r0, [sp, #4]
	.loc 1 46 14
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	str	r3, [sp, #12]
	.loc 1 47 18
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #12]
	.loc 1 47 30
	ldr	r3, [sp, #12]
	subs	r3, r2, r3
	.loc 1 48 1
	mov	r0, r3
	add	sp, sp, #16
.LCFI1:
	@ sp needed
	bx	lr
.LFE147:
	.size	fifo_length, .-fifo_length
	.section	.text.fifo_put,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	fifo_put, %function
fifo_put:
.LFB148:
	.loc 1 56 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI2:
	str	r0, [sp, #4]
	mov	r3, r1
	strb	r3, [sp, #3]
	.loc 1 57 11
	ldr	r3, [sp, #4]
	ldr	r2, [r3]
	.loc 1 57 25
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #12]
	.loc 1 57 45
	ldr	r1, [sp, #4]
	ldrh	r1, [r1, #4]
	.loc 1 57 37
	ands	r3, r3, r1
	.loc 1 57 18
	add	r3, r3, r2
	.loc 1 57 62
	ldrb	r2, [sp, #3]
	strb	r2, [r3]
	.loc 1 58 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #12]
	.loc 1 58 22
	adds	r2, r3, #1
	ldr	r3, [sp, #4]
	str	r2, [r3, #12]
	.loc 1 59 1
	nop
	add	sp, sp, #8
.LCFI3:
	@ sp needed
	bx	lr
.LFE148:
	.size	fifo_put, .-fifo_put
	.section	.text.fifo_peek,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	fifo_peek, %function
fifo_peek:
.LFB149:
	.loc 1 64 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI4:
	str	r0, [sp, #12]
	mov	r3, r1
	str	r2, [sp, #4]
	strh	r3, [sp, #10]	@ movhi
	.loc 1 65 21
	ldr	r3, [sp, #12]
	ldr	r2, [r3]
	.loc 1 65 36
	ldr	r3, [sp, #12]
	ldr	r1, [r3, #8]
	.loc 1 65 47
	ldrh	r3, [sp, #10]
	add	r3, r3, r1
	.loc 1 65 64
	ldr	r1, [sp, #12]
	ldrh	r1, [r1, #4]
	.loc 1 65 56
	ands	r3, r3, r1
	.loc 1 65 28
	add	r3, r3, r2
	ldrb	r2, [r3]	@ zero_extendqisi2
	.loc 1 65 13
	ldr	r3, [sp, #4]
	strb	r2, [r3]
	.loc 1 66 1
	nop
	add	sp, sp, #16
.LCFI5:
	@ sp needed
	bx	lr
.LFE149:
	.size	fifo_peek, .-fifo_peek
	.section	.text.fifo_get,"ax",%progbits
	.align	1
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	fifo_get, %function
fifo_get:
.LFB150:
	.loc 1 71 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI6:
	sub	sp, sp, #12
.LCFI7:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 72 5
	ldr	r2, [sp]
	movs	r1, #0
	ldr	r0, [sp, #4]
	bl	fifo_peek
	.loc 1 73 11
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #8]
	.loc 1 73 21
	adds	r2, r3, #1
	ldr	r3, [sp, #4]
	str	r2, [r3, #8]
	.loc 1 74 1
	nop
	add	sp, sp, #12
.LCFI8:
	@ sp needed
	ldr	pc, [sp], #4
.LFE150:
	.size	fifo_get, .-fifo_get
	.section	.text.app_fifo_init,"ax",%progbits
	.align	1
	.global	app_fifo_init
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_init, %function
app_fifo_init:
.LFB151:
	.loc 1 78 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #16
.LCFI9:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r3, r2
	strh	r3, [sp, #6]	@ movhi
	.loc 1 80 8
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne	.L7
	.loc 1 82 16
	movs	r3, #14
	b	.L8
.L7:
	.loc 1 86 8
	ldrh	r3, [sp, #6]
	cmp	r3, #0
	beq	.L9
	.loc 1 86 10 discriminator 1
	ldrh	r3, [sp, #6]
	subs	r2, r3, #1
	ldrh	r3, [sp, #6]
	ands	r3, r3, r2
	.loc 1 86 9 discriminator 1
	cmp	r3, #0
	beq	.L10
.L9:
	.loc 1 88 16
	movs	r3, #9
	b	.L8
.L10:
	.loc 1 91 27
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #8]
	str	r2, [r3]
	.loc 1 92 38
	ldrh	r3, [sp, #6]	@ movhi
	subs	r3, r3, #1
	uxth	r2, r3
	.loc 1 92 27
	ldr	r3, [sp, #12]
	strh	r2, [r3, #4]	@ movhi
	.loc 1 93 27
	ldr	r3, [sp, #12]
	movs	r2, #0
	str	r2, [r3, #8]
	.loc 1 94 27
	ldr	r3, [sp, #12]
	movs	r2, #0
	str	r2, [r3, #12]
	.loc 1 96 12
	movs	r3, #0
.L8:
	.loc 1 97 1
	mov	r0, r3
	add	sp, sp, #16
.LCFI10:
	@ sp needed
	bx	lr
.LFE151:
	.size	app_fifo_init, .-app_fifo_init
	.section	.text.app_fifo_put,"ax",%progbits
	.align	1
	.global	app_fifo_put
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_put, %function
app_fifo_put:
.LFB152:
	.loc 1 101 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI11:
	sub	sp, sp, #12
.LCFI12:
	str	r0, [sp, #4]
	mov	r3, r1
	strb	r3, [sp, #3]
	.loc 1 102 9
	ldr	r0, [sp, #4]
	bl	fifo_length
	mov	r2, r0
	.loc 1 102 32
	ldr	r3, [sp, #4]
	ldrh	r3, [r3, #4]
	.loc 1 102 8
	cmp	r2, r3
	bhi	.L12
	.loc 1 104 9
	ldrb	r3, [sp, #3]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #4]
	bl	fifo_put
	.loc 1 105 16
	movs	r3, #0
	b	.L13
.L12:
	.loc 1 108 12
	movs	r3, #4
.L13:
	.loc 1 109 1
	mov	r0, r3
	add	sp, sp, #12
.LCFI13:
	@ sp needed
	ldr	pc, [sp], #4
.LFE152:
	.size	app_fifo_put, .-app_fifo_put
	.section	.text.app_fifo_get,"ax",%progbits
	.align	1
	.global	app_fifo_get
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_get, %function
app_fifo_get:
.LFB153:
	.loc 1 113 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI14:
	sub	sp, sp, #12
.LCFI15:
	str	r0, [sp, #4]
	str	r1, [sp]
	.loc 1 114 9
	ldr	r0, [sp, #4]
	bl	fifo_length
	mov	r3, r0
	.loc 1 114 8
	cmp	r3, #0
	beq	.L15
	.loc 1 116 9
	ldr	r1, [sp]
	ldr	r0, [sp, #4]
	bl	fifo_get
	.loc 1 117 16
	movs	r3, #0
	b	.L16
.L15:
	.loc 1 120 12
	movs	r3, #5
.L16:
	.loc 1 122 1
	mov	r0, r3
	add	sp, sp, #12
.LCFI16:
	@ sp needed
	ldr	pc, [sp], #4
.LFE153:
	.size	app_fifo_get, .-app_fifo_get
	.section	.text.app_fifo_peek,"ax",%progbits
	.align	1
	.global	app_fifo_peek
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_peek, %function
app_fifo_peek:
.LFB154:
	.loc 1 126 1
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI17:
	sub	sp, sp, #20
.LCFI18:
	str	r0, [sp, #12]
	mov	r3, r1
	str	r2, [sp, #4]
	strh	r3, [sp, #10]	@ movhi
	.loc 1 127 9
	ldr	r0, [sp, #12]
	bl	fifo_length
	mov	r2, r0
	.loc 1 127 23
	ldrh	r3, [sp, #10]
	.loc 1 127 8
	cmp	r2, r3
	bls	.L18
	.loc 1 129 9
	ldrh	r3, [sp, #10]
	ldr	r2, [sp, #4]
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	fifo_peek
	.loc 1 130 16
	movs	r3, #0
	b	.L19
.L18:
	.loc 1 133 12
	movs	r3, #5
.L19:
	.loc 1 134 1
	mov	r0, r3
	add	sp, sp, #20
.LCFI19:
	@ sp needed
	ldr	pc, [sp], #4
.LFE154:
	.size	app_fifo_peek, .-app_fifo_peek
	.section	.text.app_fifo_flush,"ax",%progbits
	.align	1
	.global	app_fifo_flush
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_flush, %function
app_fifo_flush:
.LFB155:
	.loc 1 138 1
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	sub	sp, sp, #8
.LCFI20:
	str	r0, [sp, #4]
	.loc 1 139 30
	ldr	r3, [sp, #4]
	ldr	r2, [r3, #12]
	.loc 1 139 22
	ldr	r3, [sp, #4]
	str	r2, [r3, #8]
	.loc 1 140 12
	movs	r3, #0
	.loc 1 141 1
	mov	r0, r3
	add	sp, sp, #8
.LCFI21:
	@ sp needed
	bx	lr
.LFE155:
	.size	app_fifo_flush, .-app_fifo_flush
	.section	.text.app_fifo_read,"ax",%progbits
	.align	1
	.global	app_fifo_read
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_read, %function
app_fifo_read:
.LFB156:
	.loc 1 145 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{lr}
.LCFI22:
	sub	sp, sp, #36
.LCFI23:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 146 5
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L23
	.loc 1 146 5 is_stmt 0 discriminator 1
	movs	r3, #14
	b	.L24
.L23:
	.loc 1 147 5 is_stmt 1
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L25
	.loc 1 147 5 is_stmt 0 discriminator 1
	movs	r3, #14
	b	.L24
.L25:
	.loc 1 149 36 is_stmt 1
	ldr	r0, [sp, #12]
	bl	fifo_length
	str	r0, [sp, #24]
	.loc 1 150 20
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #20]
	.loc 1 151 20
	movs	r3, #0
	str	r3, [sp, #28]
	.loc 1 152 20
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	str	r3, [sp, #16]
	.loc 1 154 15
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #24]
	str	r2, [r3]
	.loc 1 157 8
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L26
	.loc 1 159 16
	movs	r3, #5
	b	.L24
.L26:
	.loc 1 163 8
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne	.L28
	.loc 1 165 16
	movs	r3, #0
	b	.L24
.L29:
	.loc 1 171 45
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	str	r2, [sp, #28]
	.loc 1 171 9
	ldr	r2, [sp, #8]
	add	r3, r3, r2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	fifo_get
.L28:
	.loc 1 169 11
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bcc	.L29
	.loc 1 174 15
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #16]
	str	r2, [r3]
	.loc 1 176 12
	movs	r3, #0
.L24:
	.loc 1 177 1
	mov	r0, r3
	add	sp, sp, #36
.LCFI24:
	@ sp needed
	ldr	pc, [sp], #4
.LFE156:
	.size	app_fifo_read, .-app_fifo_read
	.section	.text.app_fifo_write,"ax",%progbits
	.align	1
	.global	app_fifo_write
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	app_fifo_write, %function
app_fifo_write:
.LFB157:
	.loc 1 181 1
	@ args = 0, pretend = 0, frame = 32
	@ frame_needed = 0, uses_anonymous_args = 0
	push	{r4, lr}
.LCFI25:
	sub	sp, sp, #32
.LCFI26:
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	.loc 1 182 5
	ldr	r3, [sp, #12]
	cmp	r3, #0
	bne	.L31
	.loc 1 182 5 is_stmt 0 discriminator 1
	movs	r3, #14
	b	.L32
.L31:
	.loc 1 183 5 is_stmt 1
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bne	.L33
	.loc 1 183 5 is_stmt 0 discriminator 1
	movs	r3, #14
	b	.L32
.L33:
	.loc 1 185 44 is_stmt 1
	ldr	r3, [sp, #12]
	ldrh	r3, [r3, #4]
	mov	r4, r3
	.loc 1 185 62
	ldr	r0, [sp, #12]
	bl	fifo_length
	mov	r3, r0
	.loc 1 185 60
	subs	r3, r4, r3
	.loc 1 185 20
	adds	r3, r3, #1
	str	r3, [sp, #24]
	.loc 1 186 20
	ldr	r3, [sp, #4]
	ldr	r3, [r3]
	str	r3, [sp, #20]
	.loc 1 187 20
	movs	r3, #0
	str	r3, [sp, #28]
	.loc 1 188 20
	ldr	r2, [sp, #24]
	ldr	r3, [sp, #20]
	cmp	r3, r2
	it	cs
	movcs	r3, r2
	str	r3, [sp, #16]
	.loc 1 190 15
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #24]
	str	r2, [r3]
	.loc 1 193 8
	ldr	r3, [sp, #24]
	cmp	r3, #0
	bne	.L34
	.loc 1 195 16
	movs	r3, #4
	b	.L32
.L34:
	.loc 1 199 8
	ldr	r3, [sp, #8]
	cmp	r3, #0
	bne	.L36
	.loc 1 201 16
	movs	r3, #0
	b	.L32
.L37:
	.loc 1 207 44
	ldr	r3, [sp, #28]
	adds	r2, r3, #1
	str	r2, [sp, #28]
	.loc 1 207 38
	ldr	r2, [sp, #8]
	add	r3, r3, r2
	.loc 1 207 9
	ldrb	r3, [r3]	@ zero_extendqisi2
	mov	r1, r3
	ldr	r0, [sp, #12]
	bl	fifo_put
.L36:
	.loc 1 205 11
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #16]
	cmp	r2, r3
	bcc	.L37
	.loc 1 210 15
	ldr	r3, [sp, #4]
	ldr	r2, [sp, #16]
	str	r2, [r3]
	.loc 1 212 12
	movs	r3, #0
.L32:
	.loc 1 213 1
	mov	r0, r3
	add	sp, sp, #32
.LCFI27:
	@ sp needed
	pop	{r4, pc}
.LFE157:
	.size	app_fifo_write, .-app_fifo_write
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.uleb128 0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.byte	0x4
	.4byte	.LCFI0-.LFB147
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.byte	0x4
	.4byte	.LCFI2-.LFB148
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.byte	0x4
	.4byte	.LCFI4-.LFB149
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE4:
.LSFDE6:
	.4byte	.LEFDE6-.LASFDE6
.LASFDE6:
	.4byte	.Lframe0
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.byte	0x4
	.4byte	.LCFI6-.LFB150
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE6:
.LSFDE8:
	.4byte	.LEFDE8-.LASFDE8
.LASFDE8:
	.4byte	.Lframe0
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.byte	0x4
	.4byte	.LCFI9-.LFB151
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE8:
.LSFDE10:
	.4byte	.LEFDE10-.LASFDE10
.LASFDE10:
	.4byte	.Lframe0
	.4byte	.LFB152
	.4byte	.LFE152-.LFB152
	.byte	0x4
	.4byte	.LCFI11-.LFB152
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI13-.LCFI12
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE10:
.LSFDE12:
	.4byte	.LEFDE12-.LASFDE12
.LASFDE12:
	.4byte	.Lframe0
	.4byte	.LFB153
	.4byte	.LFE153-.LFB153
	.byte	0x4
	.4byte	.LCFI14-.LFB153
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.4byte	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE12:
.LSFDE14:
	.4byte	.LEFDE14-.LASFDE14
.LASFDE14:
	.4byte	.Lframe0
	.4byte	.LFB154
	.4byte	.LFE154-.LFB154
	.byte	0x4
	.4byte	.LCFI17-.LFB154
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.4byte	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE14:
.LSFDE16:
	.4byte	.LEFDE16-.LASFDE16
.LASFDE16:
	.4byte	.Lframe0
	.4byte	.LFB155
	.4byte	.LFE155-.LFB155
	.byte	0x4
	.4byte	.LCFI20-.LFB155
	.byte	0xe
	.uleb128 0x8
	.byte	0x4
	.4byte	.LCFI21-.LCFI20
	.byte	0xe
	.uleb128 0
	.align	2
.LEFDE16:
.LSFDE18:
	.4byte	.LEFDE18-.LASFDE18
.LASFDE18:
	.4byte	.Lframe0
	.4byte	.LFB156
	.4byte	.LFE156-.LFB156
	.byte	0x4
	.4byte	.LCFI22-.LFB156
	.byte	0xe
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x4
	.align	2
.LEFDE18:
.LSFDE20:
	.4byte	.LEFDE20-.LASFDE20
.LASFDE20:
	.4byte	.Lframe0
	.4byte	.LFB157
	.4byte	.LFE157-.LFB157
	.byte	0x4
	.4byte	.LCFI25-.LFB157
	.byte	0xe
	.uleb128 0x8
	.byte	0x84
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.byte	0x4
	.4byte	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	.LCFI27-.LCFI26
	.byte	0xe
	.uleb128 0x8
	.align	2
.LEFDE20:
	.text
.Letext0:
	.file 2 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.16/include/stdint.h"
	.file 3 "C:/Program Files/SEGGER/SEGGER Embedded Studio for ARM 4.16/include/__crossworks.h"
	.file 4 "C:/Users/ptath/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/CMSIS_5/CMSIS/Core/Include/core_cm4.h"
	.file 5 "C:/Users/ptath/AppData/Local/SEGGER/SEGGER Embedded Studio/v3/packages/nRF/CMSIS/Device/Include/system_nrf52.h"
	.file 6 "../../../nRF5_SDK_14.2.0/components/libraries/util/app_util.h"
	.file 7 "C:\\Users\\ptath\\Downloads\\dwm1001-examples-master\\dwm1001-examples-master\\nRF5_SDK_14.2.0\\components\\libraries\\fifo\\app_fifo.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x9b3
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF119
	.byte	0xc
	.4byte	.LASF120
	.4byte	.LASF121
	.4byte	.Ldebug_ranges0+0
	.4byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF3
	.byte	0x2
	.byte	0x30
	.byte	0x1c
	.4byte	0x3d
	.uleb128 0x4
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x4
	.4byte	0x3d
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x36
	.byte	0x1c
	.4byte	0x5c
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x3
	.4byte	.LASF6
	.byte	0x2
	.byte	0x37
	.byte	0x1c
	.4byte	0x74
	.uleb128 0x5
	.4byte	0x63
	.uleb128 0x6
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF7
	.byte	0x2
	.byte	0x38
	.byte	0x1c
	.4byte	0x91
	.uleb128 0x5
	.4byte	0x7b
	.uleb128 0x4
	.4byte	0x7b
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x7
	.4byte	.LASF61
	.byte	0x8
	.byte	0x3
	.byte	0x7e
	.byte	0x8
	.4byte	0xce
	.uleb128 0x8
	.4byte	.LASF11
	.byte	0x3
	.byte	0x7f
	.byte	0x7
	.4byte	0x74
	.byte	0
	.uleb128 0x8
	.4byte	.LASF12
	.byte	0x3
	.byte	0x80
	.byte	0x8
	.4byte	0xce
	.byte	0x4
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF13
	.uleb128 0x9
	.4byte	0x74
	.4byte	0xee
	.uleb128 0xa
	.4byte	0xee
	.uleb128 0xa
	.4byte	0x91
	.uleb128 0xa
	.4byte	0x100
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0xf4
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF14
	.uleb128 0x4
	.4byte	0xf4
	.uleb128 0xb
	.byte	0x4
	.4byte	0xa6
	.uleb128 0x9
	.4byte	0x74
	.4byte	0x124
	.uleb128 0xa
	.4byte	0x124
	.uleb128 0xa
	.4byte	0x12a
	.uleb128 0xa
	.4byte	0x91
	.uleb128 0xa
	.4byte	0x100
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x91
	.uleb128 0xb
	.byte	0x4
	.4byte	0xfb
	.uleb128 0xc
	.byte	0x58
	.byte	0x3
	.byte	0x86
	.byte	0x9
	.4byte	0x2da
	.uleb128 0x8
	.4byte	.LASF15
	.byte	0x3
	.byte	0x88
	.byte	0xf
	.4byte	0x12a
	.byte	0
	.uleb128 0x8
	.4byte	.LASF16
	.byte	0x3
	.byte	0x89
	.byte	0xf
	.4byte	0x12a
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF17
	.byte	0x3
	.byte	0x8a
	.byte	0xf
	.4byte	0x12a
	.byte	0x8
	.uleb128 0x8
	.4byte	.LASF18
	.byte	0x3
	.byte	0x8c
	.byte	0xf
	.4byte	0x12a
	.byte	0xc
	.uleb128 0x8
	.4byte	.LASF19
	.byte	0x3
	.byte	0x8d
	.byte	0xf
	.4byte	0x12a
	.byte	0x10
	.uleb128 0x8
	.4byte	.LASF20
	.byte	0x3
	.byte	0x8e
	.byte	0xf
	.4byte	0x12a
	.byte	0x14
	.uleb128 0x8
	.4byte	.LASF21
	.byte	0x3
	.byte	0x8f
	.byte	0xf
	.4byte	0x12a
	.byte	0x18
	.uleb128 0x8
	.4byte	.LASF22
	.byte	0x3
	.byte	0x90
	.byte	0xf
	.4byte	0x12a
	.byte	0x1c
	.uleb128 0x8
	.4byte	.LASF23
	.byte	0x3
	.byte	0x91
	.byte	0xf
	.4byte	0x12a
	.byte	0x20
	.uleb128 0x8
	.4byte	.LASF24
	.byte	0x3
	.byte	0x92
	.byte	0xf
	.4byte	0x12a
	.byte	0x24
	.uleb128 0x8
	.4byte	.LASF25
	.byte	0x3
	.byte	0x94
	.byte	0x8
	.4byte	0xf4
	.byte	0x28
	.uleb128 0x8
	.4byte	.LASF26
	.byte	0x3
	.byte	0x95
	.byte	0x8
	.4byte	0xf4
	.byte	0x29
	.uleb128 0x8
	.4byte	.LASF27
	.byte	0x3
	.byte	0x96
	.byte	0x8
	.4byte	0xf4
	.byte	0x2a
	.uleb128 0x8
	.4byte	.LASF28
	.byte	0x3
	.byte	0x97
	.byte	0x8
	.4byte	0xf4
	.byte	0x2b
	.uleb128 0x8
	.4byte	.LASF29
	.byte	0x3
	.byte	0x98
	.byte	0x8
	.4byte	0xf4
	.byte	0x2c
	.uleb128 0x8
	.4byte	.LASF30
	.byte	0x3
	.byte	0x99
	.byte	0x8
	.4byte	0xf4
	.byte	0x2d
	.uleb128 0x8
	.4byte	.LASF31
	.byte	0x3
	.byte	0x9a
	.byte	0x8
	.4byte	0xf4
	.byte	0x2e
	.uleb128 0x8
	.4byte	.LASF32
	.byte	0x3
	.byte	0x9b
	.byte	0x8
	.4byte	0xf4
	.byte	0x2f
	.uleb128 0x8
	.4byte	.LASF33
	.byte	0x3
	.byte	0x9c
	.byte	0x8
	.4byte	0xf4
	.byte	0x30
	.uleb128 0x8
	.4byte	.LASF34
	.byte	0x3
	.byte	0x9d
	.byte	0x8
	.4byte	0xf4
	.byte	0x31
	.uleb128 0x8
	.4byte	.LASF35
	.byte	0x3
	.byte	0x9e
	.byte	0x8
	.4byte	0xf4
	.byte	0x32
	.uleb128 0x8
	.4byte	.LASF36
	.byte	0x3
	.byte	0x9f
	.byte	0x8
	.4byte	0xf4
	.byte	0x33
	.uleb128 0x8
	.4byte	.LASF37
	.byte	0x3
	.byte	0xa0
	.byte	0x8
	.4byte	0xf4
	.byte	0x34
	.uleb128 0x8
	.4byte	.LASF38
	.byte	0x3
	.byte	0xa1
	.byte	0x8
	.4byte	0xf4
	.byte	0x35
	.uleb128 0x8
	.4byte	.LASF39
	.byte	0x3
	.byte	0xa6
	.byte	0xf
	.4byte	0x12a
	.byte	0x38
	.uleb128 0x8
	.4byte	.LASF40
	.byte	0x3
	.byte	0xa7
	.byte	0xf
	.4byte	0x12a
	.byte	0x3c
	.uleb128 0x8
	.4byte	.LASF41
	.byte	0x3
	.byte	0xa8
	.byte	0xf
	.4byte	0x12a
	.byte	0x40
	.uleb128 0x8
	.4byte	.LASF42
	.byte	0x3
	.byte	0xa9
	.byte	0xf
	.4byte	0x12a
	.byte	0x44
	.uleb128 0x8
	.4byte	.LASF43
	.byte	0x3
	.byte	0xaa
	.byte	0xf
	.4byte	0x12a
	.byte	0x48
	.uleb128 0x8
	.4byte	.LASF44
	.byte	0x3
	.byte	0xab
	.byte	0xf
	.4byte	0x12a
	.byte	0x4c
	.uleb128 0x8
	.4byte	.LASF45
	.byte	0x3
	.byte	0xac
	.byte	0xf
	.4byte	0x12a
	.byte	0x50
	.uleb128 0x8
	.4byte	.LASF46
	.byte	0x3
	.byte	0xad
	.byte	0xf
	.4byte	0x12a
	.byte	0x54
	.byte	0
	.uleb128 0x3
	.4byte	.LASF47
	.byte	0x3
	.byte	0xae
	.byte	0x3
	.4byte	0x130
	.uleb128 0x4
	.4byte	0x2da
	.uleb128 0xc
	.byte	0x20
	.byte	0x3
	.byte	0xc4
	.byte	0x9
	.4byte	0x35d
	.uleb128 0x8
	.4byte	.LASF48
	.byte	0x3
	.byte	0xc6
	.byte	0x9
	.4byte	0x371
	.byte	0
	.uleb128 0x8
	.4byte	.LASF49
	.byte	0x3
	.byte	0xc7
	.byte	0x9
	.4byte	0x386
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF50
	.byte	0x3
	.byte	0xc8
	.byte	0x9
	.4byte	0x386
	.byte	0x8
	.uleb128 0x8
	.4byte	.LASF51
	.byte	0x3
	.byte	0xcb
	.byte	0x9
	.4byte	0x3a0
	.byte	0xc
	.uleb128 0x8
	.4byte	.LASF52
	.byte	0x3
	.byte	0xcc
	.byte	0xa
	.4byte	0x3b5
	.byte	0x10
	.uleb128 0x8
	.4byte	.LASF53
	.byte	0x3
	.byte	0xcd
	.byte	0xa
	.4byte	0x3b5
	.byte	0x14
	.uleb128 0x8
	.4byte	.LASF54
	.byte	0x3
	.byte	0xd0
	.byte	0x9
	.4byte	0x3bb
	.byte	0x18
	.uleb128 0x8
	.4byte	.LASF55
	.byte	0x3
	.byte	0xd1
	.byte	0x9
	.4byte	0x3c1
	.byte	0x1c
	.byte	0
	.uleb128 0x9
	.4byte	0x74
	.4byte	0x371
	.uleb128 0xa
	.4byte	0x74
	.uleb128 0xa
	.4byte	0x74
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x35d
	.uleb128 0x9
	.4byte	0x74
	.4byte	0x386
	.uleb128 0xa
	.4byte	0x74
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x377
	.uleb128 0x9
	.4byte	0x74
	.4byte	0x3a0
	.uleb128 0xa
	.4byte	0xce
	.uleb128 0xa
	.4byte	0x74
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x38c
	.uleb128 0x9
	.4byte	0xce
	.4byte	0x3b5
	.uleb128 0xa
	.4byte	0xce
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x3a6
	.uleb128 0xb
	.byte	0x4
	.4byte	0xd5
	.uleb128 0xb
	.byte	0x4
	.4byte	0x106
	.uleb128 0x3
	.4byte	.LASF56
	.byte	0x3
	.byte	0xd2
	.byte	0x3
	.4byte	0x2eb
	.uleb128 0x4
	.4byte	0x3c7
	.uleb128 0xc
	.byte	0xc
	.byte	0x3
	.byte	0xd4
	.byte	0x9
	.4byte	0x409
	.uleb128 0x8
	.4byte	.LASF57
	.byte	0x3
	.byte	0xd5
	.byte	0xf
	.4byte	0x12a
	.byte	0
	.uleb128 0x8
	.4byte	.LASF58
	.byte	0x3
	.byte	0xd6
	.byte	0x25
	.4byte	0x409
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF59
	.byte	0x3
	.byte	0xd7
	.byte	0x28
	.4byte	0x40f
	.byte	0x8
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x2e6
	.uleb128 0xb
	.byte	0x4
	.4byte	0x3d3
	.uleb128 0x3
	.4byte	.LASF60
	.byte	0x3
	.byte	0xd8
	.byte	0x3
	.4byte	0x3d8
	.uleb128 0x4
	.4byte	0x415
	.uleb128 0x7
	.4byte	.LASF62
	.byte	0x14
	.byte	0x3
	.byte	0xdc
	.byte	0x10
	.4byte	0x441
	.uleb128 0x8
	.4byte	.LASF63
	.byte	0x3
	.byte	0xdd
	.byte	0x20
	.4byte	0x441
	.byte	0
	.byte	0
	.uleb128 0xd
	.4byte	0x451
	.4byte	0x451
	.uleb128 0xe
	.4byte	0x91
	.byte	0x4
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x421
	.uleb128 0xf
	.4byte	.LASF64
	.byte	0x3
	.2byte	0x106
	.byte	0x1a
	.4byte	0x426
	.uleb128 0xf
	.4byte	.LASF65
	.byte	0x3
	.2byte	0x10d
	.byte	0x24
	.4byte	0x421
	.uleb128 0xf
	.4byte	.LASF66
	.byte	0x3
	.2byte	0x110
	.byte	0x2c
	.4byte	0x3d3
	.uleb128 0xf
	.4byte	.LASF67
	.byte	0x3
	.2byte	0x111
	.byte	0x2c
	.4byte	0x3d3
	.uleb128 0xd
	.4byte	0x44
	.4byte	0x49b
	.uleb128 0xe
	.4byte	0x91
	.byte	0x7f
	.byte	0
	.uleb128 0x4
	.4byte	0x48b
	.uleb128 0xf
	.4byte	.LASF68
	.byte	0x3
	.2byte	0x113
	.byte	0x23
	.4byte	0x49b
	.uleb128 0xd
	.4byte	0xfb
	.4byte	0x4b8
	.uleb128 0x10
	.byte	0
	.uleb128 0x4
	.4byte	0x4ad
	.uleb128 0xf
	.4byte	.LASF69
	.byte	0x3
	.2byte	0x115
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF70
	.byte	0x3
	.2byte	0x116
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF71
	.byte	0x3
	.2byte	0x117
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF72
	.byte	0x3
	.2byte	0x118
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF73
	.byte	0x3
	.2byte	0x11a
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF74
	.byte	0x3
	.2byte	0x11b
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF75
	.byte	0x3
	.2byte	0x11c
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF76
	.byte	0x3
	.2byte	0x11d
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF77
	.byte	0x3
	.2byte	0x11e
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0xf
	.4byte	.LASF78
	.byte	0x3
	.2byte	0x11f
	.byte	0x13
	.4byte	0x4b8
	.uleb128 0x9
	.4byte	0x74
	.4byte	0x54e
	.uleb128 0xa
	.4byte	0x54e
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x559
	.uleb128 0x11
	.4byte	.LASF122
	.uleb128 0x4
	.4byte	0x554
	.uleb128 0xf
	.4byte	.LASF79
	.byte	0x3
	.2byte	0x135
	.byte	0xe
	.4byte	0x56b
	.uleb128 0xb
	.byte	0x4
	.4byte	0x53f
	.uleb128 0x9
	.4byte	0x74
	.4byte	0x580
	.uleb128 0xa
	.4byte	0x580
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x554
	.uleb128 0xf
	.4byte	.LASF80
	.byte	0x3
	.2byte	0x136
	.byte	0xe
	.4byte	0x593
	.uleb128 0xb
	.byte	0x4
	.4byte	0x571
	.uleb128 0x12
	.4byte	.LASF81
	.byte	0x3
	.2byte	0x14d
	.byte	0x18
	.4byte	0x5a6
	.uleb128 0xb
	.byte	0x4
	.4byte	0x5ac
	.uleb128 0x9
	.4byte	0x12a
	.4byte	0x5bb
	.uleb128 0xa
	.4byte	0x74
	.byte	0
	.uleb128 0x13
	.4byte	.LASF82
	.byte	0x8
	.byte	0x3
	.2byte	0x14f
	.byte	0x10
	.4byte	0x5e6
	.uleb128 0x14
	.4byte	.LASF83
	.byte	0x3
	.2byte	0x151
	.byte	0x1c
	.4byte	0x599
	.byte	0
	.uleb128 0x14
	.4byte	.LASF84
	.byte	0x3
	.2byte	0x152
	.byte	0x21
	.4byte	0x5e6
	.byte	0x4
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x5bb
	.uleb128 0x12
	.4byte	.LASF85
	.byte	0x3
	.2byte	0x153
	.byte	0x3
	.4byte	0x5bb
	.uleb128 0xf
	.4byte	.LASF86
	.byte	0x3
	.2byte	0x157
	.byte	0x1f
	.4byte	0x606
	.uleb128 0xb
	.byte	0x4
	.4byte	0x5ec
	.uleb128 0xf
	.4byte	.LASF87
	.byte	0x4
	.2byte	0x804
	.byte	0x19
	.4byte	0x6f
	.uleb128 0x15
	.4byte	.LASF88
	.byte	0x5
	.byte	0x21
	.byte	0x11
	.4byte	0x7b
	.uleb128 0x15
	.4byte	.LASF89
	.byte	0x6
	.byte	0x4b
	.byte	0x11
	.4byte	0x7b
	.uleb128 0x15
	.4byte	.LASF90
	.byte	0x6
	.byte	0x4c
	.byte	0x11
	.4byte	0x7b
	.uleb128 0xb
	.byte	0x4
	.4byte	0x2c
	.uleb128 0xc
	.byte	0x10
	.byte	0x7
	.byte	0x40
	.byte	0x9
	.4byte	0x681
	.uleb128 0x8
	.4byte	.LASF91
	.byte	0x7
	.byte	0x42
	.byte	0x18
	.4byte	0x63d
	.byte	0
	.uleb128 0x8
	.4byte	.LASF92
	.byte	0x7
	.byte	0x43
	.byte	0x18
	.4byte	0x50
	.byte	0x4
	.uleb128 0x8
	.4byte	.LASF93
	.byte	0x7
	.byte	0x44
	.byte	0x18
	.4byte	0x87
	.byte	0x8
	.uleb128 0x8
	.4byte	.LASF94
	.byte	0x7
	.byte	0x45
	.byte	0x18
	.4byte	0x87
	.byte	0xc
	.byte	0
	.uleb128 0x3
	.4byte	.LASF95
	.byte	0x7
	.byte	0x46
	.byte	0x3
	.4byte	0x643
	.uleb128 0x16
	.4byte	.LASF103
	.byte	0x1
	.byte	0xb4
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB157
	.4byte	.LFE157-.LFB157
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x711
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0xb4
	.byte	0x26
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x17
	.4byte	.LASF97
	.byte	0x1
	.byte	0xb4
	.byte	0x3e
	.4byte	0x717
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x17
	.4byte	.LASF98
	.byte	0x1
	.byte	0xb4
	.byte	0x57
	.4byte	0x71d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x18
	.4byte	.LASF99
	.byte	0x1
	.byte	0xb9
	.byte	0x14
	.4byte	0x8c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x18
	.4byte	.LASF100
	.byte	0x1
	.byte	0xba
	.byte	0x14
	.4byte	0x8c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x18
	.4byte	.LASF101
	.byte	0x1
	.byte	0xbb
	.byte	0x14
	.4byte	0x7b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x18
	.4byte	.LASF102
	.byte	0x1
	.byte	0xbc
	.byte	0x14
	.4byte	0x7b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0xb
	.byte	0x4
	.4byte	0x681
	.uleb128 0xb
	.byte	0x4
	.4byte	0x38
	.uleb128 0xb
	.byte	0x4
	.4byte	0x7b
	.uleb128 0x16
	.4byte	.LASF104
	.byte	0x1
	.byte	0x90
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB156
	.4byte	.LFE156-.LFB156
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7a7
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x90
	.byte	0x25
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x17
	.4byte	.LASF97
	.byte	0x1
	.byte	0x90
	.byte	0x37
	.4byte	0x63d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x17
	.4byte	.LASF98
	.byte	0x1
	.byte	0x90
	.byte	0x50
	.4byte	0x71d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x18
	.4byte	.LASF105
	.byte	0x1
	.byte	0x95
	.byte	0x14
	.4byte	0x8c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x18
	.4byte	.LASF100
	.byte	0x1
	.byte	0x96
	.byte	0x14
	.4byte	0x8c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x18
	.4byte	.LASF101
	.byte	0x1
	.byte	0x97
	.byte	0x14
	.4byte	0x7b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x18
	.4byte	.LASF106
	.byte	0x1
	.byte	0x98
	.byte	0x14
	.4byte	0x7b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x19
	.4byte	.LASF107
	.byte	0x1
	.byte	0x89
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB155
	.4byte	.LFE155-.LFB155
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x7d1
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x89
	.byte	0x26
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.uleb128 0x16
	.4byte	.LASF108
	.byte	0x1
	.byte	0x7d
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB154
	.4byte	.LFE154-.LFB154
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x819
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x7d
	.byte	0x25
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x17
	.4byte	.LASF101
	.byte	0x1
	.byte	0x7d
	.byte	0x36
	.4byte	0x50
	.uleb128 0x2
	.byte	0x91
	.sleb128 -14
	.uleb128 0x17
	.4byte	.LASF109
	.byte	0x1
	.byte	0x7d
	.byte	0x47
	.4byte	0x63d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.uleb128 0x16
	.4byte	.LASF110
	.byte	0x1
	.byte	0x70
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB153
	.4byte	.LFE153-.LFB153
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x852
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x70
	.byte	0x24
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x17
	.4byte	.LASF109
	.byte	0x1
	.byte	0x70
	.byte	0x36
	.4byte	0x63d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x16
	.4byte	.LASF111
	.byte	0x1
	.byte	0x64
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB152
	.4byte	.LFE152-.LFB152
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x88b
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x64
	.byte	0x24
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x17
	.4byte	.LASF112
	.byte	0x1
	.byte	0x64
	.byte	0x34
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -13
	.byte	0
	.uleb128 0x19
	.4byte	.LASF113
	.byte	0x1
	.byte	0x4d
	.byte	0xa
	.4byte	0x7b
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x8d3
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x4d
	.byte	0x25
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x17
	.4byte	.LASF91
	.byte	0x1
	.byte	0x4d
	.byte	0x37
	.4byte	0x63d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.uleb128 0x17
	.4byte	.LASF114
	.byte	0x1
	.byte	0x4d
	.byte	0x47
	.4byte	0x50
	.uleb128 0x2
	.byte	0x91
	.sleb128 -10
	.byte	0
	.uleb128 0x1a
	.4byte	.LASF115
	.byte	0x1
	.byte	0x46
	.byte	0x16
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x908
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x46
	.byte	0x2c
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x17
	.4byte	.LASF109
	.byte	0x1
	.byte	0x46
	.byte	0x3e
	.4byte	0x63d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.byte	0
	.uleb128 0x1b
	.4byte	.LASF116
	.byte	0x1
	.byte	0x3f
	.byte	0x16
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x94c
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x3f
	.byte	0x2d
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x17
	.4byte	.LASF101
	.byte	0x1
	.byte	0x3f
	.byte	0x3e
	.4byte	0x50
	.uleb128 0x2
	.byte	0x91
	.sleb128 -6
	.uleb128 0x17
	.4byte	.LASF109
	.byte	0x1
	.byte	0x3f
	.byte	0x4f
	.4byte	0x63d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x1b
	.4byte	.LASF117
	.byte	0x1
	.byte	0x37
	.byte	0x16
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x981
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x37
	.byte	0x2c
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.uleb128 0x17
	.4byte	.LASF112
	.byte	0x1
	.byte	0x37
	.byte	0x3c
	.4byte	0x2c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -5
	.byte	0
	.uleb128 0x1c
	.4byte	.LASF118
	.byte	0x1
	.byte	0x2c
	.byte	0x1a
	.4byte	0x7b
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x17
	.4byte	.LASF96
	.byte	0x1
	.byte	0x2c
	.byte	0x33
	.4byte	0x711
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0x1d
	.ascii	"tmp\000"
	.byte	0x1
	.byte	0x2e
	.byte	0xe
	.4byte	0x7b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -4
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x2134
	.uleb128 0x19
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x21
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0xc4
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x9b7
	.4byte	0x68d
	.ascii	"app_fifo_write\000"
	.4byte	0x723
	.ascii	"app_fifo_read\000"
	.4byte	0x7a7
	.ascii	"app_fifo_flush\000"
	.4byte	0x7d1
	.ascii	"app_fifo_peek\000"
	.4byte	0x819
	.ascii	"app_fifo_get\000"
	.4byte	0x852
	.ascii	"app_fifo_put\000"
	.4byte	0x88b
	.ascii	"app_fifo_init\000"
	.4byte	0x8d3
	.ascii	"fifo_get\000"
	.4byte	0x908
	.ascii	"fifo_peek\000"
	.4byte	0x94c
	.ascii	"fifo_put\000"
	.4byte	0x981
	.ascii	"fifo_length\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0x1a8
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x9b7
	.4byte	0x25
	.ascii	"signed char\000"
	.4byte	0x3d
	.ascii	"unsigned char\000"
	.4byte	0x2c
	.ascii	"uint8_t\000"
	.4byte	0x49
	.ascii	"short int\000"
	.4byte	0x5c
	.ascii	"short unsigned int\000"
	.4byte	0x50
	.ascii	"uint16_t\000"
	.4byte	0x74
	.ascii	"int\000"
	.4byte	0x63
	.ascii	"int32_t\000"
	.4byte	0x91
	.ascii	"unsigned int\000"
	.4byte	0x7b
	.ascii	"uint32_t\000"
	.4byte	0x98
	.ascii	"long long int\000"
	.4byte	0x9f
	.ascii	"long long unsigned int\000"
	.4byte	0xce
	.ascii	"long int\000"
	.4byte	0xa6
	.ascii	"__mbstate_s\000"
	.4byte	0xf4
	.ascii	"char\000"
	.4byte	0x2da
	.ascii	"__RAL_locale_data_t\000"
	.4byte	0x3c7
	.ascii	"__RAL_locale_codeset_t\000"
	.4byte	0x415
	.ascii	"__RAL_locale_t\000"
	.4byte	0x426
	.ascii	"__locale_s\000"
	.4byte	0x599
	.ascii	"__RAL_error_decoder_fn_t\000"
	.4byte	0x5bb
	.ascii	"__RAL_error_decoder_s\000"
	.4byte	0x5ec
	.ascii	"__RAL_error_decoder_t\000"
	.4byte	0x681
	.ascii	"app_fifo_t\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x6c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB147
	.4byte	.LFE147-.LFB147
	.4byte	.LFB148
	.4byte	.LFE148-.LFB148
	.4byte	.LFB149
	.4byte	.LFE149-.LFB149
	.4byte	.LFB150
	.4byte	.LFE150-.LFB150
	.4byte	.LFB151
	.4byte	.LFE151-.LFB151
	.4byte	.LFB152
	.4byte	.LFE152-.LFB152
	.4byte	.LFB153
	.4byte	.LFE153-.LFB153
	.4byte	.LFB154
	.4byte	.LFE154-.LFB154
	.4byte	.LFB155
	.4byte	.LFE155-.LFB155
	.4byte	.LFB156
	.4byte	.LFE156-.LFB156
	.4byte	.LFB157
	.4byte	.LFE157-.LFB157
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB147
	.4byte	.LFE147
	.4byte	.LFB148
	.4byte	.LFE148
	.4byte	.LFB149
	.4byte	.LFE149
	.4byte	.LFB150
	.4byte	.LFE150
	.4byte	.LFB151
	.4byte	.LFE151
	.4byte	.LFB152
	.4byte	.LFE152
	.4byte	.LFB153
	.4byte	.LFE153
	.4byte	.LFB154
	.4byte	.LFE154
	.4byte	.LFB155
	.4byte	.LFE155
	.4byte	.LFB156
	.4byte	.LFE156
	.4byte	.LFB157
	.4byte	.LFE157
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF28:
	.ascii	"p_sep_by_space\000"
.LASF121:
	.ascii	"C:\\Users\\ptath\\Downloads\\dwm1001-examples-maste"
	.ascii	"r\\dwm1001-examples-master\\examples\\ss_twr_init\\"
	.ascii	"SES\000"
.LASF61:
	.ascii	"__mbstate_s\000"
.LASF66:
	.ascii	"__RAL_codeset_ascii\000"
.LASF115:
	.ascii	"fifo_get\000"
.LASF110:
	.ascii	"app_fifo_get\000"
.LASF114:
	.ascii	"buf_size\000"
.LASF23:
	.ascii	"positive_sign\000"
.LASF100:
	.ascii	"requested_len\000"
.LASF36:
	.ascii	"int_n_sep_by_space\000"
.LASF101:
	.ascii	"index\000"
.LASF2:
	.ascii	"short int\000"
.LASF62:
	.ascii	"__locale_s\000"
.LASF12:
	.ascii	"__wchar\000"
.LASF120:
	.ascii	"C:\\Users\\ptath\\Downloads\\dwm1001-examples-maste"
	.ascii	"r\\dwm1001-examples-master\\nRF5_SDK_14.2.0\\compon"
	.ascii	"ents\\libraries\\fifo\\app_fifo.c\000"
.LASF30:
	.ascii	"n_sep_by_space\000"
.LASF68:
	.ascii	"__RAL_ascii_ctype_map\000"
.LASF50:
	.ascii	"__tolower\000"
.LASF32:
	.ascii	"n_sign_posn\000"
.LASF9:
	.ascii	"long long int\000"
.LASF58:
	.ascii	"data\000"
.LASF47:
	.ascii	"__RAL_locale_data_t\000"
.LASF102:
	.ascii	"write_size\000"
.LASF71:
	.ascii	"__RAL_c_locale_month_names\000"
.LASF27:
	.ascii	"p_cs_precedes\000"
.LASF63:
	.ascii	"__category\000"
.LASF3:
	.ascii	"uint8_t\000"
.LASF38:
	.ascii	"int_n_sign_posn\000"
.LASF67:
	.ascii	"__RAL_codeset_utf8\000"
.LASF97:
	.ascii	"p_byte_array\000"
.LASF117:
	.ascii	"fifo_put\000"
.LASF35:
	.ascii	"int_p_sep_by_space\000"
.LASF82:
	.ascii	"__RAL_error_decoder_s\000"
.LASF85:
	.ascii	"__RAL_error_decoder_t\000"
.LASF45:
	.ascii	"time_format\000"
.LASF15:
	.ascii	"decimal_point\000"
.LASF112:
	.ascii	"byte\000"
.LASF93:
	.ascii	"read_pos\000"
.LASF98:
	.ascii	"p_size\000"
.LASF41:
	.ascii	"month_names\000"
.LASF59:
	.ascii	"codeset\000"
.LASF42:
	.ascii	"abbrev_month_names\000"
.LASF103:
	.ascii	"app_fifo_write\000"
.LASF17:
	.ascii	"grouping\000"
.LASF53:
	.ascii	"__towlower\000"
.LASF13:
	.ascii	"long int\000"
.LASF88:
	.ascii	"SystemCoreClock\000"
.LASF70:
	.ascii	"__RAL_c_locale_abbrev_day_names\000"
.LASF89:
	.ascii	"__StackTop\000"
.LASF94:
	.ascii	"write_pos\000"
.LASF65:
	.ascii	"__RAL_c_locale\000"
.LASF39:
	.ascii	"day_names\000"
.LASF24:
	.ascii	"negative_sign\000"
.LASF83:
	.ascii	"decode\000"
.LASF57:
	.ascii	"name\000"
.LASF106:
	.ascii	"read_size\000"
.LASF31:
	.ascii	"p_sign_posn\000"
.LASF81:
	.ascii	"__RAL_error_decoder_fn_t\000"
.LASF118:
	.ascii	"fifo_length\000"
.LASF37:
	.ascii	"int_p_sign_posn\000"
.LASF79:
	.ascii	"__user_set_time_of_day\000"
.LASF1:
	.ascii	"unsigned char\000"
.LASF86:
	.ascii	"__RAL_error_decoder_head\000"
.LASF75:
	.ascii	"__RAL_data_utf8_space\000"
.LASF0:
	.ascii	"signed char\000"
.LASF119:
	.ascii	"GNU C99 8.2.1 20181213 (release) [gcc-8-branch revi"
	.ascii	"sion 267074] -fmessage-length=0 -mcpu=cortex-m4 -ml"
	.ascii	"ittle-endian -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mt"
	.ascii	"humb -mtp=soft -munaligned-access -std=gnu99 -g2 -g"
	.ascii	"pubnames -fomit-frame-pointer -fno-dwarf2-cfi-asm -"
	.ascii	"fno-builtin -ffunction-sections -fdata-sections -fs"
	.ascii	"hort-enums -fno-common\000"
.LASF10:
	.ascii	"long long unsigned int\000"
.LASF7:
	.ascii	"uint32_t\000"
.LASF22:
	.ascii	"mon_grouping\000"
.LASF8:
	.ascii	"unsigned int\000"
.LASF108:
	.ascii	"app_fifo_peek\000"
.LASF43:
	.ascii	"am_pm_indicator\000"
.LASF52:
	.ascii	"__towupper\000"
.LASF18:
	.ascii	"int_curr_symbol\000"
.LASF76:
	.ascii	"__RAL_data_utf8_plus\000"
.LASF25:
	.ascii	"int_frac_digits\000"
.LASF20:
	.ascii	"mon_decimal_point\000"
.LASF16:
	.ascii	"thousands_sep\000"
.LASF40:
	.ascii	"abbrev_day_names\000"
.LASF84:
	.ascii	"next\000"
.LASF109:
	.ascii	"p_byte\000"
.LASF14:
	.ascii	"char\000"
.LASF4:
	.ascii	"uint16_t\000"
.LASF80:
	.ascii	"__user_get_time_of_day\000"
.LASF6:
	.ascii	"int32_t\000"
.LASF29:
	.ascii	"n_cs_precedes\000"
.LASF46:
	.ascii	"date_time_format\000"
.LASF107:
	.ascii	"app_fifo_flush\000"
.LASF91:
	.ascii	"p_buf\000"
.LASF78:
	.ascii	"__RAL_data_empty_string\000"
.LASF19:
	.ascii	"currency_symbol\000"
.LASF5:
	.ascii	"short unsigned int\000"
.LASF26:
	.ascii	"frac_digits\000"
.LASF72:
	.ascii	"__RAL_c_locale_abbrev_month_names\000"
.LASF64:
	.ascii	"__RAL_global_locale\000"
.LASF92:
	.ascii	"buf_size_mask\000"
.LASF111:
	.ascii	"app_fifo_put\000"
.LASF95:
	.ascii	"app_fifo_t\000"
.LASF122:
	.ascii	"timeval\000"
.LASF69:
	.ascii	"__RAL_c_locale_day_names\000"
.LASF113:
	.ascii	"app_fifo_init\000"
.LASF96:
	.ascii	"p_fifo\000"
.LASF56:
	.ascii	"__RAL_locale_codeset_t\000"
.LASF73:
	.ascii	"__RAL_data_utf8_period\000"
.LASF105:
	.ascii	"byte_count\000"
.LASF55:
	.ascii	"__mbtowc\000"
.LASF60:
	.ascii	"__RAL_locale_t\000"
.LASF44:
	.ascii	"date_format\000"
.LASF99:
	.ascii	"available_count\000"
.LASF49:
	.ascii	"__toupper\000"
.LASF74:
	.ascii	"__RAL_data_utf8_comma\000"
.LASF21:
	.ascii	"mon_thousands_sep\000"
.LASF90:
	.ascii	"__StackLimit\000"
.LASF34:
	.ascii	"int_n_cs_precedes\000"
.LASF87:
	.ascii	"ITM_RxBuffer\000"
.LASF48:
	.ascii	"__isctype\000"
.LASF11:
	.ascii	"__state\000"
.LASF77:
	.ascii	"__RAL_data_utf8_minus\000"
.LASF33:
	.ascii	"int_p_cs_precedes\000"
.LASF51:
	.ascii	"__iswctype\000"
.LASF54:
	.ascii	"__wctomb\000"
.LASF104:
	.ascii	"app_fifo_read\000"
.LASF116:
	.ascii	"fifo_peek\000"
	.ident	"GCC: (GNU) 8.2.1 20181213 (release) [gcc-8-branch revision 267074]"
