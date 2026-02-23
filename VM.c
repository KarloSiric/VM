/* VM.c */

#include "VM.h"

void execinstr( VM *vm, Instruction *ip ) { 
    /*
      // @TODO(KARLO): to be implemented because right now it is still not done 
    */ 
}

void execute( VM *vm ) {
    Program *pp;
    Instruction *ip;
    int16 size;
    
    assert( vm && *vm->m );
    pp = vm->m;
    
    while ( ( *pp != ( Opcode )hlt ) && ( *pp <= vm->b ) ) {
        ip = ( Instruction * )pp;
        size = map( ip->o );
        execinstr( vm, ip );
        
        ip( vm ) += size;
        pp += size; 
        
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
        error( p, ErrMem );
        // return (VM *)0;
    }
    
    zero( (int8 * ) p, ( int16 )sizeof( *p ) ); 

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
    
    if ( sa1 ) { 
        a1 = i1->a;
        zero( a1, sa1 );
        *a1 = 0x0005;
    }
    
    assert( a1 );
    
    ps = ( si1 + si2 );
    program = vm->m;
    copy( program, $1 i1, si1 );
    program += si1;
    
    i2->o = nop;
    copy( program, $1 i2, si2 );
    
    vm->b = ( si1 + sa1 + si2 );
    ax( vm ) = ( Reg )*vm->m;
    sp( vm ) = ( Reg )-1;
       
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
