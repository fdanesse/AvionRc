// Radiofrecuencia https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html

#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> // Not actually used but needed to compile
#endif

RH_ASK driver(2000, 11, 12, -1); //Default settings (Speed = 2000bps; RX pin = 11; TX pin = 12; Ptt pin = 10) RX pin (receiver) - TX pin (transmitter)


struct Data {
    int x = 512;  //Servo2 funciona de 750 - 2250 https://www.hackster.io/ashraf_minhaj/how-to-use-servotimer2-library-simple-explain-servo-sweep-512fd9
    int y = 512;
    int motor = 0;
};

Data data;


#include<Servo.h>
Servo motor;
Servo timon;
Servo aleron;

#define buz 7

/*
 * D7 buzzer
 * D11 RF433 MHz
 * D4 motor
 * D5 timon
 * D6 aleron
 */


void tonos(){
    tone(buz, 523.25, 50); //Do 554.37
    delay(100);
    tone(buz, 587.33, 50); //RE 622.25 1174.66
    delay(100);
    tone(buz, 659.26, 50); //MI
    delay(100);
    tone(buz, 698.46, 50); //FA 739.99 1479.98
    delay(100);
    tone(buz, 783.99, 50); //SOL 830.61
    delay(100);
    tone(buz, 880, 50); //LA 932.33
    delay(100);
    tone(buz, 987.77, 50); //SI
    delay(100);
}


void setup() {
    digitalWrite(LED_BUILTIN, HIGH);
    tonos();

    pinMode(5, OUTPUT);
    timon.attach(5);
    timon.write(90);

    pinMode(6, OUTPUT);
    aleron.attach(6);
    aleron.write(90);
    /*
    pinMode(4, OUTPUT);
    motor.attach(4);
    motor.writeMicroseconds(1000);*/
    delay(5000);
    tonos();
    
    Serial.begin(9600);
    Serial.println(F("Iniciado"));

    // Radio Frecuencia
    if (driver.init()){digitalWrite (LED_BUILTIN, LOW);}
    else{Serial.println(F("ERROR Radiofrecuencia"));}
}

void loop() {
    recepcion();
    
    //control_motor();
    //control_timon();
    //control_aleron();
    
    delay(100);
}

void recepcion(){
    static unsigned long ultima = 0;
    unsigned long tiempo = millis();  // Control perdida de señal
    
    uint8_t buflen = sizeof(data);
    if (driver.recv((uint8_t*)&data, &buflen)){
        ultima = tiempo;
        Serial.print(data.x); Serial.print(" "); Serial.print(data.y); Serial.print(" "); Serial.println(data.motor);
    }else{
        if (tiempo - ultima > 3000) {
            tone(buz, 523.25, 10);
            //Serial.println(tiempo - ultima);
            }
    }
}

void control_motor(){
    //vel = analogRead(A0);  //potentiometer (value between 0 and 1023)
    //vel = map(vel, 0, 1023, 1000, 2100);  // funciona de 1500 a 2100
    //motor.writeMicroseconds(vel);  
}

void control_timon(){
    int grados = data.y;
    grados = constrain(map(grados, 0, 1023, 0, 180), 0, 180);
    timon.write(grados);
    Serial.print(timon.read()); Serial.print(" "); Serial.println(grados);
}

void control_aleron(){
    int grados = data.x;
    grados = constrain(map(grados, 0, 1023, 0, 180), 0, 180);
    aleron.write(grados);
    Serial.print(aleron.read()); Serial.print(" "); Serial.println(grados);
}
