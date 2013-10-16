#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

long getinstanttime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return((tv.tv_sec*1000000) + tv.tv_usec);
}


typedef int8_t int8;

float calcAccel(int8 raw)
{
  float v;
  //-- calculate acceleration, unit g, range -2, +2
  v = (raw * 1.0) / (64);
  return v;
}

int main(int argc, char **argv) {
	int8 hexX, hexY, hexZ;

	int hex1, hex2, hex3;
	float X, Y, Z;
	FILE *fp;
	fp = fopen("/home/optimus-prime/DR-SensorTag/op.txt", "a+");
	
	sscanf(argv[1], "%x", &hex1);
	sscanf(argv[2], "%x", &hex2);
	sscanf(argv[3], "%x", &hex3);

	hexX = (int8)(hex1);
	hexY = (int8)(hex2);
	hexZ = (int8)(hex3);

	// Swapping X to Y bcoz the data shown on Ipad is exactly swapped between
	// X and Y. Conforming to the data shown on Ipad as correct.
	X = calcAccel(hexY); // the swap X --> hexY
	Y = calcAccel(hexX);
	Z = calcAccel(-1*hexZ);

	fprintf(fp, "\n---------------------------------------------------------");
	fprintf(fp, "\nAccelerometer Data [%ld]\n", getinstanttime());
	fprintf(fp, "X : %3.2f", X);
	fprintf(fp, "\tY : %3.2f", Y);
	fprintf(fp, "\tZ : %3.2f", Z);
	fprintf(fp, "\n---------------------------------------------------------\n");
	fclose(fp);
       
}
