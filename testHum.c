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


typedef uint16_t uint16;
typedef int16_t qint16;
typedef uint8_t uint8;

/* Conversion algorithm, temperature */

double calcHumTmp(uint16 rawT)
{
  double v;

  //-- calculate temperature [deg C] --
  v = -46.85 + 175.72/65536 *(double)(qint16)rawT;

  return v;
}

/*  Conversion algorithm, humidity */

double calcHumRel(uint16 rawH)
{
  double v;

  rawH &= ~0x0003; // clear bits [1..0] (status bits)
  //-- calculate relative humidity [%RH] --
  v = -6.0 + 125.0/65536 * (double)rawH; // RH= -6 + 125 * SRH/2^16

  return v;
}

 #define BUILD_UINT16(loByte, hiByte) ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))


int main(int argc, char **argv) {
	uint16 hexTemp;
	uint16 hexHum;
	uint8 pData[4];
	
	int inp, i;
	FILE *fp;

	char filename[200];

	filename[0] = 0;
	strcat(filename, "/home/optimus-prime/DR-SensorTag/");
	strcat(filename, argv[argc-1]);
	fp = fopen(filename, "a+");

	//fp = fopen("/home/optimus-prime/DR-SensorTag/op.txt", "a+");

	printf("Got data");
	for(i=0;i<4;i++) {
		sscanf(argv[i+1], "%x", &inp);		
		pData[i] = (uint8)inp;
	}

	hexTemp = BUILD_UINT16(pData[0], pData[1]);
	hexHum = BUILD_UINT16(pData[2], pData[3]);

	fprintf(fp, "\n---------------------------------------------------------");
	fprintf(fp, "\nHumidity Data [%ld]\n", getinstanttime());
	fprintf(fp, "Temp : %3.2f", calcHumTmp(hexTemp));
	fprintf(fp, "\tHumidity : %3.2f", calcHumRel(hexHum));
	fprintf(fp, "\n---------------------------------------------------------\n");

}
