#include"16f628a.h"
#include<stdio.h>
#include"xray.h"

// Lcd pinout settings
sbit LCD_RS at RA4_bit;
sbit LCD_EN at RA6_bit;
sbit LCD_D6 at RA3_bit;
sbit LCD_D7 at RA2_bit;
sbit LCD_D5 at RA1_bit;
sbit LCD_D4 at RA0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISA4_bit;
sbit LCD_EN_Direction at TRISA6_bit;
sbit LCD_D7_Direction at TRISA2_bit;
sbit LCD_D6_Direction at TRISA3_bit;
sbit LCD_D5_Direction at TRISA1_bit;
sbit LCD_D4_Direction at TRISA0_bit;


#define keypad_row_1 PORTB.B3
#define keypad_row_2 PORTB.B1
#define keypad_row_3 PORTB.B5
#define keypad_row_4 PORTB.B6

#define keypad_col_1_port PORTB
#define keypad_col_1_pin  2
#define keypad_col_2_port PORTA
#define keypad_col_2_pin  5
#define keypad_col_3_port PORTB
#define keypad_col_3_pin  0
#define keypad_col_4_port PORTB
#define keypad_col_4_pin  7


int j = 0;
char ch;

char pin[5] = "0000";
char buf[5] = "0000";

char screen_row[17];
char keys[4][4] = { '1','2','3','A',
					'4','5','6','B',
					'7','8','9','C',
					'*','0','#','D'};
char keypad_col;

enum State {
	KEYPAD_LOCKED, IDLE, USER_AUTH, MENU,
	START_SENSOR, START_BUZZER, STOP_BUZZER,
	ENTER_NEW_PIN, SAVE_NEW_PIN, ABOUT, EMPTY
};
enum State state = EMPTY;

struct StateObject {
	char delay;
	char(*function)();
};

struct StateObject object;

//void null_func() {}

void interrupt() {

	if (EXTERNAL_INTERRUPT)
	{
		Constructor = CreateUserAuth;
		CLEAR_EXTERNAL_INTERRUPT;
		return;
	}
	else if (PORTB_CHANGE_INTERRUPT)
	{
		Constructor = CreateActiveBuzzer;
		CLEAR_PORTB_CHANGE_INTERRUPT;
		return;
	}

}

char menu() {
	DISABLE_EXTERNAL_INTERRUPT;
	ch = Getchar();
	if (ch) {
		switch (ch)
		{
		case 'A':
			Constructor = CreateActiveSensor;
			return 1;
		case 'B':
			Constructor = CreateInactiveBuzzer;
			return 1;
		case 'C':
			Constructor = CreateNewPin;
			return 1;
		case 'D':
			Constructor = CreateUserAboutPage;
			return 1;
		default:
			return 0;
		}
	}
	return 0;
}

char new_pin() {
	ch = Getchar();
	if (ch) {
		buf[j++] = ch;
		Lcd_Chr_Cp(ch);
		if (j > 3) {
			j = 0;
			return 1;
		}
	}
	return 0;
}

void screen_first_new_pin_saved() {
	//"Starting Buzzer";
	screen_row[0] = 'N';
	screen_row[1] = 'e';
	screen_row[2] = 'w';
	screen_row[3] = ' ';
	screen_row[4] = 'P';
	screen_row[5] = 'i';
	screen_row[6] = 'n';
	screen_row[7] = ' ';
	screen_row[8] = 'S';
	screen_row[9] = 'a';
	screen_row[10] = 'v';
	screen_row[11] = 'e';
	screen_row[12] = 'd';
	screen_row[13] = '.';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

char save_new_pin() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_new_pin_saved();
	Lcd_Out(1, 1, screen_row);
	//save in eeprom
	EEPROM_Write(0x00, buf[0]);
	EEPROM_Write(0x01, buf[1]);
	EEPROM_Write(0x02, buf[2]);
	EEPROM_Write(0x03, buf[3]);
	return 1;
}

void screen_first_student_info() {
	//"Starting Buzzer";
	screen_row[0] = 'O';
	screen_row[1] = 'y';
	screen_row[2] = 'e';
	screen_row[3] = 'y';
	screen_row[4] = 'i';
	screen_row[5] = 'o';
	screen_row[6] = 'l';
	screen_row[7] = 'a';
	screen_row[8] = ' ';
	screen_row[9] = 'A';
	screen_row[10] = '.';
	screen_row[11] = 'K';
	screen_row[12] = '.';
	screen_row[13] = '.';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

char about() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_student_info();
	Lcd_Out(1, 1, screen_row);
	return 1;
}

