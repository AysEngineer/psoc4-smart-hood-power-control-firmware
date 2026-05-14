/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
/*----------------------------------------------------------------------------
                            manifest constants
----------------------------------------------------------------------------*/
#define EEPROMDATA_CNT   16              // EEPROM DATA COUNTER                        
#define CY_TEST_FLASH_ROW       (CY_FLASH_NUMBER_ROWS - 1u)
#define CY_TEST_FLASH_ADDR      (CY_TEST_FLASH_ROW * CY_FLASH_SIZEOF_ROW)
#define	ZERO_ARR(o)		    memset((uint8_t *)&(o), 0x0, sizeof( (o)))
/*****************************************************************************
* Macros 
*****************************************************************************/
#define MTR1_ON       Motor1_Write(1)
#define MTR1_OFF      Motor1_Write(0)
#define MTR2_ON       Motor2_Write(1)
#define MTR2_OFF      Motor2_Write(0)
#define MTR3_ON       Motor3_Write(1)
#define MTR3_OFF      Motor3_Write(0)
#define MTR4_ON       Motor4_Write(1)
#define MTR4_OFF      Motor4_Write(0)
#define MTR5_ON       Motor5_Write(1)
#define MTR5_OFF      Motor5_Write(0)

#define setSegA		SEG_A_SWONOFF_Write(1)
#define clrSegA		SEG_A_SWONOFF_Write(0)
#define setSegD		SEG_D_LAMP_Write(1)
#define clrSegD		SEG_D_LAMP_Write(0)
#define setSegC		SEG_C_SWUP_Write(1)
#define clrSegC		SEG_C_SWUP_Write(0)
#define setSegG		SEG_G_Write(0)
#define clrSegG		SEG_G_Write(1)
#define setSegF		SEG_F_Write(1)
#define clrSegF		SEG_F_Write(0)
#define setSegB		SEG_B_SWDOWN_Write(1)
#define clrSegB		SEG_B_SWDOWN_Write(0)
#define setSegE     SEG_E_SWTIMER_Write(1)
#define clrSegE     SEG_E_SWTIMER_Write(0)

#define LAMP_ON                 LAMP_Write(1)
#define LAMP_OFF                LAMP_Write(0)
#define BZ1_ON                  BZR_Write(1)
#define BZ1_OFF                 BZR_Write(0)
#define KR_ON                  KR_Write(1)
#define KR_OFF                 KR_Write(0)

#define set_DSP5V         BG_Write(0)
#define clr_DSP5V         BG_Write(1)

#define TXDATACNT           8          // TX DATA COUNT
#define RXDATACNT           8          // RX DATA COUNTER
/*----------------------------------------------------------------------------
                            manifest constants
----------------------------------------------------------------------------*/
#define MAXSPD		4
#define TURBOTIME	7		// 7 MYNUTE
#define DEFAULTFATDK      65000

#define AUTOOFFCNT          600        // 600 * 500ms = 5dk demo 120 = 1dk
#define ZORUNLUCALISMA      600      // orjianlde  600 * 500 =  5dk  demo  30sn = 60
#define MTRONSURE2DK        240      // orjinalde  360 * 500 =  2dk  demo  1dk = 120 
#define MTRONSURE8DK        960     // orjinalde 1200 * 500 = 8dk  demo  2dk = 240 
#define MTRONSUREUST        1560     // orjinalde 1800 * 500 = 15dk  demo  4dk = 480 
/*----------------------------------------------------------------------------
                            type definitions
----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
                                prototypes
----------------------------------------------------------------------------*/
void hrmincon(void);
void butread(void);
void dspupdt(unsigned char reqspd2);
void rlymotupdt(void);
void initwait(void);
void irread(void);
void irbuteval(void);
void tmrbzrld(void);
void EEput(int iAddress, char cValue);
void EEget(int iAddress, unsigned char *cValue);
void e2rec(void);
void getbutdat(void);
void irread(void);
void motbutprs(unsigned char reqspd2);
void fatdel (void);
void preparetxdata(void);
void sendtxdata(void);
void getrxdata(void); 
void handlesilverconnectcontrol(void);
void tbprs(void);
void silverconnectTimeDecreasing(void);
void airoffauto(void);
void aironauto(void);
void aircfunc(void);
void mobprs(void);
void vdbprs(void);
void vibprs(void);
void autoButtonPress(void);
void aircbprs(void);
void aircleanTimeDecreasing(void);
uint8_t flash_write(void);
uint8_t fe2reccnt ;
void curmBufHandle(void);
void hassasiyetinc(void);
void hassasiyetdec(void);
void loadDisplayBright(void);
void loadDisplayDark(void);
void carbonfilterdel (void);
void ClearWorkTime (void);
void ModeOff(void);
//static void init_system(void);	/*  initialise host app, pins, watchdog, etc    */
//static void init_timer_isr(void);	/*  configure timer ISR to fire regularly   */

