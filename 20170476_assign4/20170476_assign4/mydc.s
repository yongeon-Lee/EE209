### --------------------------------------------------------------------
### mydc.s
###
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	
.section ".rodata"
scanfFormat:
	.asciz "%s"
printfFormat:
  .asciz "%d\n"
print_empty:
  .asciz "dc: stack empty\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE

### --------------------------------------------------------------------

	.section ".text"

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------


	.globl  main
	.type   main,@function

main:

	pushl   %ebp
	movl    %esp, %ebp

input:

	## dc number stack initialized. %esp = %ebp
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call  scanf
	addl  $8, %esp
	## check if user input EOF
	cmp	$EOF, %eax
	je	quit
  ## if(!isdigit(buffer[0]))
  movl $buffer, %edx
  movl (%edx), %ebx
  andl $0xFF, %ebx
  pushl %ebx
  call isdigit
  addl $4 , %esp
  cmpl $0 , %eax
  je letter
  ## atoi(buffer)
  movl $buffer, %edx
  pushl %edx
  call atoi
  addl $4, %esp
  pushl %eax
  jmp input

letter:

  ## if(buffer[0] == 'c')
  cmpl $0x63, %ebx
  je clear
  ## if(buffer[0] == 'd')
  cmpl $0x64, %ebx
  je duplicate
  ## if(buffer[0] == 'f')
  cmpl $0x66, %ebx
  je stack
  ## if(buffer[0] == 'p')
  cmpl $0x70, %ebx
  je peek
  ## if(buffer[0] == 'q')
  cmpl $0x71, %ebx
  je quit
  ## if(buffer[0] == 'r')
  cmpl $0x72, %ebx
  je replace
  ## if(buffer[0] == '+')
  cmpl $0x2B, %ebx
  je plus
  ## if(buffer[0] == '-')
  cmpl $0x2D, %ebx
  je deduct
  ## if(buffer[0] == '*')
  cmpl $0x2A, %ebx
  je multi
  ## if(buffer[0] == '/')
  cmpl $0x2F, %ebx
  je divide
  ## if(buffer[0] == '^')
  cmpl $0x5E, %ebx
  je pwr
  ## if(buffer[0] == '%')
  cmpl $0x25, %ebx
  je rmd
  ## if(buffer[0] == '_')
  cmpl $0x5F, %ebx
  je underbar
  jmp input

## --------------------------------
## if(buffer[0] == '_'){
##   int a = atoi(buffer+1);
##   a = !a + 1;
##   stack.push(a);
## }
## --------------------------------
underbar:

  movl $buffer, %edx
  addl $1, %edx
  pushl %edx
  call atoi
  addl $4, %esp
  notl %eax
  addl $1, %eax
  pushl %eax
  jmp input

## --------------------------------
## print the most top of
## the stack
## put the top most value
## at the esi register
## --------------------------------
peek:

  cmpl %esp, %ebp
  je empty
  movl (%esp), %esi
  pushl %esi
  pushl $printfFormat
  call printf
  addl $8, %esp
  jmp input

## --------------------------------
## pop two top most value,
## add it, and push at the
## top of the stack
## --------------------------------
plus:

  cmpl %esp, %ebp
  je empty
  popl %ecx
  cmpl %esp, %ebp
  je empty1
  popl %edx
  addl %ecx, %edx
  pushl %edx
  jmp input

## --------------------------------
## pop two top most value,
## substract two, and push
## at the top of the stack
## --------------------------------
deduct:

  cmpl %esp, %ebp
  je empty
  popl %ecx
  cmpl %esp, %ebp
  je empty1
  popl %edx
  subl %ecx, %edx
  pushl %edx
  jmp input

## --------------------------------
## pop two top most value,
## multiply, and push at
## the top of the stack
## --------------------------------
multi:

  cmpl %esp, %ebp
  je empty
  popl %ecx
  cmpl %esp, %ebp
  je empty1
  popl %edx
  imull %ecx, %edx
  pushl %edx
  jmp input

## --------------------------------
## pop two top most value,
## divide, and push the
## quotient at the
## top of the stack
## --------------------------------
divide:

  cmpl %esp, %ebp
  je empty
  movl $0, %edx
  popl %ecx
  cmpl %esp, %ebp
  je empty1
  popl %eax
  idivl %ecx 
  pushl %eax
  jmp input

## --------------------------------
## pop two top most value,
## divide, and push the
## remainder at the
## top of the stack
## --------------------------------
rmd:

  cmpl %esp, %ebp
  je empty
  movl $0, %edx
  popl %ecx
  cmpl %esp, %ebp
  je empty1
  popl %eax
  idivl %ecx
  pushl %edx
  jmp input

