/*
 * File:   main.c
 * Author: surya
 *
 * Created on 3 April, 2025, 6:31 PM
 */


#include "black_box.h"

State_t state;
unsigned char head=0,count=0;
int flag=0;

void init_config()
{
    init_adc();
    init_clcd();
    init_matrix_keypad();
    init_i2c();
	init_ds1307();
    state = e_dashboard;
}

void main(void) 
{
    init_config();

    while(1)
    {
        // Detect key press
        switch (state)
        {
           
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;
            
            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;
            
            case e_view_log:
                // Display dashboard
                view_log();
                break;
                 
            case e_download_log:
                download_log();
                break;
                
            case e_clear_log:
                clear_log();
                break;
                
                      
            case e_set_time:
                set_time();
                break;
                
        }
        
    }
    
}
