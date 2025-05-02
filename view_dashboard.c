/*
 * File:   view_dashboard.c
 * Author: surya
 *
 * Created on 4 April, 2025, 5:21 PM
 */


#include "black_box.h"

extern State_t state;
extern unsigned char head,count;
unsigned char clock_reg[3];
unsigned char time[9];
unsigned short res;
extern int flag;

static const char events[9][3]={"GR","ON","GN","G1","G2","G3","G4","c "};
static int i=1;
void view_dashboard(void)
{
    CLEAR_DISP_SCREEN;
    clcd_print("              ",LINE1(0));
    clcd_print("              ",LINE2(0));
     clcd_print("TIME     EV  SP ",LINE1(0));
     get_time();
     clcd_print(time,LINE2(0));
     clcd_print(events[i],LINE2(9));
    unsigned char key;
    res=read_adc(4);
    res=res/10.23;
    if(res>99)
        res=99;
    if(res<0)
        res=0;
    clcd_putch((char)(res/10)+'0',LINE2(13));
    clcd_putch((char)(res%10)+'0',LINE2(14));
	key = read_switches(STATE_CHANGE);
    
    if(key==1 && i>0)
    {
        event_store();
        count++;
        if(i==2)
        {
            i=i-2;
        }
        else
        {
            i--;
        }
    }
    else if(key==2 && i<6)
    {
        event_store();
        count++;
        if(i==0)
        {
            i=i+2;
        }
        else
        {
            i++;
        }
    }
    else if(key==3)
    {
        event_store();
        count++;
        i=7;   
    }
    else if(key==12)
    {
        state=e_main_menu;
    }
        
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void event_store(void)
{
    
    
    unsigned char add;
    add=head*12;
    for(int i=0;i<8;i++)
    {
        write_ext_eeprom(add++,time[i]);
    }
    write_ext_eeprom(add++,events[i][0]);
    write_ext_eeprom(add++,events[i][1]);
    write_ext_eeprom(add++,(char)(res/10)+'0');
    write_ext_eeprom(add,(char)(res%10)+'0');
    head = (head + 1) % 10;
}

void set_time(void)
{
    unsigned char key;
    key=read_switches(STATE_CHANGE);
    clcd_print("   HH:MM:SS  ",LINE1(0));
    static int min,hr,sec,i=3;
    if(flag)
    {
        hr=((time[0]-48)*10)+(time[1]-48);
        min=((time[3]-48)*10)+(time[4]-48);
        sec=((time[3]-48)*10)+(time[4]-48);
        flag=0;
    }
    
    clcd_putch((hr/10)+'0',LINE2(3));
    clcd_putch((hr%10)+'0',LINE2(4));
    clcd_putch((min/10)+'0',LINE2(6));
    clcd_putch((min%10)+'0',LINE2(7));
    clcd_putch((sec/10)+'0',LINE2(9));
    clcd_putch((sec%10)+'0',LINE2(10));
    static unsigned int count=0;
    if(count<100)
    {   
        clcd_putch(0xff,LINE2(i));
        clcd_putch(0xff,LINE2(i+1));
        count++;
    }
    else if(count<200)
    {
        clcd_putch('0',LINE2(3));
        count++;
    }
    else
        count=0;
    if(key==2)
    {
        i=i+3;
        if(i==12)
            i=3;
    }
    else if(key==1)
    {
        if(i==3)
            hr++;
        else if(i==6)
            min++;
        else if(i==9)
            sec++;
    }
    else if(key==12)
    {
        state=e_main_menu;
    }
    else if(key==11)
    {
        write_ds1307(HOUR_ADDR,((hr/10)<<4)|(hr%10));
        write_ds1307(MIN_ADDR,((min/10)<<4)|(min%10));
        write_ds1307(SEC_ADDR,((sec/10)<<4)|(sec%10));
        clcd_print("  TIME SAVED  ",LINE1(0));
        for(unsigned long int i=1000000;i--;);
        state=e_main_menu;
    }
}
