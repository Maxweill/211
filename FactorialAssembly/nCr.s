.globl nCr
        .type   nCr, @function
nCr:
  # Your code for nCr should go here
	pushq %rdi
	pushq %rsi
	
	call Factorial
	
	popq %rsi
	popq %rdi
	movq %rax,%rbx
	subq %rsi,%rdi
	
	pushq %rdi
	pushq %rsi
	call Factorial
	popq %rsi
	popq %rdi
	
	movq %rax,%rdx
	movq %rsi,%rdi
	
	pushq %rdi
	pushq %rsi
	pushq %rdx
	call  Factorial
	popq  %rdx
	popq  %rsi
	popq  %rdi
	
	mul   %rdx
	
	movq  %rbx,%rcx
	movq  %rax,%rbx
	movq  %rcx,%rax
	
	movq  $0,%rdx
	div   %rbx
	
	ret
  
.globl Factorial
    .type   Factorial, @function
	
Factorial:
	# your code for Factorial should go here


	movq $1,%rax
	cmpq $0,%rdi
	je return
	jne loop
	
return:
	ret

loop:
	mul %rdi
	dec %rdi
	cmpq $0,%rdi
	je return
	jne loop