uint8_t rx_con_reg ;             // rx control register
uint8_t rxbuf_notzero_cnt ;      // rx buffer 60ms boyunca tamamen boş yada dolu değilse tamamen boşaltılır. olaki her 20ms iletişimin tam ortasında oluyorsa bu şekilde senkron sağlanmış olur
//uint8_t ftusilet ;

uint8_t dspdary[15];
uint8_t aircdspdary[7];
uint8_t dspdata ;
uint8_t ti125uscnt ;
uint8_t presdb ;
uint8_t butdat ;
uint8_t prvdat ;
uint8_t button ;
uint8_t prvbut ;
uint8_t vdcnt ;
uint8_t lvdcnt ;
uint8_t lmpstt ;
uint8_t reqspd ;
uint8_t actspd ;
uint8_t tmrmot ;
uint8_t tmrtgl ;
uint8_t digdsp ;
uint8_t tmroff ;
uint8_t tmrof1s ;
uint8_t tmrof10ms ;
uint8_t bzrstt ;
volatile uint8_t f20ms ;		
uint8_t fatwrkcnt20ms ;		//filter 10ms counter
uint8_t fatwrksec ;			//filter work secend	
uint16_t fatwrkmin ;			//filter work minute
//uint8_t fatwrk2hr ;			//filter work hour
uint8_t fdltok ;			// filter work time deleted flag,
uint8_t fatfull ;			// filter work time full
uint16_t fatmaxsure ;
uint8_t fatclear ;
uint8_t irstt ;
uint8_t ticnt ;
uint8_t fstartok ;
uint8_t irdatacnt ;
uint8_t fstop1ok ;
uint8_t irdb1 ;
uint8_t irdb2 ;
uint8_t irdb3 ;
uint8_t irdb4 ;
uint8_t irbut ;	
uint8_t prvirbut ;
uint8_t vircnt ;
uint8_t lvircnt ;
uint8_t ircnt20ms ;
uint8_t tglf ;
uint8_t tmrwait ;
uint8_t firprs ;

uint8_t tmrcdsp ;
uint8_t fdigdsp ;
uint8_t mltbutprsd ;

uint8_t turbo10mstmr ;		//
uint8_t turbo1stmr ;		//
uint8_t turbotmr ;			//
uint8_t fturbooff ;

uint8_t fwdt16ms ;
uint32_t tmrentslp ;
uint32_t tmrbzr ;
uint8_t fremdat ;
uint8_t ircnt ;

uint8_t t500msbuf ;		// 60*10ms=600ms
uint8_t t1secbuf ;		// 100
uint8_t cmp60msval ;		// 6	6*10ms = 60ms

uint8_t t1secbuf  ;			// 50*20ms = 1sec(50 hz)
uint8_t t500msbuf ;			// 25*20ms = 500ms(50 hz)
uint8_t t200msbuf ;         // 10*20ms = 200ms(50 hz)
uint8_t t100msbuf ;		    // 5*20ms = 100ms(50 hz)

