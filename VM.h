/* VM.h */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <birchutils.h>

typedef unsigned short int int16;
typedef unsigned char int8;
typedef unsigned int int32;
typedef unsigned long long int int64;

#define $1 ( int8 * )
#define $2 ( int16 )
#define $4 ( int32 )
#define $8 ( int64 )
#define $c ( char * )
#define $i ( int )

#define segfault( x )          error( ( x ), ErrSegv )

#define ErrMem      0x01        // 00 01
#define ErrArgs     0x02        // 00 10
#define ErrSegv     0x04        // 01 00


#define NoArgs      { 0x00, 0x00 }



/*
 *  16 bit CPU
 *  65356 ~ 65K 65 KB
 *
 *  registers:
 *  AX
 *  BX
 *  CX
 *  DX
 *
 *  SP ~stack pointer
 *  IP ~instruction pointer
 *
 */

enum e_opcode {
	mov = 0x01,
	nop = 0x02,
    hlt = 0x03,
};

typedef enum e_opcode Opcode;

struct s_instrmap {
	Opcode o;
	int8 size;
};

typedef struct s_instrmap IM;

typedef int8 Args;

struct s_instruction {
	Opcode o;
	Args a[2];
};

typedef unsigned short int Reg;

typedef struct s_instruction Instruction;

typedef unsigned char Errorcode;

typedef int8 Memory[( (unsigned short)( -1 ) )];

typedef int8 Program;

struct s_registers {
	Reg ax;
	Reg bx;
	Reg cx;
	Reg dx;
	Reg sp;
	Reg ip;
};

typedef struct s_registers Registers;

struct s_cpu {
	Registers r;
};

typedef struct s_cpu CPU;

struct s_vm {
	CPU c;
	Memory m;
    int16 b;    // break line
};

typedef struct s_vm VM;

typedef Memory *Stack;

#define IMs ( sizeof( instrmap ) / sizeof( struct s_instrmap ) )

static IM instrmap[] = {
	{ mov, 0x03 },
	{ nop, 0x01 },
    { hlt, 0x01 }
};

void error( VM *, Errorcode );

void executeprogram( VM * );

VM *virtualmachine( void );

int16 map( Opcode o );

Program *exampleprogram( VM * );

int main( int, char ** );
