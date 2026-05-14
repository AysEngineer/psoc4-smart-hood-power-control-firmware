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
#include "project.h"
#include <main.h>

uint8_t rx_con_reg = 0;             // rx control register
uint8_t rxbuf_notzero_cnt = 0;      // rx buffer 60ms boyunca tamamen boş yada dolu değilse tamamen boşaltılır. olaki her 20ms iletişimin tam ortasında oluyorsa bu şekilde senkron sağlanmış olur
static const uint8  eepromArray[EEPROMDATA_CNT] __attribute__ ((section (".EEPROMDATA"))) = { 0xAC,0x00,0x00,0x00,0x00,0x00,0x36, 0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};     //first values after programming


uint8_t eeprmwrt_buf[EEPROMDATA_CNT] = {0};
uint8 rowPattern[CY_FLASH_SIZEOF_ROW];

uint8_t dspdary[15] = {0xFF,0xBE, 0xC4, 0x8C, 0x9A, 0x89, 0x81, 0xBC, 0x80, 0x88, 0xE1, 0xC1, 0x83, 0x90, 0xA0};
uint8_t aircdspdary[7] = {0x20,0xFD, 0xFE, 0xBF, 0xEF, 0xF7, 0xFB};
uint8_t dspdata = 0;
uint8_t ti125uscnt = 0;
uint8_t presdb = 0;
uint8_t butdat = 0;
uint8_t prvdat = 0;
uint8_t button = 0;
uint8_t prvbut = 0;
uint8_t vdcnt = 0;
uint8_t lvdcnt = 0;
uint8_t lmpstt = 0;
uint8_t reqspd = 0;
uint8_t actspd = 0;
uint8_t tmrmot = 0;
uint8_t tmrtgl = 0;
uint8_t digdsp = 0;
uint8_t tmroff = 0;
uint8_t tmrof1s = 0;
uint8_t tmrof10ms = 0;
uint8_t bzrstt = 0;
volatile uint8_t f20ms = 0;		
uint8_t fatwrkcnt20ms = 0;		//filter 10ms counter
uint8_t fatwrksec = 0;			//filter work secend	
uint16_t fatwrkmin = 0;			//filter work minute
//uint8_t fatwrk2hr = 0;			//filter work hour
uint8_t fdltok = 0;			// filter work time deleted flag,
uint8_t fatfull = 0;			// filter work time full
uint16_t fatmaxsure = 0;
uint8_t fatclear = 0;
uint8_t irstt = 0;
uint8_t ticnt = 0;
uint8_t fstartok = 0;
uint8_t irdatacnt = 0;
uint8_t fstop1ok = 0;
uint8_t irdb1 = 0;
uint8_t irdb2 = 0;
uint8_t irdb3 = 0;
uint8_t irdb4 = 0;
uint8_t irbut = 0;	
uint8_t prvirbut = 0;
uint8_t vircnt = 0;
uint8_t lvircnt = 0;
uint8_t ircnt20ms = 0;
uint8_t tglf = 0;
uint8_t tmrwait = 0;
uint8_t firprs = 0;

uint8_t tmrcdsp = 0;
uint8_t fdigdsp = 0;
uint8_t mltbutprsd = 0;

uint8_t turbo10mstmr = 0;		//
uint8_t turbo1stmr = 0;		//
uint8_t turbotmr = 0;			//
uint8_t fturbooff = 0;

uint8_t fwdt16ms = 0;
uint32_t tmrentslp = 0;
uint32_t tmrbzr = 0;
uint8_t fremdat = 0;
uint8_t ircnt = 0;

//uint8_t t500msbuf = 25;		// 25*20ms=500ms
//uint8_t t1secbuf = 100;		// 100
uint8_t cmp60msval = 3;		// 6	3*20ms = 60ms

uint8_t t1secbuf = 50;			// 50*20ms = 1sec(50 hz)
uint8_t t500msbuf = 25;			// 25*20ms = 500ms(50 hz)
uint8_t t200msbuf = 10;         // 10*20ms = 200ms(50 hz)
uint8_t t100msbuf = 5;		    // 5*20ms = 100ms(50 hz)

uint8_t MessageType = 0;
uint8_t Requested_A = 0;
uint8_t Requested_B = 0;
uint8_t MessageTypeRX = 0;
uint8_t Byte1_RX = 0;
uint8_t Byte2_RX = 0;
uint8_t uart_periodcnt = 0;
uint8_t txdata[TXDATACNT] = {};
uint8_t rxdatabuf[RXDATACNT] = {};
uint8_t rxdatacntr = 0;
uint8_t frxread_ok = 0;         // data read succesfull
uint8_t com_errcnt = 0;         // communication error counter
uint8_t com_okcnt = 0;          // communication succesfull counter

