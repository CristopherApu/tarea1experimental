#include <cstdio>   //Librería para printf
#include <cstdlib>  // Librería para exit(0)
#include "funciones.h"  // Agregamos la cabecera de funciones
#include "protocolo.h"  // Agregamos el protocolo
#include <wiringPi.h>   // Agregamos la librería para GPIO



int main(){
    if(wiringPiSetup() == -1){  // Condición en caso de que haya algún fallo con la librería
        exit(1);
        }

    pinMode(TX_PIN, OUTPUT);

    if(wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &callback_emisor) < 0){   // Delcaramos la interrupción
        printf("No se puede iniciar la función de interrupción\n");
        }

    grupo6 proto;
    int SN2 = 1;
    while (SN2 == 1){
        printf("Aviso: se iniciará el while\n");
        menu();
        int rim = 0;
        scanf("%d", &rim);   // rim = respuesta menú inicial
        switch (rim) {
            case 1:
                printf("Aviso: se iniciará el caso 1\n");
                proto.cmd = 1;
                proto.lng = 0;
                proto.frame[0] = (proto.cmd & 0x0F) | ((proto.lng & 0x0F) << 4);
                proto.frame[1] = ((proto.lng >> 4) & 0x01);
                void callback_emisor(void);
                SN2 = 1;
                break;
            case 2:
            printf("Aviso: se iniciará el caso 2\n");
                mensaje_prueba(proto);
                SN2 = 1;
                break;
            case 3:
            printf("Aviso: se iniciará el caso 3\n");
                proto.cmd=3;
                enviar(proto);
                SN2 = 1;
                break;
           case 4:
                printf("Aviso: se iniciará el caso 4\n");
                proto.cmd=4;
                archivo_texto();
                SN2 = 1;
                break;
           default:
                printf("Aviso: se iniciará el caso default\n");
                proto.cmd=5;
                printf("Acción no encontrada :'(\n");
                SN2 = 1;
                break;
        }
        while(transmision_iniciada_emisor) {
          delay(1000); // Espera mientras se realiza la transmisión
        }
        printf("Qusieras realizar otra acción?\n");
        printf("1.- Sí\n2.- No\n");
        scanf("%d", SN2);
    }
        printf("Aviso: fin del while\n");
    return 0;
}