char user_auth() {
	ch = Getchar();
	if (ch) {
		pin[j++] = ch;
		Lcd_Chr_Cp(ch);
		if (j > 3) {
			if (pin[0] == buf[0]
				&& pin[1] == buf[1]
				&& pin[2] == buf[2]
				&& pin[3] == buf[3]) {
				j = 0;
				return 1;
			}
		}
	}
	return 0;
}

void screen_first_active_sensor() {
	//"Starting Buzzer";
	screen_row[0] = 'S';
	screen_row[1] = 'e';
	screen_row[2] = 'n';
	screen_row[3] = 's';
	screen_row[4] = 'o';
	screen_row[5] = 'r';
	screen_row[6] = ' ';
	screen_row[7] = 'S';
	screen_row[8] = 't';
	screen_row[9] = 'a';
	screen_row[10] = 'r';
	screen_row[11] = 't';
	screen_row[12] = 'e';
	screen_row[13] = 'd';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

char infrared_sensor() {
	OFF_RA7;
	PORTB = 0xFF;
	ENABLE_PORTB_CHANGE_INTERRUPT;
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_active_sensor();
	Lcd_Out(1, 1, screen_row);
	return 1;
}

void screen_first_inactive_buzzer() {
	//"Starting Buzzer";
	screen_row[0] = 'B';
	screen_row[1] = 'u';
	screen_row[2] = 'z';
	screen_row[3] = 'z';
	screen_row[4] = 'e';
	screen_row[5] = 'r';
	screen_row[6] = ' ';
	screen_row[7] = 'S';
	screen_row[8] = 't';
	screen_row[9] = 'o';
	screen_row[10] = 'p';
	screen_row[11] = 'p';
	screen_row[12] = 'e';
	screen_row[13] = 'd';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

char stop_buzzerr() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_inactive_buzzer();
	Lcd_Out(1, 1, screen_row);
	OFF_RA7;
	return 1;
}

void screen_first_active_buzzer() {
	//"Starting Buzzer";
	screen_row[0] = 'B';
	screen_row[1] = 'u';
	screen_row[2] = 'z';
	screen_row[3] = 'z';
	screen_row[4] = 'e';
	screen_row[5] = 'r';
	screen_row[6] = ' ';
	screen_row[7] = 'S';
	screen_row[8] = 't';
	screen_row[9] = 'a';
	screen_row[10] = 'r';
	screen_row[11] = 't';
	screen_row[12] = 'e';
	screen_row[13] = 'd';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}


char start_buzzerr() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_active_buzzer();
	Lcd_Out(1, 1, screen_row);
	ON_RA7;
	DISABLE_PORTB_CHANGE_INTERRUPT;
	return 1;
}

char delay_func() {
	Delay_ms(1000);
	if (!object.delay--) {
		return 1;
	}
	return 0;
}

char button_delay() {
	Delay_ms(BUTTON_DELAY);
	return 1;
}

char idle() {
	asm SLEEP;
	return 1;
}

char lock_keypad() {
	PORTB = 0b10111111;
	ENABLE_EXTERNAL_INTERRUPT;
	return 1;
}

void screen_row_unlock_screen() {
	//"Press#to Unlock";
	screen_row[0] = 'P';
	screen_row[1] = 'r';
	screen_row[2] = 'e';
	screen_row[3] = 's';
	screen_row[4] = 's';
	screen_row[5] = ' ';
	screen_row[6] = '#';
	screen_row[7] = 't';
	screen_row[8] = 'o';
	screen_row[9] = ' ';
	screen_row[10] = 'U';
	screen_row[11] = 'n';
	screen_row[12] = 'l';
	screen_row[13] = 'o';
	screen_row[14] = 'c';
	screen_row[15] = 'k';
	screen_row[16] = 0;
}

void CreateKeypadLock() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_row_unlock_screen();
	Lcd_Out(2, 1, screen_row);
	object.delay = 120;
	object.function = lock_keypad;
	state = KEYPAD_LOCKED;
}