//uint8_t RGB_RValue = 0;
//uint8_t RGB_GValue = 0;
//uint8_t RGB_BValue = 0;
//uint8_t fRGBstt = 0;
//uint8_t fIOTLEDstt = 0;
//uint8_t Tustmr_1 = 0;
//uint8_t Tustmr_2 = 0;
//uint8_t Tustmr_3 = 0;
//uint8_t Tustmr_4 = 0;
//uint8_t FTustmr_1 = 0;
//uint8_t FTustmr_2 = 0;
//uint8_t FTustmr_3 = 0;
//uint8_t FTustmr_4 = 0;
//uint8_t TusbasimTmr = 0;

uint8_t fmotprs = 0;
uint8_t led2mscnt = 0;
//uint8_t ftusilet = 0;

uint16_t maxCurrentValue = 0;
uint16_t presentCurrentValue = 0;

uint8_t lvl1cnt = 0;
uint8_t lvl2cnt = 0;
uint8_t lvl3cnt = 0;
uint8_t lvl4cnt = 0;
uint8_t lvloffcnt = 0;
uint16_t akim_totadc = 0;
uint16_t max_deger = 0;
uint16_t zorunlucalisma = 0; 
uint8_t hassasiyetkdm = 2;
uint8_t hassasiyetadj = 0;
uint8_t fautotimeroff = 0;      // autonun sonunda auto tarafından kapanma timeri kurulursa bu flag set olur.
uint8_t mtrofftmrsn = 0;
uint8_t ftmrautoon = 0;
uint8_t ftmrautooff = 0;        // auto modun mon-off ile geçici olarak kapatıldıgında set olur.
uint16_t bufmax_deger = 0;
uint8_t bufhassasiyetkdm = 0;
uint16_t katsayi1 = 0; 
uint16_t katsayi2 = 0;
uint16_t akimkatsayi = 0;
uint8_t fauto = 0;
uint16_t akimkatsayiyuzde = 0;
uint8_t fmtroff = 0;
uint8_t motofftmr = 0;
uint16_t cntoffauto = 0;
uint8_t bufreqspd = 0;
uint8_t fautomon = 0;
uint16_t mtrruncnt = 0;

uint8_t tmrairc = 0;
uint8_t fairc = 0;        //thermex 
uint8_t faircmod = 0;     //thermex 
uint8_t tmrofacon = 0;    //thermex 
uint8_t tmrofacoff = 0;   //thermex 
uint8_t aircnt10ms = 0;   //thermex 
uint8_t aircnt10ms_2 = 0;   //thermex 
uint8_t airsec = 0;       //thermex 
uint8_t airmin = 0;       //thermex 
uint8_t acled = 0;        //thermex 
uint8_t fairc20s = 0;     //thermex 
uint8_t tmrairc20s = 0;   //thermex 
uint8_t tmrac200ms = 0;   //thermex 
uint8_t tmrlst200ms = 0;  //thermex 
uint8_t valofacon = 5;
uint8_t acledcnt = 1;
uint8_t flampLongPress = 1;
uint8_t segval = 1;
uint8_t tmrsegspdbuf = 10;	//20*20ms = 200ms
uint8_t tmrsegspd = 10;
uint8_t ftmrLongPress = 0;
uint8_t fe2rec = 0;             // eeprom write request
uint8_t fe2reccnt = 0;
uint8_t fatEraseShownTime = 0;
uint8_t curmPressAuto = 0;
uint8_t curmPressAirmode = 0;
uint8_t fiotlongpres = 0;
uint8_t fiotclear = 0;
uint16_t tmrPwronScanPressButton = 0;
uint8_t tmrShownS5Second = 0;
//uint8_t cntCurrentAverage = 0;
uint16_t totalAverageValue = 0;
uint8_t fCurrentIsReady = 0;
uint8_t tmrCurrentSensitivtyAdjustment = 0;
uint8_t fIrlampLongPress = 0;
uint8_t fIrIncLongPress = 0;
uint8_t fIrMonoffLongPress = 0;
uint8_t cntAutowithTimerShown = 0;
uint8_t tmrAirmodeSegmentShown = 0;
uint8_t fairqualitymode = 0;

//carbon filter flags
uint16_t carbon_filter_timer_value = 0;             //başlangıç sıfır olmalı IoT kartı bize süreyi söyledikten sonra sayma başlayacak
uint8_t carbon_filter_active = 0;
uint16_t ex_carbon_filter_timer_value = 0;

