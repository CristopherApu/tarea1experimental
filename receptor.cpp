#include <cstdio>   //Librería para printf
#include "funciones.h"  // Agregamos la cabecera de funciones
#include <unistd.h>               // for linux 
#include <wiringPi.h>
#include <cstdlib>

    //PROTOTIPOS DE FUNCIONES
    int desempaquetamiento(grupo6 &proto, int tam);
    void callback_receptor(void);
    void procesa_bit(bool level);

    //VARIABLE GLOBAL
    grupo6 proto;
    bool transmision_iniciada_receptor = false;
    volatile int nbits_receptor = 0; //  Declaramos una variable para contar los bits enviados
    volatile int nbytes_receptor = 0;    // Declaramos una variable para contar los bytes enviados
    int par = 0;    // Declaramos la variable que nos ayudará a buscar la paridad
    int impar = 0;  // Declaramos la variable que nos alertará que nos falta un 1 para ser par
    int s1 = 0;
    int comando;

int main(){
    // Agregar do-while

        if(wiringPiSetup() == -1){  // Condición en caso de que haya algún fallo con la librería
        exit(1);
        }

    pinMode(RX_PIN, INPUT);

    if(wiringPiISR(DELAY_PIN, INT_EDGE_RISING, &callback_receptor) < 0){   // Delcaramos la interrupción
        printf("No se puede iniciar la función de interrupción\n");
        }

        int s1 = 0;
        int tam = proto.lng +2;
        
        desempaquetamiento(proto, tam);

        sleep(1000);

        printf("Aviso: se recibió el empaquetamiento\n");
        if (proto.cmd == 1)
            {
                printf("Aviso: se iniciará la función cerrar_receptor desde receptor\n");
                cerrar_receptor();
            } else if (proto.cmd == 2){
                printf("Aviso: se iniciará la opción de recepción 2\n");
                for (size_t i = 1; i < 10; i++)
                {
                    bool estado = desempaquetamiento(proto, proto.lng);
                    if (estado == true){
                        //c = c++;
                    } else {
                          //  end = end++;
                        }          
                }
                    void porcentajesmensajes();
                } else if (proto.cmd == 3){
                    printf("Aviso: se iniciará la función recibir_guardar desde receptor\n");
                } else if (proto.cmd == 4){
                    printf("Aviso: se iniciará la función mostrar_archivo desde receptor\n");
                    mostrar_archivo();
                }
    
    return 0;
}

void callback_receptor(void){
  printf("Aviso: se iniciará la función callcabk_receptor\n");
  bool level = digitalRead(RX_PIN);
  if (transmision_iniciada_receptor){
    procesa_bit(level);
  } else if(level == 0 && !transmision_iniciada_receptor){
    transmision_iniciada_receptor = true;
    nbits_receptor = 1;
  }
}
void procesa_bit(bool nivel){
    printf("Aviso: se iniciará la función procesa_bit\n");

    //AQUI SACO EL CMD DESDE EL 2 HASTA EL 6
    
    //AQUI SACAMOS EL LNG 7 A 12
    if(7 <= nbits_receptor <= 12){
        int nbits_lng;
        for(int i = 0; i<=5; i++){
            (proto.lng & 0x01) << 1;        
            }
    }

    printf("%b", nivel);
    if(nbits_receptor < 9){
    proto.frame[nbytes_receptor] |= nivel << (nbits_receptor-1);
    } else if (nbits_receptor == 9) {
        par = nivel;
        for (size_t i = 0; i < 8; i++)  {
            s1 += (proto.frame[nbytes_receptor] >> i) & 0x01;
		}   if (par != (s1 % 2 == 0)) {
			impar = true;
		}
		nbytes_receptor++;
		transmision_iniciada_receptor = false;
	}
	nbits_receptor++;

}

int desempaquetamiento(grupo6 &proto, int tam) {
    // Implementar int LeerMensaje
    printf("Aviso: se iniciará la función desempaquetamiento\n");
    proto.cmd = proto.frame[0] & 0x0F;
    comando = proto.cmd;
    proto.lng = ((proto.frame[0] >> 4) & 0x0F) | ((proto.frame[1] & 0x01) << 4);
    if (tam != (proto.lng + 2)) { // Arroja error en caso de que el tamaño no coincida
        return false;
    }
    if (proto.lng > 0 && (proto.lng <= LARGO_DATO)) {   // En caso de que lng sea mayor a 0, y menor o igual a largo dato, comienza a desempaquetar
        for (int i = 0; i < proto.lng; i++) {
            proto.data[i] = ((proto.frame[i+1] >> 4) & 0x0F) | ((proto.frame[i+2] & 0x0F) << 4);
        }
    }
    int fcs_calculado = fcs(proto.frame, proto.lng + 2); // Calcula el fcs sobre los bits del paquete
    int fcs_recibido = proto.frame[proto.lng + 1] | ((proto.frame[proto.lng + 2] & 0x03) << 4); // Extrae el fcs
    if (fcs_calculado != fcs_recibido) {
        return false;
    }
    return comando;
}