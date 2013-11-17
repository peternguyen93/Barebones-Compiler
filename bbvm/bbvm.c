/*  Bare Bones Virtual Machine
	File : bbvm.c
	Author : Peternguyen
	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License version 3 as
	published by the Free Software Foundation.  Note that I am not
	granting permission to redistribute or modify this program under the
	terms of any other version of the General Public License.

	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program (in the file "COPYING"); if not, see
	<http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if _WIN32 || __MINGW32__ || WIN32
	#include <Windows.h>
#endif
#include "bbvm.h"

const int instr_list_size = 100;//default number of instruction for program
int *reg_list; 
int max_reg;//save number of register
int lengh_program;
int debug = 0;//enable or disable debug-mode
instruct_t **instruct_list;//save list of instruction

unsigned long getFileSize(char *filename){
	//get size of file
	FILE *file;
	unsigned long size;
	file = fopen(filename,"r");
	if(file){
		fseek(file,0,SEEK_END);
		size = ftell(file);
	}
	fclose(file);
	return size;
}

instruct_t *decode_ins(char instr,char *code,int size){
	/*
	- decode each instruction
	- including 1 byte opcode 4 or 8 bytes for param
	- <opcode 1 byte><param1 4bytes><param2 4bytes> -> mov,load
	- <opcode 1 byte><param1 4bytes> -> clear,cmp,je,jmp,inc,dec
	*/
	union Convert data1,data2;
	instruct_t *instruct;

	instruct = (instruct_t *)malloc(sizeof(instruct_t));
	instruct->opcode = instr;
	if(size == 8){//if opcode is mov or load
		strncpy(data1.bytes,code,4);
		strncpy(data2.bytes,code+4,4);
		if(MOV_INS == instr){//mov instruction
			instruct->param1 = data2.integer;//load register value 1
			instruct->param2 = data1.integer;//load register value 2
		}else{//load instruction	
			instruct->param1 = data1.integer;//load register value 1
			instruct->param2 = data2.integer;//load register value 2
		}
	}
	if(size == 4){//use for clear,cmp,je,jmp,inc,dec
		strncpy(data1.bytes,code,4);
		instruct->param1 = data1.integer;
	}
	return instruct;
}

int decode_program(char *fn){
	//decode a program from byte code to instruction
	FILE *input;
	char *opcode_list;
	unsigned long size = getFileSize(fn); //get file size
	int i = 0,j = 0;

	opcode_list = (char *)malloc(size*sizeof(char)+2);
	instruct_list = (instruct_t **)malloc(sizeof(instruct_t *)*instr_list_size);

	input = fopen(fn,"rb");
	fread(opcode_list,1,size,input);
	fclose(input);

	while(i < size){
		if(j+1 >= instr_list_size)
			instruct_list = (instruct_t **)realloc(instruct_list,(j+10)*sizeof(instruct_t *));

		switch(opcode_list[i]){
			case LOAD_INS:
			case MOV_INS://read 8 byte after byte opcode
				instruct_list[j++] = (instruct_t *)decode_ins(opcode_list[i],opcode_list+i+1,8);
				i+=9;
				break;
			case JE_INS:
			case CLEAR_INS:
			case INC_INS:
			case DEC_INS:
			case CMP_INS:
			case JMP_INS://read 4 byte after byte opcode
				instruct_list[j++] = (instruct_t *)decode_ins(opcode_list[i],opcode_list+i+1,4);
				i+=5;
				break;
		}
	}
	free(opcode_list);
	//resize instruct_list fit with size j
	instruct_list = (instruct_t **)realloc(instruct_list,j*sizeof(instruct_t *));
	return j;
}

int calculate_reg(int lengh_program){
	//find number of register , use to malloc memmory fit with program
	int i;
	int max;

	max = instruct_list[0]->param1;
	for(i = 1; i < lengh_program; i++)
		if(instruct_list[i]->opcode > 1 && instruct_list[i]->opcode < 7)
			if(max < instruct_list[i]->param1)
				max = instruct_list[i]->param1;

	return max + 1;
}

void print_register(int length){
	int i;
	for(i = 0; i < length; i++)
		printf("%d ", reg_list[i]);
	printf("\n");
}

void decode_mode(char opcode,int step,int v1,int v2,int flag){
	print_register(max_reg);
	switch(opcode){
		case LOAD_INS:
			printf("%d load r%d,#%d\n", step,v1,v2);
			break;
		case MOV_INS:
			printf("%d mov r%d,r%d\n", step,v2,v1);
			break;
		case JE_INS:
			printf("Flag: %d\n", flag);
			printf("%d je %d\n", step,v1);
			break;
		case JMP_INS:
			printf("%d jmp %d\n", step,v1);
			break;
		case CLEAR_INS:
			printf("%d clear r%d\n", step,v1);
			break;
		case INC_INS:
			printf("%d inc r%d\n", step,v1);
			break;
		case DEC_INS:
			printf("%d dec r%d\n", step,v1);
			break;
		case CMP_INS:
			printf("%d cmp r%d,0\n", step,v1);
			break;
	}
	puts("---------------------");
#if __linux__
	sleep(1);
#elif _WIN32 || __MINGW32__ || WIN32
	Sleep(1000);
#endif
}

void implement_prorgram(){
	/* implement virtual instruction */
	int step = 0,last_step;
	int value1,value2;
	char flag = 0,check = 0;
	char opcode;

	while(step < lengh_program){
		//extract value
		opcode = instruct_list[step]->opcode;
		value1 = instruct_list[step]->param1;
		value2 = instruct_list[step]->param2;
		if(debug)
			last_step = step;
		switch(opcode){
			case LOAD_INS://load value to register
				reg_list[value1] = value2;
				break;
			case MOV_INS://mov value from register 1 to register 2
				reg_list[value2] = reg_list[value1];
				break;
			case CLEAR_INS://load 0 to register
				reg_list[value1] = 0;
				break;
			case INC_INS://increase value in register 
				reg_list[value1]++;
				break;
			case DEC_INS://decrease value in register 
				reg_list[value1]--;
				break;
			case CMP_INS://compare value of register with 0
				if(!reg_list[value1])
					flag = 1;
				else
					flag = 0;
				break;
			case JE_INS://jump if flag was set
				if(flag){
					step = value1;
					check = 1;
				}
				break;
			case JMP_INS://jump to addr
				check = 1;
				step = value1;
				break;
		}
		if(!check)
			step++;
		else
			check = 0;
		if(debug)
			decode_mode(opcode,last_step,value1,value2,flag);
	}
}

void usage (char *prog_name){
	fprintf (stderr, "%s src-file [options]\n", prog_name);
	fprintf (stderr, "options:\n");
	fprintf (stderr, "  -d            enable debug mode\n");
	fprintf (stderr, "  -h            Help\n");
	exit(0);
}
 
int main(int argc,char **argv){
	if(argc < 2)
		usage(argv[0]);
	if(argc > 2){
		if(strcmp(argv[2],"-d") == 0)
			debug = 1;
		if(strcmp(argv[2],"-h") == 0)
			usage(argv[0]);
	}

	lengh_program = decode_program(argv[1]);
	max_reg = calculate_reg(lengh_program);

	reg_list = (int *)malloc(max_reg*sizeof(int));
	memset(reg_list,0,max_reg*sizeof(int));

	implement_prorgram();

	puts("---Result---");
	print_register(max_reg);
	return 0;
}