// http://comsci.liu.edu/~jrodriguez/testandset2.html
	
// This is an implentation of TestAndSet for gcc/linux/x86.
// The caller defines int lock. Calling TestAndSet(&lock) sets lock
// to 1 and returns the old value of lock. So, if lock is zero, then
// TestAndSet(&lock) returns zero and sets lock to one. This means
// the lock has been succesfully acquired.  On the other hand, if the lock
// had already been set to one by another process or thread,  then 1
// would be returned. This would indicate to the caller that the lock is
// already being held by another process or thread. The caller keeps retrying
// TestAndSet(&lock) until it returns 0.

	.text
	.globl _lw6sys_test_and_set_asm
_lw6sys_test_and_set_asm:
	// Assume it is called as lw6sys_test_and_set(&lock).
	// This code is gcc/linux/intel x86 specific.

	// Preserve ebx, which is about to be modified.
	pushl    %ebx
	
	// https://en.wikipedia.org/wiki/X86_calling_conventions
	movl    8(%esp), %ebx       # &lock to ebx
	movl    $1, %eax            # 1 (true) to eax
	
	// Swap eax and lock. Value 1 (true) is copied to lock, eax receives
	// old lock value
	xchgl   %eax, (%ebx)    # Atomically exchange eax with lock.
	
	// Restore ebx
	popl    %ebx
	
	// Return value (old value of lock) is already in register eax
	
	ret
	
