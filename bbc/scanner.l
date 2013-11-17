/*  Scanner for Bare Bones interpreter
    File : Scanner.y
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
#include <string.h>

#include "barebones.h"
#include "util.h"
#include "parser.tab.h"

static keyword_t keywords [] =
{
  { "clear",      CLEAR },
  { "copy",       COPY },
  { "decr",       DECR },
  { "do",         DO },
  { "end",        END },
  { "incr",       INCR },
  { "init",       INIT },
  { "not",        NOT },
  { "to",         TO },
  { "while",      WHILE },
  { NULL,         0 }
};

void yyerror (const char *s){
    error_report(s);
}

%}

%option noyywrap

digit		[0-9]
alpha		[a-zA-Z]
alphanum	[a-zA-Z0-9_]
%%
{digit}+	{ yylval.integer = atoi(yytext); return INTEGER; }

{alpha}{alphanum}*	{
                        if ((yylval.integer = keyword(yytext, keywords)))
                            return yylval.integer;
                        yylval.string = newstr(yytext);
                        return IDENT;
                    }


\#.*		;
[ \t]+		;
\n		;

.		{ return yytext [0]; }

%%