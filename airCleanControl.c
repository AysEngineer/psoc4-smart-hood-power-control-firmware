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
#include <main.h>

void airoffauto(void)
{
//    jmpk34();
    if ((faircmod != 0) && (fairc != 0))	
	{
		fairc = 0;
		acled = 0;
		fairc20s = 0;
		tmrairc20s = 0;
		tmrac200ms = 0;
		tmrofacon = 0;					//default 5dk airclean on olacak
		tmrofacoff = 0;					//Off olduktan sonra airclean 60 dk off kalacak
//        fairclock = 0;
	}
}
void aironauto(void)
{
//    jmpk34();
	if ((faircmod != 0) && (fairc == 0))
	{
		fairc = 1;
		airsec = 0;
		aircnt10ms = 0;
        aircnt10ms_2 = 0;
		tmrofacon = valofacon;
		tmrofacoff = 60-valofacon;			//orjinal 60
		aircfunc();		
	} 
}

void aircfunc(void)
{
	if (valofacon == 20)
	{
		acledcnt = 4;
	}
	else if (valofacon == 15)
	{
		acledcnt = 3;
	}
	else if (valofacon == 10)
	{
		acledcnt = 2;
	}
	else
	{
		acledcnt = 1;
	}
	acled = 0;
	tmrairc20s = 20;
	fairc20s = 1;
	tmrac200ms = 0;
}
void aircleanTimeDecreasing(void)
{
    if (reqspd != 0)
    {
        if (!(tmroff == 0 && tmrof1s == 0))
    	{
    		++tmrof10ms;
    		if (tmrof10ms >= t1secbuf)
    		{
    			tmrof10ms = 0;
                if (tmrof1s != 0)
                {
        			if (--tmrof1s == 0)				
        			{
        				if (tmroff == 0)
        				{
        					reqspd = 0;
        					if (fauto)
                            {
                                if (fautotimeroff == 1)
                                    fautotimeroff = 0;
                                else
                                {
                                    fauto = 0;
                                    ftmrautooff = 1;
                                }
                            }
        					lmpstt = 0;	
                            aironauto();
        				}
        			}
                }
                else if (tmroff != 0)
                {
                    --tmroff;
                    tmrof1s = 59;
                }
    		}
    	}
    }
    else
    {
        tmroff = 0;
        tmrof1s = 0;
        tmrof10ms = 0;
    }
    if(++aircnt10ms_2 >= t1secbuf)
	{
		aircnt10ms_2 = 0;
		if (tmrairc20s != 0)
		{
			--tmrairc20s;
			if (tmrairc20s == 0)
			{
				aircfunc();
			}				
		}
	}
    if (fairc == 1)
	{
		if (fairc20s == 1)
		{
			if (tmrac200ms != 0)
				--tmrac200ms;
			if (tmrac200ms == 0)
			{
				if (acled == 4)
				{
					acled = 0;
					if (acledcnt != 0)
						--acledcnt;
					if (acledcnt == 0)
						fairc20s = 0;
				}
			}
		}		
		if(++aircnt10ms == t1secbuf)
		{
			aircnt10ms = 0;
            if(tmrairc > 0)   
            {
    			if (++airsec == 60)				//orjinal 60   demo 2
    			{
    				airsec = 0;
    				if (tmrofacon != 0)
    				{
    					--tmrofacon;
    					if (tmrofacon == 0)
    					{
                            --tmrairc;
    						tmrofacoff = 60-valofacon;		//orjinal 60
    						reqspd = 0;
    						//loadDisplayDark();
    					}
    				}	
    				else if (tmrofacoff != 0)
    				{
    					--tmrofacoff;
    					if (tmrofacoff == 0)
    					{
    						tmrofacon = valofacon;
    						reqspd = 1;
    						actspd = 2;
    						tmrmot = t500msbuf;		//50*10ms=500ms
    //                        jmpk34();
    					}					
    				}
    			}
            }
		}
	}
}
/* [] END OF FILE */
