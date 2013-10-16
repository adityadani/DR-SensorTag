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


typedef uint16_t uint16;
typedef int16_t qint16;
typedef uint8_t uint8;

double m_tmpAmb;
/*  Conversion algorithm for die temperature */
double calcTmpLocal(uint16 rawT)
{
  //-- calculate die temperature [°C] --
  m_tmpAmb = (double)((qint16)rawT)/128.0; // Used in also in the calc. below
  return m_tmpAmb;
}


/* Conversion algorithm for target temperature */
double calcTmpTarget(uint16 rawT)
{
  //-- calculate target temperature [°C] -
  double Vobj2 = (double)(qint16)rawT;
  Vobj2 *= 0.00000015625;

  double Tdie2 = m_tmpAmb + 273.15;
  const double S0 = 6.4E-14;            // Calibration factor

  const double a1 = 1.75E-3;
  const double a2 = -1.678E-5;
  const double b0 = -2.94E-5;
  const double b1 = -5.7E-7;
  const double b2 = 4.63E-9;
  const double c2 = 13.4;
  const double Tref = 298.15;
  double S = S0*(1+a1*(Tdie2 - Tref)+a2*pow((Tdie2 - Tref),2));
  double Vos = b0 + b1*(Tdie2 - Tref) + b2*pow((Tdie2 - Tref),2);
  double fObj = (Vobj2 - Vos) + c2*pow((Vobj2 - Vos),2);
  double tObj = pow(pow(Tdie2,4) + (fObj/S),.25);
  tObj = (tObj - 273.15);

  return tObj;
}

 #define BUILD_UINT16(loByte, hiByte) ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

 
int main(int argc, char **argv) {
	uint16 hexObj;
	uint16 hexAmb;
	uint8 pData[4]; 
	int inp, i;
	FILE *fp;
	char filename[200];

	filename[0] = 0;
	strcat(filename, "/home/optimus-prime/DR-SensorTag/");
	strcat(filename, argv[argc-1]);
	fp = fopen(filename, "a+");

	printf("Got data");
	for(i=0;i<4;i++) {
		sscanf(argv[i+1], "%x", &inp);		
		pData[i] = (uint8)inp;
	}


	hexObj = BUILD_UINT16(pData[0], pData[1]);
	hexAmb = BUILD_UINT16(pData[2], pData[3]);

	fprintf(fp, "\n---------------------------------------------------------");
	fprintf(fp, "\nTemperature Data [%ld]\n", getinstanttime());
	fprintf(fp, "Ambient Temp : %3.2f", calcTmpLocal(hexAmb));
	fprintf(fp, "\tObject(IR) Temp : %3.2f", calcTmpTarget(hexObj));
	fprintf(fp, "\n---------------------------------------------------------");
	fclose(fp);
}
