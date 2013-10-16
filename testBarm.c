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
typedef uint8_t uint8;
typedef int64_t int64;
typedef int16_t int16;

typedef struct Calib {
	uint16 c1, c2, c3, c4, c5, c6, c7, c8;
}barCalib;

uint16 m_raw_temp;

barCalib m_barCalib;
/*  Conversion algorithm for barometer temperature
 * 
 *  Formula from application note, rev_X:
 *  Ta = ((c1 * Tr) / 2^24) + (c2 / 2^10)
 *
 *  c1 - c8: calibration coefficients the can be read from the sensor
 *  c1 - c4: unsigned 16-bit integers
 *  c5 - c8: signed 16-bit integers
 */
double calcBarTmp(uint16 rawT)
{
  uint16 c1, c2;

  c1 = m_barCalib.c1;
  c2 = m_barCalib.c2;
  m_raw_temp = rawT;

  int64 temp, val;
  val = ((int64)(c1 * m_raw_temp) * 100);
  temp = (val >> 24);
  val = ((int64)c2 * 100);
  temp += (val >> 10);

  return ((double)temp) / 100;
}

/* Conversion algorithm for barometer pressure (hPa)
 * 
 * Formula from application note, rev_X:
 * Sensitivity = (c3 + ((c4 * Tr) / 2^17) + ((c5 * Tr^2) / 2^34))
 * Offset = (c6 * 2^14) + ((c7 * Tr) / 2^3) + ((c8 * Tr^2) / 2^19)
 * Pa = (Sensitivity * Pr + Offset) / 2^14
 */

double TcalcBarPress(uint16 rawT)
{
  int64 s, o, pres, val;
  uint16 c3, c4;
  int16 c5, c6, c7, c8;
  uint16 Pr;
  int16 Tr;

  Pr = rawT;
  Tr = m_raw_temp;
  c3 = m_barCalib.c3;
  c4 = m_barCalib.c4;
  c5 = m_barCalib.c5;
  c6 = m_barCalib.c6;
  c7 = m_barCalib.c7;
  c8 = m_barCalib.c8;

  // Sensitivity
  s = (int64)c3;
  val = (int64)c4 * Tr;
  s += (val >> 17);
  val = (int64)c5 * Tr * Tr;
  s += (val >> 34);

  // Offset
  o = (int64)c6 << 14;
  val = (int64)c7 * Tr;
  o += (val >> 3);
  val = (int64)c8 * Tr * Tr;
  o += (val >> 19);
  
  // Pressure (Pa)
  pres = ((int64)(s * Pr) + o) >> 14;

  return (double)pres/100;
}

 #define BUILD_UINT16(loByte, hiByte) ((uint16)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

void storeCalibrationData(uint8 *pData)
{
  m_barCalib.c1 = BUILD_UINT16(pData[0],pData[1]);
  m_barCalib.c2 = BUILD_UINT16(pData[2],pData[3]);
  m_barCalib.c3 = BUILD_UINT16(pData[4],pData[5]);
  m_barCalib.c4 = BUILD_UINT16(pData[6],pData[7]);
  m_barCalib.c5 = BUILD_UINT16(pData[8],pData[9]);
  m_barCalib.c6 = BUILD_UINT16(pData[10],pData[11]);
  m_barCalib.c7 = BUILD_UINT16(pData[12],pData[13]);
  m_barCalib.c8 = BUILD_UINT16(pData[14],pData[15]);
}

int main(int argc, char **argv) {
	uint8 pData[20];
	uint16 rawTemp, rawPress;
	int i, inp;
	FILE *fp;

	char filename[200];

	filename[0] = 0;
	strcat(filename, "/home/optimus-prime/DR-SensorTag/");
	strcat(filename, argv[argc-1]);
	fp = fopen(filename, "a+");

	//fp = fopen("/home/optimus-prime/DR-SensorTag/op.txt", "a+");


	for(i=0;i<20;i++) {
		sscanf(argv[i+1], "%x", &inp);		
		pData[i] = (uint8)inp;
	}
	
	rawTemp = BUILD_UINT16(pData[16], pData[17]);
	rawPress = BUILD_UINT16(pData[18], pData[19]);

	storeCalibrationData(pData);
	
	fprintf(fp, "\n---------------------------------------------------------");
	fprintf(fp, "\nBarometer Data [%ld]", getinstanttime());
	fprintf(fp, "\nBarometric Temp : %3.2f", calcBarTmp(rawTemp));
	fprintf(fp, "\tBarometric Pressure : %3.2f", TcalcBarPress(rawPress));
	fprintf(fp, "\n---------------------------------------------------------");
	fprintf(fp, "\n\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
	fprintf(fp, "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n");

}
