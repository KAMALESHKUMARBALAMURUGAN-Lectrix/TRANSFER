//1. fingerprint.search() is the function which causing the issue -rectified 

//2. comparison and deletion mode is not working properly(i can't able to compare or delete) -rectified 

//3. add a if else statement at line 100 that will do the work for the counts - Did

//4. if i want to get out of the loop and again to do the comparison mode i have to press 5 times - and I have to do the registration mode compulsorily and after that i have to delay for 3 seconds to come out of the loop to make the "count as 1" - rectified(User can avoid touching the sensor to avoid entering into the particular mode)



/*  

Press 2 times to enter comparison mode 

Press 3 times to enter deletion mode 

Press 4 times to enter registration mode 

press 6 times to delete all the fingerprints.
*/ 


#include <ezButton.h>               //Library for Toggle Switch
#include <DFRobot_ID809_I2C.h>      //Library for Fingerprint sensor
#define COLLECT_NUMBER 3            //Fingerprint sampling times, can be set to 1-3 
#define IRQ 2                       //IRQ pin (This is used to create a if condition for the first touch in the sensor(SEN0359) but i used a toggle switch to initiate or change the mode, so this is not used in this code ) 
DFRobot_ID809_I2C fingerprint;      //String desc for the fingerprint sensor

unsigned long t1, t2;               //Used to count the number of Toggle switch counts
int count; 
uint16_t t0, tr;                //I used "tr" to find the difference between first on->off and next "on" 

void countZero(); 

void fingerprintDeletion(); 

void fingerprintRegistration(); 

void fingerprintComparison();

ezButton toggleSwitch(4);       // create ezButton object that attach to pin 4; 

int relay = 13 ; // Plug the relay into Digital Pin 13 

 
void setup() { 

Serial.begin(9600); 
toggleSwitch.setDebounceTime(50);  // set debounce time to 50 milliseconds 
pinMode(relay, OUTPUT);            //Relay input pin is programmed with the pin !3

/*Take FPSerial as communication serial port of the fingerprint module*/ 
fingerprint.begin(); 
/*Wait for Serial to open*/ 
while (!Serial); 

 /*Test whether the device can properly communicate with mainboard Return true or false */ 
while (fingerprint.isConnected() == false) 
 { 
    Serial.println("Communication with device failed, please check connection"); 
    /*Get error code information*/ 
    //desc = fingerprint.getErrorDescription(); 
    //Serial.println(desc); 
    delay(1000); 
  } 
} 


void loop()  
{ 
  toggleSwitch.loop();                  // MUST call the loop() function first 
  if (toggleSwitch.isPressed())         //turning "ON" FROM "OFF"  -HIGH signal from Toggle Switch
  { 
    t1 = millis(); 
    
    if (t1 > (t2 + 3000) && (count < 2 || count >4))  //This is for limiting the duration between the consecutive toggle switches(On,off -> On). the exception is given for mode changing counts like 2,3,4.   
    { 
      countZero();                         // count=0(this function is to make count =0 when the condition in the "if passes");
    } 
     
    Serial.print("t1= "); 
    Serial.println(t1); 
  } 

  if (toggleSwitch.isReleased())        //turning "OFF" FROM "ON" -LOW signal from Toggle Switch
  { 
    t2 = millis(); 
    Serial.print("t2= "); 
    Serial.println(t2); 

    // Calculate the time difference    //For ensuring particular time duration between switches "to add it to the count". 
    if (t2 >= t1) 
    { 
      t0 = t2 - t1; 
    } 
  

    if (t0 < 3000)                      //for t0<3000, the count Will be incremented
    { 
      Serial.println("yes, Within limit"); 
      count++; 
    }  

    else                                //for t0>3000, the count Will be changed to zero                              
    { 
      Serial.println("Not Within limit"); 
      count = 0; 
    } 
    Serial.print("Count= "); 
    Serial.println(count); 
    Serial.println("------------------------------");

    if((fingerprint.collectionFingerprint(/*timeout=*/0)) != ERR_ID809)                 //Calling the collectionFingerprint function to detect the placement of fingerprint on the sensor and timeout is given as "0". If we want more timeout we can add but it will cause a delay in the detection.
      {
        /*  &  */ Serial.println("Entered in CollectionFingerprint statement");
        if (/*(count > 0|| count==0) && */ count ==0 || count ==1)  
        { 
          Serial.println("Toggle the switch 2 times to enter into the comparison mode"); 
          Serial.println("Toggle the switch 3 times to enter into the Deletion mode"); 
          Serial.println("Toggle the switch 4 times to enter into the Registration mode"); 
          Serial.println("Toggle the switch 6 times to enter into the Complete deletion mode"); 
          Serial.println("------------------------------");
          fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFadeIn, /*LEDColor = */fingerprint.eLEDWhite, /*blinkCount = */0);  //blue LED blinks quickly 3 times, means it's in fingerprint comparison mode now  
        } 

    
        else if (count == 2 )                    // 2 button switch for comparison
        {   
        fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */3);     //blue LED blinks quickly 3 times, representing fingerprint comparison mode now .
        delay(250); 
        fingerprintComparison();          //once the fingerprint is detected using collectionFingerprint function and count==2, fingerprintComparison Function will be called.
        }

        else if (count == 3 )              // 3 button switch for deletion
        {   
          fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */3);     //Red LED blinks quickly 3 times, representing fingerprint Deletion mode now .
          delay(250); 
          fingerprintDeletion(); 
        } 

        else if (count == 4)                // 4 button switch for registration 
        {  
          fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3); //blue LED blinks quickly 3 times, representing fingerprint Registration mode now .
          fingerprintRegistration(); 
        }  

        else if (count == 6) 
        { 
        Serial.println("All fingerprints deleted"); 
        fingerprint.ctrlLED(/*LEDMode = */fingerprint.eSlowBlink, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */6);     //blue LED blinks quickly 6 times, representing Complete deletion mode now .
        fingerprint.delFingerprint(DELALL); 
        } 

        else
        {
          Serial.println("Enter the appropriate count to enter into the Respective modes");
        }
      } 
  }   //Here the void loop ends 
}
 

void fingerprintRegistration()  
{ 
Serial.println(); 
Serial.println("Entered Registration mode"); 
  uint8_t ID, i; 
  fingerprint.search(); 
  if ((ID = fingerprint.getEmptyID()) == ERR_ID809)  
  { 
    while (1)  
    { 
      delay(1000); 
    } 
  } 
  Serial.print("Unregistered ID,ID="); 
  Serial.println(ID); 
  i = 0; 
  while (i < COLLECT_NUMBER) 
  { 
    fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eBreathing, /*LEDColor = */ fingerprint.eLEDBlue, /*blinkCount = */ 0); 
    Serial.print("The fingerprint sampling of the"); 
    Serial.print(i + 1); 
    Serial.println("(th) time is being taken"); 
    Serial.println("Please press down your finger"); 

    if ((fingerprint.collectionFingerprint(/*timeout = */ 10)) != ERR_ID809) 
    { 
      fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eFastBlink, /*LEDColor = */ fingerprint.eLEDYellow, /*blinkCount = */ 3); 
      Serial.println("Capturing succeeds"); 
      i++; 
    } 

    else 
    { 
      Serial.println("Capturing fails"); 
    } 
    Serial.println("Please release your finger"); 
    while (fingerprint.detectFinger());
  } 
  if (fingerprint.storeFingerprint(/*Empty ID = */ ID) != ERR_ID809) 
  { 
    Serial.print("Saving succeed,ID="); 
    Serial.println(ID); 
    fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eKeepsOn, /*LEDColor = */ fingerprint.eLEDGreen, /*blinkCount = */ 0); 
    delay(1000); 
    fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eNormalClose, /*LEDColor = */ fingerprint.eLEDBlue, /*blinkCount = */ 0); 
  } 
  else 
  { 
    Serial.println("Saving failed"); 
  } 
  Serial.println("-----------------------------"); 
} 



void fingerprintComparison()  
{ 
  Serial.println(); 
  Serial.println("Entered Comparison mode"); 

  uint8_t ID= fingerprint.search(); 

  if (ID !=0)                                 //If fingerprint is not matched, we will get a return ID=0(from fingerprint.search() function)
  { 
    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0); 
    Serial.print("Successfully matched, ID="); 
    Serial.println(ID); 
    digitalWrite(relay, HIGH);                // Turn the relay input high if the fingerprint matched correctly.
  }  

  else  
  { 
    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0); 
    Serial.println("Matching failed"); 
  } 

  fingerprint.ctrlLED(fingerprint.eNormalClose, fingerprint.eLEDBlue, 0); 
  Serial.println("-----------------------------"); 
} 



void fingerprintDeletion()  
{ 
  Serial.println(); 
  Serial.println("Entered Deletion mode"); 
  uint8_t ret = fingerprint.search(); 
  if (ret != ERR_ID809 && ret !=0)                  //If fingerprint is not matched, we will get a return ID=0(from fingerprint.search() function)
  { 
    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0); 
    fingerprint.delFingerprint(ret); 
    Serial.print("Deleted fingerprint, ID="); 
    Serial.println(ret); 
  } 
  
  else 
  {
    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0); 
    Serial.println("Matching failed or the fingerprint hasn't been registered"); 
  } 
  // delay(1000); 
  fingerprint.ctrlLED(fingerprint.eNormalClose, fingerprint.eLEDBlue, 0); 
  Serial.println("-----------------------------"); 
} 

void countZero() 
{ 
  /*@*/ Serial.println("entered countZero function(for late button switch)"); 
    count = 0; 
} 

 