void screen_first_row_Enter_new_pin() {
	//Lcd_Out(1, 1, "Saving new Pin");
	screen_row[0] = 'E';
	screen_row[1] = 'n';
	screen_row[2] = 't';
	screen_row[3] = 'e';
	screen_row[4] = 'r';
	screen_row[5] = ' ';
	screen_row[6] = 'N';
	screen_row[7] = 'e';
	screen_row[8] = 'w';
	screen_row[9] = ' ';
	screen_row[10] = 'P';
	screen_row[11] = 'i';
	screen_row[12] = 'n';
	screen_row[13] = 0;
	screen_row[14] = 0;
}

void screen_second_row_Enter_new_pin() {
	//Lcd_Out(1, 1, "Saving new Pin");
	screen_row[0] = 'P';
	screen_row[1] = 'i';
	screen_row[2] = 'n';
	screen_row[3] = '?';
	screen_row[4] = '_';
	screen_row[5] = '_';
	screen_row[6] = 0;
	screen_row[7] = 0;
	screen_row[8] = 0;
	screen_row[9] = 0;
	screen_row[10] = 0;
	screen_row[11] = 0;
	screen_row[12] = 0;
	screen_row[13] = 0;
	screen_row[14] = 0;
}


void CreateNewPin() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_row_Enter_new_pin();
	Lcd_Out(1, 1, screen_row);
	screen_second_row_Enter_new_pin();
	Lcd_Out(2, 1, screen_row);
	object.delay = 120;
	object.function = new_pin;
	state = ENTER_NEW_PIN;
}

void screen_first_row_Save_new_pin() {
	//Lcd_Out(1, 1, "Saving new Pin");
	screen_row[0] = 'S';
	screen_row[1] = 'a';
	screen_row[2] = 'v';
	screen_row[3] = 'i';
	screen_row[4] = 'n';
	screen_row[5] = 'g';
	screen_row[6] = ' ';
	screen_row[7] = 'n';
	screen_row[8] = 'e';
	screen_row[9] = 'w';
	screen_row[10] = ' ';
	screen_row[11] = 'P';
	screen_row[12] = 'i';
	screen_row[13] = 'n';
	screen_row[14] = 0;
}

void screen_second_row_Save_new_pin() {
	//Lcd_Out(1, 1, "Saving new Pin");
	screen_row[0] = buf[0];
	screen_row[1] = buf[1];
	screen_row[2] = buf[2];
	screen_row[3] = buf[3];
	screen_row[4] = 0;
	screen_row[5] = 0;
	screen_row[6] = 0;
	screen_row[7] = 0;
	screen_row[8] = 0;
	screen_row[9] = 0;
	screen_row[10] = 0;
	screen_row[11] = 0;
	screen_row[12] = 0;
	screen_row[13] = 0;
	screen_row[14] = 0;
}

void CreateSaveNewPin() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_row_Save_new_pin();
	Lcd_Out(1, 1, screen_row);
	screen_second_row_Save_new_pin();
	Lcd_Out(2, 1, screen_row);
	object.delay = 8;
	object.function = save_new_pin;
	state = SAVE_NEW_PIN;
}

void screen_first_starting_sensor() {
	//"Starting Buzzer";
	screen_row[0] = 'S';
	screen_row[1] = 't';
	screen_row[2] = 'a';
	screen_row[3] = 'r';
	screen_row[4] = 't';
	screen_row[5] = 'i';
	screen_row[6] = 'n';
	screen_row[7] = 'g';
	screen_row[8] = ' ';
	screen_row[9] = 'S';
	screen_row[10] = 'e';
	screen_row[11] = 'n';
	screen_row[12] = 's';
	screen_row[13] = 'o';
	screen_row[14] = 'r';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

void CreateActiveSensor() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_starting_sensor();
	Lcd_Out(1, 1, screen_row);
	object.delay = 120;
	object.function = infrared_sensor;
	state = START_SENSOR;
}

void screen_first_stop_buzzer() {
	//"Starting Buzzer";
	screen_row[0] = 'S';
	screen_row[1] = 't';
	screen_row[2] = 'o';
	screen_row[3] = 'p';
	screen_row[4] = 'p';
	screen_row[5] = 'i';
	screen_row[6] = 'n';
	screen_row[7] = 'g';
	screen_row[8] = ' ';
	screen_row[9] = 'B';
	screen_row[10] = 'u';
	screen_row[11] = 'z';
	screen_row[12] = 'z';
	screen_row[13] = 'e';
	screen_row[14] = 'r';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

void CreateInactiveBuzzer() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_stop_buzzer();
	Lcd_Out(1, 1, screen_row);
	object.delay = 5;
	object.function = stop_buzzerr;
	state = STOP_BUZZER;
}

