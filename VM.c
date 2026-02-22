/* VM.c */
#include "VM.h"

void executeprogram( VM *vm ) {
    Program *pp;
    
    assert( vm && *vm->m );
    
    pp = vm->m;
    
    while ( ( *pp != ( Opcode )hlt ) && ( *pp <= vm->b ) ) {
        
    }
    
    if ( *pp > vm->b ) {
        segfault( vm );
    } 
}

void error( VM *vm, Errorcode e ) {
    if ( vm ) {
        free( vm );
    }   
    
    switch( e ) {
        case ErrSegv:
            fprintf( stderr, "%s\n", "VM Segmentation Fault" );
            break;
        default:
            break;
    }
    
    exit( -1 );
}

int16 map( Opcode o ) {
	int16 n, ret = 0;
	IM *p;

	for ( n = IMs, p = instrmap; n; n--, p++ ) {
		if ( p->o == o ) {
			ret = p->size;
            break;
		}
	}

	return ( int16 )ret;
}

VM *virtualmachine( void ) {
	VM *p;
    
	p = ( VM * )malloc( sizeof( *p ) );

	if ( !p ) {
		errno = ErrMem;
		return (VM *)0;
	}
    
    zero( (int8 * ) p, ( int16 )sizeof( *p ) );

	p->c.r.ip = 0;
	p->c.r.sp = 0xFFFF;

	return p;
}

Program *exampleprogram( VM *vm ) { 
    Program *program;
    Instruction *i1, *i2;
    Args *a1 = NULL;
    int16 si1, si2, sa1, ps;
    
    si1 = map( mov );
    si2 = map( nop );
    
    i1 = ( Instruction * )malloc( si1 );
    i2 = ( Instruction * )malloc( si2 );
    
    assert( i1 && i2 );
    
    if ( !si1 || !si2 ) {
        errno = ErrMem;
        return ( Program *)0;
    }
    
    zero( $1 i1, si1 );
    zero( $1 i2, si2 ); 
    
    i1->o = mov;
    sa1 = ( si1 - 1 );
    
    if ( si1 ) {
        a1 = ( Args *)malloc( sizeof( sa1 ) );
        // 0000 0001 mov eax
        // 0000 0000
        // 0000 0005 mov eax, 0x05
    }  
    ps = ( si1 + si2 );
    program = vm->m;
    copy( program, $1 i1, 1 );
    program++;
    
    if ( a1 ) {
        assert( a1 );
        zero( a1, sa1 );
        *a1         = 0x00;
        *( a1 + 1 ) = 0x05;
    }

    i2->o = nop;
    copy( program, $1 i2, 1 );
    
    vm->b = ( si1 + sa1 + si2 );
    
    free( i1 );
    free( i2 );
      
    return ( vm->m );    
}




int main( int argc, char *argv[] ) {
	VM *vm;
	Program *prog;
    
	vm = virtualmachine();
    printf( "VM = %p\n", ( void * )vm );
    
	prog = exampleprogram( vm );
    printf( "prog = %p\n", ( void * )prog );
    
    printhex($1 prog, ( map( mov ) + map ( nop ) ), ' ' );

	return 0;
}