//max work time

uint8_t wrkhour = 4;		// orjinalde 4 (max çalışma süresi)
uint8_t wrkmin = 0;
uint8_t	wrksec = 0;
uint8_t	wrkcnt10ms = 0;


CY_ISR(ISR_TIMER_LOC)
{
	if (tmrbzr != 0)		//buzzer on
	{
		if (bzrstt == 1)
		{	
			bzrstt = 0;
            BZ1_ON;
			--tmrbzr;
		}
		else 
		{
			bzrstt = 1;
			BZ1_OFF;
		}
	}
	else
		BZ1_OFF;
	if(++ti125uscnt >= 160)   //80 * 125us = 10ms
	{
		ti125uscnt = 0;
		f20ms = 1;
	}
	irread();
    Timer_ClearInterrupt(Timer_INTR_MASK_TC);    /* Clear interrupt request from terminal count */
}
/***********************************************
*
***********************************************/
uint8_t flash_write(void)
{
    uint16 i;
    cystatus returnValue = CYRET_SUCCESS;
    volatile uint8 source;
    volatile uint8 destination;
    uint8 result = 0u;
    
    for(i = 0u; i < EEPROMDATA_CNT; i++)
        rowPattern[i] = eeprmwrt_buf[i];

    returnValue = CySysFlashWriteRow(CY_TEST_FLASH_ROW, rowPattern);
    
    for (i = 0u; i < EEPROMDATA_CNT; i++)
    {
        source = (*((uint8 *) (CY_TEST_FLASH_ADDR + i)));
        destination = rowPattern[i];
        if ( source != destination) result++;
    }

    return 0;
}
/***********************************************
*
***********************************************/
uint8_t flash_read(void)
{
    uint16 i;
    
    for (i = 0u; i < EEPROMDATA_CNT; i++)
    {
        eeprmwrt_buf[i] = (*((uint8 *) (CY_TEST_FLASH_ADDR + i)));
    }

    return 0;    
}
/***********************************************
*
***********************************************/
void read_eeprom(void)
{
    if (flash_read()) return;
    
    if(eeprmwrt_buf[0] == 0xAC)                                     //control byte
    {
        if (eeprmwrt_buf[1] <= 60)
            fatwrkcnt20ms = eeprmwrt_buf[1];
        else
            fatclear = 1;
            
        if (eeprmwrt_buf[2] <= 60)
            fatwrksec = eeprmwrt_buf[2];
        else
            fatclear = 1;
        if (eeprmwrt_buf[7] <= 5)
            hassasiyetkdm = eeprmwrt_buf[7];
        else
            hassasiyetkdm = 2;
        fatmaxsure = eeprmwrt_buf[6] << 8;
        fatmaxsure = (fatmaxsure & 0xFF00) | eeprmwrt_buf[5];        
            
        fatwrkmin = eeprmwrt_buf[4] << 8;
        fatwrkmin = (fatwrkmin & 0xFF00) | eeprmwrt_buf[3];        
        
        if (fatwrkmin > fatmaxsure)
            fatclear = 1;
        else if (fatwrkmin == fatmaxsure && fatmaxsure != 0)
            fatfull = 1;      
//        if (fatclear == 1)
//            fatdel();
        carboncnt10ms = eeprmwrt_buf[8];
        carbonwrksec = eeprmwrt_buf[9]; 
        carbonwrkmin = eeprmwrt_buf[10]; 
        carbonwrkhr = eeprmwrt_buf[11];
        carbonwrkhr |= (eeprmwrt_buf[12] << 8) & 0xFF00;
        carbon_filter_timer_value = eeprmwrt_buf[13];
        carbon_filter_timer_value |= (eeprmwrt_buf[14] << 8) & 0xFF00;
        if(eeprmwrt_buf[15] > 0 && eeprmwrt_buf[15] < 60)
            valofacon = eeprmwrt_buf[15];
            
    }
    else
    {
        eeprmwrt_buf[0] = *(volatile uint8 *) &eepromArray[0];  
        eeprmwrt_buf[1] = *(volatile uint8 *) &eepromArray[1];  // 20ms
        eeprmwrt_buf[2] = *(volatile uint8 *) &eepromArray[2];  // sec
        eeprmwrt_buf[3] = *(volatile uint8 *) &eepromArray[3];  // dkLow
        eeprmwrt_buf[4] = *(volatile uint8 *) &eepromArray[4];  // dkHigh
        eeprmwrt_buf[5] = *(volatile uint8 *) &eepromArray[5];  // maxdkLow
        eeprmwrt_buf[6] = *(volatile uint8 *) &eepromArray[6];  // maxdkHigh
        hassasiyetkdm = *(volatile uint8 *) &eepromArray[7];  // maxdkHigh
        
        carboncnt10ms = *(volatile uint8 *) &eepromArray[8];
        carbonwrksec = *(volatile uint8 *) &eepromArray[9];
        carbonwrkmin = *(volatile uint8 *) &eepromArray[10];
        carbonwrkhr = *(volatile uint8 *) &eepromArray[11];
        carbonwrkhr |= (*(volatile uint8 *) &eepromArray[12] << 8) & 0xFF00;
        
        carbon_filter_timer_value = *(volatile uint8 *) &eepromArray[13];
        carbon_filter_timer_value |= (*(volatile uint8 *) &eepromArray[14] << 8) & 0xFF00;
        if(eeprmwrt_buf[15] > 0 && eeprmwrt_buf[15] < 60)
            valofacon = eeprmwrt_buf[15];
        
        if (eeprmwrt_buf[1] <= 60)
            fatwrkcnt20ms = eeprmwrt_buf[1];
        if (eeprmwrt_buf[2] <= 60)
            fatwrksec = eeprmwrt_buf[2];
//        if(fatmaxsure == 0)
//            fatmaxsure = DEFAULTFATDK;

        fatwrkmin = eeprmwrt_buf[4] << 8;
        fatwrkmin = (fatwrkmin & 0xFF00) | eeprmwrt_buf[3];    
    }
    if (hassasiyetkdm > 4 || hassasiyetkdm == 0)
        hassasiyetkdm = 2;
    if(eeprmwrt_buf[15] > 0 && eeprmwrt_buf[15] < 60)
            valofacon = eeprmwrt_buf[15];
}
/*=====================================================================
				*********** SLEEP CONTROL ************			
=====================================================================*/
void sleepcon(void)
{
    if (fe2rec == 1 )
    {
        fe2rec = 0;
        ZERO_ARR(eeprmwrt_buf);         //Toplam 10mSec zaman alir.
        eeprmwrt_buf[0] = 0xAC;             //control byte
        eeprmwrt_buf[1] = fatwrkcnt20ms;
        eeprmwrt_buf[2] = fatwrksec;     
        eeprmwrt_buf[3] = (unsigned char)((fatwrkmin&0x00FF));
        eeprmwrt_buf[4] = (unsigned char)((fatwrkmin&0xFF00)>>8);
        eeprmwrt_buf[5] = (unsigned char)((fatmaxsure&0x00FF));
        eeprmwrt_buf[6] = (unsigned char)((fatmaxsure&0xFF00)>>8);
        eeprmwrt_buf[7] = hassasiyetkdm;
        eeprmwrt_buf[8] = carboncnt10ms;
        eeprmwrt_buf[9] = carbonwrksec;
        eeprmwrt_buf[10] = carbonwrkmin;
        eeprmwrt_buf[11] = carbonwrkhr & 0x00FF;
        eeprmwrt_buf[12] = (carbonwrkhr >> 8) & 0x00FF;
        
        eeprmwrt_buf[13] = carbon_filter_timer_value & 0x00FF;
        eeprmwrt_buf[14] = (carbon_filter_timer_value >> 8) & 0x00FF;
        eeprmwrt_buf[15] = valofacon;
        
        flash_write();
    }
}
/*=====================================================================
				*********** ?R sub ************			
=====================================================================*/
void stc(void)
{
	fstartok = 0;
	fstop1ok = 0;
}
void alc(void)
{
	irdatacnt = 0;
	stc();
}