void screen_first_start_buzzer() {
	//"Starting Buzzer";
	screen_row[0] = 'S';
	screen_row[1] = 't';
	screen_row[2] = 'a';
	screen_row[3] = 'r';
	screen_row[4] = 't';
	screen_row[5] = 'i';
	screen_row[6] = 'n';
	screen_row[7] = 'g';
	screen_row[8] = ' ';
	screen_row[9] = 'B';
	screen_row[10] = 'u';
	screen_row[11] = 'z';
	screen_row[12] = 'z';
	screen_row[13] = 'e';
	screen_row[14] = 'r';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

void CreateActiveBuzzer() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_start_buzzer();
	Lcd_Out(1, 1, screen_row);
	object.delay = 5;
	object.function = start_buzzerr;
	state = START_BUZZER;
}

void screen_row_idle() {
	//"Sleeping Mode";
	screen_row[0] = 'S';
	screen_row[1] = 'l';
	screen_row[2] = 'e';
	screen_row[3] = 'e';
	screen_row[4] = 'p';
	screen_row[5] = 'i';
	screen_row[6] = 'n';
	screen_row[7] = 'g';
	screen_row[8] = ' ';
	screen_row[9] = 'M';
	screen_row[10] = 'o';
	screen_row[11] = 'd';
	screen_row[12] = 'e';
	screen_row[13] = '.';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}


void CreateIdle() {
	screen_row_idle();
	Lcd_Out(2, 1, screen_row);
	object.delay = 0;
	object.function = idle;
	state = IDLE;
}

void screen_first_row_user_auth() {
	//"Authenticating";
	screen_row[0] = 'A';
	screen_row[1] = 'u';
	screen_row[2] = 't';
	screen_row[3] = 'h';
	screen_row[4] = 'e';
	screen_row[5] = 'n';
	screen_row[6] = 't';
	screen_row[7] = 'i';
	screen_row[8] = 'c';
	screen_row[9] = 'a';
	screen_row[10] = 't';
	screen_row[11] = 'i';
	screen_row[12] = 'n';
	screen_row[13] = 'g';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

void CreateUserAuth() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_row_user_auth();
	Lcd_Out(1, 1, screen_row);
	screen_second_row_Enter_new_pin();
	Lcd_Out(2, 1, screen_row);
	object.delay = 120;
	object.function = user_auth;
	state = USER_AUTH;
}


void screen_first_row_menu() {
	//"Options{A,B,C,D}";
	screen_row[0] = 'O';
	screen_row[1] = 'p';
	screen_row[2] = 't';
	screen_row[3] = 'i';
	screen_row[4] = 'o';
	screen_row[5] = 'n';
	screen_row[6] = 's';
	screen_row[7] = '{';
	screen_row[8] = 'A';
	screen_row[9] = 'B';
	screen_row[10] = 'C';
	screen_row[11] = 'D';
	screen_row[12] = '}';
	screen_row[13] = 0;
	screen_row[14] = 0;
	screen_row[15] = 0;
	screen_row[16] = 0;
}

void screen_second_row_menu() {
	//"Options{A,B,C,D}";
	screen_row[0] = 'P';
	screen_row[1] = 'l';
	screen_row[2] = 'e';
	screen_row[3] = 'a';
	screen_row[4] = 's';
	screen_row[5] = 'e';
	screen_row[6] = ' ';
	screen_row[7] = 'S';
	screen_row[8] = 'e';
	screen_row[9] = 'l';
	screen_row[10] = 'e';
	screen_row[11] = 'c';
	screen_row[12] = 't';
	screen_row[13] = 0;
	screen_row[14] = 0;
	screen_row[15] = 0;
	screen_row[16] = 0;
}


void CreateMenu() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_row_menu();
	Lcd_Out(1, 1, screen_row);
	screen_second_row_menu();
	Lcd_Out(2, 1, screen_row);
	Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
	object.delay = 120;
	object.function = menu;
	state = MENU;
}

