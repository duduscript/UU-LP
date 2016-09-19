#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc,char** argv){
	//int fd;
	//DIR* dir;
	//struct dirent* direntry;
	if ( mkdir("./demodir",0777) != 0 ){
		perror("Error in mkdir demodir!");
	}
/*
	if ( (dir = opendir("demodir")) == NULL){
		perror("Error in opendir!");
	}
*/
	if ( mkdir("./demodir/a",0777) != 0 ){
		perror("Error in mkdir a!");
		goto error;
	}
	if ( mkdir("./demodir/c",0777) != 0 ){
		perror("Error in mkdir c!");
		goto error;
	}
	if ( mkdir("./demodir/c/d1",0777) != 0 ){
		perror("Error in mkdir d1!");
		goto error;
	}
	if ( mkdir("./demodir/c/d2",0777) != 0 ){
		perror("Error in mkdir d2!");
		goto error;
	}
	if ( open("./demodir/y",O_CREAT|O_WRONLY|O_TRUNC) != 0 ){
		perror("Error in creat y");
		goto error;
	}
	if ( open("./demodir/a/x",O_CREAT|O_WRONLY|O_TRUNC) != 0){
		perror("Error in creat x");
		goto error;
	}
	if ( open("./demodir/c/xcopy",O_CREAT|O_WRONLY|O_TRUNC) != 0){
		perror("Error in creat xcopy");
		goto error;
	}
	if ( link("./demodir/a/x","./demodir/c/d1/xlink") != 0){
		perror("Error in link x");
		goto error;
	}
	return 0;
error:
	exit(1);
}
