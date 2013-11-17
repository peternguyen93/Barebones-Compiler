/*  Bare Bones Compiler
    File : barebones_utils.c
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

//loggin error when compiler program
void logging(int ret, char *format, ...){
	va_list list_param;
	if(format){
		fprintf(stderr, "logging: ");
		va_start(list_param, format);
		vfprintf(stderr, format, list_param);
		va_end(list_param);
		fprintf(stderr, "\n");
	}
	if(ret == 1){
		fprintf(stderr, "usage:\n");
		//usage(stderr);
		exit(1);
	}
}

//malloc a object
void *alloc(size_t size){
	void *p = NULL;
	if(!(p = (void *)malloc(size)))
		logging(2, "Memory allocation failed\n");
	return p;
}

//lookup a IDENT (string) is in table
int keyword(char *string, keyword_t *table){
	while(table->name){
		if(strcasecmp(string,table->name) == 0)
			return table->value;
		table++;
	}
	return 0;
}

//create line is matched with syntax
char *newstr(char *orig){
	int l = strlen(orig);
	char *new_str;

	new_str = (char *)alloc(l+1);
	memcpy(new_str,orig,l);
	return new_str;
}

//create line is matched with syntax
char *newstrn(char *orig, int max_len){
	int l = strlen(orig);
	char *new_str;

	if(l > max_len)
		l = max_len;
	new_str = (char *)alloc(l+1);
	memcpy(new_str,orig,l);
	return new_str;
}

//make sure value of var is not overflow or not is integer
// intmax_t a_to_intmax (char *s){
// 	intmax_t val;
// 	int cc;

// 	int i = sscanf (s, "%" SCNiMAX "%n", & val, & cc);
// 	if ((i != 1) || (cc != strlen(s)))
// 		logging(2, "can't interpret '%s' as an integer", s);
// 	return val;
// }