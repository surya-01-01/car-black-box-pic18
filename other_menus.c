/*
 * File:   view_log.c
 * Author: surya
 *
 * Created on 18 April, 2025, 12:23 PM
 */


#include"black_box.h"
char read_event[10][16];
extern unsigned char head,count;
extern State_t state;
void view_log(void)
{
    CLEAR_DISP_SCREEN;
    unsigned char key;
    key = read_switches(STATE_CHANGE);
    static int i=0;
    if(count==0)
    {
        clcd_print("  TIME     EV SP",LINE1(0));
        clcd_print(" NO LOGS FOUND ",LINE2(0));
        if(key==12)
        {
            state=e_main_menu;
        }
        return;
    }
    event_reader();
    clcd_print(" TIME     EV SP",LINE1(0));
    clcd_print(read_event[i],LINE2(0));
    if(key==1 && i>0)
    {
        i--;
    }
    else if(key==2 && i<10)
    {
        i++;
    }
    else if(key==12)
    {
        state=e_main_menu;
    }
    
}

void event_reader(void)
{
   
   
    unsigned char start = (count > 10) ? head : 0;

    for (int i = 0; i < (count < 10 ? count : 10); i++)
    {
        unsigned char ev_add = ((start + i) % 10) * 12;

        read_event[i][0] = i + '0'; 
        int k = 1; 

        for (int j = 0; j < 12; j++) 
        {
            if (k == 9 || k == 12)
            {
                read_event[i][k++] = ' ';
            }
            read_event[i][k++] = read_ext_eeprom(ev_add++);
        }

        read_event[i][k] = '\0'; 
    }
    
    
}



void download_log(void)
{
    CLEAR_DISP_SCREEN;
    clcd_print("  download_log ", LINE1(4));
     for(unsigned long int i=1000000;i--;);
    if(count==0)
    {
        uart_puts("LIST EMPTY\n\r");
        for(unsigned long int i=1000000;i--;);
        state=e_main_menu;
    }
    else
    {
        event_reader();
        uart_puts("download\n\r");
        unsigned char start = (count > 10) ? head : 0;
         for (int i = 0; i < (count < 10 ? count : 10); i++)
         {
            uart_puts(read_event[i]); // Send event
            uart_putch('\n'); // New line
            uart_putch('\r'); // Carriage return 
         }
         for(unsigned long int i=1000000;i--;);
            state=e_main_menu;
    }
}


void clear_log(void)
{
    head=0;
    count=0;
    CLEAR_DISP_SCREEN;
    clcd_print(" LOGS CLEARED  ",LINE2(0));
    for(unsigned long int i=500000;i--;);
    state=e_main_menu;
}

