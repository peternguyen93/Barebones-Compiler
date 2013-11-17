/*  Bare Bones Compiler
    File : barebones.h
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

typedef enum 
{
  CLEAR_STMT,
  INCR_STMT,
  DECR_STMT,
  WHILE_STMT,
  COPY_STMT,
  ADD_CLEAR_STMT  // optimizer may transform a while statement into this
} stmt_type_t;

typedef struct var_t
{
  struct var_t *next;
  char *name;
  int num;
  bool init;
  uintmax_t val;
} var_t; //store all vars on program to linked list

typedef struct stmt_t
{
  struct stmt_t *next;	// used to chain a statement list
  struct stmt_t *tail;  // only used for head of a statement list
  int line;
  stmt_type_t type;
  var_t *var;
  var_t *dest;  // only used for COPY_STMT
  struct stmt_t *stmt_list;  // only used for WHILE_STMT
} stmt_t;

void error_report(const char *fmt, ...);

var_t *find_var(char *name);
void set_var(var_t *var, uintmax_t value);

stmt_t *new_stmt(stmt_type_t type, var_t *var);

void add_stmt_to_list(stmt_t *head, stmt_t *stmt);

void check_var_init(var_t *var);

extern stmt_t *main_prog;
