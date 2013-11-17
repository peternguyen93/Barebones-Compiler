/*  Bare Bones Compiler
    File : util.h
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


#define UNUSED __attribute__ ((unused))

// extern char *progname;  // must be set by main program

// void usage (FILE *f);    // must be implemented in main program

// generate fatal error message to stderr, doesn't return
void logging(int ret, char *format, ...) __attribute__ ((noreturn));

void *alloc(size_t size);

char *newstr(char *orig);

char *newstrn(char *orig, int max_len);

typedef struct keyword
{
	char *name;
	int value;
} keyword_t;

int keyword(char *string, keyword_t *table);

//intmax_t a_to_intmax(char *s);