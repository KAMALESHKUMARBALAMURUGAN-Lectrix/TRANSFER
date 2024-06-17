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

void counto(); 

void fingerprintDeletion(); 

void fingerprintRegistration(); 

ezButton toggleSwitch(4);       // create ezButton object that attach to pin 4; 

int relay = 13 ; // Plug the relay into Digital Pin 13 

 
 

void setup() { 

  Serial.begin(9600); 

  toggleSwitch.setDebounceTime(50);  // set debounce time to 50 milliseconds 

  pinMode(relay, OUTPUT); 

 
 

  /*Take FPSerial as communication serial port of the fingerprint module*/ 

  fingerprint.begin(); 

  /*Wait for Serial to open*/ 

  while (!Serial); 

 
 

  /*Test whether the device can properly communicate with mainboard Return true or false */ 

  while (fingerprint.isConnected() == false) { 

    Serial.println("Communication with device failed, please check connection"); 

    /*Get error code information*/ 

    //desc = fingerprint.getErrorDescription(); 

    //Serial.println(desc); 

    delay(1000); 

  } 

} 

 
 

void loop()  

{ 

  toggleSwitch.loop();  // MUST call the loop() function first 

 
 

  if (toggleSwitch.isPressed())         //turning "ON" FROM "OFF"  

  { 

    t1 = millis(); 

    /*for debugging */  // Serial.println("switch is pressed"); /*$$ success*/ 

 
 

    if (t1 > (t2 + 3000) && (count < 2 || count >4))  //This is for limiting the duration between the consecutive toggle switches(On,off -> On). the exception is given for mode changing counts like 2,3,4.   

    { 

      counto(); 

      // count=0; 

      // Serial.println("Switching is not within limit, so counting started again"); 

    } 

     

    Serial.print("t1= "); 

    Serial.println(t1); 

    // delay(250); 

  } 

 
 

  if (toggleSwitch.isReleased())        //turning "OFF" FROM "ON". 

  { 

    t2 = millis(); 

    Serial.print("t2= "); 

    Serial.println(t2); 

 
 

    // Calculate the time difference    //For ensuring particular time duration between switches "to add it to the count". 

    if (t2 >= t1) { 

      t0 = t2 - t1; 

    } 

 
 

//this following code i got from "perplexity AI" and without this snippet also i can able to get the same result, so i commented it. 

    /* else { 

      t0 = (ULONG_MAX - t1) + t2 + 1; // ULONG_MAX is the maximum value of an unsigned long 

    } 

 
 

    tr = t1 - t2; 

*/ 

    // Check if the time difference is less than 3000 ms for either press or release 

    if (t0 < 3000)// || (count>2 && count <6) /*|| (tr < 3000) */)       //for t0<3000 and count=2,3,4(that is the mode change counts), the count wont go to zero  

    { 

      Serial.println("yes, Within limit"); 

      count++; 

    }  

 
 

    // else if(t0 >3000 && (count>2 && count <6)) 

    // { 

    //   Serial.println("In mode change"); 

    //   count++; 

    // } 

 
 

    else  

    { 

      Serial.println("Not Within limit"); 

      count = 0; 

    } 

    Serial.print("Count= "); 

    Serial.println(count); 
    Serial.println("------------------------------");
 
 

     // /* & */ Serial.println(); 

     // /* & */ Serial.println("Before entering into the collectionFingerprint loop"); 

 
 

      // if (count==0) 

      // { 

      // Serial.println("Toggle the switch 2 times to enter into the comparison mode");  

      // } 

     

       if((fingerprint.collectionFingerprint(/*timeout=*/0)) != ERR_ID809) 

    { 

     // /* & */  Serial.println(); 

     // /* & */  Serial.println("entered into the collectionFingerprint loop"); 

      // while(fingerprint.detectFinger()) 

      // { 

      // delay(50); 

      //    if (count > 0 && count < 2)  

      //   { 

      //     Serial.println("Toggle the switch 2 times to enter into the comparison mode"); 

      //   } 

 
 

      //   else  if (count == 2) 

      //   { 

      //   fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDBlue, /*blinkCount = */3);  //blue LED blinks quickly 3 times, means it's in fingerprint comparison mode now 

      //   /*Wait for finger to relase */ 

      //   } 

         

      //   else if(count == 3) 

      //   {      //Red LED blinks quickly 3 times, means it's in fingerprint deletion mode now  

      //     /*Set fingerprint LED ring to always ON in red*/ 

      //     fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */3); 

      //   } 

 
 

      //   else if(count ==4) 

      //   {             //Yellow LED blinks quickly 3 times, means it's in fingerprint regisrtation mode now 

      //     /*Set fingerprint LED ring to always ON in yellow*/ 

      //     fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3); 

      //   } 

      // } 

     

    

 
 

  

    if (count >= 0 && count < 2)  

    { 

      Serial.println("Toggle the switch 2 times to enter into the comparison mode"); 
      Serial.println("Toggle the switch 3 times to enter into the Deletion mode"); 
      Serial.println("Toggle the switch 4 times to enter into the Registration mode"); 
      Serial.println("Toggle the switch 6 times to enter into the Complete deletion mode"); 
      Serial.println("------------------------------");

      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFadeIn, /*LEDColor = */fingerprint.eLEDWhite, /*blinkCount = */0);  //blue LED blinks quickly 3 times, means it's in fingerprint comparison mode now  

 
 

    } 

 
 

    if (count == 2 )  

    {  // 2 button switch for comparison 

       //  /* @*/ delay(2000); 

     fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDGreen, /*blinkCount = */3);  //blue LED blinks quickly 3 times, means it's in fingerprint comparison mode now 

     delay(250); 

     fingerprintComparison(); 

 
 
 

            //   int forId=fingerprint.verify(2); 

            //   if (forId != 0) 

            //   { 

            //     Serial.println("ID: "); 

            //     Serial.println(); 

            //   } 

            //   else 

            //   { 

            //     Serial.println("ID not found"); 

            //   } 

            // } 

    } 

 
 

    else if (count == 3 ) /*@*/ //&& count <4)  

    {  // 3 button switch for deletion 

      // /* @*/ delay(2000); 

      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDRed, /*blinkCount = */3); 

      delay(250); 

      fingerprintDeletion(); 

    } 

 
 

    else if (count == 4)  

    {  // 4 button switch for registration 

      fingerprintRegistration(); 

      fingerprint.ctrlLED(/*LEDMode = */fingerprint.eFastBlink, /*LEDColor = */fingerprint.eLEDYellow, /*blinkCount = */3); 

    }  

    else if (count == 6) 

    { 

     Serial.println("All fingerprints deleted"); 

     fingerprint.delFingerprint(DELALL); 

    } 

  } 

 }   //Here the void loop ends 

} 

 
 

