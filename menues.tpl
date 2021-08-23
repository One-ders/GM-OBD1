
$DefinePanel RawData
$Layout
$Option LabelSize 4
$Option Indata char *indata
================================= RAW Data ====================================
Sample  !   0!  -1!  -2!  -3!  -4!  -5!  -6!  -7!  -8!  -9! -10! -11! -12! -13!
MW2     !$a01!$b01!$c01!$d01!$e01!$f01!$g01!$h01!$i01!$j01!$k01!$l01!$m01!$n01!
PROMIDA !$a02!$b02!$c02!$d02!$e02!$f02!$g02!$h02!$i02!$j02!$k02!$l02!$m02!$n02!
PROMIDB !$a03!$b03!$c03!$d03!$e03!$f03!$g03!$h03!$i03!$j03!$k03!$l03!$m03!$n03!
IAC     !$a04!$b04!$c04!$d04!$e04!$f04!$g04!$h04!$i04!$j04!$k04!$l04!$m04!$n04!
CT      !$a05!$b05!$c05!$d05!$e05!$f05!$g05!$h05!$i05!$j05!$k05!$l05!$m05!$n05!
MPH     !$a06!$b06!$c06!$d06!$e06!$f06!$g06!$h06!$i06!$j06!$k06!$l06!$m06!$n06!
MAP     !$a07!$b07!$c07!$d07!$e07!$f07!$g07!$h07!$i07!$j07!$k07!$l07!$m07!$n07!
RPM     !$a08!$b08!$c08!$d08!$e08!$f08!$g08!$h08!$i08!$j08!$k08!$l08!$m08!$n08!
TPS     !$a09!$b09!$c09!$d09!$e09!$f09!$g09!$h09!$i09!$j09!$k09!$l09!$m09!$n09!
INT     !$a10!$b10!$c10!$d10!$e10!$f10!$g10!$h10!$i10!$j10!$k10!$l10!$m10!$n10!
O2      !$a11!$b11!$c11!$d11!$e11!$f11!$g11!$h11!$i11!$j11!$k11!$l11!$m11!$n11!
MALFFLG1!$a12!$b12!$c12!$d12!$e12!$f12!$g12!$h12!$i12!$j12!$k12!$l12!$m12!$n12!
MALFFLG2!$a13!$b13!$c13!$d13!$e13!$f13!$g13!$h13!$i13!$j13!$k13!$l13!$m13!$n13!
MALFFLG3!$a14!$b14!$c14!$d14!$e14!$f14!$g14!$h14!$i14!$j14!$k14!$l14!$m14!$n14!
MALFFLG4!$a15!$b15!$c15!$d15!$e15!$f15!$g15!$h15!$i15!$j15!$k15!$l15!$m15!$n15!
MWAF1   !$a16!$b16!$c16!$d16!$e16!$f16!$g16!$h16!$i16!$j16!$k16!$l16!$m16!$n16!
VOLT    !$a17!$b17!$c17!$d17!$e17!$f17!$g17!$h17!$i17!$j17!$k17!$l17!$m17!$n17!
MCUINST !$a18!$b18!$c18!$d18!$e18!$f18!$g18!$h18!$i18!$j18!$k18!$l18!$m18!$n18!
LV8     !$a19!$b19!$c19!$d19!$e19!$f19!$g19!$h19!$i19!$j19!$k19!$l19!$m19!$n19!
BLM     !$a20!$b20!$c20!$d20!$e20!$f20!$g20!$h20!$i20!$j20!$k20!$l20!$m20!$n20!
O2_CNT  !$a21!$b21!$c21!$d21!$e21!$f21!$g21!$h21!$i21!$j21!$k21!$l21!$m21!$n21!
AIRFLOW2!$a22!$b22!$c22!$d22!$e22!$f22!$g22!$h22!$i22!$j22!$k22!$l22!$m22!$n22!
AIRFLOW1!$a23!$b23!$c23!$d23!$e23!$f23!$g23!$h23!$i23!$j23!$k23!$l23!$m23!$n23!
INJ_T2  !$a24!$b24!$c24!$d24!$e24!$f24!$g24!$h24!$i24!$j24!$k24!$l24!$m24!$n24!
INJ_T1  !$a25!$b25!$c25!$d25!$e25!$f25!$g25!$h25!$i25!$j25!$k25!$l25!$m25!$n25!
$EndLayout
$FieldInfo
	DECL(struct $Panel_data {)
	REPEAT(X=(0-13),DECL(unsigned char col(X)[25];))
	DECL(} rdd;)

	REPEAT(X=(0-13),INIT(memset(&rdd.col(X),0,25);))

	REPEAT(X=(13-1),UPDATE(memcpy(&rdd.col(X),&rdd.col(X-1),25);))
	UPDATE(memcpy(&rdd.col0,indata,25);)
	REPEAT(Y=(0-13),REPEAT(X=(01-25),UPDATE(outf($(Y+97,%c)(X,%02d),"0x%02x",rdd.col(Y)[(X-1)]);)))
