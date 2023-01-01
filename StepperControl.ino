#define dirPin 8   // Microstep driver DIR+ pin
#define PulsePin 9 // Microstep driver PUL+ pin
int DutyCounter;
volatile int PulseTime = 4; //pulseTime*25us = OCR2A = 49 // pulse uzunluğunu belirlemek için kullanılmakta.
volatile int turnnCntr =0; // Gelen pulse sayısını sınırlandırmak için kullanıldı

void setup() {
       DDRB = 0b00000011;
      
       cli();//stop interrupts
       //set timer2 interrupt at 8kHz
       TCCR2A = 0;// set entire TCCR2A register to 0
       TCCR2B = 0;// same for TCCR2B
       TCNT2  = 0;//initialize counter value to 0
       // set compare match register for 8khz increments
       OCR2A = 49;// = (16*10^6) / (40000*8) - 1 (must be <256)//////// 25us
       // turn on CTC mode
       TCCR2A |= (1 << WGM21);
       // Set CS21 bit for 8 prescaler
       TCCR2B |= (1 << CS21);  
       // enable timer compare interrupt
       TIMSK2 |= (1 << OCIE2A);
       sei();//allow interrupts



       for(int i=0;i<1;i++){  //sağa 0.4 adım sola 0.8 adım sağa 0.4 adım hareket edip durduruldu
              PORTB &= ~B00000001;  //1 devir 3200 Step
              turnnCntr = 1280;    //başlat 0.4 step hareket etti
              while(turnnCntr>0);  //bitmesini bekle
              delay(2000);
              
              PORTB |= B00000001; 
              turnnCntr = 2560;    //başlat 
              while(turnnCntr>0);  //bitmesini bekle
              delay(2000);
        
              PORTB &= ~B00000001;
              turnnCntr = 1280;    //başlat 
              while(turnnCntr>0);  //bitmesini bekle
              delay(2000);
       }

      

}

ISR(TIMER2_COMPA_vect){

    DutyCounter++; if(DutyCounter >= PulseTime) DutyCounter=0;
  
    if(turnnCntr>0){  // Pulse üretildi
        if(DutyCounter == 0){digitalWrite(PulsePin, HIGH);}
        if(DutyCounter == PulseTime/2){digitalWrite(PulsePin, LOW);
           if(turnnCntr>0) turnnCntr--;
         }      
     }  
}




void loop() {
  // put your main code here, to run repeatedly:

}