#include <stdio.h>
//#include <stdlib.h>

int main( int argc, char *argv[] )
{
	// Static buffer on the stack.
	char buffer[1024];
	if ( argc != 2 )
	{
		printf("Usage: %s argument\n", argv[0] );
		return( -1 );
	}
	// Unbound string copy.
	strcpy( buffer, argv[1]);

	printf( "Copied argument\n" );

	return(0);
}