// 

void fingerprintRegistration()  

{ 

   /* @ */ Serial.println(); 

  /* @ */ Serial.println("Entered Registration mode"); 

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

  while (i < COLLECT_NUMBER) { 

    fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eBreathing, /*LEDColor = */ fingerprint.eLEDBlue, /*blinkCount = */ 0); 

    Serial.print("The fingerprint sampling of the"); 

    Serial.print(i + 1); 

    Serial.println("(th) time is being taken"); 

    Serial.println("Please press down your finger"); 

    if ((fingerprint.collectionFingerprint(/*timeout = */ 10)) != ERR_ID809) { 

      fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eFastBlink, /*LEDColor = */ fingerprint.eLEDYellow, /*blinkCount = */ 3); 

      Serial.println("Capturing succeeds"); 

      i++; 

    } else { 

      Serial.println("Capturing fails"); 

    } 

    Serial.println("Please release your finger"); 

    while (fingerprint.detectFinger()) 

      ; 

  } 

  if (fingerprint.storeFingerprint(/*Empty ID = */ ID) != ERR_ID809) { 

    Serial.print("Saving succeed,ID="); 

    Serial.println(ID); 

    fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eKeepsOn, /*LEDColor = */ fingerprint.eLEDGreen, /*blinkCount = */ 0); 

    delay(1000); 

    fingerprint.ctrlLED(/*LEDMode = */ fingerprint.eNormalClose, /*LEDColor = */ fingerprint.eLEDBlue, /*blinkCount = */ 0); 

  } else { 

    Serial.println("Saving failed"); 

  } 

  Serial.println("-----------------------------"); 

} 

 
 
 

void fingerprintComparison()  

{ 

 /* @ */ 

 // while(fingerprint.detectFinger()) 

   

  Serial.println(); 

  Serial.println("Entered Comparison mode"); 

 /*@ */Serial.println("After delay"); 

  uint8_t ID= fingerprint.search(); 

  // Serial.print("id: "); 

  // Serial.println(ret); 

  if (/*ret != ERR_ID809 && */ID !=0)  

  { 

    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0); 

    Serial.print("Successfully matched, ID="); 

    Serial.println(ID); 

    digitalWrite(relay, HIGH); // Turn the relay on 

    // delay(1000); // Wait 1 second 

  }  

  else  

  { 

    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0); 

    Serial.println("Matching failed"); 

  } 

  // delay(1000); 

  fingerprint.ctrlLED(fingerprint.eNormalClose, fingerprint.eLEDBlue, 0); 

  Serial.println("-----------------------------"); 

  //@ Serial.println("Before break"); 

  //break; 

  //@ Serial.println("After break"); 

} 

 
 
 

void fingerprintDeletion()  

{ 

  Serial.println(); 

  Serial.println("Entered Deletion mode"); 

  uint8_t ret = fingerprint.search(); 

  if (ret != ERR_ID809 && ret !=0)  

  { 

    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDGreen, 0); 

    fingerprint.delFingerprint(ret); 

    Serial.print("Deleted fingerprint, ID="); 

    Serial.println(ret); 

  } else { 

    fingerprint.ctrlLED(fingerprint.eKeepsOn, fingerprint.eLEDRed, 0); 

    Serial.println("Matching failed or the fingerprint hasn't been registered"); 

  } 

  delay(1000); 

  fingerprint.ctrlLED(fingerprint.eNormalClose, fingerprint.eLEDBlue, 0); 

  Serial.println("-----------------------------"); 

} 

 
 

void counto() { 

  /*@*/ Serial.println("entered counto function(for late button switch)"); 

    count = 0; 

} 

 