// http://comsci.liu.edu/~jrodriguez/testandset2.html
	
// This is an implentation of TestAndSet for gcc/linux/amd64.
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
	// This code is gcc/linux/intel amd64 specific.

	// Preserve rbx, which is about to be modified.
	pushq    %rbx

	// https://en.wikipedia.org/wiki/X86_calling_conventions
	movq    %rdi, %rbx          # &lock to rbx
	movq    $1, %rax            # 1 (true) to rax
	
	// Swap rax and lock. Value 1 (true) is copied to lock, rax receives
	// old lock value
	xchgq    %rax, (%rbx)    # Atomically exchange rax with lock.
	
	// Restore rbx
	popq    %rbx
	
	// Return value (old value of lock) is already in register rax
	
	ret
	