uint8_t MessageType ;
uint8_t Requested_A ;
uint8_t Requested_B ;
uint8_t MessageTypeRX ;
uint8_t Byte1_RX ;
uint8_t Byte2_RX ;
uint8_t uart_periodcnt ;
uint8_t txdata[TXDATACNT] ;
uint8_t rxdatabuf[RXDATACNT] ;
uint8_t rxdatacntr ;
uint8_t frxread_ok ;         // data read succesfull
uint8_t com_errcnt ;         // communication error counter
uint8_t com_okcnt ;          // communication succesfull counter

uint8_t fmotprs ;
uint8_t led2mscnt ;

uint16_t maxCurrentValue ;
uint16_t presentCurrentValue ;

uint8_t lvl1cnt  ;
uint8_t lvl2cnt  ;
uint8_t lvl3cnt ;
uint8_t lvl4cnt  ;
uint8_t lvloffcnt  ;
uint16_t akim_totadc ;
uint16_t max_deger ;
uint16_t zorunlucalisma ; 
uint8_t hassasiyetkdm ;
uint8_t hassasiyetadj ;
uint8_t fautotimeroff ;
uint8_t mtrofftmrsn ;
uint8_t ftmrautoon ;
uint8_t ftmrautooff ;
uint16_t bufmax_deger ;
uint8_t bufhassasiyetkdm ;
uint16_t katsayi1 ; 
uint16_t katsayi2 ;
uint16_t akimkatsayi ;
uint8_t fauto ;
uint16_t akimkatsayiyuzde ;
uint8_t fmtroff ;
uint8_t motofftmr ;
uint16_t cntoffauto ;
uint8_t bufreqspd ;
uint8_t fautomon ;
uint16_t mtrruncnt ;

//carbon filter flags
uint8_t carbon_filter_active;
uint16_t carbonwrkhr;
uint8_t carboncnt10ms;
uint8_t carbonwrksec;
uint8_t carbonwrkmin;
uint16_t carbon_filter_timer_value;
uint8_t fatfull_carbon;
uint16_t ex_carbon_filter_timer_value;

uint8_t tmrairc;
uint8_t fairc ;        //thermex 
uint8_t faircmod ;     //thermex 
uint8_t tmrofacon ;    //thermex 
uint8_t tmrofacoff ;   //thermex 
uint8_t aircnt10ms ;   //thermex 
uint8_t aircnt10ms_2 ;   //thermex 
uint8_t airsec ;       //thermex 
uint8_t airmin ;       //thermex 
uint8_t acled ;        //thermex 
uint8_t fairc20s ;     //thermex 
uint8_t tmrairc20s ;   //thermex 
uint8_t tmrac200ms ;   //thermex 
uint8_t tmrlst200ms ;  //thermex 
uint8_t valofacon ;
uint8_t acledcnt ;
uint8_t flampLongPress ;
uint8_t segval ;
uint8_t tmrsegspdbuf ;	//20*20ms = 200ms
uint8_t tmrsegspd ;
uint8_t ftmrLongPress ;
uint8_t fe2rec ;             // eeprom write request
uint8_t fatEraseShownTime ;
uint8_t curmPressAuto ;
uint8_t curmPressAirmode ;
uint8_t fiotlongpres ;
uint8_t fiotclear ;
uint16_t tmrPwronScanPressButton ;
uint8_t tmrShownS5Second ;
//uint8_t cntCurrentAverage ;
uint16_t totalAverageValue ;
uint8_t fCurrentIsReady ;
uint8_t tmrCurrentSensitivtyAdjustment ;
uint8_t fIrlampLongPress ;
uint8_t fIrIncLongPress ;
uint8_t fIrMonoffLongPress ;
uint8_t cntAutowithTimerShown ;
uint8_t tmrAirmodeSegmentShown ;
uint8_t fairqualitymode;

//max work time

uint8_t wrkhour;		// orjinalde 4 (max çalışma süresi)
uint8_t wrkmin;
uint8_t	wrksec;
uint8_t	wrkcnt10ms;

/* [] END OF FILE */
