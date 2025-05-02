/*
 * File:   display_menu.c
 * Author: surya
 *
 * Created on 15 April, 2025, 5:30 PM
 */

#include"black_box.h"
extern State_t state;
extern int flag;
static const char menu[4][16]={"    View Logs   ","    Clear logs ","  Download Logs","     Set Time   "};


void display_main_menu(void)
{
    CLEAR_DISP_SCREEN;
    unsigned char key;
    static int flag=0,i=0;
    if(flag==0)
    {
        clcd_putch('*',LINE1(0));
        clcd_putch(' ',LINE2(0));
    }
    else
    {
        clcd_putch('*',LINE2(0));
        clcd_putch(' ',LINE1(0));
    }
    clcd_print(menu[i],LINE1(1));
    clcd_print(menu[i+1],LINE2(1));
    key = read_switches(STATE_CHANGE);
    
    if(key==MK_SW1)
    {
        if(flag==0)
        {
            i--;
            if(i<=0)
                i=0;
        }
        flag=0;
    }
    else if(key==MK_SW2)
    {
         if(flag)
        {
            i++;
            if(i>=2)
                i=2;
        }
        flag=1;
    }
    else if(key==12)
    {
        state=e_dashboard;
    }
    else if(key==11)
    {
        if(flag==0)
        {
            if(i==0)
                state=e_view_log;
            else if(i==1)
                state=e_clear_log;
            else if(i==2)
                state=e_download_log;
        }
        else
        {
            if(i+1==1)
                state=e_clear_log;
            else if(i+1==2)
                state=e_download_log;
            else if(i+1==3)
            {
                 CLEAR_DISP_SCREEN;
                 flag=1;
                state=e_set_time;
            }
        }
    }
}