$EndFieldInfo
$EndPanel

$DefinePanel FlagData
$Layout
$Option LabelSize 3
$Option Indata struct ECM_Regs *eregs
================================= Flag Data ====================================
============ MW2 ================
 $01Road speed pulse
 $02Not Used
 $03Ref Pulse occured
 $04ALDL Factory Test (3.9K)
 $05ALDL Diag mode    (0 ohms)
 $06ALDL ALDL Mode    (10K)
 $07High battery voltage
 $08Air meter pulse

============== MWAF1 ============         ============ MCUISW =================
 $09Not Used                               $17P/N Flag
 $10Learn control enable                   $18Not in third gear
 $11Not Used                               $19Overdrive req
 $12Not Used                               $20Not Used
 $13VSS Failure                            $21EGR Diag switch closed
 $14EECC Slow O2                           $22TCC locked
 $15Rich-RIch                              $23FAN Request
 $16Closed loop                            $24A/C Request
$EndLayout
$FieldInfo
DECL(struct ECM_Regs {)
DECL(unsigned char MW2;)
DECL(unsigned char PROMIDA;)
DECL(unsigned char PROMIDB;)
DECL(unsigned char ISSPMP;)
DECL(unsigned char COOLDEGA;)
DECL(unsigned char FILTMPH;)
DECL(unsigned char EGRDC;)
DECL(unsigned char NTRPMX;)
DECL(unsigned char ADTHROT;)
DECL(unsigned char CORRCL;)
DECL(unsigned char ADO2AF;)
DECL(unsigned char MALFFLG1;)
DECL(unsigned char MALFFLG2;)
DECL(unsigned char MALFFLG3;)
DECL(unsigned char MALFFLG4;)
DECL(unsigned char MWFA;)
DECL(unsigned char MAT;)
DECL(unsigned char MCUINST;)
DECL(unsigned char LV8;)
DECL(unsigned char BLM;)
DECL(unsigned char ALDLCNTR;)
DECL(unsigned char DISPFLOW;)
DECL(unsigned char DISPFLOW1;)
DECL(unsigned char OBPINJ;)
DECL(unsigned char OBPINJ1;)
DECL(};)
UPDATE(outb($01,"+",eregs->MW2&0x01);)
UPDATE(outb($02,"+",eregs->MW2&0x02);)
UPDATE(outb($03,"+",eregs->MW2&0x04);)
UPDATE(outb($04,"+",eregs->MW2&0x08);)
UPDATE(outb($05,"+",eregs->MW2&0x10);)
UPDATE(outb($06,"+",eregs->MW2&0x20);)
UPDATE(outb($07,"+",eregs->MW2&0x40);)
UPDATE(outb($08,"+",eregs->MW2&0x80);)

UPDATE(outb($09,"+",eregs->MWFA&0x01);)
UPDATE(outb($10,"+",eregs->MWFA&0x02);)
UPDATE(outb($11,"+",eregs->MWFA&0x04);)
UPDATE(outb($12,"+",eregs->MWFA&0x08);)
UPDATE(outb($13,"+",eregs->MWFA&0x10);)
UPDATE(outb($14,"+",eregs->MWFA&0x20);)
UPDATE(outb($15,"+",eregs->MWFA&0x40);)
UPDATE(outb($16,"+",eregs->MWFA&0x80);)

UPDATE(outb($17,"+",eregs->MCUINST&0x01);)
UPDATE(outb($18,"+",eregs->MCUINST&0x02);)
UPDATE(outb($19,"+",eregs->MCUINST&0x04);)
UPDATE(outb($20,"+",eregs->MCUINST&0x08);)
UPDATE(outb($21,"+",eregs->MCUINST&0x10);)
UPDATE(outb($22,"+",eregs->MCUINST&0x20);)
UPDATE(outb($23,"+",eregs->MCUINST&0x40);)
UPDATE(outb($24,"+",!eregs->MCUINST&0x80);)
$EndFieldInfo
$EndPanel