/*=====================================================================
				*********** ?R READ ************			
=====================================================================*/
void irread(void)
{
	if(ticnt != 255)
		ticnt++;
	if(IR_RX_Read() != 0)
	{
		if (irstt == 0)
		{
			if (++ircnt >= 2)
			{
				ircnt = 0;
				irstt = 1;			//irhigh
			}
		}
		else
			ircnt = 0;
	}
	else					//irlow
	{
		if(irstt == 1)
		{
			if (++ircnt >= 2)
			{
				ircnt = 0;
				irstt = 0;
				if((ticnt >= 97) && (ticnt < 122))			//12ms < ticnt < 15.2ms
				{
					fstartok = 1;
					irdatacnt = 0;
					irdb1 = 0;
					irdb2 = 0;
					irdb3 = 0;
					irdb4 = 0;
 					ircnt20ms = 0;
				}
				else			//startltr
				{
					if(fstartok == 1)
					{
						if((ticnt > 3) && (ticnt < 22))		// 400us < ticnt < 2.8ms
						{
							irdatacnt++;
							irdb1 = irdb1 >> 1;
							if((irdb2 & 0x01) == 0)
								irdb1 &= 0x7F;
							else
								irdb1 |= 0x80;
							irdb2 = irdb2 >> 1;
							if((irdb3 & 0x01) == 0)
								irdb2 &= 0x7F;
							else
								irdb2 |= 0x80;
							irdb3 = irdb3 >> 1;
							if((irdb4 & 0x01) == 0)
								irdb3 &= 0x7F;
							else
								irdb3 |= 0x80;
							irdb4 = irdb4 >> 1;
							if(ticnt <= 13)				//ticnt <= 1.6ms
								irdb4 &= 0x7F;
							 else
								irdb4 |= 0x80;
							if(irdatacnt == 32)
							{
								asm("nop");
								if((irdatacnt == 0x20) && (irdb1 == 0x81) && (irdb2 == 0x66) && !( irdb4 & irdb3 ))
								{
									irbut = irdb3;
									prvirbut = 0;
								}
								else
									irbut = 0;
								stc();
							}
						}
						else 
						{
							asm("nop");
							alc();
						}							
					}
					else				//stopcon
					{
						if(irbut != 0)
						{
							if((ticnt > 64) && (ticnt < 97))		//8ms <= ticnt <= 12ms
							{
								ircnt20ms = 0;
							}
						}
						else
						{
							alc();
							asm("nop");
						}
					}
				}
				ticnt = 0;
			}
		}
		else
			ircnt = 0;
	}
}
/*============================================================================
			*********** INIT WAIT FOR STABİLİZATİON *************
============================================================================*/
void initwait(void)
{
	tmrwait = 60;			// 60 * 10ms
	do
	{
		CySysWdtClearInterrupt();
		if (f20ms == 1)
		{
			f20ms = 0;
			--tmrwait ;
		}
	}
	while (tmrwait != 0);
//	tmrentslp =(unsigned int) (t500msbuf * 8);		//ent sleep after 5sec
}

