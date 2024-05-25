#include <wiringPi.h>   // Agregamos la librería para GPIO
#include <cstdlib>  // Librería para exit(0)
#include "funciones.h"  // Agregamos la cabecera de funciones
#include "protocolo.h"  // Agregamos el protocolo

int main(){

    if(wiringPiSetup() == -1){  // Condición en caso de que haya algún fallo con la librería
    exit(1);
    }

    pinMode(PWM_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS); //Mark Space Mode  ??
    pwmSetRange(PWM_RANGE); // Definimos valores de la macro
    pwmSetClock(PWM_CLOCK_DIVISOR); // Definimos valores de la macro
    pwmWrite(PWM_PIN, PWM_RANGE/2); // Medio ciclo del rango, para tratar de dar en medio de la señal

    while(1){   // Consumimos el bit
    delay(PWM_RANGE/4);
    }

    return 0;
}