$DefinePanel SensorData
$Layout
$Option LabelSize 3
$Option Indata struct ECM_Regs *eregs
================================= Sensor Data =========================
                    ! RAW   !  Converted ! Unit   !  Converted ! Unit !
 IAC                !  $01  !        $02 !  #     !            !      !
 Coolant Temp       !  $03  !        $04 !  F     !        $05 !  C   !
 MPH                !  $06  !        $07 ! MPH    !        $08 ! KMH  !
 RPM                !  $09  !        $10 ! RPM    !            !      !
 TPS                !  $11  !        $12 ! Volt   !        $13 ! %    !
 INT                !  $14  !        $15 !  #     !            !      !
 O2                 !  $16  !        $17 ! mVolt  !            !      !
 MAT                !  $18  !        $19 !  F     !        $20 !  C   !
 BLM                !  $21  !        $22 !  #     !            !      !
 Rich/Lean cnt      !  $23  !        $24 !  #     !            !      !
 MAF                !  $25  !        $26 !  g/sec !            !      !
=======================================================================
$EndLayout
$FieldInfo
#DECL(struct ECM_Regs {)
#DECL(unsigned char MW2;)
#DECL(unsigned char PROMIDA;)
#DECL(unsigned char PROMIDB;)
#DECL(unsigned char ISSPMP;)
#DECL(unsigned char COOLDEGA;)
#DECL(unsigned char FILTMPH;)
#DECL(unsigned char EGRDC;)
#DECL(unsigned char NTRPMX;)
#DECL(unsigned char ADTHROT;)
#DECL(unsigned char CORRCL;)
#DECL(unsigned char ADO2AF;)
#DECL(unsigned char MALFFLG1;)
#DECL(unsigned char MALFFLG2;)
#DECL(unsigned char MALFFLG3;)
#DECL(unsigned char MALFFLG4;)
#DECL(unsigned char MWFA;)
#DECL(unsigned char MAT;)
#DECL(unsigned char MCUINST;)
#DECL(unsigned char LV8;)
#DECL(unsigned char BLM;)
#DECL(unsigned char ALDLCNTR;)
#DECL(unsigned char DISPFLOW;)
#DECL(unsigned char DISPFLOW1;)
#DECL(unsigned char OBPINJ;)
#DECL(unsigned char OBPINJ1;)
#DECL(};)
DECL(char *xtoS(int val, int scale, char *buf, int bsize) {)
DECL( char neg=' ';)
DECL( if (val<0) {)
DECL( neg='-';)
DECL( val=-val;)
DECL( })
DECL(  sprintf(buf,"%c%d.%02d",neg,val/scale,val%scale);)
DECL(  return buf; )
DECL(})
UPDATE(char buf1[16];)
UPDATE(outf($01,"0x%02x",eregs->ISSPMP);)
UPDATE(outf($02,"%3d",eregs->ISSPMP);)
UPDATE(outf($03,"0x%02x",eregs->COOLDEGA);)
UPDATE(outf($04,"%6s", xtoS((eregs->COOLDEGA*135)-4000, 100, buf1,sizeof(buf1)));)
UPDATE(outf($05,"%6s", xtoS((eregs->COOLDEGA*75)-4000,100,buf1,sizeof(buf1)));)
UPDATE(outf($06,"0x%02x",eregs->FILTMPH);)
UPDATE(outf($07,"%3d",eregs->FILTMPH);)
UPDATE(outf($08,"%4s",xtoS(eregs->FILTMPH*161,100,buf1,sizeof(buf1)));)
UPDATE(outf($09,"0x%02x",eregs->NTRPMX);)
UPDATE(outf($10,"%3d",eregs->NTRPMX*25);)
UPDATE(outf($11,"0x%02x",eregs->ADTHROT);)
UPDATE(outf($12,"%6s", xtoS((eregs->ADTHROT*100)/51,100,buf1,sizeof(buf1)));)
UPDATE(outf($13,"%6s", xtoS((eregs->ADTHROT*39),100,buf1,sizeof(buf1)));)
UPDATE(outf($14,"0x%02x",eregs->CORRCL);)
UPDATE(outf($15,"%3d",eregs->CORRCL);)
UPDATE(outf($16,"0x%02x",eregs->ADO2AF);)
UPDATE(outf($17,"%6s",xtoS((eregs->ADO2AF*444),100,buf1,sizeof(buf1)));)
UPDATE(outf($18,"0x%02x",eregs->MAT);)
UPDATE(outf($19,"%6s",xtoS((eregs->MAT*135)-4000,100,buf1,sizeof(buf1)));)
UPDATE(outf($20,"%6s",xtoS((eregs->MAT*75)-4000,100,buf1,sizeof(buf1)));)
UPDATE(outf($21,"0x%02x",eregs->BLM);)
UPDATE(outf($22,"%3d",eregs->BLM);)
UPDATE(outf($23,"0x%02x",eregs->ALDLCNTR);)
UPDATE(outf($24,"%3d",eregs->ALDLCNTR);)
UPDATE(outf($25,"0x%02x",eregs->DISPFLOW);)
UPDATE(outf($26,"%4d",(eregs->DISPFLOW*256)+eregs->DISPFLOW1);)
$EndFieldInfo
$EndPanel



