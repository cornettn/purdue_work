/* clkdisp.s - clkdisp (x86) */

/*------------------------------------------------------------------------
 * clkdisp  -  Interrupt dispatcher for clock interrupts (x86 version)
 *------------------------------------------------------------------------
 */
#include <icu.s>
		.text
		.globl	clkdisp		# Clock interrupt dispatcher
clkdisp:
		pushal			# Save registers
		cli			# Disable further interrupts
		movb	$EOI,%al	# Reset interrupt
		outb	%al,$OCW1_2

		call	clkhandler	# Call high level handler


    /* Obtain the address of the current procs entry */

    movl currpid, %eax      // Get the currpid value
    movl $proctab, %ebx     // Get the address of the proctab
    movl $80, %edx          // Move the sizeof(struct procent) into edx
    mul  %edx               // eax = currpid * 76
    addl %eax, %ebx         // ebx = eax + ebx  -- get the address of the currpid procent

    /* Ensure that stack manipulation only happens for case (ii) */

    movl (%ebx), %eax       // Get the value of prstate into eax
    cmp $4, %eax            // Check to see if prstate is sleeping
    je _endclkdisp

    /* Ensure that the process has a cb function */

    movl %ebx, %edx         // edx = &proctab[currpid]
    addl $56, %ebx          // ebx = ebx + 56  -- (the offset of prcbvalid)
    movl (%ebx), %eax      // eax should now contain the value of prcbvalid
    cmp  $1, %eax           // check to see if eax is 1
    jne _endclkdisp         // Jump to the end of clkdisp if there
                            // is not a registered callback function

    /* Ensure that the process has a message waiting for it */

    addl $16, %ebx          // ebx = 16 + ebx (offset for prtmpvalid)
                            // ebx = address of prtmpvalid
    movl (%ebx), %eax       // eax = *ebx
    cmp $1, %eax
    jne _endclkdisp         // If prtmpvalid is 0, jump to the end of the function
                            // This means that message has not been received


   /* Reset prtmpvalid to 0, move prtmpbuf to prmbufptr */

    movl $0, (%ebx)         // Change prtmpvalid to be 0
    subl $4, %ebx           // ebx = &proctab[currpid].prtmpbuf
    movl (%ebx), %eax       // eax = prtmpbuf
    subl $4, %ebx           // ebx = &proctab[currpid].prmbufptr
    movl (%ebx), %ecx       // ecx = &prmbufptr
    movl %eax, (%ecx)       // *ebx = eax

  /* Manipulate the stack to arrange for the correct return addresses */
    /* Move the 8 general registers down one */

    movl (%esp), %eax
    movl %eax, -4(%esp)

    movl 4(%esp), %eax
    movl %eax, (%esp)

    movl 8(%esp), %eax
    movl %eax, 4(%esp)

    movl 12(%esp), %eax
    movl %eax, 8(%esp)

    movl 16(%esp), %eax
    movl %eax, 12(%esp)

    movl 20(%esp), %eax
    movl %eax, 16(%esp)

    movl 24(%esp), %eax
    movl %eax, 20(%esp)

    movl 28(%esp), %eax
    movl %eax, 24(%esp)

    /* Move EFLAGS, CS, and EIP down one */

    movl 32(%esp), %eax
    movl %eax, 28(%esp)

    movl 36(%esp), %eax
    movl %eax, 32(%esp)

    movl 40(%esp), %eax
    movl %eax, 36(%esp)

    /* Move the original EIP into the newly allocated spot */

    movl 28(%esp), %eax
    movl %eax, 40(%esp)

    /* Obtain the address for the cb function */
    /* At this point %edx = address of curr process entry */

    movl %edx, %ebx       // ebx = &proctab[currpid]
    addl $60, %ebx        // ebx = 60 + ebx (offset for prcbptr)
                          // ebx should now be the address of the function pointer

    /* Overwrite the current value of EIP to be ebx */

    movl (%ebx), %eax
    movl %eax, 28(%esp)

    /* Update the stack pointer to accomodate for shifting */

    subl $4, %esp


_endclkdisp:
		sti			# Restore interrupt status
		popal			# Restore registers
		iret			# Return from interrupt