## --------------------------------
## int tmp = 1;
## int index = stack.pop();
## int base = stack.pop();
## for(int i = 1; i <= index; i++){
##   tmp *= base;
## }
## stack.push(tmp);
## --------------------------------
pwr:

  cmpl %esp, %ebp
  je empty
  movl $1, %ecx 
  movl $1, %edx
  popl %edi
  cmpl %esp, %ebp
  je empty2
  popl %ebx 
loop:
  cmpl %ecx, %edi
  jl loopend
  imul %ebx, %edx
  incl %ecx
  jmp loop
loopend:
  pushl %edx
  jmp input

## --------------------------------
## while(ebp != esi){
## stack.pop();
## }
## --------------------------------
clear:

  movl %esp, %esi
loop2:
  cmpl %esp, %ebp
  je loopend2
  popl %esi
  jmp loop2
loopend2:
  jmp input


## --------------------------------
## int a = stack.pop();
## stack.push(a);
## stack.psuh(a); 
## --------------------------------

duplicate:

  cmpl %esp, %ebp
  je empty
  popl %esi
  pushl %esi
  pushl %esi 
  jmp input

## --------------------------------
## edi register takes the esp address
## esi register takes the value
## at the esi address
## keep add 4bytes to edi register
## until edi equals to ebp address 
## --------------------------------
stack:

  movl %esp, %edi ## set initial to esp value
loop1:
  ##if %edi = %ebp
  cmpl %edi, %ebp
  je loopend1
  movl (%edi), %esi
  pushl %esi
  pushl $printfFormat
  call printf
  addl $8, %esp
  addl $4, %edi
  jmp loop1
loopend1:
  jmp input

## --------------------------------
## int a = stack.pop();
## int b = stack.pop();
## stack.push(a);
## stack.push(b); 
## --------------------------------
replace:

  cmpl %esp, %ebp
  je empty
  popl %edi
  cmpl %esp, %ebp
  je empty2
  popl %esi
  pushl %edi
  pushl %esi
  jmp input

## --------------------------------
## case 0 : empty check
## when no pop value
## --------------------------------
empty:

  pushl $print_empty
  call printf
  addl $4, %esp
  jmp input

## --------------------------------
## case 1 : empty check
## when ecx register was used
## --------------------------------
empty1:

  pushl %ecx
  pushl $print_empty
  call printf
  add $4, %esp
  jmp input

## --------------------------------
## case 2 : empty check
## when edi register was used
## --------------------------------
empty2:

  pushl %edi
  pushl $print_empty
  call printf
  addl $4, %esp
  jmp input

	## PSEUDO-CODE
	## /*
	##  * In this pseudo-code we are assuming that no local variables are created
	##  * in the _main_ process stack. In case you want to allocate space for local
	##  * variables, please remember to update logic for 'empty dc stack' condition
	##  * (lines 6, 15 and 20) accordingly.
	##  */
	##
	##1 while (1) {
	##2	if (scanf("%s", buffer) == EOF)
	##3		return 0;
	##4 	if (!isdigit(buffer[0])) {
	##5		if (buffer[0] == 'p') {
	##6			if (stack.peek() == NULL) { /* is %esp == %ebp? */
	##7				printf("dc: stack empty\n");
	##8			} else {
	##9				printf("%d\n", (int)stack.top()); /* value is already pushed in the stack */
	##10			}
	##11		} else if (buffer[0] == 'q') {
	##12			goto quit;
	##13		} else if (buffer[0] == '+') {
	##14			int a, b;
	##15			if (stack.peek() == NULL) {
	##16				printf("dc: stack empty\n");
	##17				continue;
	##18			}
	##19			a = (int)stack.pop();
	##20			if (stack.peek() == NULL) {
	##21				printf("dc: stack empty\n");
	##22				stack.push(a); /* pushl some register value */
	##23				continue;
	##24			}
	##25			b = (int)stack.pop(); /* popl to some register */
	##26			res = a + b;
	##27 			stack.push(res);
	##28		} else if (buffer[0] == '-') {
	##29			/* ... */
	##30		} else if (buffer[0] == '^') {
	##31			/* ... powerfunc() ... */
	##32		} else if { /* ... and so on ... */ }
	##33 	} else { /* the first no. is a digit */
	##34		int no = atoi(buffer);
	##35		stack.push(no);	/* pushl some register value */
	##36	}
	##37 }

quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret
