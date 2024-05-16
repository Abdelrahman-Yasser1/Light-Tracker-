#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

#define motor1_Speed 9
#define motor2_Speed 5
#define motorUp 4
#define motorDown 7
#define motorRight 8
#define motorLeft 12l
#define ldrUp A3
#define ldrDown A2
#define ldrRight A1
#define ldrLeft A0
#define IR_REC_PIN 2
//#define Right_button 0X609FF708 //RIGHT 
//#define Left_button 0x6897F708  //LEFT 
//#define Up_button 0x6D92F708  //UP 
//#define Down_button 0x27D8F708 // DOWN 
//#define IR_button 0x28D7F708   //power MANUAL  
//#define LDR_button 0x649BF708 //Disable Remote

#define Right_button 0XA55AFF00 // RIGHT // right
#define Left_button  0XF708FF00  //LEFT //left
#define Up_button    0XE718FF00   //UP // up
#define Down_button  0XAD52FF00  //DOWN // down
#define IR_button    0XF20DFF00 //# //Power
#define LDR_button   0XE916FF00  //disable motor*/

IRrecv receiver(IR_REC_PIN);
bool IR = true;
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool Flag_R=0;
bool Flag_U=0;
void stopMotor(int motorPin1, int motorPin2) {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  if (motorPin1==motorRight || motorPin2==motorRight)Flag_R=0;
  else Flag_U=0;

}
void moveMotor(int motorPin1, int motorPin2, int speed, const char* message,int Diff,bool IR) {
  
  Serial.println(Diff);
  
  if(IR == true)
  {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  lcd.print(message);
  analogWrite(speed, 255);
  delay(400);
   stopMotor(motorPin1,motorPin2);
  }
  else
  {
   
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  lcd.print(message);
  //Serial.print(Diff);
  //Serial.print("                        ");
  if (motorPin1==motorRight || motorPin2==motorRight){
  if  (Flag_R==1){//if 1 then motor is ON
  Diff= map(Diff,10,1023,190,255);
  Serial.print(Diff);
  analogWrite(speed, Diff);
  }
  else{
      analogWrite(speed, 255);
      Flag_R=1;
    }
  }
    else{
      if  (Flag_U==1){//if 1 then motor is ON
  Diff= map(Diff,25,1023,190,255);
  Serial.print(Diff);
  analogWrite(speed, Diff);
  }
  else{
      analogWrite(speed, 255);
      Flag_U=1;
    }
      
      }
    }
  }

void setup() {
  
  pinMode(IR_REC_PIN, INPUT);
  pinMode(motorUp, OUTPUT);
  pinMode(motorDown, OUTPUT);
  pinMode(motorRight, OUTPUT);
  pinMode(motorLeft, OUTPUT);
  Serial.begin(9600);
  receiver.enableIRIn(); 
  lcd.begin();
  lcd.backlight();
   lcd.clear();
  lcd.print("Project Ready");
  delay(1000);
}

void loop() {
  Serial.println(receiver.decodedIRData.decodedRawData, HEX);
  
 if (receiver.decode())   
{
    if (receiver.decodedIRData.decodedRawData == IR_button)
    {
      stopMotor(motorRight, motorLeft);
      stopMotor(motorUp, motorDown);
      IR = true;
      Serial.println("IRTRUE");
      lcd.clear();
      lcd.print("Manual Mode");
    }
      
     else if (receiver.decodedIRData.decodedRawData == LDR_button) {
      IR = false;
      Serial.println("FALSE");
   
    } 
   if(IR == true)
   {  
   switch (receiver.decodedIRData.decodedRawData) {
        case Right_button:
         lcd.setCursor(0,1);
         moveMotor(motorRight, motorLeft, motor2_Speed, "Moving Right",255,IR);
         lcd.clear();
         lcd.print("Manual Mode");
          break;
        case Left_button:
         lcd.setCursor(0,1);
         moveMotor(motorLeft, motorRight, motor2_Speed, "Moving Left",255,IR);
         lcd.clear();
         lcd.print("Manual Mode");
          break;
        case Up_button:
          lcd.setCursor(0,1);
          moveMotor(motorUp, motorDown, motor1_Speed, "Moving Up",255,IR);
          lcd.clear();
         lcd.print("Manual Mode");
          break;
        case Down_button:
          lcd.setCursor(0,1);
          moveMotor(motorDown, motorUp, motor1_Speed, "Moving Down",255,IR);
          lcd.clear();
          lcd.print("Manual Mode");
          break;
          
         }
      }
   receiver.resume();
   delay(500);
   return;
 }
  
  if (IR==false) { 
    Serial.println("LDR TRUE");
    
    int UP_ldrINPUT = analogRead(ldrUp);
    int DOWN_ldrINPUT = analogRead(ldrDown);
    int RIGHT_ldrINPUT = analogRead(ldrRight);
    int LEFT_ldrINPUT = analogRead(ldrLeft);
    int U_D_Difference = abs(UP_ldrINPUT - DOWN_ldrINPUT); 
    int R_L_Difference = abs(RIGHT_ldrINPUT - LEFT_ldrINPUT); 
   lcd.clear();
   
  if (U_D_Difference >= 10) {
      
      if (UP_ldrINPUT > DOWN_ldrINPUT) {
        moveMotor(motorUp, motorDown, motor1_Speed, "Moving Up",U_D_Difference,IR);
      } else if(UP_ldrINPUT < DOWN_ldrINPUT) {
        moveMotor(motorDown, motorUp, motor1_Speed, "Moving Down",U_D_Difference,IR);
      }
  
    } else  {
      stopMotor(motorUp, motorDown);
      
      lcd.print("Stop1");
      
    }
    lcd.setCursor(0,1);
    if (R_L_Difference >= 10) {
      if (RIGHT_ldrINPUT > LEFT_ldrINPUT) {
        moveMotor(motorRight, motorLeft, motor2_Speed, "Moving Right",R_L_Difference,IR);
      } else if(RIGHT_ldrINPUT < LEFT_ldrINPUT) {
        moveMotor(motorLeft, motorRight, motor2_Speed, "Moving Left",R_L_Difference,IR);
      }
      
    } else {
      stopMotor(motorRight, motorLeft);
      lcd.print("Stop2");
    }
    
  receiver.resume();
   delay(500);
    }
}
