#include <string.h>

#include "panel.h"

struct Field __RawData_fields[]={
{"================================= RAW Data ====================================",1,1,0,80,},
{"Sample  !   0!  -1!  -2!  -3!  -4!  -5!  -6!  -7!  -8!  -9! -10! -11! -12! -13!",1,2,0,80,},
{"$a01",0,3,12,0,},
{"$b01",0,3,17,0,},
{"$c01",0,3,22,0,},
{"$d01",0,3,27,0,},
{"$e01",0,3,32,0,},
{"$f01",0,3,37,0,},
{"$g01",0,3,42,0,},
{"$h01",0,3,47,0,},
{"$i01",0,3,52,0,},
{"$j01",0,3,57,0,},
{"$k01",0,3,62,0,},
{"$l01",0,3,67,0,},
{"$m01",0,3,72,0,},
{"$n01",0,3,77,0,},
{"MW2     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,3,0,80,},
{"$a02",0,4,12,0,},
{"$b02",0,4,17,0,},
{"$c02",0,4,22,0,},
{"$d02",0,4,27,0,},
{"$e02",0,4,32,0,},
{"$f02",0,4,37,0,},
{"$g02",0,4,42,0,},
{"$h02",0,4,47,0,},
{"$i02",0,4,52,0,},
{"$j02",0,4,57,0,},
{"$k02",0,4,62,0,},
{"$l02",0,4,67,0,},
{"$m02",0,4,72,0,},
{"$n02",0,4,77,0,},
{"PROMIDA !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,4,0,80,},
{"$a03",0,5,12,0,},
{"$b03",0,5,17,0,},
{"$c03",0,5,22,0,},
{"$d03",0,5,27,0,},
{"$e03",0,5,32,0,},
{"$f03",0,5,37,0,},
{"$g03",0,5,42,0,},
{"$h03",0,5,47,0,},
{"$i03",0,5,52,0,},
{"$j03",0,5,57,0,},
{"$k03",0,5,62,0,},
{"$l03",0,5,67,0,},
{"$m03",0,5,72,0,},
{"$n03",0,5,77,0,},
{"PROMIDB !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,5,0,80,},
{"$a04",0,6,12,0,},
{"$b04",0,6,17,0,},
{"$c04",0,6,22,0,},
{"$d04",0,6,27,0,},
{"$e04",0,6,32,0,},
{"$f04",0,6,37,0,},
{"$g04",0,6,42,0,},
{"$h04",0,6,47,0,},
{"$i04",0,6,52,0,},
{"$j04",0,6,57,0,},
{"$k04",0,6,62,0,},
{"$l04",0,6,67,0,},
{"$m04",0,6,72,0,},
{"$n04",0,6,77,0,},
{"IAC     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,6,0,80,},
{"$a05",0,7,12,0,},
{"$b05",0,7,17,0,},
{"$c05",0,7,22,0,},
{"$d05",0,7,27,0,},
{"$e05",0,7,32,0,},
{"$f05",0,7,37,0,},
{"$g05",0,7,42,0,},
{"$h05",0,7,47,0,},
{"$i05",0,7,52,0,},
{"$j05",0,7,57,0,},
{"$k05",0,7,62,0,},
{"$l05",0,7,67,0,},
{"$m05",0,7,72,0,},
{"$n05",0,7,77,0,},
{"CT      !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,7,0,80,},
{"$a06",0,8,12,0,},
{"$b06",0,8,17,0,},
{"$c06",0,8,22,0,},
{"$d06",0,8,27,0,},
{"$e06",0,8,32,0,},
{"$f06",0,8,37,0,},
{"$g06",0,8,42,0,},
{"$h06",0,8,47,0,},
{"$i06",0,8,52,0,},
{"$j06",0,8,57,0,},
{"$k06",0,8,62,0,},
{"$l06",0,8,67,0,},
{"$m06",0,8,72,0,},
{"$n06",0,8,77,0,},
{"MPH     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,8,0,80,},
{"$a07",0,9,12,0,},
{"$b07",0,9,17,0,},
{"$c07",0,9,22,0,},
{"$d07",0,9,27,0,},
{"$e07",0,9,32,0,},
{"$f07",0,9,37,0,},
{"$g07",0,9,42,0,},
{"$h07",0,9,47,0,},
{"$i07",0,9,52,0,},
{"$j07",0,9,57,0,},
{"$k07",0,9,62,0,},
{"$l07",0,9,67,0,},
{"$m07",0,9,72,0,},
{"$n07",0,9,77,0,},
{"MAP     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,9,0,80,},
{"$a08",0,10,12,0,},
{"$b08",0,10,17,0,},
{"$c08",0,10,22,0,},
{"$d08",0,10,27,0,},
{"$e08",0,10,32,0,},
{"$f08",0,10,37,0,},
{"$g08",0,10,42,0,},
{"$h08",0,10,47,0,},
{"$i08",0,10,52,0,},
{"$j08",0,10,57,0,},
{"$k08",0,10,62,0,},
{"$l08",0,10,67,0,},
{"$m08",0,10,72,0,},
{"$n08",0,10,77,0,},
{"RPM     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,10,0,80,},
{"$a09",0,11,12,0,},
{"$b09",0,11,17,0,},
{"$c09",0,11,22,0,},
{"$d09",0,11,27,0,},
{"$e09",0,11,32,0,},
{"$f09",0,11,37,0,},
{"$g09",0,11,42,0,},
{"$h09",0,11,47,0,},
{"$i09",0,11,52,0,},
{"$j09",0,11,57,0,},
{"$k09",0,11,62,0,},
{"$l09",0,11,67,0,},
{"$m09",0,11,72,0,},
{"$n09",0,11,77,0,},
{"TPS     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,11,0,80,},
{"$a10",0,12,12,0,},
{"$b10",0,12,17,0,},
{"$c10",0,12,22,0,},
{"$d10",0,12,27,0,},
{"$e10",0,12,32,0,},
{"$f10",0,12,37,0,},
{"$g10",0,12,42,0,},
{"$h10",0,12,47,0,},
{"$i10",0,12,52,0,},
{"$j10",0,12,57,0,},
{"$k10",0,12,62,0,},
{"$l10",0,12,67,0,},
{"$m10",0,12,72,0,},
{"$n10",0,12,77,0,},
{"INT     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,12,0,80,},
{"$a11",0,13,12,0,},
{"$b11",0,13,17,0,},
{"$c11",0,13,22,0,},
{"$d11",0,13,27,0,},
{"$e11",0,13,32,0,},
{"$f11",0,13,37,0,},
{"$g11",0,13,42,0,},
{"$h11",0,13,47,0,},
{"$i11",0,13,52,0,},
{"$j11",0,13,57,0,},
{"$k11",0,13,62,0,},
{"$l11",0,13,67,0,},
{"$m11",0,13,72,0,},
{"$n11",0,13,77,0,},
{"O2      !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,13,0,80,},
{"$a12",0,14,12,0,},
{"$b12",0,14,17,0,},
{"$c12",0,14,22,0,},
{"$d12",0,14,27,0,},
{"$e12",0,14,32,0,},
{"$f12",0,14,37,0,},
{"$g12",0,14,42,0,},
{"$h12",0,14,47,0,},
{"$i12",0,14,52,0,},
{"$j12",0,14,57,0,},
{"$k12",0,14,62,0,},
{"$l12",0,14,67,0,},
{"$m12",0,14,72,0,},
{"$n12",0,14,77,0,},
{"MALFFLG1!    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,14,0,80,},
{"$a13",0,15,12,0,},
{"$b13",0,15,17,0,},
{"$c13",0,15,22,0,},
{"$d13",0,15,27,0,},
{"$e13",0,15,32,0,},
{"$f13",0,15,37,0,},
{"$g13",0,15,42,0,},
{"$h13",0,15,47,0,},
{"$i13",0,15,52,0,},
{"$j13",0,15,57,0,},
{"$k13",0,15,62,0,},
{"$l13",0,15,67,0,},
{"$m13",0,15,72,0,},
{"$n13",0,15,77,0,},
{"MALFFLG2!    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,15,0,80,},
{"$a14",0,16,12,0,},
{"$b14",0,16,17,0,},
{"$c14",0,16,22,0,},
{"$d14",0,16,27,0,},
{"$e14",0,16,32,0,},
{"$f14",0,16,37,0,},
{"$g14",0,16,42,0,},
{"$h14",0,16,47,0,},
{"$i14",0,16,52,0,},
{"$j14",0,16,57,0,},
{"$k14",0,16,62,0,},
{"$l14",0,16,67,0,},
{"$m14",0,16,72,0,},
{"$n14",0,16,77,0,},
{"MALFFLG3!    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,16,0,80,},
{"$a15",0,17,12,0,},
{"$b15",0,17,17,0,},
{"$c15",0,17,22,0,},
{"$d15",0,17,27,0,},
{"$e15",0,17,32,0,},
{"$f15",0,17,37,0,},
{"$g15",0,17,42,0,},
{"$h15",0,17,47,0,},
{"$i15",0,17,52,0,},
{"$j15",0,17,57,0,},
{"$k15",0,17,62,0,},
{"$l15",0,17,67,0,},
{"$m15",0,17,72,0,},
{"$n15",0,17,77,0,},
{"MALFFLG4!    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,17,0,80,},
{"$a16",0,18,12,0,},
{"$b16",0,18,17,0,},
{"$c16",0,18,22,0,},
{"$d16",0,18,27,0,},
{"$e16",0,18,32,0,},
{"$f16",0,18,37,0,},
{"$g16",0,18,42,0,},
{"$h16",0,18,47,0,},
{"$i16",0,18,52,0,},
{"$j16",0,18,57,0,},
{"$k16",0,18,62,0,},
{"$l16",0,18,67,0,},
{"$m16",0,18,72,0,},
{"$n16",0,18,77,0,},
{"MWAF1   !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,18,0,80,},
{"$a17",0,19,12,0,},
{"$b17",0,19,17,0,},
{"$c17",0,19,22,0,},
{"$d17",0,19,27,0,},
{"$e17",0,19,32,0,},
{"$f17",0,19,37,0,},
{"$g17",0,19,42,0,},
{"$h17",0,19,47,0,},
{"$i17",0,19,52,0,},
{"$j17",0,19,57,0,},
{"$k17",0,19,62,0,},
{"$l17",0,19,67,0,},
{"$m17",0,19,72,0,},
{"$n17",0,19,77,0,},
{"VOLT    !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,19,0,80,},
{"$a18",0,20,12,0,},
{"$b18",0,20,17,0,},
{"$c18",0,20,22,0,},
{"$d18",0,20,27,0,},
{"$e18",0,20,32,0,},
{"$f18",0,20,37,0,},
{"$g18",0,20,42,0,},
{"$h18",0,20,47,0,},
{"$i18",0,20,52,0,},
{"$j18",0,20,57,0,},
{"$k18",0,20,62,0,},
{"$l18",0,20,67,0,},
{"$m18",0,20,72,0,},
{"$n18",0,20,77,0,},
{"MCUINST !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,20,0,80,},
{"$a19",0,21,12,0,},
{"$b19",0,21,17,0,},
{"$c19",0,21,22,0,},
{"$d19",0,21,27,0,},
{"$e19",0,21,32,0,},
{"$f19",0,21,37,0,},
{"$g19",0,21,42,0,},
{"$h19",0,21,47,0,},
{"$i19",0,21,52,0,},
{"$j19",0,21,57,0,},
{"$k19",0,21,62,0,},
{"$l19",0,21,67,0,},
{"$m19",0,21,72,0,},
{"$n19",0,21,77,0,},
{"LV8     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,21,0,80,},
{"$a20",0,22,12,0,},
{"$b20",0,22,17,0,},
{"$c20",0,22,22,0,},
{"$d20",0,22,27,0,},
{"$e20",0,22,32,0,},
{"$f20",0,22,37,0,},
{"$g20",0,22,42,0,},
{"$h20",0,22,47,0,},
{"$i20",0,22,52,0,},
{"$j20",0,22,57,0,},
{"$k20",0,22,62,0,},
{"$l20",0,22,67,0,},
{"$m20",0,22,72,0,},
{"$n20",0,22,77,0,},
{"BLM     !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,22,0,80,},
{"$a21",0,23,12,0,},
{"$b21",0,23,17,0,},
{"$c21",0,23,22,0,},
{"$d21",0,23,27,0,},
{"$e21",0,23,32,0,},
{"$f21",0,23,37,0,},
{"$g21",0,23,42,0,},
{"$h21",0,23,47,0,},
{"$i21",0,23,52,0,},
{"$j21",0,23,57,0,},
{"$k21",0,23,62,0,},
{"$l21",0,23,67,0,},
{"$m21",0,23,72,0,},
{"$n21",0,23,77,0,},
{"O2_CNT  !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,23,0,80,},
{"$a22",0,24,12,0,},
{"$b22",0,24,17,0,},
{"$c22",0,24,22,0,},
{"$d22",0,24,27,0,},
{"$e22",0,24,32,0,},
{"$f22",0,24,37,0,},
{"$g22",0,24,42,0,},
{"$h22",0,24,47,0,},
{"$i22",0,24,52,0,},
{"$j22",0,24,57,0,},
{"$k22",0,24,62,0,},
{"$l22",0,24,67,0,},
{"$m22",0,24,72,0,},
{"$n22",0,24,77,0,},
{"AIRFLOW2!    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,24,0,80,},
{"$a23",0,25,12,0,},
{"$b23",0,25,17,0,},
{"$c23",0,25,22,0,},
{"$d23",0,25,27,0,},
{"$e23",0,25,32,0,},
{"$f23",0,25,37,0,},
{"$g23",0,25,42,0,},
{"$h23",0,25,47,0,},
{"$i23",0,25,52,0,},
{"$j23",0,25,57,0,},
{"$k23",0,25,62,0,},
{"$l23",0,25,67,0,},
{"$m23",0,25,72,0,},
{"$n23",0,25,77,0,},
{"AIRFLOW1!    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,25,0,80,},
{"$a24",0,26,12,0,},
{"$b24",0,26,17,0,},
{"$c24",0,26,22,0,},
{"$d24",0,26,27,0,},
{"$e24",0,26,32,0,},
{"$f24",0,26,37,0,},
{"$g24",0,26,42,0,},
{"$h24",0,26,47,0,},
{"$i24",0,26,52,0,},
{"$j24",0,26,57,0,},
{"$k24",0,26,62,0,},
{"$l24",0,26,67,0,},
{"$m24",0,26,72,0,},
{"$n24",0,26,77,0,},
{"INJ_T2  !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,26,0,80,},
{"$a25",0,27,12,0,},
{"$b25",0,27,17,0,},
{"$c25",0,27,22,0,},
{"$d25",0,27,27,0,},
{"$e25",0,27,32,0,},
{"$f25",0,27,37,0,},
{"$g25",0,27,42,0,},
{"$h25",0,27,47,0,},
{"$i25",0,27,52,0,},
{"$j25",0,27,57,0,},
{"$k25",0,27,62,0,},
{"$l25",0,27,67,0,},
{"$m25",0,27,72,0,},
{"$n25",0,27,77,0,},
{"INJ_T1  !    !    !    !    !    !    !    !    !    !    !    !    !    !    !",1,27,0,80,},
{0,0,0,0,0,},};


struct RawData_data {
unsigned char col0[25];
unsigned char col1[25];
unsigned char col2[25];
unsigned char col3[25];
unsigned char col4[25];
unsigned char col5[25];
unsigned char col6[25];
unsigned char col7[25];
unsigned char col8[25];
unsigned char col9[25];
unsigned char col10[25];
unsigned char col11[25];
unsigned char col12[25];
unsigned char col13[25];
} rdd;


int init_RawData() {
	memset(&rdd.col0,0,25);
	memset(&rdd.col1,0,25);
	memset(&rdd.col2,0,25);
	memset(&rdd.col3,0,25);
	memset(&rdd.col4,0,25);
	memset(&rdd.col5,0,25);
	memset(&rdd.col6,0,25);
	memset(&rdd.col7,0,25);
	memset(&rdd.col8,0,25);
	memset(&rdd.col9,0,25);
	memset(&rdd.col10,0,25);
	memset(&rdd.col11,0,25);
	memset(&rdd.col12,0,25);
	memset(&rdd.col13,0,25);
	return 0;
}


int update_RawData(int fd, char *indata) {
	memcpy(&rdd.col13,&rdd.col12,25);
	memcpy(&rdd.col12,&rdd.col11,25);
	memcpy(&rdd.col11,&rdd.col10,25);
	memcpy(&rdd.col10,&rdd.col9,25);
	memcpy(&rdd.col9,&rdd.col8,25);
	memcpy(&rdd.col8,&rdd.col7,25);
	memcpy(&rdd.col7,&rdd.col6,25);
	memcpy(&rdd.col6,&rdd.col5,25);
	memcpy(&rdd.col5,&rdd.col4,25);
	memcpy(&rdd.col4,&rdd.col3,25);
	memcpy(&rdd.col3,&rdd.col2,25);
	memcpy(&rdd.col2,&rdd.col1,25);
	memcpy(&rdd.col1,&rdd.col0,25);
	memcpy(&rdd.col0,indata,25);
	outf(&__RawData_fields[2],"0x%02x",rdd.col0[0]);
	outf(&__RawData_fields[17],"0x%02x",rdd.col0[1]);
	outf(&__RawData_fields[32],"0x%02x",rdd.col0[2]);
	outf(&__RawData_fields[47],"0x%02x",rdd.col0[3]);
	outf(&__RawData_fields[62],"0x%02x",rdd.col0[4]);
	outf(&__RawData_fields[77],"0x%02x",rdd.col0[5]);
	outf(&__RawData_fields[92],"0x%02x",rdd.col0[6]);
	outf(&__RawData_fields[107],"0x%02x",rdd.col0[7]);
	outf(&__RawData_fields[122],"0x%02x",rdd.col0[8]);
	outf(&__RawData_fields[137],"0x%02x",rdd.col0[9]);
	outf(&__RawData_fields[152],"0x%02x",rdd.col0[10]);
	outf(&__RawData_fields[167],"0x%02x",rdd.col0[11]);
	outf(&__RawData_fields[182],"0x%02x",rdd.col0[12]);
	outf(&__RawData_fields[197],"0x%02x",rdd.col0[13]);
	outf(&__RawData_fields[212],"0x%02x",rdd.col0[14]);
	outf(&__RawData_fields[227],"0x%02x",rdd.col0[15]);
	outf(&__RawData_fields[242],"0x%02x",rdd.col0[16]);
	outf(&__RawData_fields[257],"0x%02x",rdd.col0[17]);
	outf(&__RawData_fields[272],"0x%02x",rdd.col0[18]);
	outf(&__RawData_fields[287],"0x%02x",rdd.col0[19]);
	outf(&__RawData_fields[302],"0x%02x",rdd.col0[20]);
	outf(&__RawData_fields[317],"0x%02x",rdd.col0[21]);
	outf(&__RawData_fields[332],"0x%02x",rdd.col0[22]);
	outf(&__RawData_fields[347],"0x%02x",rdd.col0[23]);
	outf(&__RawData_fields[362],"0x%02x",rdd.col0[24]);
	outf(&__RawData_fields[3],"0x%02x",rdd.col1[0]);
	outf(&__RawData_fields[18],"0x%02x",rdd.col1[1]);
	outf(&__RawData_fields[33],"0x%02x",rdd.col1[2]);
	outf(&__RawData_fields[48],"0x%02x",rdd.col1[3]);
	outf(&__RawData_fields[63],"0x%02x",rdd.col1[4]);
	outf(&__RawData_fields[78],"0x%02x",rdd.col1[5]);
	outf(&__RawData_fields[93],"0x%02x",rdd.col1[6]);
	outf(&__RawData_fields[108],"0x%02x",rdd.col1[7]);
	outf(&__RawData_fields[123],"0x%02x",rdd.col1[8]);
	outf(&__RawData_fields[138],"0x%02x",rdd.col1[9]);
	outf(&__RawData_fields[153],"0x%02x",rdd.col1[10]);
	outf(&__RawData_fields[168],"0x%02x",rdd.col1[11]);
	outf(&__RawData_fields[183],"0x%02x",rdd.col1[12]);
	outf(&__RawData_fields[198],"0x%02x",rdd.col1[13]);
	outf(&__RawData_fields[213],"0x%02x",rdd.col1[14]);
	outf(&__RawData_fields[228],"0x%02x",rdd.col1[15]);
	outf(&__RawData_fields[243],"0x%02x",rdd.col1[16]);
	outf(&__RawData_fields[258],"0x%02x",rdd.col1[17]);
	outf(&__RawData_fields[273],"0x%02x",rdd.col1[18]);
	outf(&__RawData_fields[288],"0x%02x",rdd.col1[19]);
	outf(&__RawData_fields[303],"0x%02x",rdd.col1[20]);
	outf(&__RawData_fields[318],"0x%02x",rdd.col1[21]);
	outf(&__RawData_fields[333],"0x%02x",rdd.col1[22]);
	outf(&__RawData_fields[348],"0x%02x",rdd.col1[23]);
	outf(&__RawData_fields[363],"0x%02x",rdd.col1[24]);
	outf(&__RawData_fields[4],"0x%02x",rdd.col2[0]);
	outf(&__RawData_fields[19],"0x%02x",rdd.col2[1]);
	outf(&__RawData_fields[34],"0x%02x",rdd.col2[2]);
	outf(&__RawData_fields[49],"0x%02x",rdd.col2[3]);
	outf(&__RawData_fields[64],"0x%02x",rdd.col2[4]);
	outf(&__RawData_fields[79],"0x%02x",rdd.col2[5]);
	outf(&__RawData_fields[94],"0x%02x",rdd.col2[6]);
	outf(&__RawData_fields[109],"0x%02x",rdd.col2[7]);
	outf(&__RawData_fields[124],"0x%02x",rdd.col2[8]);
	outf(&__RawData_fields[139],"0x%02x",rdd.col2[9]);
	outf(&__RawData_fields[154],"0x%02x",rdd.col2[10]);
	outf(&__RawData_fields[169],"0x%02x",rdd.col2[11]);
	outf(&__RawData_fields[184],"0x%02x",rdd.col2[12]);
	outf(&__RawData_fields[199],"0x%02x",rdd.col2[13]);
	outf(&__RawData_fields[214],"0x%02x",rdd.col2[14]);
	outf(&__RawData_fields[229],"0x%02x",rdd.col2[15]);
	outf(&__RawData_fields[244],"0x%02x",rdd.col2[16]);
	outf(&__RawData_fields[259],"0x%02x",rdd.col2[17]);
	outf(&__RawData_fields[274],"0x%02x",rdd.col2[18]);
	outf(&__RawData_fields[289],"0x%02x",rdd.col2[19]);
	outf(&__RawData_fields[304],"0x%02x",rdd.col2[20]);
	outf(&__RawData_fields[319],"0x%02x",rdd.col2[21]);
	outf(&__RawData_fields[334],"0x%02x",rdd.col2[22]);
	outf(&__RawData_fields[349],"0x%02x",rdd.col2[23]);
	outf(&__RawData_fields[364],"0x%02x",rdd.col2[24]);
	outf(&__RawData_fields[5],"0x%02x",rdd.col3[0]);
	outf(&__RawData_fields[20],"0x%02x",rdd.col3[1]);
	outf(&__RawData_fields[35],"0x%02x",rdd.col3[2]);
	outf(&__RawData_fields[50],"0x%02x",rdd.col3[3]);
	outf(&__RawData_fields[65],"0x%02x",rdd.col3[4]);
	outf(&__RawData_fields[80],"0x%02x",rdd.col3[5]);
	outf(&__RawData_fields[95],"0x%02x",rdd.col3[6]);
	outf(&__RawData_fields[110],"0x%02x",rdd.col3[7]);
	outf(&__RawData_fields[125],"0x%02x",rdd.col3[8]);
	outf(&__RawData_fields[140],"0x%02x",rdd.col3[9]);
	outf(&__RawData_fields[155],"0x%02x",rdd.col3[10]);
	outf(&__RawData_fields[170],"0x%02x",rdd.col3[11]);
	outf(&__RawData_fields[185],"0x%02x",rdd.col3[12]);
	outf(&__RawData_fields[200],"0x%02x",rdd.col3[13]);
	outf(&__RawData_fields[215],"0x%02x",rdd.col3[14]);
	outf(&__RawData_fields[230],"0x%02x",rdd.col3[15]);
	outf(&__RawData_fields[245],"0x%02x",rdd.col3[16]);
	outf(&__RawData_fields[260],"0x%02x",rdd.col3[17]);
	outf(&__RawData_fields[275],"0x%02x",rdd.col3[18]);
	outf(&__RawData_fields[290],"0x%02x",rdd.col3[19]);
	outf(&__RawData_fields[305],"0x%02x",rdd.col3[20]);
	outf(&__RawData_fields[320],"0x%02x",rdd.col3[21]);
	outf(&__RawData_fields[335],"0x%02x",rdd.col3[22]);
	outf(&__RawData_fields[350],"0x%02x",rdd.col3[23]);
	outf(&__RawData_fields[365],"0x%02x",rdd.col3[24]);
	outf(&__RawData_fields[6],"0x%02x",rdd.col4[0]);
	outf(&__RawData_fields[21],"0x%02x",rdd.col4[1]);
	outf(&__RawData_fields[36],"0x%02x",rdd.col4[2]);
	outf(&__RawData_fields[51],"0x%02x",rdd.col4[3]);
	outf(&__RawData_fields[66],"0x%02x",rdd.col4[4]);
	outf(&__RawData_fields[81],"0x%02x",rdd.col4[5]);
	outf(&__RawData_fields[96],"0x%02x",rdd.col4[6]);
	outf(&__RawData_fields[111],"0x%02x",rdd.col4[7]);
	outf(&__RawData_fields[126],"0x%02x",rdd.col4[8]);
	outf(&__RawData_fields[141],"0x%02x",rdd.col4[9]);
	outf(&__RawData_fields[156],"0x%02x",rdd.col4[10]);
	outf(&__RawData_fields[171],"0x%02x",rdd.col4[11]);
	outf(&__RawData_fields[186],"0x%02x",rdd.col4[12]);
	outf(&__RawData_fields[201],"0x%02x",rdd.col4[13]);
	outf(&__RawData_fields[216],"0x%02x",rdd.col4[14]);
	outf(&__RawData_fields[231],"0x%02x",rdd.col4[15]);
	outf(&__RawData_fields[246],"0x%02x",rdd.col4[16]);
	outf(&__RawData_fields[261],"0x%02x",rdd.col4[17]);
	outf(&__RawData_fields[276],"0x%02x",rdd.col4[18]);
	outf(&__RawData_fields[291],"0x%02x",rdd.col4[19]);
	outf(&__RawData_fields[306],"0x%02x",rdd.col4[20]);
	outf(&__RawData_fields[321],"0x%02x",rdd.col4[21]);
	outf(&__RawData_fields[336],"0x%02x",rdd.col4[22]);
	outf(&__RawData_fields[351],"0x%02x",rdd.col4[23]);
	outf(&__RawData_fields[366],"0x%02x",rdd.col4[24]);
	outf(&__RawData_fields[7],"0x%02x",rdd.col5[0]);
	outf(&__RawData_fields[22],"0x%02x",rdd.col5[1]);
	outf(&__RawData_fields[37],"0x%02x",rdd.col5[2]);
	outf(&__RawData_fields[52],"0x%02x",rdd.col5[3]);
	outf(&__RawData_fields[67],"0x%02x",rdd.col5[4]);
	outf(&__RawData_fields[82],"0x%02x",rdd.col5[5]);
	outf(&__RawData_fields[97],"0x%02x",rdd.col5[6]);
	outf(&__RawData_fields[112],"0x%02x",rdd.col5[7]);
	outf(&__RawData_fields[127],"0x%02x",rdd.col5[8]);
	outf(&__RawData_fields[142],"0x%02x",rdd.col5[9]);
	outf(&__RawData_fields[157],"0x%02x",rdd.col5[10]);
	outf(&__RawData_fields[172],"0x%02x",rdd.col5[11]);
	outf(&__RawData_fields[187],"0x%02x",rdd.col5[12]);
	outf(&__RawData_fields[202],"0x%02x",rdd.col5[13]);
	outf(&__RawData_fields[217],"0x%02x",rdd.col5[14]);
	outf(&__RawData_fields[232],"0x%02x",rdd.col5[15]);
	outf(&__RawData_fields[247],"0x%02x",rdd.col5[16]);
	outf(&__RawData_fields[262],"0x%02x",rdd.col5[17]);
	outf(&__RawData_fields[277],"0x%02x",rdd.col5[18]);
	outf(&__RawData_fields[292],"0x%02x",rdd.col5[19]);
	outf(&__RawData_fields[307],"0x%02x",rdd.col5[20]);
	outf(&__RawData_fields[322],"0x%02x",rdd.col5[21]);
	outf(&__RawData_fields[337],"0x%02x",rdd.col5[22]);
	outf(&__RawData_fields[352],"0x%02x",rdd.col5[23]);
	outf(&__RawData_fields[367],"0x%02x",rdd.col5[24]);
	outf(&__RawData_fields[8],"0x%02x",rdd.col6[0]);
	outf(&__RawData_fields[23],"0x%02x",rdd.col6[1]);
	outf(&__RawData_fields[38],"0x%02x",rdd.col6[2]);
	outf(&__RawData_fields[53],"0x%02x",rdd.col6[3]);
	outf(&__RawData_fields[68],"0x%02x",rdd.col6[4]);
	outf(&__RawData_fields[83],"0x%02x",rdd.col6[5]);
	outf(&__RawData_fields[98],"0x%02x",rdd.col6[6]);
	outf(&__RawData_fields[113],"0x%02x",rdd.col6[7]);
	outf(&__RawData_fields[128],"0x%02x",rdd.col6[8]);
	outf(&__RawData_fields[143],"0x%02x",rdd.col6[9]);
	outf(&__RawData_fields[158],"0x%02x",rdd.col6[10]);
	outf(&__RawData_fields[173],"0x%02x",rdd.col6[11]);
	outf(&__RawData_fields[188],"0x%02x",rdd.col6[12]);
	outf(&__RawData_fields[203],"0x%02x",rdd.col6[13]);
	outf(&__RawData_fields[218],"0x%02x",rdd.col6[14]);
	outf(&__RawData_fields[233],"0x%02x",rdd.col6[15]);
	outf(&__RawData_fields[248],"0x%02x",rdd.col6[16]);
	outf(&__RawData_fields[263],"0x%02x",rdd.col6[17]);
	outf(&__RawData_fields[278],"0x%02x",rdd.col6[18]);
	outf(&__RawData_fields[293],"0x%02x",rdd.col6[19]);
	outf(&__RawData_fields[308],"0x%02x",rdd.col6[20]);
	outf(&__RawData_fields[323],"0x%02x",rdd.col6[21]);
	outf(&__RawData_fields[338],"0x%02x",rdd.col6[22]);
	outf(&__RawData_fields[353],"0x%02x",rdd.col6[23]);
	outf(&__RawData_fields[368],"0x%02x",rdd.col6[24]);
	outf(&__RawData_fields[9],"0x%02x",rdd.col7[0]);
	outf(&__RawData_fields[24],"0x%02x",rdd.col7[1]);
	outf(&__RawData_fields[39],"0x%02x",rdd.col7[2]);
	outf(&__RawData_fields[54],"0x%02x",rdd.col7[3]);
	outf(&__RawData_fields[69],"0x%02x",rdd.col7[4]);
	outf(&__RawData_fields[84],"0x%02x",rdd.col7[5]);
	outf(&__RawData_fields[99],"0x%02x",rdd.col7[6]);
	outf(&__RawData_fields[114],"0x%02x",rdd.col7[7]);
	outf(&__RawData_fields[129],"0x%02x",rdd.col7[8]);
	outf(&__RawData_fields[144],"0x%02x",rdd.col7[9]);
	outf(&__RawData_fields[159],"0x%02x",rdd.col7[10]);
	outf(&__RawData_fields[174],"0x%02x",rdd.col7[11]);
	outf(&__RawData_fields[189],"0x%02x",rdd.col7[12]);
	outf(&__RawData_fields[204],"0x%02x",rdd.col7[13]);
	outf(&__RawData_fields[219],"0x%02x",rdd.col7[14]);
	outf(&__RawData_fields[234],"0x%02x",rdd.col7[15]);
	outf(&__RawData_fields[249],"0x%02x",rdd.col7[16]);
	outf(&__RawData_fields[264],"0x%02x",rdd.col7[17]);
	outf(&__RawData_fields[279],"0x%02x",rdd.col7[18]);
	outf(&__RawData_fields[294],"0x%02x",rdd.col7[19]);
	outf(&__RawData_fields[309],"0x%02x",rdd.col7[20]);
	outf(&__RawData_fields[324],"0x%02x",rdd.col7[21]);
	outf(&__RawData_fields[339],"0x%02x",rdd.col7[22]);
	outf(&__RawData_fields[354],"0x%02x",rdd.col7[23]);
	outf(&__RawData_fields[369],"0x%02x",rdd.col7[24]);
	outf(&__RawData_fields[10],"0x%02x",rdd.col8[0]);
	outf(&__RawData_fields[25],"0x%02x",rdd.col8[1]);
	outf(&__RawData_fields[40],"0x%02x",rdd.col8[2]);
	outf(&__RawData_fields[55],"0x%02x",rdd.col8[3]);
	outf(&__RawData_fields[70],"0x%02x",rdd.col8[4]);
	outf(&__RawData_fields[85],"0x%02x",rdd.col8[5]);
	outf(&__RawData_fields[100],"0x%02x",rdd.col8[6]);
	outf(&__RawData_fields[115],"0x%02x",rdd.col8[7]);
	outf(&__RawData_fields[130],"0x%02x",rdd.col8[8]);
	outf(&__RawData_fields[145],"0x%02x",rdd.col8[9]);
	outf(&__RawData_fields[160],"0x%02x",rdd.col8[10]);
	outf(&__RawData_fields[175],"0x%02x",rdd.col8[11]);
	outf(&__RawData_fields[190],"0x%02x",rdd.col8[12]);
	outf(&__RawData_fields[205],"0x%02x",rdd.col8[13]);
	outf(&__RawData_fields[220],"0x%02x",rdd.col8[14]);
	outf(&__RawData_fields[235],"0x%02x",rdd.col8[15]);
	outf(&__RawData_fields[250],"0x%02x",rdd.col8[16]);
	outf(&__RawData_fields[265],"0x%02x",rdd.col8[17]);
	outf(&__RawData_fields[280],"0x%02x",rdd.col8[18]);
	outf(&__RawData_fields[295],"0x%02x",rdd.col8[19]);
	outf(&__RawData_fields[310],"0x%02x",rdd.col8[20]);
	outf(&__RawData_fields[325],"0x%02x",rdd.col8[21]);
	outf(&__RawData_fields[340],"0x%02x",rdd.col8[22]);
	outf(&__RawData_fields[355],"0x%02x",rdd.col8[23]);
	outf(&__RawData_fields[370],"0x%02x",rdd.col8[24]);
	outf(&__RawData_fields[11],"0x%02x",rdd.col9[0]);
	outf(&__RawData_fields[26],"0x%02x",rdd.col9[1]);
	outf(&__RawData_fields[41],"0x%02x",rdd.col9[2]);
	outf(&__RawData_fields[56],"0x%02x",rdd.col9[3]);
	outf(&__RawData_fields[71],"0x%02x",rdd.col9[4]);
	outf(&__RawData_fields[86],"0x%02x",rdd.col9[5]);
	outf(&__RawData_fields[101],"0x%02x",rdd.col9[6]);
	outf(&__RawData_fields[116],"0x%02x",rdd.col9[7]);
	outf(&__RawData_fields[131],"0x%02x",rdd.col9[8]);
	outf(&__RawData_fields[146],"0x%02x",rdd.col9[9]);
	outf(&__RawData_fields[161],"0x%02x",rdd.col9[10]);
	outf(&__RawData_fields[176],"0x%02x",rdd.col9[11]);
	outf(&__RawData_fields[191],"0x%02x",rdd.col9[12]);
	outf(&__RawData_fields[206],"0x%02x",rdd.col9[13]);
	outf(&__RawData_fields[221],"0x%02x",rdd.col9[14]);
	outf(&__RawData_fields[236],"0x%02x",rdd.col9[15]);
	outf(&__RawData_fields[251],"0x%02x",rdd.col9[16]);
	outf(&__RawData_fields[266],"0x%02x",rdd.col9[17]);
	outf(&__RawData_fields[281],"0x%02x",rdd.col9[18]);
	outf(&__RawData_fields[296],"0x%02x",rdd.col9[19]);
	outf(&__RawData_fields[311],"0x%02x",rdd.col9[20]);
	outf(&__RawData_fields[326],"0x%02x",rdd.col9[21]);
	outf(&__RawData_fields[341],"0x%02x",rdd.col9[22]);
	outf(&__RawData_fields[356],"0x%02x",rdd.col9[23]);
	outf(&__RawData_fields[371],"0x%02x",rdd.col9[24]);
	outf(&__RawData_fields[12],"0x%02x",rdd.col10[0]);
	outf(&__RawData_fields[27],"0x%02x",rdd.col10[1]);
	outf(&__RawData_fields[42],"0x%02x",rdd.col10[2]);
	outf(&__RawData_fields[57],"0x%02x",rdd.col10[3]);
	outf(&__RawData_fields[72],"0x%02x",rdd.col10[4]);
	outf(&__RawData_fields[87],"0x%02x",rdd.col10[5]);
	outf(&__RawData_fields[102],"0x%02x",rdd.col10[6]);
	outf(&__RawData_fields[117],"0x%02x",rdd.col10[7]);
	outf(&__RawData_fields[132],"0x%02x",rdd.col10[8]);
	outf(&__RawData_fields[147],"0x%02x",rdd.col10[9]);
	outf(&__RawData_fields[162],"0x%02x",rdd.col10[10]);
	outf(&__RawData_fields[177],"0x%02x",rdd.col10[11]);
	outf(&__RawData_fields[192],"0x%02x",rdd.col10[12]);
	outf(&__RawData_fields[207],"0x%02x",rdd.col10[13]);
	outf(&__RawData_fields[222],"0x%02x",rdd.col10[14]);
	outf(&__RawData_fields[237],"0x%02x",rdd.col10[15]);
	outf(&__RawData_fields[252],"0x%02x",rdd.col10[16]);
	outf(&__RawData_fields[267],"0x%02x",rdd.col10[17]);
	outf(&__RawData_fields[282],"0x%02x",rdd.col10[18]);
	outf(&__RawData_fields[297],"0x%02x",rdd.col10[19]);
	outf(&__RawData_fields[312],"0x%02x",rdd.col10[20]);
	outf(&__RawData_fields[327],"0x%02x",rdd.col10[21]);
	outf(&__RawData_fields[342],"0x%02x",rdd.col10[22]);
	outf(&__RawData_fields[357],"0x%02x",rdd.col10[23]);
	outf(&__RawData_fields[372],"0x%02x",rdd.col10[24]);
	outf(&__RawData_fields[13],"0x%02x",rdd.col11[0]);
	outf(&__RawData_fields[28],"0x%02x",rdd.col11[1]);
	outf(&__RawData_fields[43],"0x%02x",rdd.col11[2]);
	outf(&__RawData_fields[58],"0x%02x",rdd.col11[3]);
	outf(&__RawData_fields[73],"0x%02x",rdd.col11[4]);
	outf(&__RawData_fields[88],"0x%02x",rdd.col11[5]);
	outf(&__RawData_fields[103],"0x%02x",rdd.col11[6]);
	outf(&__RawData_fields[118],"0x%02x",rdd.col11[7]);
	outf(&__RawData_fields[133],"0x%02x",rdd.col11[8]);
	outf(&__RawData_fields[148],"0x%02x",rdd.col11[9]);
	outf(&__RawData_fields[163],"0x%02x",rdd.col11[10]);
	outf(&__RawData_fields[178],"0x%02x",rdd.col11[11]);
	outf(&__RawData_fields[193],"0x%02x",rdd.col11[12]);
	outf(&__RawData_fields[208],"0x%02x",rdd.col11[13]);
	outf(&__RawData_fields[223],"0x%02x",rdd.col11[14]);
	outf(&__RawData_fields[238],"0x%02x",rdd.col11[15]);
	outf(&__RawData_fields[253],"0x%02x",rdd.col11[16]);
	outf(&__RawData_fields[268],"0x%02x",rdd.col11[17]);
	outf(&__RawData_fields[283],"0x%02x",rdd.col11[18]);
	outf(&__RawData_fields[298],"0x%02x",rdd.col11[19]);
	outf(&__RawData_fields[313],"0x%02x",rdd.col11[20]);
	outf(&__RawData_fields[328],"0x%02x",rdd.col11[21]);
	outf(&__RawData_fields[343],"0x%02x",rdd.col11[22]);
	outf(&__RawData_fields[358],"0x%02x",rdd.col11[23]);
	outf(&__RawData_fields[373],"0x%02x",rdd.col11[24]);
	outf(&__RawData_fields[14],"0x%02x",rdd.col12[0]);
	outf(&__RawData_fields[29],"0x%02x",rdd.col12[1]);
	outf(&__RawData_fields[44],"0x%02x",rdd.col12[2]);
	outf(&__RawData_fields[59],"0x%02x",rdd.col12[3]);
	outf(&__RawData_fields[74],"0x%02x",rdd.col12[4]);
	outf(&__RawData_fields[89],"0x%02x",rdd.col12[5]);
	outf(&__RawData_fields[104],"0x%02x",rdd.col12[6]);
	outf(&__RawData_fields[119],"0x%02x",rdd.col12[7]);
	outf(&__RawData_fields[134],"0x%02x",rdd.col12[8]);
	outf(&__RawData_fields[149],"0x%02x",rdd.col12[9]);
	outf(&__RawData_fields[164],"0x%02x",rdd.col12[10]);
	outf(&__RawData_fields[179],"0x%02x",rdd.col12[11]);
	outf(&__RawData_fields[194],"0x%02x",rdd.col12[12]);
	outf(&__RawData_fields[209],"0x%02x",rdd.col12[13]);
	outf(&__RawData_fields[224],"0x%02x",rdd.col12[14]);
	outf(&__RawData_fields[239],"0x%02x",rdd.col12[15]);
	outf(&__RawData_fields[254],"0x%02x",rdd.col12[16]);
	outf(&__RawData_fields[269],"0x%02x",rdd.col12[17]);
	outf(&__RawData_fields[284],"0x%02x",rdd.col12[18]);
	outf(&__RawData_fields[299],"0x%02x",rdd.col12[19]);
	outf(&__RawData_fields[314],"0x%02x",rdd.col12[20]);
	outf(&__RawData_fields[329],"0x%02x",rdd.col12[21]);
	outf(&__RawData_fields[344],"0x%02x",rdd.col12[22]);
	outf(&__RawData_fields[359],"0x%02x",rdd.col12[23]);
	outf(&__RawData_fields[374],"0x%02x",rdd.col12[24]);
	outf(&__RawData_fields[15],"0x%02x",rdd.col13[0]);
	outf(&__RawData_fields[30],"0x%02x",rdd.col13[1]);
	outf(&__RawData_fields[45],"0x%02x",rdd.col13[2]);
	outf(&__RawData_fields[60],"0x%02x",rdd.col13[3]);
	outf(&__RawData_fields[75],"0x%02x",rdd.col13[4]);
	outf(&__RawData_fields[90],"0x%02x",rdd.col13[5]);
	outf(&__RawData_fields[105],"0x%02x",rdd.col13[6]);
	outf(&__RawData_fields[120],"0x%02x",rdd.col13[7]);
	outf(&__RawData_fields[135],"0x%02x",rdd.col13[8]);
	outf(&__RawData_fields[150],"0x%02x",rdd.col13[9]);
	outf(&__RawData_fields[165],"0x%02x",rdd.col13[10]);
	outf(&__RawData_fields[180],"0x%02x",rdd.col13[11]);
	outf(&__RawData_fields[195],"0x%02x",rdd.col13[12]);
	outf(&__RawData_fields[210],"0x%02x",rdd.col13[13]);
	outf(&__RawData_fields[225],"0x%02x",rdd.col13[14]);
	outf(&__RawData_fields[240],"0x%02x",rdd.col13[15]);
	outf(&__RawData_fields[255],"0x%02x",rdd.col13[16]);
	outf(&__RawData_fields[270],"0x%02x",rdd.col13[17]);
	outf(&__RawData_fields[285],"0x%02x",rdd.col13[18]);
	outf(&__RawData_fields[300],"0x%02x",rdd.col13[19]);
	outf(&__RawData_fields[315],"0x%02x",rdd.col13[20]);
	outf(&__RawData_fields[330],"0x%02x",rdd.col13[21]);
	outf(&__RawData_fields[345],"0x%02x",rdd.col13[22]);
	outf(&__RawData_fields[360],"0x%02x",rdd.col13[23]);
	outf(&__RawData_fields[375],"0x%02x",rdd.col13[24]);
	return 0;
}
struct Panel __RawData={"RawData", __RawData_fields};
struct Panel *RawData=&__RawData;




struct Field __FlagData_fields[]={
{"================================= Flag Data ====================================",1,1,0,81,},
{"============ MW2 ================",1,2,0,34,},
{"$01",0,3,3,0,},
{"    Road speed pulse",1,3,0,21,},
{"$02",0,4,3,0,},
{"    Not Used",1,4,0,13,},
{"$03",0,5,3,0,},
{"    Ref Pulse occured",1,5,0,22,},
{"$04",0,6,3,0,},
{"    ALDL Factory Test (3.9K)",1,6,0,29,},
{"$05",0,7,3,0,},
{"    ALDL Diag mode    (0 ohms)",1,7,0,31,},
{"$06",0,8,3,0,},
{"    ALDL ALDL Mode    (10K)",1,8,0,28,},
{"$07",0,9,3,0,},
{"    High battery voltage",1,9,0,25,},
{"$08",0,10,3,0,},
{"    Air meter pulse",1,10,0,20,},
{"",1,11,0,1,},
{"============== MWAF1 ============         ============ MCUISW =================",1,12,0,80,},
{"$09",0,13,3,0,},
{"$17",0,13,45,0,},
{"    Not Used                                  P/N Flag",1,13,0,55,},
{"$10",0,14,3,0,},
{"$18",0,14,45,0,},
{"    Learn control enable                      Not in third gear",1,14,0,64,},
{"$11",0,15,3,0,},
{"$19",0,15,45,0,},
{"    Not Used                                  Overdrive req",1,15,0,60,},
{"$12",0,16,3,0,},
{"$20",0,16,45,0,},
{"    Not Used                                  Not Used",1,16,0,55,},
{"$13",0,17,3,0,},
{"$21",0,17,45,0,},
{"    VSS Failure                               EGR Diag switch closed",1,17,0,69,},
{"$14",0,18,3,0,},
{"$22",0,18,45,0,},
{"    EECC Slow O2                              TCC locked",1,18,0,57,},
{"$15",0,19,3,0,},
{"$23",0,19,45,0,},
{"    Rich-RIch                                 FAN Request",1,19,0,58,},
{"$16",0,20,3,0,},
{"$24",0,20,45,0,},
{"    Closed loop                               A/C Request",1,20,0,58,},
{0,0,0,0,0,},};


struct ECM_Regs {
unsigned char MW2;
unsigned char PROMIDA;
unsigned char PROMIDB;
unsigned char ISSPMP;
unsigned char COOLDEGA;
unsigned char FILTMPH;
unsigned char EGRDC;
unsigned char NTRPMX;
unsigned char ADTHROT;
unsigned char CORRCL;
unsigned char ADO2AF;
unsigned char MALFFLG1;
unsigned char MALFFLG2;
unsigned char MALFFLG3;
unsigned char MALFFLG4;
unsigned char MWFA;
unsigned char MAT;
unsigned char MCUINST;
unsigned char LV8;
unsigned char BLM;
unsigned char ALDLCNTR;
unsigned char DISPFLOW;
unsigned char DISPFLOW1;
unsigned char OBPINJ;
unsigned char OBPINJ1;
};


int init_FlagData() {
	return 0;
}


int update_FlagData(int fd, struct ECM_Regs *eregs) {
	outb(&__FlagData_fields[2],"+",eregs->MW2&0x01);
	outb(&__FlagData_fields[4],"+",eregs->MW2&0x02);
	outb(&__FlagData_fields[6],"+",eregs->MW2&0x04);
	outb(&__FlagData_fields[8],"+",eregs->MW2&0x08);
	outb(&__FlagData_fields[10],"+",eregs->MW2&0x10);
	outb(&__FlagData_fields[12],"+",eregs->MW2&0x20);
	outb(&__FlagData_fields[14],"+",eregs->MW2&0x40);
	outb(&__FlagData_fields[16],"+",eregs->MW2&0x80);
	outb(&__FlagData_fields[20],"+",eregs->MWFA&0x01);
	outb(&__FlagData_fields[23],"+",eregs->MWFA&0x02);
	outb(&__FlagData_fields[26],"+",eregs->MWFA&0x04);
	outb(&__FlagData_fields[29],"+",eregs->MWFA&0x08);
	outb(&__FlagData_fields[32],"+",eregs->MWFA&0x10);
	outb(&__FlagData_fields[35],"+",eregs->MWFA&0x20);
	outb(&__FlagData_fields[38],"+",eregs->MWFA&0x40);
	outb(&__FlagData_fields[41],"+",eregs->MWFA&0x80);
	outb(&__FlagData_fields[21],"+",eregs->MCUINST&0x01);
	outb(&__FlagData_fields[24],"+",eregs->MCUINST&0x02);
	outb(&__FlagData_fields[27],"+",eregs->MCUINST&0x04);
	outb(&__FlagData_fields[30],"+",eregs->MCUINST&0x08);
	outb(&__FlagData_fields[33],"+",eregs->MCUINST&0x10);
	outb(&__FlagData_fields[36],"+",eregs->MCUINST&0x20);
	outb(&__FlagData_fields[39],"+",eregs->MCUINST&0x40);
	outb(&__FlagData_fields[42],"+",!eregs->MCUINST&0x80);
	return 0;
}
struct Panel __FlagData={"FlagData", __FlagData_fields};
struct Panel *FlagData=&__FlagData;




struct Field __SensorData_fields[]={
{"================================= Sensor Data =========================",1,1,0,72,},
{"                    ! RAW   !  Converted ! Unit   !  Converted ! Unit !",1,2,0,72,},
{"$01",0,3,25,0,},
{"$02",0,3,39,0,},
{" IAC                !       !            !  #     !            !      !",1,3,0,72,},
{"$03",0,4,25,0,},
{"$04",0,4,39,0,},
{"$05",0,4,61,0,},
{" Coolant Temp       !       !            !  F     !            !  C   !",1,4,0,72,},
{"$06",0,5,25,0,},
{"$07",0,5,39,0,},
{"$08",0,5,61,0,},
{" MPH                !       !            ! MPH    !            ! KMH  !",1,5,0,72,},
{"$09",0,6,25,0,},
{"$10",0,6,39,0,},
{" RPM                !       !            ! RPM    !            !      !",1,6,0,72,},
{"$11",0,7,25,0,},
{"$12",0,7,39,0,},
{"$13",0,7,61,0,},
{" TPS                !       !            ! Volt   !            ! %    !",1,7,0,72,},
{"$14",0,8,25,0,},
{"$15",0,8,39,0,},
{" INT                !       !            !  #     !            !      !",1,8,0,72,},
{"$16",0,9,25,0,},
{"$17",0,9,39,0,},
{" O2                 !       !            ! mVolt  !            !      !",1,9,0,72,},
{"$18",0,10,25,0,},
{"$19",0,10,39,0,},
{"$20",0,10,61,0,},
{" MAT                !       !            !  F     !            !  C   !",1,10,0,72,},
{"$21",0,11,25,0,},
{"$22",0,11,39,0,},
{" BLM                !       !            !  #     !            !      !",1,11,0,72,},
{"$23",0,12,25,0,},
{"$24",0,12,39,0,},
{" Rich/Lean cnt      !       !            !  #     !            !      !",1,12,0,72,},
{"$25",0,13,25,0,},
{"$26",0,13,39,0,},
{" MAF                !       !            !  g/sec !            !      !",1,13,0,72,},
{"=======================================================================",1,14,0,72,},
{0,0,0,0,0,},};


char *xtoS(int val, int scale, char *buf, int bsize) {
char neg=' ';
if (val<0) {
neg='-';
val=-val;
}
sprintf(buf,"%c%d.%02d",neg,val/scale,val%scale);
return buf; 
}


int init_SensorData() {
	return 0;
}


int update_SensorData(int fd, struct ECM_Regs *eregs) {
	char buf1[16];
	outf(&__SensorData_fields[2],"0x%02x",eregs->ISSPMP);
	outf(&__SensorData_fields[3],"%3d",eregs->ISSPMP);
	outf(&__SensorData_fields[5],"0x%02x",eregs->COOLDEGA);
	outf(&__SensorData_fields[6],"%6s", xtoS((eregs->COOLDEGA*135)-4000, 100, buf1,sizeof(buf1)));
	outf(&__SensorData_fields[7],"%6s", xtoS((eregs->COOLDEGA*75)-4000,100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[9],"0x%02x",eregs->FILTMPH);
	outf(&__SensorData_fields[10],"%3d",eregs->FILTMPH);
	outf(&__SensorData_fields[11],"%4s",xtoS(eregs->FILTMPH*161,100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[13],"0x%02x",eregs->NTRPMX);
	outf(&__SensorData_fields[14],"%3d",eregs->NTRPMX*25);
	outf(&__SensorData_fields[16],"0x%02x",eregs->ADTHROT);
	outf(&__SensorData_fields[17],"%6s", xtoS((eregs->ADTHROT*100)/51,100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[18],"%6s", xtoS((eregs->ADTHROT*39),100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[20],"0x%02x",eregs->CORRCL);
	outf(&__SensorData_fields[21],"%3d",eregs->CORRCL);
	outf(&__SensorData_fields[23],"0x%02x",eregs->ADO2AF);
	outf(&__SensorData_fields[24],"%6s",xtoS((eregs->ADO2AF*444),100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[26],"0x%02x",eregs->MAT);
	outf(&__SensorData_fields[27],"%6s",xtoS((eregs->MAT*135)-4000,100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[28],"%6s",xtoS((eregs->MAT*75)-4000,100,buf1,sizeof(buf1)));
	outf(&__SensorData_fields[30],"0x%02x",eregs->BLM);
	outf(&__SensorData_fields[31],"%3d",eregs->BLM);
	outf(&__SensorData_fields[33],"0x%02x",eregs->ALDLCNTR);
	outf(&__SensorData_fields[34],"%3d",eregs->ALDLCNTR);
	outf(&__SensorData_fields[36],"0x%02x",eregs->DISPFLOW);
	outf(&__SensorData_fields[37],"%4d",(eregs->DISPFLOW*256)+eregs->DISPFLOW1);
	return 0;
}
struct Panel __SensorData={"SensorData", __SensorData_fields};
struct Panel *SensorData=&__SensorData;




struct Field __ErrorData_fields[]={
{"================================= Error Codes  ====================================",1,1,0,84,},
{"============ MALFFLG1 ==============   ============ MALFFLG3 ==============",1,2,0,76,},
{"$01",0,3,3,0,},
{"$17",0,3,41,0,},
{"     23 MAT sensor LO                      51 Prom err",1,3,0,55,},
{"$02",0,4,3,0,},
{"$18",0,4,41,0,},
{"     22 TPS LO                             46 Not Used",1,4,0,55,},
{"$03",0,5,3,0,},
{"$19",0,5,41,0,},
{"     21 TPS HI                             45 O2 Sensor Rich",1,5,0,61,},
{"$04",0,6,3,0,},
{"$20",0,6,41,0,},
{"     16 Not Used                           44 02 Sensor Lean",1,6,0,61,},
{"$05",0,7,3,0,},
{"$21",0,7,41,0,},
{"     15 Coolant sens temp HI               43 ESC Fail",1,7,0,55,},
{"$06",0,8,3,0,},
{"$22",0,8,41,0,},
{"     14 Coolant sens temp LO               42 EST Mon err",1,8,0,58,},
{"$07",0,9,3,0,},
{"$23",0,9,41,0,},
{"     13 O2 sensor                          41 Cyl Sel err",1,9,0,58,},
{"$08",0,10,3,0,},
{"$24",0,10,41,0,},
{"     12 No ref pulses                      36 Burn off diag",1,10,0,60,},
{"",1,11,0,1,},
{"============== MALFFLG2 ============   ============ MALFFLG4  =============",1,12,0,76,},
{"$09",0,13,3,0,},
{"$25",0,13,41,0,},
{"     35 Not Used                           63 Not Used",1,13,0,55,},
{"$10",0,14,3,0,},
{"$26",0,14,41,0,},
{"     34 MAF sens LO                        62 Not Used",1,14,0,55,},
{"$11",0,15,3,0,},
{"$27",0,15,41,0,},
{"     33 MAF sens HI                        61 Not Used",1,15,0,55,},
{"$12",0,16,3,0,},
{"$28",0,16,41,0,},
{"     32 EGR Diag                           55 Not Used",1,16,0,55,},
{"$13",0,17,3,0,},
{"$29",0,17,41,0,},
{"     31 Not Used                           54 ADU Err",1,17,0,54,},
{"$14",0,18,3,0,},
{"$30",0,18,41,0,},
{"     26 Not Used                           53 Fuel Pump volts",1,18,0,62,},
{"$15",0,19,3,0,},
{"$31",0,19,41,0,},
{"     25 MAT Sens HI                        52 Over voltage",1,19,0,59,},
{"$16",0,20,3,0,},
{"$32",0,20,41,0,},
{"     24 VSS                                51 Cal pack missing",1,20,0,63,},
{0,0,0,0,0,},};




int init_ErrorData() {
	return 0;
}


int update_ErrorData(int fd, struct ECM_Regs *eregs) {
	outb(&__ErrorData_fields[2],"+",eregs->MALFFLG1&0x01);
	outb(&__ErrorData_fields[5],"+",eregs->MALFFLG1&0x02);
	outb(&__ErrorData_fields[8],"+",eregs->MALFFLG1&0x04);
	outb(&__ErrorData_fields[11],"+",eregs->MALFFLG1&0x08);
	outb(&__ErrorData_fields[14],"+",eregs->MALFFLG1&0x10);
	outb(&__ErrorData_fields[17],"+",eregs->MALFFLG1&0x20);
	outb(&__ErrorData_fields[20],"+",eregs->MALFFLG1&0x40);
	outb(&__ErrorData_fields[23],"+",eregs->MALFFLG1&0x80);
	outb(&__ErrorData_fields[28],"+",eregs->MALFFLG2&0x01);
	outb(&__ErrorData_fields[31],"+",eregs->MALFFLG2&0x02);
	outb(&__ErrorData_fields[34],"+",eregs->MALFFLG2&0x04);
	outb(&__ErrorData_fields[37],"+",eregs->MALFFLG2&0x08);
	outb(&__ErrorData_fields[40],"+",eregs->MALFFLG2&0x10);
	outb(&__ErrorData_fields[43],"+",eregs->MALFFLG2&0x20);
	outb(&__ErrorData_fields[46],"+",eregs->MALFFLG2&0x40);
	outb(&__ErrorData_fields[49],"+",eregs->MALFFLG2&0x80);
	outb(&__ErrorData_fields[3],"+",eregs->MALFFLG3&0x01);
	outb(&__ErrorData_fields[6],"+",eregs->MALFFLG3&0x02);
	outb(&__ErrorData_fields[9],"+",eregs->MALFFLG3&0x04);
	outb(&__ErrorData_fields[12],"+",eregs->MALFFLG3&0x08);
	outb(&__ErrorData_fields[15],"+",eregs->MALFFLG3&0x10);
	outb(&__ErrorData_fields[18],"+",eregs->MALFFLG3&0x20);
	outb(&__ErrorData_fields[21],"+",eregs->MALFFLG3&0x40);
	outb(&__ErrorData_fields[24],"+",eregs->MALFFLG3&0x80);
	outb(&__ErrorData_fields[29],"+",eregs->MALFFLG4&0x01);
	outb(&__ErrorData_fields[32],"+",eregs->MALFFLG4&0x02);
	outb(&__ErrorData_fields[35],"+",eregs->MALFFLG4&0x04);
	outb(&__ErrorData_fields[38],"+",eregs->MALFFLG4&0x08);
	outb(&__ErrorData_fields[41],"+",eregs->MALFFLG4&0x10);
	outb(&__ErrorData_fields[44],"+",eregs->MALFFLG4&0x20);
	outb(&__ErrorData_fields[47],"+",eregs->MALFFLG4&0x40);
	outb(&__ErrorData_fields[50],"+",eregs->MALFFLG4&0x80);
	return 0;
}
struct Panel __ErrorData={"ErrorData", __ErrorData_fields};
struct Panel *ErrorData=&__ErrorData;




