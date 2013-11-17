/*  Bare Bones Virtual Machine
	File : bbvm.h
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

typedef struct Instruction
{
	char opcode;
	int param1;
	int param2;
}instruct_t;

union Convert
{
	char bytes[4];
	int integer;
};

typedef enum 
{
	JE_INS = 1,
	LOAD_INS = 2,
	MOV_INS = 3,
	CLEAR_INS = 4,
	INC_INS = 5,
	DEC_INS = 6,
	CMP_INS = 7,
	JMP_INS = 8,
}instruct_type_t;