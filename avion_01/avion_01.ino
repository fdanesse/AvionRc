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


#include<ServoTimer2.h> // Esta librería tiene conflicto con Tone.cpp, pues ambas usan timer2

ServoTimer2 motor;
ServoTimer2 timon;
ServoTimer2 aleron;

/*
 * D11 RF433 MHz
 * D4 motor
 * D5 timon
 * D6 aleron
 */


void setup() {
    digitalWrite(LED_BUILTIN, HIGH);

    pinMode(5, OUTPUT);
    timon.attach(5);

    pinMode(6, OUTPUT);
    aleron.attach(6);
    
    pinMode(4, OUTPUT);
    motor.attach(4);
    motor.write(1000);
    delay(5000);
    
    Serial.begin(9600);
    Serial.println(F("Iniciado"));

    // Radio Frecuencia
    if (driver.init()){digitalWrite (LED_BUILTIN, LOW);}
    else{Serial.println(F("ERROR Radiofrecuencia"));}
}

void loop() {
    recepcion();
    
    control_motor();
    control_timon();
    control_aleron();
    
    delay(50);
}

void recepcion(){
    static unsigned long ultima = 0;
    unsigned long tiempo = millis();  // Control perdida de señal
    
    uint8_t buflen = sizeof(data);
    if (driver.recv((uint8_t*)&data, &buflen)){
        ultima = tiempo;
        /*
        Serial.print(" x: "); Serial.print(data.x);
        Serial.print(" y: "); Serial.print(data.y);
        Serial.print(" m: "); Serial.println(data.motor);
        */
    }else{
        if (tiempo - ultima > 3000) {}
    }
}

void control_motor(){
    int vel = data.motor;
    vel = constrain(map(vel, 0, 1023, 1000, 2100), 750, 2250);
    motor.write(vel);
}

void control_timon(){
    int grados = data.y;
    grados = constrain(map(grados, 0, 1023, 750, 2250), 750, 2250);
    timon.write(grados);
}

void control_aleron(){
    int grados = data.x;
    grados = constrain(map(grados, 0, 1023, 750, 2250), 750, 2250);
    aleron.write(grados);
}
