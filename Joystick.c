/*
Nintendo Switch Fightstick - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
    (C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
    (C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

#include "Joystick.h"

typedef enum {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  H_UP,
  H_DOWN,
  H_LEFT,
  H_RIGHT,
  R_UP,
  R_DOWN,
  R_LEFT,
  R_RIGHT,
  UPLEFT,     // UP + LEFT
  UPRIGHT,
  DOWNLEFT,
  DOWNRIGHT,
  HOLD_UP,    // Hold L Stick UP
  HOLD_DOWN,  // using duration = 0
  HOLD_LEFT,
  HOLD_RIGHT,
  HOLD_CLEAR, // clear Hold L Stick = center
  HOLD_CAM_L, // Hold R Stick Left
  HOLD_CAM_R, // Hold R Stick Right
  HOLD_CAM_C, // clear Hold R Stick = center
  X,
  Y,
  A,
  B,
  L,
  R,
  PLUS,
  HOME,
  LOOP_START,
  DO,
  WHILE,
  NOTHING,
  UPXB
} Buttons_t;

typedef struct {
  Buttons_t button;
  uint16_t duration;
} command;

static const command step[] = {
  // Setup controller
  { B,          0 },
  { B,          0 },
  // ここまでコントローラーを認識させるおまじない

  // Start

  // loop Start
  { LOOP_START,     0 },
  // これより下を無限ループ
  
{ DO,                 0 },
  { X,                2 },//Open Menu
  { NOTHING,         20 },
  { A,                2 },//flying option
  { NOTHING,         30 },
  { B,                2 },
  { NOTHING,        100 },//loading map
  { UPLEFT,           2 },
  { NOTHING,         25 },
  { A,                2 },//location
  { NOTHING,          5 },
  { B,                2 },
  { NOTHING,         20 },
  { A,                2 },//flying
  { NOTHING,        100 },//
  { DOWN,             5 },//to bag option
  { RIGHT,          170 }, 
  { UP,              12 },
  { RIGHT,           47 },
  { DO,               0 },
    { UPRIGHT,         58 },   
    { UP,             125 }, 
    { RIGHT,            5 },   
    { B,                5 },
    { RIGHT,         80.3 },
    { UPLEFT,           7 },     
  { WHILE,              1 },   
  { NOTHING,        450 }, //Shiny or not, adjust here
  { UP,               1 },
  { NOTHING,          5 },
  { A,                2 },//run away
  { NOTHING,         15 },
  { A,                2 },
  { NOTHING,        130 },//intentional period
  { DO,               0 },  
    { UP,               3 },//to bag option
    { NOTHING,          2 },
  { WHILE,            2 },  
  { B,                2 },
  { NOTHING,          5 },
  { A,                2 },//open bag
  { NOTHING,          5 },
  { B,                2 },
  { NOTHING,         50 },
  { HOME,             3 },//home
  { NOTHING,         30 },
  { LEFT,           1 },
  { NOTHING,          3 },
  { H_DOWN,           1 },
  { LEFT,           1 },
  { NOTHING,          1 },
  { A,                1 },//設定選択
  { NOTHING,          5 },
  { B,                2 },
  { NOTHING,         30 },
  { DO,               0 },  
    { H_DOWN,           1 },
    { DOWN,             1 },
  { WHILE,            8 },  
  { A,                1 },//設定>本体 選択
  { NOTHING,          5 },
  //{ B,                2 },
  { NOTHING,          5 },
  { DO,               0 },  
    { H_DOWN,           1 },
    { DOWN,             1 },
  { WHILE,            20 },  
  { A,                1 },//日付と時刻選択
  { NOTHING,          2 },
  { B,                2 },
  { NOTHING,          8 },
  { H_DOWN,           1 },
  { DOWN,             1 },
  { A,                1 },//現在の日付と時刻
  { NOTHING,          7 },
  { H_UP,             1 },//1 year
  { DO,               0 },
    { RIGHT,            1 },
  { WHILE,            5 },
  { A,                1 }, 
  { NOTHING,          8 },
  { DO,               0 },  
    { HOME,             3 },
    { NOTHING,         48 },
  { WHILE,            2 },  
{ WHILE,             20 },// Repeat 20 times
  { HOME,             3 },//home
  { NOTHING,         30 },
  { LEFT,             1 },
  { NOTHING,          3 },
  { H_DOWN,           1 },
  { LEFT,             1 },
  { NOTHING,          1 },
  { A,                1 },//設定選択
  { NOTHING,          5 },
  { B,                2 },
  { NOTHING,         30 },
{ DO,                 0 },
  { H_DOWN,           1 },
  { DOWN,             1 },
{ WHILE,              8 },
  { A,                1 },// 設定>本体 選択
  { NOTHING,         10 },
{ DO,                 0 },
  { H_DOWN,           1 },
  { DOWN,             1 },
{ WHILE,              20 },
  { A,                1 },// 日付と時刻選択
  { NOTHING,          2 },
  { B,                2 },
  { NOTHING,          8 },  
  { H_DOWN,           1 },
  { DOWN,             1 },
  { A,                1 },// 現在の日付と時刻
  { NOTHING,          4 }, 
{ DO,                 0 },
  { DOWN,             1 },
  { H_DOWN,           1 },
{ WHILE,             10 },// Go back to 20 years ago
  { NOTHING,          4 },
{ DO,                 0 },
  { RIGHT,              1 },
{ WHILE,              5 }, 
  { A,                1 },
  { NOTHING,          8 },
  { DO,               0 },  
    { HOME,             3 },
    { NOTHING,         48 },
  { WHILE,            2 },
};

// Main entry point.
int main(void) {
  // We'll start by performing hardware and peripheral setup.
  SetupHardware();
  // We'll then enable global interrupts for our use.
  GlobalInterruptEnable();
  // Once that's done, we'll enter an infinite loop.
  for (;;)
  {
    // We need to run our task to process and deliver data for our IN and OUT endpoints.
    HID_Task();
    // We also need to run the main USB management task.
    USB_USBTask();
  }
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
  // We need to disable watchdog if enabled by bootloader/fuses.
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  // We need to disable clock division before initializing the USB hardware.
  clock_prescale_set(clock_div_1);
  // We can then initialize our hardware and peripherals, including the USB stack.

  #ifdef ALERT_WHEN_DONE
  // Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
  #warning LED and Buzzer functionality enabled. All pins on both PORTB and \
           PORTD will toggle when printing is done.
  DDRD  = 0xFF; //Teensy uses PORTD
  PORTD =  0x0;
                  //We'll just flash all pins on both ports since the UNO R3
  DDRB  = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
  PORTB =  0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
  #endif
  // The USB stack should be initialized last.
  USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
  // We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
  // We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
  bool ConfigSuccess = true;

  // We setup the HID report endpoints.
  ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
  ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

  // We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
  // We can handle two control requests: a GetReport and a SetReport.

  // Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
  // If the device isn't connected and properly configured, we can't do anything here.
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  // We'll start with the OUT endpoint.
  Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
  // We'll check to see if we received something on the OUT endpoint.
  if (Endpoint_IsOUTReceived())
  {
    // If we did, and the packet has data, we'll react to it.
    if (Endpoint_IsReadWriteAllowed())
    {
      // We'll create a place to store our data received from the host.
      USB_JoystickReport_Output_t JoystickOutputData;
      // We'll then take in that data, setting it up in our storage.
      while(Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
      // At this point, we can react to this data.

      // However, since we're not doing anything with this data, we abandon it.
    }
    // Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
    Endpoint_ClearOUT();
  }

  // We'll then move on to the IN endpoint.
  Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
  // We first check to see if the host is ready to accept data.
  if (Endpoint_IsINReady())
  {
    // We'll create an empty report.
    USB_JoystickReport_Input_t JoystickInputData;
    // We'll then populate this report with what we want to send to the host.
    GetNextReport(&JoystickInputData);
    // Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
    while(Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
    // We then send an IN packet on this endpoint.
    Endpoint_ClearIN();
  }
}

int find_loop_start_step(void){
  for(int i=0;i<(int)(sizeof(step)/sizeof(step[0]))-1;i++){
    if(step[i].button==LOOP_START) return i;
  }
  return 0;
}

typedef enum {
  SYNC_CONTROLLER,
  BREATHE,
  PROCESS,
} State_t;
State_t state = SYNC_CONTROLLER;

#define ECHOES 2
int echoes = 0;
USB_JoystickReport_Input_t last_report;

int report_count = 0;
int hold_LX = STICK_CENTER;
int hold_LY = STICK_CENTER;
int hold_RX = STICK_CENTER;
int bufindex = 0;
int duration_count = 0;
int loop_start_step = 0;

#define MAX_NEST 5
int do_nest_num = 0;
int do_index[MAX_NEST];
int loop_counter[MAX_NEST];

void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {
  bool do_or_while = false;
  // Prepare an empty report
  memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
  ReportData->LX = STICK_CENTER;
  ReportData->LY = STICK_CENTER;
  ReportData->RX = STICK_CENTER;
  ReportData->RY = STICK_CENTER;
  ReportData->HAT = HAT_CENTER;

  //Hold stick
  ReportData->LX = hold_LX;
  ReportData->LY = hold_LY;
  ReportData->RX = hold_RX;

  // Repeat ECHOES times the last report
  if (echoes > 0)
  {
    memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
    echoes--;
    return;
  }

  // States and moves management
  switch (state)
  {
    case SYNC_CONTROLLER:
			loop_start_step = find_loop_start_step();
			state = PROCESS;
      break;

    case PROCESS:
      switch (step[bufindex].button)
      {
        case UPXB:
          ReportData->LY = STICK_MIN;
          ReportData->Button |= SWITCH_X;
          ReportData->Button |= SWITCH_B;
          break;

        case UP:
          ReportData->LY = STICK_MIN;
          break;

        case LEFT:
          ReportData->LX = STICK_MIN;
          break;

        case DOWN:
          ReportData->LY = STICK_MAX;
          break;

        case RIGHT:
          ReportData->LX = STICK_MAX;
          break;

        case UPLEFT:
          ReportData->LY = STICK_MIN;
          ReportData->LX = STICK_MIN;
          break;

        case UPRIGHT:
          ReportData->LY = STICK_MIN;
          ReportData->LX = STICK_MAX;
          break;

        case DOWNLEFT:
          ReportData->LY = STICK_MAX;
          ReportData->LX = STICK_MIN;
          break;

        case DOWNRIGHT:
          ReportData->LY = STICK_MAX;
          ReportData->LX = STICK_MAX;
          break;

        case HOLD_UP:
          hold_LY = STICK_MIN;
          break;

        case HOLD_LEFT:
          hold_LX = STICK_MIN;
          break;

        case HOLD_DOWN:
          hold_LY = STICK_MAX;
          break;

        case HOLD_RIGHT:
          hold_LX = STICK_MAX;
          break;

        case HOLD_CLEAR:
          hold_LX = STICK_CENTER;
          hold_LY = STICK_CENTER;
          break;

        case HOLD_CAM_L:
          hold_RX = STICK_MIN;
          break;

        case HOLD_CAM_R:
          hold_RX = STICK_MAX;
          break;

        case HOLD_CAM_C:
          hold_RX = STICK_CENTER;
          break;

        case A:
          ReportData->Button |= SWITCH_A;
          break;

        case B:
          ReportData->Button |= SWITCH_B;
          break;

        case X:
          ReportData->Button |= SWITCH_X;
          break;

        case Y:
          ReportData->Button |= SWITCH_Y;
          break;

        case R:
          ReportData->Button |= SWITCH_R;
          break;

        case PLUS:
          ReportData->Button |= SWITCH_PLUS;
          break;

        case HOME:
          ReportData->Button |= SWITCH_HOME;
          break;

        case H_UP:
          ReportData->HAT = HAT_TOP;
          break;

        case H_DOWN:
          ReportData->HAT = HAT_BOTTOM;
          break;

        case H_LEFT:
          ReportData->HAT = HAT_LEFT;
          break;

        case H_RIGHT:
          ReportData->HAT = HAT_RIGHT;
          break;

        case DO:
          do_index[do_nest_num] = bufindex;
          loop_counter[do_nest_num] = 0;
          do_nest_num++;

          bufindex++;
          duration_count = 0; //念の為
          do_or_while = true;
          break;

        case WHILE:
          loop_counter[do_nest_num - 1]++;
          if(step[bufindex].duration > loop_counter[do_nest_num - 1]){
            bufindex = do_index[do_nest_num - 1] + 1; //DOは無視
            duration_count = 0; //念の為
          }
          else{
            bufindex++;
            loop_counter[do_nest_num - 1] = 0; //念の為
            do_nest_num--;
            duration_count = 0; //念の為
          }
          do_or_while = true;
          break;

        default:
          break;
      }

    if(!do_or_while){
      duration_count++;

      if (duration_count > step[bufindex].duration)
      {
        bufindex++;
        duration_count = 0;
      }
    }

    if (bufindex > (int)( sizeof(step) / sizeof(step[0])) - 1)
    {
      bufindex = loop_start_step;
      duration_count = 0;
    }
  }

  // Prepare to echo this report
  memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
  echoes = ECHOES;
}
