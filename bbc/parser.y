/*  Bare Bones Parser
    File : parser.y
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


%{
#include <stdbool.h>
#include <stdint.h>

#include "barebones.h"

%}

%union {
  int integer;
  char *string;
  var_t *var;
  stmt_t *stmt;
}

%token CLEAR COPY DECR DO END INCR INIT NOT TO WHILE

%token <string> IDENT
%token <integer> INTEGER

%type <var> var
%type <stmt> stmt clear_stmt incr_stmt decr_stmt while_stmt copy_stmt
%type <stmt> stmt_list
%%

program:	stmt_list { main_prog = $1; }
		| init_list stmt_list { main_prog = $2; };

init_list:	init
		| init_list init;

init:		INIT var '=' INTEGER ';'
		{
		  set_var($2, $4);
		};

stmt_list:	stmt { $$ = $1; }
		| stmt_list stmt
			{
			  add_stmt_to_list($1, $2);
			  $$ = $1;
			};

stmt:		clear_stmt | incr_stmt | decr_stmt | while_stmt | copy_stmt;

var:		IDENT
		{
		  $$ = find_var($1);
		};

clear_stmt:	CLEAR var ';'
		{
		  $$ = new_stmt(CLEAR_STMT, $2);
		};

incr_stmt:	INCR var ';'
		{
		  $$ = new_stmt(INCR_STMT, $2);
		};

decr_stmt:	DECR var ';'
		{
		  $$ = new_stmt(DECR_STMT, $2);
		};

while_stmt:	WHILE var NOT INTEGER DO ';' stmt_list END ';'
		{
		  if ($4 != 0)
		    error_report("literal in while statement must be zero");
		  $$ = new_stmt(WHILE_STMT, $2);
		  $$->stmt_list = $7;
		};

copy_stmt:	COPY var TO var ';'
		{
		  $$ = new_stmt(COPY_STMT, $2);
		  $$->dest = $4;
		};