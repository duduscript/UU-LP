/** sol09.8a.c
 ** ------------------------------------------------------------
	Adding background processing requires minor changes to four files.
		The replacement to smsh2.c is 
		sol09.8a.c,
	the replacement to splitline.c is 
	sol09.8b.c,
	   This version looks for & as a command terminator
	   This version passes the termination char back to the caller

		the replacement to execute.c is 
		sol09.8c.c, 
		the replacement to process2.c is 
		sol09.8d.c, and
		the replacement to smsh.h is 
		sol09.8.h.

	Therefore, linking:

	cc sol09.8a.c sol09.8b.c sol09.8c.c sol09.8d.c \
		controlflow.c builtin.c varlib.c -o sol8

	will produce a shell that supports use of the & symbol to run 
	a command in the background.  The splitline replacement also 
	includes the feature to launch several commands on one line.  
	Therefore

	cmd1 & cmd2 & cmd3 ; cmd4 ; cmd5

	will work as expected.  As before, you can replace 
	controlflow.c with sol09.6.c or sol09.7.c for else support, 
	and you can replace builtin.c with sol09.5.c for exit and cd.

 ** ------------------------------------------------------------
 **
 **
 *    A version of splitline.c that splits on '\n', ';', and '&'
 *    The next_cmd function has a new parameter: a pointer to the
 *    termination character.
 *
 *    A version of splitline.c with a revised next_cmd.  This next_cmd
 *    breaks on ';'s and keeps track if the last cmd ended with a ';'
 *    If it did, then no prompt is printed.
 *
 * splitline.c - commmand reading and parsing functions for smsh
 *    
 *    char *next_cmd(char *prompt, FILE *fp) - get next command
 *    char **splitline(char *str);           - parse a string
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	"sol09.8.h"

char * next_cmd(char *prompt, FILE *fp, int *termcp)
/*
 * purpose: read next command line from fp
 * returns: dynamically allocated string holding command line
 *  errors: NULL at EOF (not really an error)
 *          calls fatal from emalloc()
 *   notes: allocates space in BUFSIZ chunks.  
 */
{
	char	*buf ; 				/* the buffer		*/
	int	bufspace = 0;			/* total size		*/
	int	pos = 0;			/* current position	*/
	int	c;				/* input char		*/

	static  char prevterm = '\n';		/* terminator for prev	*/
						/* command		*/

	if ( prevterm == '\n' )
		printf("%s", prompt);			/* prompt user	*/
	while( ( c = getc(fp)) != EOF ) {

		/* need space? */
		if( pos+1 >= bufspace ){		/* 1 for \0	*/
			if ( bufspace == 0 )		/* y: 1st time	*/
				buf = emalloc(BUFSIZ);
			else				/* or expand	*/
				buf = erealloc(buf,bufspace+BUFSIZ);
			bufspace += BUFSIZ;		/* update size	*/
		}

		/* end of command? */
		if ( c == '\n' || c == ';' || c == '&' ){
			*termcp  = c;
			prevterm = c;
			break;
		}

		/* no, add to buffer */
		buf[pos++] = c;
	}
	if ( c == EOF && pos == 0 )		/* EOF and no input	*/
		return NULL;			/* say so		*/
	buf[pos] = '\0';
	return buf;
}

/**
 **	splitline ( parse a line into an array of strings )
 **/
#define	is_delim(x) ((x)==' '||(x)=='\t')

char ** splitline(char *line)
/*
 * purpose: split a line into array of white-space separated tokens
 * returns: a NULL-terminated array of pointers to copies of the tokens
 *          or NULL if line if no tokens on the line
 *  action: traverse the array, locate strings, make copies
 *    note: strtok() could work, but we may want to add quotes later
 */
{
	char	*newstr();
	char	**args ;
	int	spots = 0;			/* spots in table	*/
	int	bufspace = 0;			/* bytes in table	*/
	int	argnum = 0;			/* slots used		*/
	char	*cp = line;			/* pos in string	*/
	char	*start;
	int	len;

	if ( line == NULL )			/* handle special case	*/
		return NULL;

	args     = emalloc(BUFSIZ);		/* initialize array	*/
	bufspace = BUFSIZ;
	spots    = BUFSIZ/sizeof(char *);

	while( *cp != '\0' )
	{
		while ( is_delim(*cp) )		/* skip leading spaces	*/
			cp++;
		if ( *cp == '\0' )		/* quit at end-o-string	*/
			break;

		/* make sure the array has room (+1 for NULL) */
		if ( argnum+1 >= spots ){
			args = erealloc(args,bufspace+BUFSIZ);
			bufspace += BUFSIZ;
			spots += (BUFSIZ/sizeof(char *));
		}

		/* mark start, then find end of word */
		start = cp;
		len   = 1;
		while (*++cp != '\0' && !(is_delim(*cp)) )
			len++;
		args[argnum++] = newstr(start, len);
	}
	args[argnum] = NULL;
	return args;
}

/*
 * purpose: constructor for strings
 * returns: a string, never NULL
 */
char *newstr(char *s, int l)
{
	char *rv = emalloc(l+1);

	rv[l] = '\0';
	strncpy(rv, s, l);
	return rv;
}

void 
freelist(char **list)
/*
 * purpose: free the list returned by splitline
 * returns: nothing
 *  action: free all strings in list and then free the list
 */
{
	char	**cp = list;
	while( *cp )
		free(*cp++);
	free(list);
}

void * emalloc(size_t n)
{
	void *rv ;
	if ( (rv = malloc(n)) == NULL )
		fatal("out of memory","",1);
	return rv;
}
void * erealloc(void *p, size_t n)
{
	void *rv;
	if ( (rv = realloc(p,n)) == NULL )
		fatal("realloc() failed","",1);
	return rv;
}

