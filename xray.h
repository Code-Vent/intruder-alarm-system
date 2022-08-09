#define BIT0_ON 0b00000001
#define BIT1_ON 0b00000010
#define BIT2_ON 0b00000100
#define BIT3_ON 0b00001000
#define BIT4_ON 0b00010000
#define BIT5_ON 0b00100000
#define BIT6_ON 0b01000000
#define BIT7_ON 0b10000000
#define ZERO    0b00000000
#define ONE     0b11111111

#define BIT0_OFF 0b11111110
#define BIT1_OFF 0b11111101
#define BIT2_OFF 0b11111011
#define BIT3_OFF 0b11110111
#define BIT4_OFF 0b11101111
#define BIT5_OFF 0b11011111
#define BIT6_OFF 0b10111111
#define BIT7_OFF 0b01111111

#define DISABLE_INTERRUPT (INTCON.F7 = 0)
#define ENABLE_INTERRUPT  (INTCON.F7 = 1)
#define DISABLE_TMR0_INTERRUPT (INTCON.F5 = 0)
#define ENABLE_TMR0_INTERRUPT  (INTCON.F5 = 1)
#define ENABLE_EXTERNAL_INTERRUPT (INTCON.F4 = 1)
#define DISABLE_EXTERNAL_INTERRUPT (INTCON.F4 = 0)
#define ENABLE_PORTB_CHANGE_INTERRUPT (INTCON.F3 = 1)
#define DISABLE_PORTB_CHANGE_INTERRUPT (INTCON.F3 = 0)
#define CLEAR_TMR0_INTERRUPT (INTCON.F2 = 0)
#define CLEAR_EXTERNAL_INTERRUPT (INTCON.F1 = 0)
#define CLEAR_PORTB_CHANGE_INTERRUPT (INTCON.F0 = 0)
#define TMR0_INTERRUPT (INTCON.F2 == 1)
#define EXTERNAL_INTERRUPT (INTCON.F1 == 1)
#define PORTB_CHANGE_INTERRUPT (INTCON.F0 == 1)

#define TOGGLE_RA0 ( PORTA.B0 = (PORTA.B0 == 0)? 1 : 0 )
#define ON_RA0 (PORTA.B0 = 1)
#define OFF_RA0 (PORTA.B0 = 0)
#define IS_RA0_ON (PORTA.B0 == 1)

#define TOGGLE_RA1 ( PORTA.B1 = (PORTA.B1 == 0)? 1 : 0 )
#define ON_RA1 (PORTA.B1 = 1)
#define OFF_RA1 (PORTA.B1 = 0)
#define IS_RA1_ON (PORTA.B1 == 1)

#define TOGGLE_RA2 ( PORTA.B2 = (PORTA.B2 == 0)? 1 : 0 )
#define ON_RA2 (PORTA.B2 = 1)
#define OFF_RA2 (PORTA.B2 = 0)
#define IS_RA2_ON (PORTA.B2 == 1)

#define TOGGLE_RA3 ( PORTA.B3 = (PORTA.B3 == 0)? 1 : 0 )
#define ON_RA3 (PORTA.B3 = 1)
#define OFF_RA3 (PORTA.B3 = 0)
#define IS_RA3_ON (PORTA.B3 == 1)

#define TOGGLE_RA4 ( PORTA.B4 = (PORTA.B4 == 0)? 1 : 0 )
#define ON_RA4 (PORTA.B4 = 1)
#define OFF_RA4 (PORTA.B4 = 0)
#define IS_RA4_ON (PORTA.B4 == 1)

#define TOGGLE_RA5 ( PORTA.B5 = (PORTA.B5 == 0)? 1 : 0 )
#define ON_RA5 (PORTA.B5 = 1)
#define OFF_RA5 (PORTA.B5 = 0)
#define IS_RA5_ON (PORTA.B5 == 1)