$DefinePanel ErrorData
$Layout
$Option LabelSize 3
$Option Indata struct ECM_Regs *eregs
================================= Error Codes  ====================================
============ MALFFLG1 ==============   ============ MALFFLG3 ==============
 $01 23 MAT sensor LO                  $17 51 Prom err
 $02 22 TPS LO                         $18 46 Not Used
 $03 21 TPS HI                         $19 45 O2 Sensor Rich
 $04 16 Not Used                       $20 44 02 Sensor Lean
 $05 15 Coolant sens temp HI           $21 43 ESC Fail
 $06 14 Coolant sens temp LO           $22 42 EST Mon err
 $07 13 O2 sensor                      $23 41 Cyl Sel err
 $08 12 No ref pulses                  $24 36 Burn off diag

============== MALFFLG2 ============   ============ MALFFLG4  =============
 $09 35 Not Used                       $25 63 Not Used
 $10 34 MAF sens LO                    $26 62 Not Used
 $11 33 MAF sens HI                    $27 61 Not Used
 $12 32 EGR Diag                       $28 55 Not Used
 $13 31 Not Used                       $29 54 ADU Err
 $14 26 Not Used                       $30 53 Fuel Pump volts
 $15 25 MAT Sens HI                    $31 52 Over voltage
 $16 24 VSS                            $32 51 Cal pack missing
$EndLayout
$FieldInfo
UPDATE(outb($01,"+",eregs->MALFFLG1&0x01);)
UPDATE(outb($02,"+",eregs->MALFFLG1&0x02);)
UPDATE(outb($03,"+",eregs->MALFFLG1&0x04);)
UPDATE(outb($04,"+",eregs->MALFFLG1&0x08);)
UPDATE(outb($05,"+",eregs->MALFFLG1&0x10);)
UPDATE(outb($06,"+",eregs->MALFFLG1&0x20);)
UPDATE(outb($07,"+",eregs->MALFFLG1&0x40);)
UPDATE(outb($08,"+",eregs->MALFFLG1&0x80);)

UPDATE(outb($09,"+",eregs->MALFFLG2&0x01);)
UPDATE(outb($10,"+",eregs->MALFFLG2&0x02);)
UPDATE(outb($11,"+",eregs->MALFFLG2&0x04);)
UPDATE(outb($12,"+",eregs->MALFFLG2&0x08);)
UPDATE(outb($13,"+",eregs->MALFFLG2&0x10);)
UPDATE(outb($14,"+",eregs->MALFFLG2&0x20);)
UPDATE(outb($15,"+",eregs->MALFFLG2&0x40);)
UPDATE(outb($16,"+",eregs->MALFFLG2&0x80);)

UPDATE(outb($17,"+",eregs->MALFFLG3&0x01);)
UPDATE(outb($18,"+",eregs->MALFFLG3&0x02);)
UPDATE(outb($19,"+",eregs->MALFFLG3&0x04);)
UPDATE(outb($20,"+",eregs->MALFFLG3&0x08);)
UPDATE(outb($21,"+",eregs->MALFFLG3&0x10);)
UPDATE(outb($22,"+",eregs->MALFFLG3&0x20);)
UPDATE(outb($23,"+",eregs->MALFFLG3&0x40);)
UPDATE(outb($24,"+",eregs->MALFFLG3&0x80);)

UPDATE(outb($25,"+",eregs->MALFFLG4&0x01);)
UPDATE(outb($26,"+",eregs->MALFFLG4&0x02);)
UPDATE(outb($27,"+",eregs->MALFFLG4&0x04);)
UPDATE(outb($28,"+",eregs->MALFFLG4&0x08);)
UPDATE(outb($29,"+",eregs->MALFFLG4&0x10);)
UPDATE(outb($30,"+",eregs->MALFFLG4&0x20);)
UPDATE(outb($31,"+",eregs->MALFFLG4&0x40);)
UPDATE(outb($32,"+",eregs->MALFFLG4&0x80);)
$EndFieldInfo
$EndPanel

