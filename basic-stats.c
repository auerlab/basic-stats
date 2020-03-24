/***************************************************************************
 *  Description:
 *  
 *  Arguments:
 *
 *  Returns:
 *
 *  History: 
 *  Date        Name        Modification
 *  2020-03-24  Jason Bacon Begin
 ***************************************************************************/

#include <stdio.h>
#include <sysexits.h>
#include <getopt.h>

int     main(int argc,char *argv[])

{
    /* options descriptor */
    static struct option longopts[] = {
	{ "median",     required_argument,  NULL,           'm' },
	{ "average",    required_argument,  NULL,           'a' },
	{ 0,            0,                  0,              0 }
    };
    int     ch;
    
    while ((ch = getopt_long(argc, argv, "m:a:", longopts, NULL)) != -1)
    {
	switch(ch)
	{
	    case    'm':
		puts("Got -m");
		break;
	    case    'a':
		puts("Got -a");
		break;
	}
    }
    return EX_OK;
}