#define TOGGLE_RA6 ( PORTA.B6 = (PORTA.B6 == 0)? 1 : 0 )
#define ON_RA6 (PORTA.B6 = 1)
#define OFF_RA6 (PORTA.B6 = 0)
#define IS_RA6_ON (PORTA.B6 == 1)

#define TOGGLE_RA7 ( PORTA.B7 = (PORTA.B7 == 0)? 1 : 0 )
#define ON_RA7 (PORTA.B7 = 1)
#define OFF_RA7 (PORTA.B7 = 0)
#define IS_RA7_ON (PORTA.B7 == 1)

#define TOGGLE_RB0 ( PORTB.B0 = (PORTB.B0 == 0)? 1 : 0 )
#define ON_RB0 (PORTB.B0 = 1)
#define OFF_RB0 (PORTB.B0 = 0)
#define IS_RB0_ON (PORTB.B0 == 1)

#define TOGGLE_RB1 ( PORTB.B1 = (PORTB.B1 == 0)? 1 : 0 )
#define ON_RB1 (PORTB.B1 = 1)
#define OFF_RB1 (PORTB.B1 = 0)
#define IS_RB1_ON (PORTB.B1 == 1)

#define TOGGLE_RB2 ( PORTB.B2 = (PORTB.B2 == 0)? 1 : 0 )
#define ON_RB2 (PORTB.B2 = 1)
#define OFF_RB2 (PORTB.B2 = 0)
#define IS_RB2_ON (PORTB.B2 == 1)

#define TOGGLE_RB3 ( PORTB.B3 = (PORTB.B3 == 0)? 1 : 0 )
#define ON_RB3 (PORTB.B3 = 1)
#define OFF_RB3 (PORTB.B3 = 0)
#define IS_RB3_ON (PORTB.B3 == 1)

#define TOGGLE_RB4 ( PORTB.B4 = (PORTB.B4 == 0)? 1 : 0 )
#define ON_RB4 (PORTB.B4 = 1)
#define OFF_RB4 (PORTB.B4 = 0)
#define IS_RB4_ON (PORTB.B4 == 1)

#define TOGGLE_RB5 ( PORTB.B5 = (PORTB.B5 == 0)? 1 : 0 )
#define ON_RB5 (PORTB.B5 = 1)
#define OFF_RB5 (PORTB.B5 = 0)
#define IS_RB5_ON (PORTB.B5 == 1)

#define TOGGLE_RB6 ( PORTB.B6 = (PORTB.B6 == 0)? 1 : 0 )
#define ON_RB6 (PORTB.B6 = 1)
#define OFF_RB6 (PORTB.B6 = 0)
#define IS_RB6_ON (PORTB.B6 == 1)

#define TOGGLE_RB7 ( PORTB.B7 = (PORTB.B7 == 0)? 1 : 0 )
#define ON_RB7 (PORTB.B7 = 1)
#define OFF_RB7 (PORTB.B7 = 0)
#define IS_RB7_ON (PORTB.B7 == 1)
#define BUTTON_DELAY 10

#undef keypad_row_1;
#undef keypad_row_2;
#undef keypad_row_3;
#undef keypad_row_4;
#undef keypad_row_5;
#undef keypad_row_6;
#undef keypad_row_7;
#undef keypad_row_8;

#undef keypad_col_1;
#undef keypad_col_2;
#undef keypad_col_3;
#undef keypad_col_4;
#undef keypad_col_5;
#undef keypad_col_6;
#undef keypad_col_7;
#undef keypad_col_8;

void(*Constructor)();
void CreateKeypadLock();
void CreateMenu();
void CreateActiveSensor();
void CreateInactiveBuzzer();
void CreateActiveBuzzer();
void CreateIdle();
void CreateUserAuth();
void CreateUserAboutPage();
void CreateNewPin();
void CreateSaveNewPin();
char Getchar();
char lock_keypad();
char delay_func();
char menu();
char idle();
char about();
char infrared_sensor();
char stop_buzzerr();
char start_buzzerr();
char user_auth();
char new_pin();
char save_new_pin();
void interrupt();
void keypad_init();
void keypad_get_key(char);