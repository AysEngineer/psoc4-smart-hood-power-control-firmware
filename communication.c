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
#define POLY               0x8408
/*=============================================
            *** GENERATE CRC  ***
===============================================*/
/*
//                                      16   12   5
// this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
// This works out to be 0x1021, but the way the algorithm works
// lets us use 0x8408 (the reverse of the bit pattern).  The high
// bit is always assumed to be set, thus we only use 16 bits to
// represent the 17 bit value.
*/
uint16_t gen_crc16(volatile uint8_t *data_p, uint8_t length)
{
    uint8_t i, j;
    uint8_t mBuffer[length];
    uint8_t tmp;
    uint8_t nible0;
    uint8_t nible1;
    uint16_t crc = 0xffff;

    if (length == 0)
        return (~crc);
    for(i = 0; i < length; i++)
    {
      tmp = *data_p;
      nible0 =(tmp << 4) & 0xF0;
      nible1 = (tmp >> 4) & 0x0F;
      mBuffer[i] = nible0 | nible1;
      ++data_p;
    }
    //microchip POC400 swapped nibbles
    for (i = 0 ; i < length ; i++) {
            crc ^= (uint16_t)(mBuffer[i] & 0xFF);
            for (j = 0 ; j < 8 ; j++) {
                    if ((crc & 0x0001) > 0)
                            crc = (crc >> 1) ^ POLY;
                    else
                            crc = (crc >> 1);
            }
    }
    crc &= 0xFFFF;
    return (crc);
}
/*=============================================
            *** CHECK REC?EVED DATA  ***
===============================================*/
void checkrxdata(void)
{
    uint16_t crc16 = 0xFF;
    frxread_ok = 0;     // data read succesfull
    crc16 = rxdatabuf[RXDATACNT - 1] ^ rxdatabuf[RXDATACNT - 2] ^ rxdatabuf[RXDATACNT - 3] ^ rxdatabuf[RXDATACNT - 4];
    if (crc16 == 0)
    {       
        if ((rxdatabuf[0] == 0x00) && (rxdatabuf[1] == 0x00)&& (rxdatabuf[2] == 0x00) && (rxdatabuf[3] == 0x00))
        {
            frxread_ok = 1;
            MessageTypeRX = rxdatabuf[4];
            Byte1_RX = rxdatabuf[5];
            Byte2_RX = rxdatabuf[6];
        }
//        else
//        {
//            // Bozuk data gelmesi sonucu
//            MessageTypeRX = 0x00;
////            Byte1_RX = rxdatabuf[5];
////            Byte2_RX = rxdatabuf[6];
//        }
    }
}
/*=============================================
            *** PREPARE TX DATA  ***
===============================================*/
void preparetxdata(void)
{
    txdata[0] = 0x00;
    txdata[1] = 0x00;
    txdata[2] = 0x00;
    txdata[3] = 0x00;
    txdata[4] = 0x00;
    txdata[5] = 0x00;
    txdata[6] = 0x00;
    txdata[7] = 0xFF;
    MessageType = 0x00;
    Requested_A = 0x00;
    Requested_B = 0x00;  
    if (MessageTypeRX != 0x00)
    {
        if (MessageTypeRX == 0x01)          // Cihaz türü gönderiliyor.
        {
            MessageType = 0x02;
            Requested_A = 0x00;
            Requested_B = 0x01;
        }
        else if (MessageTypeRX == 0x03)     // SİLVERLİNE KODU GÖNDERILIYOR 6 / 1  C457.1002.38.00
        {
            MessageType = 0x04;     // ASCII 0x43 = C -- 0x48 = H -- 0x4E = N -- 0x30 = 0 -- 0x31 = 1 -- 0x39 = 9
            Requested_A = 0x43;     // C457.1002.38.00
            Requested_B = 0x34; 
        }
        else if (MessageTypeRX == 0x05)     // SİLVERLİNE KODU GÖNDERILIYOR 6 / 2
        {
            MessageType = 0x06;
            Requested_A = 0x35;
            Requested_B = 0x37; 
        }
        else if (MessageTypeRX == 0x07)     // SİLVERLİNE KODU GÖNDERILIYOR 6 / 3
        {
            MessageType = 0x08;
            Requested_A = 0x31;
            Requested_B = 0x30; 
        }
        else if (MessageTypeRX == 0x09)     // SİLVERLİNE KODU GÖNDERILIYOR 6 / 4
        {
            MessageType = 0x0A;
            Requested_A = 0x30;
            Requested_B = 0x32; 
        }
        else if (MessageTypeRX == 0x0B)     // SİLVERLİNE KODU GÖNDERILIYOR 6 / 5
        {
            MessageType = 0x0C;
            Requested_A = 0x33;
            Requested_B = 0x38; 
        }
        else if (MessageTypeRX == 0x0D)     // SİLVERLİNE KODU GÖNDERILIYOR 6 / 6
        {
            MessageType = 0x0E;
            Requested_A = 0x30;
            Requested_B = 0x30; 
        }
        else if (MessageTypeRX == 0x0F)     // PROTOKOL VERSIYONU GONDERILIYOR
        {
            MessageType = 0x10;
            Requested_A = 0x04;     //  A= Noktanın Solu B= Noktanın Sağı 
            Requested_B = 0x00; 
        }
        else if (MessageTypeRX == 0x11)     // Maksimum SPD gönderiliyor
        {
            MessageType = 0x12;
            Requested_A = 0x00;     // 16Bit AB olarak gonderiliyor.
            Requested_B = MAXSPD;  
        }
        else if (MessageTypeRX == 0x13)     // Maksimum IŞIK gönderiliyor.
        {
            MessageType = 0x14;
            Requested_A = 0x00;
            Requested_B = 0x01;     // 16Bit AB olarak gonderiliyor.
        }
        else if (MessageTypeRX == 0x15)     // Maksimum TIMER gönderiliyor.
        {
            MessageType = 0x16;
            Requested_A = 0xFF;     // A = Dakika = 255 Dakika
            Requested_B = 0x3B;     // B = Saniye = 0x3B = 59 Saniye
        }
//        else if (MessageTypeRX == 0x17)     // Support for automatic control of ventilation speed and light intensity is returned.
//        {
//            MessageType = 0x18;                 //AB= Support for automatic control of ventilation speed and light intensity, concatenated as 16-bit value. A is the most significant byte, B is the least significant byte.
//            Requested_A = 0x00;                                     // Bit order: 15->0; Bits are set if the statements are TRUE.
//            Requested_B = 0x07;                                     // Bit0: Automatic ventilation control according to current measurement (Hob to Hood control) is supported
//                                                                    // Bit1: Automatic ventilation control according to air-quality sensor information is supported.
//                                                                    // Bit2: Automatic ventilation control according to active timer (air clean) is supported.
//                                                                    // Bit3: Automatic light intensity control is supported. (Note: Current appliances does not support this feature.)            
//        }
        else if (MessageTypeRX == 0x17)     // Lighting Features ve Moving özelliklerini destekliyor mu?
        {
            MessageType = 0x18;     //A = 0x01 Grease filter destekleniyor
            Requested_A = 0x03;     //A = 0x02 Carbon Filter destekleniyor
            Requested_B = 0x00;     //A = 0x04 Moving up/down destekleniyor
                                    //B = 0x00 Lighting feature: (Classic)
                                    //B = 0x01 Lighting feature: (Lightline))
                                    //B = 0x02 Lighting feature: (Star Lighting)
        }         
        
        /****************************************************************
        ***************rutin gönderilen paket****************************/
        else if (MessageTypeRX == 0x31)     // Mevcut SPD gönderiliyor.
        {
            MessageType = 0x32;
            Requested_A = 0x00;   // 16Bit AB olarak gonderiliyor.
            Requested_B = reqspd;   
        }
        else if (MessageTypeRX == 0x33)     // Mevcut IŞIK gönderiliyor.
        {
            MessageType = 0x34;
            Requested_A = 0x00;   // 16Bit AB olarak gonderiliyor.
            Requested_B = lmpstt;   
        }
        else if (MessageTypeRX == 0x35)     // Mevcut TIMER gonderiliyor.
        {
            MessageType = 0x36;     // Geçerli Fan Zamanlayıcı durumu döndürülür. 
            Requested_A = 0x00;     // Fan Zamanlayıcı etkin değilse, A = 0 ve B = 0 olur.
            Requested_B = 0x00;
            if (!(tmroff == 0 && tmrof1s == 0))
            {
                if(reqspd < MAXSPD /*&& faircmod == 0*/)
                {
                    Requested_A = tmroff;   // Fan zamanlayıcı etkinleştirilmişse, 
                    Requested_B = tmrof1s;  // A dakika sayısı, B saniye sayısıdır.
                }
            }
        }
        else if (MessageTypeRX == 0x37) // Mevcut FİLTRE ZAMANI gönderiliyor.
        {
            MessageType = 0x38;
//            Requested_A = (unsigned char)((fatwrkmin&0xFF00)>>8);     //16Bit AB olarak gonderiliyor. Dakika olarak
//            Requested_B = (unsigned char)((fatwrkmin&0x00FF)); 
            if (Byte2_RX == 0x00)
            {
                uint16 _filterTimerValueMin;
                if(fatmaxsure == 0)
                    _filterTimerValueMin = fatwrkmin;    
                else
                    _filterTimerValueMin = (fatmaxsure - fatwrkmin);
                Requested_A = (unsigned char)((_filterTimerValueMin&0xFF00)>>8);     //16Bit AB olarak gonderiliyor. Dakika olarak
                Requested_B = (unsigned char)((_filterTimerValueMin&0x00FF));                  
            }
            else if(Byte2_RX == 0x01)
            {
                uint16 _carbonfilterTimerValueMin;
                if(carbon_filter_timer_value == 0)
                   _carbonfilterTimerValueMin = carbonwrkhr;
                else
                    _carbonfilterTimerValueMin = (carbon_filter_timer_value - carbonwrkhr);
                Requested_A = (unsigned char)((_carbonfilterTimerValueMin&0xFF00)>>8);     //16Bit AB olarak gonderiliyor. Dakika olarak
                Requested_B = (unsigned char)((_carbonfilterTimerValueMin&0x00FF));                 
            }
        }
        else if (MessageTypeRX == 0x39)     // IOT TUSUNA Basma flagi gönderiliyor.
        {
            MessageType = 0x3A;
            Requested_A = 0x00;             // Burası 0x00 olacak
            Requested_B = 0x00;
            if(fiotlongpres == 1)
            {
                Requested_B = fiotlongpres;
                fiotlongpres = 0;
            }
//            if (fiotlongpres != 0)
//            {
//                if (fiotclear == 0)
//                    fiotclear = 1;
//                else
//                {
//                    fiotlongpres = 0;
//                    fiotclear = 0;
//                }
//            }
//            else
//                fiotclear = 0;
//            Requested_B = fiotlongpres;
        }
        else if (MessageTypeRX == 0x3B)     // Cihaz hakkında bilgi isteniyor.
        {
            MessageType = 0x3C;
            Requested_A = 0x00;                     // 16Bit AB olarak gonderiliyor.
            Requested_B = 0x00; 
            
//            if (reqpstbuf > MAXSPD)
//                Requested_B |= 0x02;                //Bit1: Cihaz servis modunda. (Popout'a özel, diğer ürünlere genişletilebilir)
            
            if ((reqspd != 0 && reqspd <= MAXSPD) || lmpstt != 0)
                Requested_B |= 0x01;                //Bit0: Cihaz etkin. (Havalandırma veya aydınlatma çalışıyor)
            if(fauto != 0)
            {
                 Requested_B |= 0x04;                //Bit2: Cihaz, sensör tabanlı, otomatik havalandırma kontrol modunda.
            }
            if(faircmod != 0)
            {
                  Requested_B |= 0x08;                //Bit3: Cihaz otomatik ventilasyon kontrol modunda, aktif timer bazlı.
            }
            //Bit2: Cihaz, sensör tabanlı, otomatik havalandırma kontrol modunda.
            //Bit3: Cihaz otomatik ventilasyon kontrol modunda, aktif timer bazlı.
            //Bit4: Cihaz otomatik aydınlatma modunda.
            //Bit5: Bu bit “Window Switch” durumu için ayrılmıştır.
            //Bit6-15: Daha fazla revizyon için ayrılmıştır.
        }
        else if (MessageTypeRX == 0x3D)     // Yeni RGB değerleri set ediliyor.
        {
            MessageType = 0x3E;         // Byte1_RX = IAQ and Byte2_RX = humidity
            Requested_A = 0x00;
            Requested_B = 0x00;
//            if (Byte1_RX > 100)
//            {
//                Requested_B = 0x01;
//            }
//            else if (Byte1_RX >= 90)         //100% - 90% YESIL(0-255-0)
//            {                           // 89% - 80% SARI(255-255-0)
//                RGB_RValue = 0;         // 79% - 70% SARI(255-255-0)
//                RGB_GValue = 255;       // 69% - 60% KIRMIZI(255-0-0)
//                RGB_BValue = 0;         // 59% - 40% KIRMIZ(255-0-0)
//            }                           // 39% -  0% KIRMIZI(255-0-0)   
//            else if (Byte1_RX >= 70)
//            {
//                RGB_RValue = 255;
//                RGB_GValue = 255;
//                RGB_BValue = 0;
//            }
//            else
//            {
//                RGB_RValue = 255;
//                RGB_GValue = 0;
//                RGB_BValue = 0;
//            }
        }
        else if (MessageTypeRX == 0x3F)     // IOT KENDI DURUMU hakkında bilgiler veriyor
        {
            MessageType = 0x40;
            
            Requested_A = 0x00;
            Requested_B = 0x00;
//            fIOTLED1sn = 0;
//            fIOTLED500ms = 0;
            if ((Byte2_RX & 0x01) != 0)
            {
                fairqualitymode = 1;
                fairc = 0;
                faircmod = 0;
                fauto = 0;
                ClearWorkTime();
            }
            else
            {
                if(fairqualitymode == 1)
                {
                    fairqualitymode = 0;
                    ClearWorkTime();
                }
            }
//            else if ((Byte2_RX & 0x01) == 0)
//            {
//                fauto = 0;
////                fRGBstt = 1;
////                fRGBLED2sn = 0;
////                tmrbzrld();
//            }
            
//            if ((Byte2_RX & 0x02) != 0)
//            {
//                fIOTLEDstt = 1;     //Bit1: Cihaz normal modda.(IotC is in regular mode.)
////                tmrbzrld();
//            }
//            else if ((Byte2_RX & 0x04) != 0)
//            {
//                fIOTLEDstt = 0;
////                fIOTLED1sn = 1;     //Bit2: Cihaz çift modunda.(IotC is in pair mode.)
////                tmrbzrld();
//            }
//            else if ((Byte2_RX & 0x08) != 0)
//            {
//                fIOTLEDstt = 0;
////                fIOTLED500ms = 1;   //Bit3: Cihaz üretici yazılımı güncelleme modunda.(IotC is in firmware update mode.)
////                tmrbzrld();
//            }
//            else
//            {
//                fIOTLEDstt = 0; 
////                tmrbzrld();
//            }
            
            
            //Bit4: Cihaz erişim noktasına bağlanır, yalnızca Bit1 ayarlanmışsa geçerlidir.
            //Bit5: Cihaz bulut hizmetine bağlı, yalnızca Bit1 ve Bit4 ayarlanmışsa geçerlidir. 
        }
        else if (MessageTypeRX == 0x41)     // airqulitiy mode bilgisi gönderiliyor.
        {
            MessageType = 0x42;
            Requested_A = 0x00;             // Burası 0x00 olacak
            Requested_B = fairqualitymode;
        }
        else if (MessageTypeRX == 0x4B)          //Maximum filter time value is requested
        {
            MessageType = 0x4C;
            if(Byte2_RX == 0x00)
            {
                Requested_A = (unsigned char)((fatmaxsure&0xFF00)>>8);     //16Bit AB olarak gonderiliyor. Dakika olarak
                Requested_B = (unsigned char)((fatmaxsure&0x00FF)); 
            }
            else if (Byte2_RX == 0x01)
            {
                Requested_A = (unsigned char)((carbon_filter_timer_value&0xFF00)>>8);     //16Bit AB olarak gonderiliyor. Dakika olarak
                Requested_B = (unsigned char)((carbon_filter_timer_value&0x00FF)); 
            }
        }
        else if (MessageTypeRX == 0x4D)          //Error code of AC is requested
        {
            MessageType = 0x4E;
            Requested_A = 0x00;
            Requested_B = 0x00;
        }
        else if (MessageTypeRX == 0x4F)         //Hob to hood setting value is requested
        {
            MessageType = 0x50;
            Requested_A = 0x00;
            Requested_B = hassasiyetkdm;            //A = Hob to hood setting value.
        }        
        else if (MessageTypeRX == 0x51)         // Active timer (air clean) setting value is requested
        {
            MessageType = 0x52;                 //A = The time value that the ventilation will be ON in a 60-minute cycle. 
                                                //    The value is in minutes. Range of the value is 1-59.
            Requested_A = 0x00;
            Requested_B = valofacon;
        }   
        //***********************************************************//
        //***********************************************************//        
        
        else if (MessageTypeRX == 0x71)     // Yeni SPD set ediliyor.(16Bit AB olarak gonderiliyor.)
        {
            MessageType = 0x72;
            Requested_A = 0x00;
            if (Byte2_RX <= MAXSPD)
            {
                fremdat = 1;
                if(Byte2_RX == 0 && reqspd != 0) // mesela 3. devirdesin ve sana 0 komutu geldi ise buraya girer.
                {
                    mobprs();
                }
                    motbutprs(Byte2_RX);
                    Requested_B = 0x00;         //Ack == 0x00 Yeni havalandırma hızı ayarlandı. 
                    loadDisplayDark();
                 
            }
            else
            {
                Requested_B = 0x01;             //Ack == 0x01 İstenen havalandırma hızı aralık dışında.
            }
        }
        else if (MessageTypeRX == 0x73)     // Yeni IŞIK set ediliyor. (16Bit AB olarak gonderiliyor.)
        {
            MessageType = 0x74;
            Requested_A = 0x00;
            if (Byte2_RX <= 1)
            {
                lmpstt = Byte2_RX;
                ClearWorkTime();
                Requested_B = 0x00;         //Ack == 0x00 Yeni aydınlatma yoğunluğu ayarlandı.
                fremdat = 1;
                tmrbzrld();
            }
            else
            {
                Requested_B = 0x01;             //Ack == 0x01 İstenen aydınlatma yoğunluğu aralık dışında.
            }
        }
        else if (MessageTypeRX == 0x75)     // Yeni TIMER set ediliyor.
        {
            MessageType = 0x76;     
            Requested_A = 0x00;     
            Requested_B = 0x00;     
            if (Byte1_RX != 0 || Byte2_RX != 0)
            {
                if (reqspd != 0 && reqspd < MAXSPD && (faircmod == 0 || (faircmod == 1 && fauto == 1)))
                {
                    tmroff = Byte1_RX;      // A dakikadır. B saniye sayısıdır.
                    if (Byte2_RX > 59)
                        Requested_B = 0x01;     //Ack == 0x01 Değer aralık dışında.
                    else
                        tmrof1s = Byte2_RX;     //  A ve B 0 ise, zamanlayıcı, fan durumunu değiştirmeden devre dışı bırakılmalıdır.
                    
                    if (Requested_B == 0 && Requested_A == 0)
                    {
//        				tmrof20ms = 0;			
//        				tglf = 0xFF;
//        				tmrtgl = t500msbuf;		//50 * 10 = 0.5s
                    }
                    else
                    {
                        tmrof1s = 0;                     
                        tmroff = 0;
                    }
                    //Requested_B = 0x02;        //Ack == 0x02 Değer ayarlanamıyor.
//                     tmrbzrld();
                }
                else                
                    Requested_B = 0x02;     //Ack == 0x02 Değer ayarlanamaz.
                
                if(Requested_B == 0x00)
                {
                    fremdat = 1;
                    tmrbzrld();
                }
            }
            else
            {
                tmroff = 0;
                tmrof1s = 0;
                tmrbzrld();
            }
            ClearWorkTime();
            ModeOff();
        }
        else if (MessageTypeRX == 0x77) // FILTRE temizleme.
        {
            MessageType = 0x78;
            if (Byte2_RX == 0x00)    // Başarılı
            {
                fatdel();
                Requested_A = 0x00;
                Requested_B = 0x00;
                fremdat = 1;
                tmrbzrld();
                ClearWorkTime();
            }
            else if(Byte2_RX == 0x01)
            {
                carbonfilterdel();
                Requested_A = 0x00;
                Requested_B = 0x00;
                fremdat = 1;
                tmrbzrld();
                ClearWorkTime();
            }
            else
            {
                Requested_A = 0x00;
                Requested_B = 0x01;
            }
        }
        else if (MessageTypeRX == 0x79)     // Yeni OTOMATIK MOD durumu güncelleniyor.
        {
            MessageType = 0x7A;
            //Byte1_RX = A == 0x00 AUTO devre dışı bırak.
            //Byte1_RX = A == 0x01 AUTO aktif yap.
            //Byte2_RX = B == 0x00 AKTIF TAMIR devre dışı bırak.
            //Byte2_RX = B == 0x01 AKTIF TAMIR aktif yap.
            if(Byte1_RX == 1 && fauto == 0)   // Auto kapat komutu geldi.
            {
                curmPressAuto = 0x01; 
            }
            else if(Byte1_RX == 0 && fauto == 1)
            {
                curmPressAuto = 0x01; 
            }
            if(Byte2_RX == 1 && faircmod == 0)
            {
                curmPressAirmode = 0x01;
            }
            else if(Byte2_RX == 0 && faircmod == 1)
            {
                curmPressAirmode = 0x01;
            }
            curmBufHandle();    // mecburen buraya koydum. Auto ve air durum flagi hemen degerlendirilip durum atılmalı.
            Requested_A = 0x00;             
            Requested_B = 0x00; 
            if(fauto != 0)
            {
                Requested_A = 1; 
            }
            if(faircmod != 0)
            {
                Requested_B = 1; 
            }
            //A == 0x00 Sensöre göre otomatik havalandırma devre dışı.
            //A == 0x01 Sensöre göre otomatik havalandırma etkin.
            //B == 0x00 Aktif bir zamanlayıcı tarafından kontrol edilen otomatik havalandırma devre dışı.
            //B == 0x01 Aktif bir zamanlayıcı tarafından kontrol edilen otomatik havalandırma etkin.
        }
        else if (MessageTypeRX == 0x7B)     // Yeni OTOMATIK ISIK MOD durumu güncelleniyor.
        {
            MessageType = 0x7C;
            //A == 0x00 Otomatik aydınlatmayı devre dışı bırak.
            //A == 0x01 Otomatik aydınlatmayı etkinleştir.
            Requested_A = 0x00;             
            Requested_B = 0x00;  
            //A == 0x00 Otomatik aydınlatma devre dışı.
            //A == 0x01 Otomatik aydınlatma etkin.
        }  
        else if (MessageTypeRX == 0x7D)     // Güncel akım bilgisi
        {
            MessageType = 0x7E;
            Requested_A = 0x00;  
            presentCurrentValue = Byte1_RX << 8;
            presentCurrentValue = (presentCurrentValue & 0xFF00) | Byte2_RX; 
            if(/*presentCurrentValue > 20 &&*/ maxCurrentValue > 200)
            {    
//                if(++cntCurrentAverage > 5)
//                {
//                    cntCurrentAverage = 0;      
//                    akim_totadc = totalAverageValue / 6;
                fCurrentIsReady = 1;
                akim_totadc = presentCurrentValue;
//                }
//                else
//                {
//                    totalAverageValue = +presentCurrentValue;
//                }
            
                Requested_B = 0x00;  
            }
            else
            {
                Requested_B = 0x01;
            }
        }  
        else if (MessageTypeRX == 0x7F)     // maksimum akım bilgisi
        {
            MessageType = 0x80;
            Requested_A = 0x00;  
            maxCurrentValue = Byte1_RX << 8;
            maxCurrentValue = (maxCurrentValue & 0xFF00) | Byte2_RX; 
            if(maxCurrentValue > 200)
            {             
                Requested_B = 0x00;  
            }
            else
            {
                Requested_B = 0x01;
            }
        }
        else if (MessageTypeRX == 0x89)        //New maximum grease filter timer value is sent
        {
            uint16_t ex_fatmaxsure = 0;
            MessageType = 0x8A;
            float oran = 0;
            
            ex_fatmaxsure = fatmaxsure;
            fatmaxsure = Byte1_RX << 8;
            fatmaxsure = (fatmaxsure & 0xFF00) | Byte2_RX;  
            oran = ((float)fatmaxsure / (float)ex_fatmaxsure);
            
            if(ex_fatmaxsure != 0)
                oran = ((float)fatmaxsure / (float)ex_fatmaxsure);
            else
                oran = 1;
            fatwrkmin = (uint16_t)(fatwrkmin * oran);
            
            Requested_A = 0x00;
            Requested_B = 0x00;            
        }
        else if (MessageTypeRX == 0x8B) // New maximum carbon filter timer value is sent
        {
            MessageType = 0x8C;
            
            float oran = 0;
            
            ex_carbon_filter_timer_value = carbon_filter_timer_value;
            carbon_filter_timer_value = Byte1_RX << 8;
            carbon_filter_timer_value = (carbon_filter_timer_value & 0xFF00) | Byte2_RX;
            if(ex_carbon_filter_timer_value != 0)
                oran = ((float)carbon_filter_timer_value / (float)ex_carbon_filter_timer_value);
            else
                oran = 1;
            carbonwrkhr = (uint16_t)(carbonwrkhr * oran);
            if(carbon_filter_timer_value == 0)
                carbonfilterdel();
            
            Requested_A = 0x00;             
            Requested_B = 0x00;
        }
        else if (MessageTypeRX == 0x8D)                  // New hob to hood setting is sent.
        {
            fe2rec = 1;
            MessageType = 0x8E;
            Requested_A = 0x00;
            
            if(Byte2_RX > 0 && Byte2_RX < 5)
            {
                hassasiyetkdm = Byte2_RX;
                Requested_B = 0x00;
            }
            else
            {
                Requested_B = 0x01;
            }
        }
        else if (MessageTypeRX == 0x8F)               //New active timer (air clean) setting is sent.
        {
            fe2rec = 1;
            MessageType = 0x90;
            Requested_A = 0x00;
            
            if(Byte2_RX > 0 && Byte2_RX < 60)
            {
                valofacon = Byte2_RX;
                Requested_B = 0x00;
            }
            else
            {
                Requested_B = 0x01;
            }
        }        
        else
        {
            MessageType = 0xFF;             // Farklı anlaşılmayan bir mesaj geldiğinde 
            Requested_A = MessageTypeRX;    // anlaşılmayan gelen MessageType 
            Requested_B = 0x00;               
        }
    }
    txdata[4] = MessageType;
    txdata[5] = Requested_A;
    txdata[6] = Requested_B;        
    txdata[7] = txdata[RXDATACNT - 2] ^ txdata[RXDATACNT - 3] ^ txdata[RXDATACNT - 4];
    MessageTypeRX = 0;
    Byte1_RX = 0;
    Byte2_RX = 0;
}
/*=============================================
           *** GET RECEIVED DATA ***
===============================================*/
void getrxdata (void)
{
    rxdatacntr = 0;
    while(rxdatacntr < RXDATACNT)
    {
       rxdatabuf[rxdatacntr] = UART_UartGetChar();     //rxdatabuf[RXDATACNT] RXDATACNT 8 'den kücük cntr artarken GetChar calissin.
       ++rxdatacntr;
       if(rxdatacntr == RXDATACNT)    //rxdatacntr 8 olunca
       {
           checkrxdata();    //rxdatayı check eden fonksiyonu çalıştır.
       }
    }
    UART_SpiUartClearRxBuffer();
}
/*=============================================
           *** SEND TRANC?EVED DATA ***
===============================================*/
void sendtxdata(void)
{
    UART_SpiUartClearTxBuffer();
    UART_SpiUartPutArray(&txdata[0], TXDATACNT);
}

/* [] END OF FILE */
