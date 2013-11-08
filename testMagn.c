#include <stdio.h>
#include <math.h>
#include <stdint.h>
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


typedef int16_t int16;
typedef int8_t int8;

/* Converting to microTeslas */

float calcMagn(int16 raw)
{
  float v;

  //-- calculate magnetic-field strength, unit uT, range -1000, +1000
  v = (raw * 1.0) / (65536/2000);

  return v;
}

#define BUILD_INT16(loByte, hiByte) ((int16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))


int main(int argc, char **argv) {
	int16 hexX, hexY, hexZ;
	int hex1, hex2, hex3;
	int8 pData[6]; 
	int inp, i;
	FILE *fp;

	char filename[200];

	filename[0] = 0;
	strcat(filename, "/home/optimus-prime/DR-SensorTag-v2/magn_");
	strcat(filename, argv[argc-1]);
	fp = fopen(filename, "a+");

	//fp = fopen("/home/optimus-prime/DR-SensorTag/op.txt", "a+");

	int j=0;
	for(i=2;i<8;i++,j++) {
		sscanf(argv[i], "%x", &inp);		
		pData[j] = (int8)inp;
	}


	hexX = BUILD_INT16(pData[0], pData[1]);
	hexY = BUILD_INT16(pData[2], pData[3]);
	hexZ = BUILD_INT16(pData[4], pData[5]);

	fprintf(fp, "%s , ", argv[1]);
	fprintf(fp, "%5.3f , ", calcMagn(hexX));
	fprintf(fp, "%5.3f , ", calcMagn(hexY));
	fprintf(fp, "%5.3f \n", calcMagn(hexZ));
	fclose(fp);

}