/*****************************************
	            MAIN
*****************************************/
int main()
{
    
    CyDelay(50u);

//	tmrdspall = 2;
    
//    IrRx_isr_StartEx(ISR_IRRX_SIGNL);
    timer_isr_StartEx(ISR_TIMER_LOC);   /* Enable the Interrupt component connected to interrupt */
    Timer_Start();    	                /* Start the components */
    CyGlobalIntEnable;                  /* Enable the global interrupt */
    initwait(); 
    /* Set WDT reset after counter 1 reach 0x0006 orjinalde EKLENECEK*/
    CySysWdtSetMatch(0x8FFF);       // 31.25us * 20479 = ~640us
    /* Start the WDT */
    CySysWdtEnable();
    UART_Start();                       /* Start SCB UART TX+RX operation */
    read_eeprom();
	tmrtgl = t500msbuf;
    tmrPwronScanPressButton = 40;
//    cnttgl = 0x03;
    for(;;)
    {
		if (f20ms == 1)
		{
			f20ms = 0;
			CySysWdtClearInterrupt();
			dspupdt(reqspd);
			butread();
			irbuteval();
			hrmincon();
			rlymotupdt();
            sleepcon();
            rx_con_reg = UART_SpiUartGetRxBufferSize();     // rx control 
            if(rx_con_reg >= RXDATACNT /*|| ftusilet == 1*/)
            {
//                if (ftusilet == 1)      // data gelmeden data gönderilecek
//                    frxread_ok = 1;
//                else
                    getrxdata();        // tus basimi gönderilirken gelen data önemli değil.
                if (frxread_ok)
                {
                    rxbuf_notzero_cnt = 0;
                    preparetxdata();
                    sendtxdata();
//                    handlesilverconnectcontrol();
//                    curmBufHandle();
                    com_errcnt = 0;
                }
                frxread_ok = 0;
            }
            else if (rx_con_reg != 0)
            {
                if (++rxbuf_notzero_cnt >= 2)
                {
                    UART_SpiUartClearRxBuffer();
                    rxbuf_notzero_cnt = 0;
                }
            }  
            if (fCurrentIsReady == 1)
            {
                fCurrentIsReady = 0;
                handlesilverconnectcontrol();
            }
        }
    }				
}
/* [] END OF FILE */