void screen_first_row_about_page() {
	
	screen_row[0] = 'I';
	screen_row[1] = 'n';
	screen_row[2] = 't';
	screen_row[3] = 'r';
	screen_row[4] = 'u';
	screen_row[5] = 'd';
	screen_row[6] = 'e';
	screen_row[7] = 'r';
	screen_row[8] = ' ';
	screen_row[9] = 'A';
	screen_row[10] = 'l';
	screen_row[11] = 'a';
	screen_row[12] = 'r';
	screen_row[13] = 'm';
	screen_row[14] = '.';
	screen_row[15] = '.';
	screen_row[16] = 0;
}

void screen_second_row_about_page() {

	screen_row[0] = 'D';
	screen_row[1] = 'e';
	screen_row[2] = 's';
	screen_row[3] = 'i';
	screen_row[4] = 'g';
	screen_row[5] = 'n';
	screen_row[6] = 'e';
	screen_row[7] = 'd';
	screen_row[8] = ' ';
	screen_row[9] = 'B';
	screen_row[10] = 'y';
	screen_row[11] = ' ';
	screen_row[12] = ':';
	screen_row[13] = 0;
	screen_row[14] = 0;
	screen_row[15] = 0;
	screen_row[16] = 0;
}

void CreateUserAboutPage() {
	Lcd_Cmd(_LCD_CLEAR);
	screen_first_row_about_page();	
	Lcd_Out(1, 1, screen_row);
	screen_second_row_about_page();
	Lcd_Out(2, 1, screen_row);
	Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
	object.delay = 120;
	object.function = about;
	state = ABOUT;
}

char Getchar() {

	PORTB = 0b11110111;
	if (!IS_RB2_ON)
		return '1';
	else if (!IS_RA5_ON)
		return '2';
	else if (!IS_RB0_ON)
		return '3';
	else if (!IS_RB7_ON)
		return 'A';

	PORTB = 0b11111101;
	if (!IS_RB2_ON)
		return '4';
	else if (!IS_RA5_ON)
		return '5';
	else if (!IS_RB0_ON)
		return '6';
	else if (!IS_RB7_ON)
		return 'B';

	PORTB = 0b11011111;
	if (!IS_RB2_ON)
		return '7';
	else if (!IS_RA5_ON)
		return '8';
	else if (!IS_RB0_ON)
		return '9';
	else if (!IS_RB7_ON)
		return 'C';

	PORTB = 0b10111111;
	if (!IS_RB2_ON)
		return '*';
	else if (!IS_RA5_ON)
		return '0';
	else if (!IS_RB0_ON)
		return '#';
	else if (!IS_RB7_ON)
		return 'D';

	return 0;
}

void Setup() {
	TRISB = BIT0_ON | BIT4_ON | BIT2_ON | BIT7_ON; //output 3156* input 0427
	TRISA = ZERO; //All output
	CMCON = BIT0_ON | BIT1_ON | BIT2_ON;
	VRCON = ZERO;
	OPTION_REG = BIT3_ON | BIT2_ON | BIT1_ON | BIT0_ON; // Pre-Scaler for WDT 1:128
	INTCON = BIT7_ON; // Enable Global
	RCSTA = ZERO;
	PORTA = ZERO;
	PORTB = ONE;
}

void update_system() {

	switch (state)
	{
	case KEYPAD_LOCKED:
		if (object.function() && delay_func()) {
			//next state
			Constructor = CreateIdle;
		}
		break;
	case IDLE:
		object.function();
		break;
	case USER_AUTH:
		if (object.function()) {
			//next state
			Constructor = CreateMenu;
		}
		else if (delay_func())
		{
			Constructor = CreateKeypadLock;
		}
		break;
	case MENU:
		if (object.function()) {
		}
		else if (delay_func())
		{
			Constructor = CreateKeypadLock;
		}
		break;
	case START_SENSOR:
	case STOP_BUZZER:
	case START_BUZZER:
	case SAVE_NEW_PIN:
	case ABOUT:
		if (delay_func())
		{
			object.function();
			Constructor = CreateKeypadLock;
		}
		break;
	case ENTER_NEW_PIN:
		if (object.function()) {
			//next state
			Constructor = CreateSaveNewPin;
		}
		else if (delay_func())
		{
			Constructor = CreateKeypadLock;
		}
		break;
	default:
		Constructor = CreateKeypadLock;
		break;
	}
}

