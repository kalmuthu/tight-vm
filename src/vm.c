#include "vproc.h"
#include "stdio.h"
#include "vm-error.h"
#include "opcodes.h"
#include "stdlib.h"

int tick(vproc_t* current){
	var op = code[pc];
	pc++;

	var a, b;
	unsigned int data_ptr = (unsigned int) data;
	switch(op){
		//Skip
		case NOP:
			break;
		
		//Arithmetic
		case ADDI:
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = a + b;
			break;
		case SUBI:
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b - a;
			break;
		case MULI:
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = a * b;
			break;
		case DIVI:
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b / a;
			break;
		
		//Binary math
		case ANDI:
			break;
		case ORI:
			break;
		case XORI:
			break;
		
		//Constants
		case CONSTI:
			stack[sp++] = code[pc];
			pc++;
			break;
		
		//Keyboard I/O
		case PRINTI:
			printf("%u", stack[--sp]);
			break;
		case EMITB:
			printf("%c", (char) stack[--sp]);
			break;

		//Data movement instructions
		case GLOADI:
			stack[sp++] = *((var*) (data_ptr + code[pc]));
			pc++;
			break;
		case GLOADB:
			stack[sp++] = (var)  *((char*) (data_ptr + code[pc]));
			pc++;
			break;
		case GSTOREB:
			*((char*) (data_ptr + code[pc])) = (char) stack[--sp];
			break;
		case GSTOREI:
			*((var*) (data_ptr + code[pc])) = stack[--sp];
			break;
		case LOAD:
			break;
		case STORE:
			break;
		//System instructions
		case HALT:
			return VM_OK;
		case DBG:
			error("Debug instruction", current, 0);
			break;
		
		//Invalid instruction
		default:
			return error("Unrecognized opcode", current, 1);
			break;
	}
	
	if((sp < 0) || (sp >= current->stack_size))
		return error("Stack underflow", current, 0);

	if(pc > current->code_size)
		return error("Execution passed code boundaries", current, 0);

	return 0;
}

vproc_t* new_proc(int* new_code, int code_size, int data_size, int stack_size, int entry_point, int uid){
	vproc_t* current = (vproc_t*) malloc(sizeof(vproc_t));
	current->code_size = code_size;

	pc = entry_point;
	sp = 0;
	code = new_code;

	stack = (var*) malloc(sizeof(var) * stack_size);
	current->stack_size = stack_size;
	data = (void*) malloc(data_size);
	current->data_size = data_size;
	
	fp = 0;
	current->uid = uid;

	return current;
}

void cleanup_proc(vproc_t* current){
	free(stack);
	free(data);
	free(current);
}
