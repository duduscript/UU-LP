/** cp1.c
 *     version 1 of cp - uses read and write with tunable buffer size
 *
 *     usage: cp1 src dest
 */
#include        <stdio.h>
#include        <unistd.h>
#include        <fcntl.h>
#include	<sys/types.h>
#include	<dirent.h>

#define BUFFERSIZE      4096
#define COPYMODE        0644

void oops(char *, char *);
int isadir(char* path);
void copyfile(char* path_in, char* path_out);
void copydir(char* path_in, char* path_out);

main(int ac, char *av[])
{
        int     in_fd, out_fd;
						/* check args 	*/
        if ( ac != 3 ){
                fprintf( stderr, "usage: %s source destination\n", *av);
                exit(1);
        }

	if ( isadir(av[1]) && isadir(av[2]) )
		copydir(av[1], av[2]);

	copyfile(av[1],av[2]);
	
}

void oops(char *s1, char *s2)
{
        fprintf(stderr,"Error: %s ", s1);
        perror(s2);
        exit(1);
}

int isadir(char* path)
{
	struct stat st;
	if ( stat(path,&st) != -1 && S_ISDIR(st->st_mode) )
		return true;
	return false;
}
void copyfile(char* path_in, char* path_out)
{
	int in_fd, out_fd, n_chars;
        char    buf[BUFFERSIZE];

        if ( (in_fd=open(path_in, O_RDONLY)) == -1 )
                oops("Cannot open ",path_in);

        if ( (out_fd = creat(path_out , COPYMODE)) == -1 )
                oops( "Cannot creat", path_out);

        while ( (n_chars = read(in_fd , buf, BUFFERSIZE)) > 0 )
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", av[2]);

	if ( n_chars == -1 )
			oops("Read error from ", av[1]);

						/* close files	*/

        if ( close(in_fd) == -1 || close(out_fd) == -1 )
                oops("Error closing files","");
                if ( write( out_fd, buf, n_chars ) != n_chars )
                        oops("Write error to ", av[2]);
	if ( n_chars == -1 )
			oops("Read error from ", av[1]);

						/* close files	*/

        if ( close(in_fd) == -1 || close(out_fd) == -1 )
                oops("Error closing files","");
}
void copydir(char* path_in, char* path_out)
{
	int in_fd,out_fd;
	DIR* dir;
	struct dirent dir_foo;

	if ( (dir = opendir(path_in)) == NULL)
		oops("Cannot open",path_in);
	while ( (dir_foo = readdir(dir) ) != -1){
		
	}	
}
