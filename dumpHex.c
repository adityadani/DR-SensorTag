#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

long getinstanttime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return((tv.tv_sec*1000000) + tv.tv_usec);
}

int main(int argc, char **argv) {
	
	FILE *fp;
	char filename[200];
	int i;

	printf("Inside dumphex ----- > \n\n\n\n");
	filename[0] = 0;
	strcat(filename, "/home/optimus-prime/DR-SensorTag-newDesign/");
	strcat(filename, argv[argc-1]);
	fp = fopen(filename, "a+");

	for(i=1;i<argc-1;i++) {
		fprintf(fp, "%s ", argv[i]);
		printf("%s ", argv[i]);
	}
	fprintf(fp, "[%ld]\n", getinstanttime());
	fclose(fp);
}
