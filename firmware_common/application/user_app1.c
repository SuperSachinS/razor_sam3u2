/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  LcdClearChars(LINE1_START_ADDR, LINE1_END_ADDR);
  LcdClearChars(LINE2_START_ADDR, LINE2_END_ADDR);
  u8 au8Message[] = "Press to Start";
  u8 position[] = "\\";
  LcdMessage(LINE2_START_ADDR, position);
  LcdMessage(LINE1_START_ADDR, au8Message);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_StartUp;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */

static void UserApp1SM_StartUp(void){
  u8 message[] = "Click the buttons";
  u8 colour_positions[] = "R     B      G     W";


  if(WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);
    LcdMessage(LINE1_START_ADDR, message);
    LcdMessage(LINE2_START_ADDR, colour_positions);
    UserApp1_pfStateMachine = UserApp1SM_DisplayGame;
    srand(2);

  }

}

static void UserApp1SM_DisplayGame(void){
  static int counter = 0;
  static int period = 1000;
  static u8 index = 0;
  static u8 targetColour = 0;
  static bool readInput = FALSE;
  static bool correctInput = FALSE;
  counter++;
  
  //start next cycle
  if (counter == 1000){
    counter = 0;
    targetColour = rand() % 4;
    index++;
    readInput = TRUE;
    correctInput = FALSE;
    if (targetColour == 0){
      LedOn(LCD_RED);
      LedOff(LCD_BLUE);
      LedOff(LCD_GREEN);
    }
    else if (targetColour == 1){
      LedOff(LCD_RED);
      LedOn(LCD_BLUE);
      LedOff(LCD_GREEN);
    }
    else if (targetColour == 2){
      LedOff(LCD_RED);
      LedOff(LCD_BLUE);
      LedOn(LCD_GREEN);
    }
    else if (targetColour == 3){
      LedOn(LCD_RED);
      LedOn(LCD_BLUE);
      LedOn(LCD_GREEN);
    }else{
      LedOn(LCD_RED);
      LedOn(LCD_BLUE);
      LedOff(LCD_GREEN);
    }
    
  }
  
  
  //check for user input
  
  if (readInput){
    
    if (targetColour == 0){
      if(IsButtonPressed(BUTTON0)){
        correctInput = TRUE;
      }
    }
    else if (targetColour == 1){
      if(IsButtonPressed(BUTTON1)){
        correctInput = TRUE;
      }
    }
    else if (targetColour == 2){
      if(IsButtonPressed(BUTTON2)){
        correctInput = TRUE;
      }
    }
    else if (targetColour == 3){
      if(IsButtonPressed(BUTTON3)){
        correctInput = TRUE;
      }
    }
  
  }
  
  //user inputting correct input, so turn of lcd
  
  if(correctInput){
    //srand(counter);
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);
  }
  
  
}

static void UserApp1SM_Idle(void)
{
  
  
  static u16 counter = 0;
  static u8 position = 0;
  static u8 previous_position = 0;
  static u8 colour_state = 0;
  static u16 score = 0;
  
  counter++;
  
  if (counter == 1000){
    LedOn(LCD_RED);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);
    colour_state = 0;
    
  }else if (counter == 2000){
    LedOff(LCD_RED);
    LedOn(LCD_BLUE);
    LedOff(LCD_GREEN);
    colour_state = 1;
  }else if (counter == 3000){
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOn(LCD_GREEN);
        colour_state = 2;
  }else if (counter == 4000){
    LedOn(LCD_RED);
    LedOn(LCD_BLUE);
    LedOn(LCD_GREEN);
    colour_state = 3;
    counter = 0;
  }
  
  
  switch(colour_state){
    case 0:
      if(IsButtonPressed(BUTTON0)){
        //ButtonAcknowledge(BUTTON0);
        LedOff(LCD_RED);
        LedOff(LCD_BLUE);
        LedOff(LCD_GREEN);
        //update_score(score, 1);
      }
      break;
     case 1:
        if(IsButtonPressed(BUTTON1)){
          //ButtonAcknowledge(BUTTON1);
          LedOff(LCD_RED);
          LedOff(LCD_BLUE);
          LedOff(LCD_GREEN);
          //update_score(score, 1);
        }
        break;
      case 2:
        if(IsButtonPressed(BUTTON2)){
          //ButtonAcknowledge(BUTTON2);
          LedOff(LCD_RED);
          LedOff(LCD_BLUE);
          LedOff(LCD_GREEN);
          //update_score(score, 1);
        }
        break;
      case 3:
        if(IsButtonPressed(BUTTON3)){
          //ButtonAcknowledge(BUTTON3);
          LedOff(LCD_RED);
          LedOff(LCD_BLUE);
          LedOff(LCD_GREEN);
          //update_score(score, 1);
        }
        break;
  
  
  
  }
  
  
  /*
  if (counter % 25 == 0){

    LcdMessage(LINE2_START_ADDR + position, "A");
    LcdClearChars(LINE2_START_ADDR, previous_position);
    
    previous_position = position;
    if(position == 20){
      position = 0;
    }else{
      position++;
    }
    
  
  
  }
  */
  
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
