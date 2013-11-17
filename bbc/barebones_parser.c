/*  Bare Bones Compiler
    File : barebones_parser.c
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

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "util.h"
#include "barebones.h"
#include "parser.tab.h"

int stmt_line = 0;//save stmt_line
int num_reg = 0;
var_t *var_head = NULL;
stmt_t *main_prog = NULL;
FILE *outfile;
bool debug = false;

// if false, reference to uninitialized variables
// (other than in a clear statement) will result
// in a run time error
bool init_to_zero = true;  

extern FILE *yyin;

var_t *find_var(char *name){
	var_t *var;

	//find if var is exist on program
	for(var = var_head; var; var = var->next)
		if(strncmp(name,var->name,strlen(name)) == 0){ //allow use lower case variable and upper case variable
			return var;
		}
	
	//create new var
	var = (var_t *)alloc(sizeof(var_t));
	var->name = strdup(name);

	if(init_to_zero){
		var->val = 0;
		var->init = true;
	}else{
		var->init = false;
	}
	var->num = num_reg++;
	var->next = var_head;
	var_head = var;
	return var;
}

//set value like init x = 123; in bb
void set_var(var_t *var, uintmax_t value){
	var->val = value;
	var->init = true;
}

// add stmt struct to stmt list truck 
void add_stmt_to_list(stmt_t *head, stmt_t *stmt){
	if(head->tail)
		head->tail->next = stmt;
	else
		head->next = stmt;
	head->tail = stmt;
}

//parse line string to stmt_t struct
stmt_t *new_stmt(stmt_type_t type, var_t *var){
	stmt_t *stmt = (stmt_t *)alloc(sizeof(stmt_t));
	stmt->type = type;//save type of stmt
	stmt->var = var;
	return stmt;
}

void error_report(const char *fmt, ...){
	va_list list;
	fprintf(stderr, "error on line %d: ", stmt_line);
	va_start(list, fmt);
	vfprintf(stderr, fmt, list);
	va_end(list);
    fprintf(stderr, "\n");
    exit(2);
}

void check_var_init(var_t *var){
	if(!var->init)
		error_report("unitialized variable %s", var->name);
}

bool parse_program(char *fn){
	int status;
	yyin = fopen(fn, "r");
	if (!yyin)
		logging(1, "can't read program\n");
	status = yyparse();
	fclose(yyin);
	return status;
}

//use for instruction inc,dec,clear,cmp
void instr_encode_group_1(char instr,char *instr_name,int reg){
	fwrite(&instr,1,1,outfile);
	fwrite((char *)&reg,1,4,outfile);
	if(debug)
		if(instr == 7)
			printf("%d instr: %s r%d,0\n",stmt_line,instr_name,reg);
		else if(instr == 1 || instr == 8)
			printf("%d instr: %s %d\n",stmt_line,instr_name,reg);
		else
			printf("%d instr: %s r%d\n",stmt_line,instr_name,reg);
}

//use for instruction load,mov
void instr_encode_group_2(char instr,int reg1,int reg2){
	fwrite(&instr,1,1,outfile);
	fwrite((char *)&reg1,1,4,outfile);
	fwrite((char *)&reg2,1,4,outfile);
	if(debug)
		if(instr == 3)
			printf("%d instr: mov r%d,r%d\n",stmt_line,reg1,reg2);
		else
			printf("%d instr: load r%d,#%d\n",stmt_line,reg1,reg2);
}

int count_sub_stmt(stmt_t *list){
	//count sub_stmt for each while stmt
	int count = 0;
	stmt_t *loop = list;
	while(loop){
		if(loop->type == WHILE_STMT){
			count += count_sub_stmt(loop->stmt_list);
			count += 3;//add 3 stmt, cmp,je,jmp
		}else
			count++;
		loop = loop->next;
	}
	return count;
}

void encode_stmt(stmt_t *stmt){
	//encode for each stmt
	int first_stmt_while,count;
	switch(stmt->type){
		case DECR_STMT:
			check_var_init(stmt->var);
			stmt_line++;
			instr_encode_group_1(6,"dec",stmt->var->num);
			break;
		case INCR_STMT:
			check_var_init(stmt->var);
			stmt_line++;
			instr_encode_group_1(5,"inc",stmt->var->num);
			break;
		case WHILE_STMT:
			check_var_init(stmt->var);

			stmt_line++;
			instr_encode_group_1(7,"cmp",stmt->var->num);

			stmt_line++;
			count = count_sub_stmt(stmt->stmt_list);
			instr_encode_group_1(1,"je",stmt_line+count+2);

			first_stmt_while = encode_stmt_list(stmt->stmt_list);
			stmt_line++;
			instr_encode_group_1(8,"jmp",first_stmt_while-2);
			break;
		case CLEAR_STMT:
			check_var_init(stmt->var);
			stmt_line++;
			instr_encode_group_1(4,"clear",stmt->var->num);
			break;
		case COPY_STMT:
			check_var_init(stmt->var);
			stmt_line++;
			instr_encode_group_2(3,stmt->dest->num,stmt->var->num);
			break;
	}
}

int encode_stmt_list(stmt_t *list){
	int first_stmt_line, i = 0;
	while(list){
		encode_stmt(list);
		if(!i)//get first stmt in loop
			first_stmt_line = stmt_line;
		list = list->next;
		i++;
	}
	return first_stmt_line;
}

void compile_program(char *output){
	unsigned long size;
	var_t *loop = var_head;

	if(remove(output))
		logging(2,"File %s isn't exist",output);
	
	outfile = fopen(output,"a");
	if(!outfile)
		error_report("Can't open file",output);

	while(loop){
		if(loop->val)//find init stmt 
			instr_encode_group_2(2,loop->num,loop->val);
		loop = loop->next;
	}

	encode_stmt_list(main_prog);
}

void usage (char *arg){
	fprintf (stderr, "%s src-file: [options] des-file: [options]\n", arg);
	fprintf (stderr, "options:\n");
	fprintf (stderr, "  -o            set outputfile\n");
	fprintf (stderr, "  -d            enable debug mode\n");
	fprintf (stderr, "  -h            Help\n");
}

int main(int argc,char **argv){
	char *input;
	char *output;
	int c;

	while (1){
		static struct option long_options[] ={
			/* These options set a flag. */
			{"output",required_argument,0,'o'},
			{"help",no_argument,0,'h'},
			{"debug",no_argument,0,'d'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;
	    
		c = getopt_long (argc, argv, "o:dh",long_options, &option_index);
		if (c == -1)
			break;

		switch (c){
			case 0:
				fprintf(stderr,"Usage %s -h to help\n",argv[0]);
				return 0;
			case 'o':
				input = argv[1];
				output = optarg;
				break;
			case 'd':
				debug = true;
				break;
			case 'h':
				usage(argv[1]);
				break;
		}
	}
	parse_program(input);
	if(!output)
		output = strcat(input,".bbo");
	compile_program(output);
	return 0;
}