void keypad_test() {
	ch = Getchar();
	if (ch) {
		TOGGLE_RA0;
		button_delay();
	}
	/*if (!ch)
		return;
	if (!str) {
		ON_RA1;
		return;
	}
	else {
		OFF_RA1;
		return;
	}
	
	if (ch == *str++) {
		TOGGLE_RA0;
	}
	else {
		str = 0;
		OFF_RA0;
	}*/
}

void keypad_init() {
#ifdef keypad_row_1
	keypad_row_1 = 1;
#endif
#ifdef keypad_row_2
	keypad_row_2 = 1;
#endif
#ifdef keypad_row_3
	keypad_row_3 = 1;
#endif
#ifdef keypad_row_4
	keypad_row_4 = 1;
#endif
#ifdef keypad_row_5
	keypad_row_5 = 1;
#endif
#ifdef keypad_row_6
	keypad_row_6 = 1;
#endif
#ifdef keypad_row_7
	keypad_row_7 = 1;
#endif
#ifdef keypad_row_8
	keypad_row_8 = 1;
#endif
	keypad_col = 0;
}

void keypad_scan_cols() {
	keypad_col = 0;
#ifdef keypad_col_1_pin
	if (Button(&keypad_col_1_port, keypad_col_1_pin, 0, 0)) {
		keypad_col = 1;
	}
#endif
#ifdef keypad_col_2_pin
	else if (Button(&keypad_col_2_port, keypad_col_2_pin, 0, 0)) {
		keypad_col = 2;
	}
#endif
#ifdef keypad_col_3_pin
	else if (Button(&keypad_col_3_port, keypad_col_3_pin, 0, 0)) {
		keypad_col = 3;
	}
#endif
#ifdef keypad_col_4_pin
	else if (Button(&keypad_col_4_port, keypad_col_4_pin, 0, 0)) {
		keypad_col = 4;
	}
#endif
#ifdef keypad_col_5_pin
	else if (Button(&keypad_col_5_port, keypad_col_5_pin, 0, 0)) {
		keypad_col = 5;
}
#endif
#ifdef keypad_col_6_pin
	else if (Button(&keypad_col_6_port, keypad_col_6_pin, 0, 0)) {
		keypad_col = 6;
	}
#endif
#ifdef keypad_col_7_pin
	else if (Button(&keypad_col_7_port, keypad_col_7_pin, 0, 0)) {
		keypad_col = 7;
	}
#endif
#ifdef keypad_col_8_pin
	else if (Button(&keypad_col_8_port, keypad_col_8_pin, 0, 0)) {
		keypad_col = 8;
	}
#endif
}

char keypad_get_key() {

#ifdef keypad_row_1
	keypad_row_1 = 0;
	keypad_scan_cols();
	keypad_row_1 = 1;
	if (keypad_col) return 1;
#endif
#ifdef keypad_row_2
	keypad_row_2 = 0;
	keypad_scan_cols();
	keypad_row_2 = 1;
	if (keypad_col) return 2;
#endif
#ifdef keypad_row_3
	keypad_row_3 = 0;
	keypad_scan_cols();
	keypad_row_3 = 1;
	if (keypad_col) return 3;
#endif
#ifdef keypad_row_4
	keypad_row_4 = 0;
	keypad_scan_cols();
	keypad_row_4 = 1;
	if (keypad_col) return 4;
#endif
#ifdef keypad_row_5
	keypad_row_5 = 0;
	keypad_scan_cols();
	keypad_row_5 = 1;
	if (keypad_col) return 5;
#endif
#ifdef keypad_row_6
	keypad_row_6 = 0;
	keypad_scan_cols();
	keypad_row_6 = 1;
	if (keypad_col) return 6;
#endif
#ifdef keypad_row_7
	keypad_row_7 = 0;
	keypad_scan_cols();
	keypad_row_7 = 1;
	if (keypad_col) return 7;
#endif
#ifdef keypad_row_8
	keypad_row_8 = 0;
	keypad_scan_cols();
	keypad_row_8 = 1;
	if (keypad_col) return 8;
#endif
	return 0;
}

int main() {
	Setup();
	Lcd_Init();
	Constructor = 0;
	buf[0] = EEPROM_Read(0x00);
	buf[1] = EEPROM_Read(0x01);
	buf[2] = EEPROM_Read(0x02);
	buf[3] = EEPROM_Read(0x03);
	
	while (1)
	{
		if (Constructor) {
			Constructor();
			Constructor = 0;
		}
		update_system();
		asm CLRWDT;
	}
	return 0;
}