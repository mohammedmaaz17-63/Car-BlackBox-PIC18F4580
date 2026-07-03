/*
 * File:   main.c
 * Author: moham
 *
 * Created on 30 June, 2026, 6:49 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "uart.h"
#include "ext_eeprom.h"

unsigned char clock_reg[3];
unsigned char time[9];
char key;
char Gear[3];
char str_speed[4];
char screen=HOME_SCREEN;

void display_time(void)
{
	clcd_print(time, LINE2(0));
}
void display_gear(char *Gear)
{
    clcd_print(Gear,LINE2(10));
}
void display_speed(char *speed)
{
    clcd_print(speed,LINE2(13));
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


unsigned char get_gear_pos()
{
    // Implement the gear function
    //char key=read_switches(STATE_CHANGE);
    static unsigned char gear='N';
    if(key==MK_SW1)
    {
        if(gear=='N')
        {
            gear='1';
        }
        
        else if(gear>='1' && gear<'6')
        {
            gear++;
        }
        else
        {
            gear='R';
        }
        return gear;
    }
    else if(key==MK_SW2)
    {
        if(gear=='1')
        {
            gear='N';
        }
        else if(gear=='R')
        {
            gear='6';
        }
        else if(gear>'1')
        {
            gear--;
        }
        else
        {
            gear='N';
        }
        return gear;
    }
    return gear;
}

void my_itoa(int speed,char str_speed[])
{
    str_speed[0]=(speed/100)+'0';
    str_speed[1]=((speed/10)%10)+'0';
    str_speed[2]=(speed%10)+'0';
    str_speed[3]='\0';
}

int get_speed()
{
    // Implement the speed function
    int value=read_adc(CHANNEL4);//0to5v will be converted into 0 to 1023 here
    unsigned long int speed=(value*200)/1023;
    if(speed<0)
        speed=0;
    if(speed>200)
        speed=200;
    return speed;
}



void my_strcpy(char *dest,char*src)
{
    int i=0;
    while(dest[i]=src[i])
    {
        i++;
    }
}

 int my_strcmp(char *str1,char *str2)
{
    int i=0;
    while(str1[i]==str2[i])
    {
        if(str1[i]=='\0')
        {
            return 0;
        }
        i++;
    }
    return 1;
}


//void view_log(void);
//create a array of string where we can store s.no time gear and speed
char def_gear[]="GN";
char store_log[]="0 00:00:00GN 000";

//char database[10][17];
int database_count=0;

void init_database(void)
{
    char temp[17];

    database_count = 0;

    // Count valid logs stored in EEPROM
    for(unsigned char i = 0; i < 10; i++)
    {
        read_ext_eeprom_array(i * 16, temp, 16);

        // Check if the first character is a valid serial number
        if(temp[0] < '0' || temp[0] > '9')
        {
            break;
        }

        database_count++;
    }

    // EEPROM is empty
    if(database_count == 0)
    {
        get_time();

        store_log[0] = '0';

        my_strcpy(&store_log[2], time);
        my_strcpy(&store_log[10], "GN");
        my_strcpy(&store_log[13], "000");

        write_ext_eeprom_array(0, store_log, 16);

        database_count = 1;
    }
}
    
void store_event(void)
{
    if(my_strcmp(Gear,def_gear)!=0)//gear position has been changed
    {
       my_strcpy(def_gear,Gear);

       
       my_strcpy(&store_log[2],time);
       //my_strcpy(&store_log[10],Gear);
       store_log[10]=Gear[0];
       store_log[11]=Gear[1];
       store_log[12]=' ';
       my_strcpy(&store_log[13],str_speed);
       if(database_count == 10)
        {
            char temp[17];

            for(unsigned char i = 0; i < 9; i++)
            {
                read_ext_eeprom_array((i + 1) * 16, temp, 16);

                temp[0] = '0' + i;

                write_ext_eeprom_array(i * 16, temp, 16);
            }

            database_count = 9;
        }
       store_log[0] = '0' + database_count;
        unsigned char addr;
        addr = database_count * 16;
        write_ext_eeprom_array(addr, store_log, 16);
        database_count++;
    }
}
int database_index=0;
void clear_log(void)
{
    char blank[16] = "                ";   // 16 spaces

    // Erase all 10 log entries from EEPROM
    for(unsigned char i = 0; i < 10; i++)
    {
        write_ext_eeprom_array(i * 16, blank, 16);
    }

    database_count = 0;
    database_index = 0;

    my_strcpy(def_gear, "GN");
    my_strcpy(store_log, "0 00:00:00GN 000");

    int speed = get_speed();
    my_itoa(speed, str_speed);

    init_database();      // Creates the initial log: 0 <RTC_TIME> GN 000

    clcd_print("Log Cleared     ", LINE1(0));
    clcd_print("Successfully    ", LINE2(0));
}
int d_flag=0;
void download_log(void)
{
    if(d_flag==0)
    {
        char temp[17];
        for(int i = 0; i < database_count; i++)
        {
            read_ext_eeprom_array(i * 16, temp, 16);

            puts(temp);
            puts("\n\r");

            for(int j = 0; j < 5000; j++);
        }
        d_flag=1;
    }
    
    clcd_print("Downloaded Log  ",LINE1(0));
    clcd_print("Successfully    ",LINE2(0));
//    for(int i=0;i<10000;i++);
//    screen=HOME_SCREEN;
}

//void set_time(void);
void my_itoa_time(int t,char time[])
{
    time[0]=(t/10)+'0';
    time[1]=(t%10)+'0';
    time[2]='\0';
}

int field=0;
int hour=0;
int min=0;
int sec=0;

char set_time_init = 0;

char time_str[17]="    00:00:00    ";

void change_sec(void)
{
    my_itoa_time(sec,&time_str[10]);
}
void change_min(void)
{
    my_itoa_time(min,&time_str[7]);
}
void change_hr(void)
{
    my_itoa_time(hour,&time_str[4]);
}



unsigned char decimal_to_bcd(unsigned char value)
{
    return ((value / 10) << 4) | (value % 10);
}

unsigned int blink_count = 0;
unsigned char blink = 0;

void set_time(void)
{
    clcd_print("    HH:MM:SS    ",LINE1(0));
    blink_count++;
    if(blink_count >= 1000)      
    {
        blink = !blink;
        blink_count = 0;
    }
    if(set_time_init == 0)//to update the RTC 
    {
        my_strcpy(&time_str[4], time);
        
        hour = ((time[0] - '0') * 10) + (time[1] - '0');
        min  = ((time[3] - '0') * 10) + (time[4] - '0');
        sec  = ((time[6] - '0') * 10) + (time[7] - '0');
        //I have written the above three lines of code to directly edit the time in the RTC instead of editing from 00:00:00
        set_time_init = 1;
    }
    char display_time[17];
    my_strcpy(display_time, time_str);
    
    if(field==0)
    {
        if(key==MK_SW1)
        {
            sec++;
            if(sec==60)
            {
                sec=0;
            }
            change_sec();
        }
        else if(key==MK_SW2)
        {
            sec--;
            if(sec==-1)
            {
                sec=59;
            }
            change_sec();
        }
    }
    else if(field==1)
    {
        if(key==MK_SW1)
        {
            min++;
            if(min==60)
            {
                min=0;
            }
            change_min();
        }
        else if(key==MK_SW2)
        {
            min--;
            if(min==-1)
            {
                min=59;
            }
            change_min();
        }
    }
    else if(field==2)
    {
        if(key==MK_SW1)
        {
            hour++;
            if(hour==13)
            {
                hour=1;
            }
            change_hr();
        }
        else if(key==MK_SW2)
        {
            hour--;
            if(hour==0)
            {
                hour=12;
            }
            change_hr();
        }
    }
    if(key==MK_SW3)
    {
        field++;
        if(field==3)
        {
            field=0;
        }
    }
    if(blink == 0)
    {
        if(field == 0)
        {
            display_time[10] = ' ';
            display_time[11] = ' ';
        }
        else if(field == 1)
        {
            display_time[7] = ' ';
            display_time[8] = ' ';
        }
        else if(field == 2)
        {
            display_time[4] = ' ';
            display_time[5] = ' ';
        }
    }
    clcd_print(display_time, LINE2(0));
    if(key == MK_SW11)      // Save key
    {
        write_ds1307(HOUR_ADDR, decimal_to_bcd(hour));
        write_ds1307(MIN_ADDR, decimal_to_bcd(min));
        write_ds1307(SEC_ADDR, decimal_to_bcd(sec));
        
        set_time_init = 0;
        screen = HOME_SCREEN;
    }
    else if(key == MK_SW12)
    {
        set_time_init = 0;
        screen = HOME_SCREEN;
    }
}
void view_log(void)
{
    clcd_print("S TIME    GR SPD", LINE1(0));

    char temp[17];

    read_ext_eeprom_array(database_index * 16, temp, 16);

    clcd_print(temp, LINE2(0));

    if(key == MK_SW2)
    {
        if(database_index >= database_count - 1)
        {
            database_index = 0;
        }
        else
        {
            database_index++;
        }
    }
    else if(key == MK_SW1)
    {
        if(database_index == 0)
        {
            database_index = database_count - 1;
        }
        else
        {
            database_index--;
        }
    }
}
void menu(void);
void menu(void)
{
    static const char *str[4];
    str[0]="  View Log      ";
    str[1]="  Clear Log     ";
    str[2]="  Download Log  ";
    str[3]="  Set time      ";
    char str_1[20];//To store the string that needs to be printed on clcd line 1
    char str_2[20];//To store the string that needs to be printed on clcd line 2
    static int i=0;//to check if it reached the end of menu(*)
    static int index=0;//to keep track of the options in the menu
    
    if(key==MK_SW2)//scroll down
    {
        if(i > 5)
        {
            i = -1;
            index = -1;
        }

        if(i % 2 == 0)//str1 has *
        {
            i++;
        }
        else //str2 has *
        {
            index++;
            i++;
        }
    }
    else if(key==MK_SW1)//scroll up
    {
        if(i<1)
        {
            i=6;
            index=2;
        }
        if(i % 2 != 0)//str_2 has the *
        {
            i--;
        }
        else//str_1 has the *
        {
            index--;
            i--;
        }
    }
    
    my_strcpy(str_1,str[index]);
    my_strcpy(str_2,str[(index+1)%4]);//%4 to overcome the overflow
    if (i % 2 == 0)
    {
        str_1[0] = '*';
        str_2[0] = ' ';
    }
    else
    {
        str_1[0] = ' ';
        str_2[0] = '*';
    }
    
    clcd_print(str_1,LINE1(0));
    clcd_print(str_2,LINE2(0));
    
    //check which option is clicked
    if(key==MK_SW11)
    {
        if(str_1[0] == '*')
        {
            if(str_1[2]=='V')//USER HAS SELECTED VIEW LOG
            {
                screen=VIEW_SCREEN;
            }
            else if(str_1[2]=='C')//USER HAS SELECTED CLEAR LOG
            {
                screen=CLEAR_SCREEN;
            }
            else if(str_1[2]=='D')//USER HAS SELECTED DOWNLOAD LOG
            {
                screen=DOWNLOAD_SCREEN;
            }
            else if(str_1[2]=='S')//USER HAS SELECTED SET TIME
            {
                screen=TIME_SCREEN ;
            }
        }
        else if(str_2[0]=='*')
        {
            if(str_2[2]=='V')//USER HAS SELECTED VIEW LOG
            {
                screen=VIEW_SCREEN;
            }
            else if(str_2[2]=='C')//USER HAS SELECTED CLEAR LOG
            {
                screen=CLEAR_SCREEN;
            }
            else if(str_2[2]=='D')//USER HAS SELECTED DOWNLOAD LOG
            {
                screen=DOWNLOAD_SCREEN;
            }
            else if(str_2[2]=='S')//USER HAS SELECTED SET TIME
            {
                screen=TIME_SCREEN;
            }
        }
    }
        
}


void init_config(void)
{
	init_clcd();
	init_i2c();
	init_ds1307();
    init_matrix_keypad();
    init_adc();
	clcd_print("TIME      GR SPD", LINE1(0));
    init_database();
    init_uart();
}

void main(void)
{
	init_config();
    int speed=0;
    char gear='N';
	while (1)
	{
        key=read_switches(STATE_CHANGE);
        if(key == MK_SW12 && screen != HOME_SCREEN)
        {
            screen = HOME_SCREEN;
            set_time_init = 0;      // reset if leaving Set Time
            continue;
        }
        switch(screen)
        {
            case HOME_SCREEN:
            {
                clcd_print("TIME      GR SPD", LINE1(0));
                clcd_print("  ",LINE2(8));
                get_time();
                display_time();
                gear=get_gear_pos();
                //Transmit the gear
                Gear[0]='G';
                Gear[1]=gear;
                Gear[2]='\0';
                display_gear(Gear);
                speed=get_speed();

                my_itoa(speed,str_speed);
                display_speed(str_speed);
                store_event();
                if(key == MK_SW11)
                {
                    screen = MENU_SCREEN;
                }
                break;
            }
            case MENU_SCREEN:
            {
                menu();
                break;
            }
            case VIEW_SCREEN:
            {
                view_log();
                break;
            }
            case DOWNLOAD_SCREEN:
            {
                download_log();
                break;
            }
            case TIME_SCREEN:
            {
                set_time();
                break;
            }
            case CLEAR_SCREEN:
            {
                clear_log();
                break;
            }
        }
	}       
}