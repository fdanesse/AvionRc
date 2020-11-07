// https://lastminuteengineers.com/adxl335-accelerometer-arduino-tutorial/
/*
Los tres ángulos son la dirección (heading o yaw), elevación (pitch) y ángulo de alabeo (roll).
Estos tres ángulos son equivalentes a tres maniobras consecutivas. Dado un sistema de tres ejes fijos en el aeroplano,
llamados eje de guiñada (yaw en inglés), de cabeceo (pitch) y de alabeo (roll), existen tres rotaciones principales,
normalmente llamadas igual que el eje sobre el que se producen,
que permiten alcanzar el sistema del aeroplano desde el sistema de referencia.

  Cabeceo (pitch): es una inclinación del morro del avión, o rotación respecto al eje ala-ala. (Derecha - Izquierda)
  Alabeo (roll): rotación respecto de un eje morro-cola del avión. (Arriba - Abajo)
  Guiñada (yaw): rotación intrínseca alrededor del eje vertical perpendicular al avión.
*/


#define xInput A0
#define yInput A1
#define zInput A2

double x_g_value, y_g_value, z_g_value;
double roll, pitch, yaw;

const int sampleSize = 10;


void setup(){
    Serial.begin(9600);
}

void loop(){
    giroscopio();
    delay(200);
}

void giroscopio(){
    int xRaw = ReadAxis(xInput);
    int yRaw = ReadAxis(yInput);
    int zRaw = ReadAxis(zInput);

    x_g_value = ((((double)(xRaw * 5)/1024) - 1.65) / 0.330);
    y_g_value = ((((double)(yRaw * 5)/1024) - 1.65) / 0.330);
    z_g_value = ((((double)(zRaw * 5)/1024) - 1.80) / 0.330); 
  
    roll = (((atan2(y_g_value, z_g_value) * 180) / 3.14) + 180);
    pitch = (((atan2(z_g_value, x_g_value) * 180) / 3.14) + 180);
    // yaw = (((atan2 (x_g_value, y_g_value) * 180) / 3.14) + 180);
    // No es posible medir la guiñada con un acelerómetro. Se debe usar un giroscopio.
  
    Serial.print(" roll: "); Serial.print(roll);
    Serial.print(" pitch: "); Serial.println(pitch);  
}

int ReadAxis(int axisPin){
    long reading = 0;
    analogRead(axisPin);
    delay(1);
    for (int i = 0; i < sampleSize; i++){
        reading += analogRead(axisPin);
    }
    return reading/sampleSize;
}
