#include <cstdio>   //Librería para printf
#include <cstring>  // Librería para cadenas
#include <cstdlib>  // Librería para exit(0)
#include "funciones.h"  // Agregamos la cabecera de funciones
#include "protocolo.h"  // Agregamos la cabecera de protocolo
#include <wiringPi.h>   // Agregamos la librería para GPIO
#include <string>

volatile int nbits_emisor = 0; //  Declaramos una variable para contar los bits enviados
volatile int nbytes_emisor = 0;    // Declaramos una variable para contar los bytes enviados
bool transmision_iniciada_emisor = false;
int nones = 0;  // Delcaramos para calcular paridad
int tam_emp;

void menu() {
    printf("Aviso: se iniciará la función menú\n");
    printf("Bienvenid@ a la Tarea 1!\n\nFavor, indíquenos ¿Qué acción le gustaría realizar?\n");
    printf("1.- Cerrar el programa receptor         3.- Enviar mensaje de texto\n"
           "2.- Enviar mensaje de prueba            4.- Visualizar mensaje de archivo de texto\n");
}
void cerrar_receptor(){
    printf("Aviso: se iniciará la función cerrar_receptor\n");
    exit(0);
}
void mensaje_prueba(grupo6 &proto){
    printf("Aviso: se iniciará la función mensaje_prueba\n");
    for (size_t i = 0; i < 10; i++)
    {
        proto.cmd=2;
        const char* mensaje_de_prueba = "Mensaje de prueba Grupo 6";
        strcpy(reinterpret_cast<char*>(proto.data), mensaje_de_prueba); // Copiamos el mensaje en data
        proto.lng = strlen(mensaje_de_prueba);
        empaquetamiento(proto); // Empaqueta los datos antes de copiarlos en el frame
        printf("Mensaje %ld enviado correctamente!\n", i+1);
    }
    printf("Mensajes enviados correctamente!\n");
    printf("El mensaje es: %s\n",proto.data);
    printf("El largo del mensaje es de: %d bytes\n",proto.lng);
    printf("El comando para esta acción es: %d\n",proto.cmd);
}
void enviar(grupo6 &proto){
    printf("Aviso: se iniciará la función enviar\n");
    printf("Favor, ingrese su mensaje a enviar\n");
    while (true) {
        scanf("%s", proto.data); // Almacena un mensaje de máximo 31 bytes
        proto.lng = strlen((const char*) proto.data);   // Asigna a lng el largo de data con strlen
        if (proto.lng > 31) {   // Condiciona que el mensaje sea de máximo 31 bytes
                                // En caso de usar %31s generaba errores cuando el usuario ingresaba más bits,
                                // ya que a utilizaba esas entradas como valores de las siguientes variables
            printf("El mensaje sobrepasa la capacidad de almacenamiento que tiene nuestro protocolo\n");
            printf("Favor, ingrese su mensaje a enviar con un máximo de 31 bytes\n");
        } else {
            break; // Prosigue con el código en caso de que se cumpla la condición
        }
    }
    empaquetamiento(proto); // Empaqueta los datos antes de copiarlos en el frame
    //AQUI SE EMPIEZA A MANDAR LOS DATOS DEL EMPAQUETAMIENTO
    startTransmission_emisor();
    printf("Mensaje enviado correctamente!\n");
}
void archivo_texto(){
    printf("Aviso: se iniciará la función archivo_texto\n");
}
int empaquetamiento(grupo6 &proto) {
    printf("Aviso: se iniciará la función empaquetamiento\n");
    printf("inicia bool empaquetamiento");
    proto.frame[0] = (proto.cmd & 0x0F) | ((proto.lng & 0x0F) << 4); // Agregamos al paquete el cmd y los 4 bits que nos alcanzaron a entrar del lng dentro del primer byte
    proto.frame[1] = ((proto.lng >> 4) & 0x01); // Terminamos de cargar el lng
    memcpy(&proto.frame[2], proto.data, proto.lng);
    proto.fcs = fcs(proto.frame, proto.lng + 2);    // Calculamos fcs adicionando el cmd y el lng
    proto.frame[proto.lng + 2] = proto.fcs & 0xFF;  // Se agregan los bits menos significativos primero
    proto.frame[proto.lng + 3] = (proto.fcs >> 8) & 0x01;
    tam_emp = proto.lng + 4;
    return tam_emp;   // Indica el tamaño del paquete
}
int fcs(BYTE *arr, int tam_fcs) {
    printf("Aviso: se iniciará la función FCS\n");
    int valor_fcs = 0; // Inicializa el valor del fcs a 0
    for (int i = 0; i < tam_fcs; i++) {
        valor_fcs ^= arr[i]; // Realiza una operación XOR con cada byte del arreglo
        for (int j = 0; j < 8; j++) {
            if (valor_fcs & 0x01) {
                valor_fcs = ((valor_fcs >> 1) ^ 0x100);
            } else {
                valor_fcs >>= 1;
            }
        }
    }
    return valor_fcs & 0x1FF; // Ajusta el resultado para que tenga un tamaño de 9 bits
}
void callback_emisor(void){
    printf("Aviso: se iniciará la función callback_emisor\n");
    pinMode(TX_PIN, OUTPUT);
    grupo6 proto;
    if(transmision_iniciada_emisor){
    //Escribe en el pin TX
    if(nbits_emisor == 0){
      digitalWrite(TX_PIN, 0); //Bit de inicio
    }else if(nbits_emisor < 9){
      digitalWrite(TX_PIN, (proto.frame[nbytes_emisor] >> (nbits_emisor-1)) & 0x01); //Bit de dato
    }else if(nbits_emisor == 9){
      nones = (proto.frame[nbytes_emisor]&0x01) + ((proto.frame[nbytes_emisor]&0x02)>>1) + ((proto.frame[nbytes_emisor]&0x04)>>2) + 
        ((proto.frame[nbytes_emisor]&0x08)>>3) + ((proto.frame[nbytes_emisor]&0x10)>>4) + ((proto.frame[nbytes_emisor]&0x20)>>5) + 
        ((proto.frame[nbytes_emisor]&0x40)>>6) + ((proto.frame[nbytes_emisor]&0x80)>>7);
      digitalWrite(TX_PIN, nones%2==0); //Bit de paridad
    }else{
      digitalWrite(TX_PIN, 1); //Canal libre durante 2 clocks
    }
    //Actualiza contador de bits
    nbits_emisor++;
    //Actualiza contador de bytes
    if(nbits_emisor == 15){
      nbits_emisor = 0;
      nbytes_emisor++;
      //Finaliza la comunicación
      if(nbytes_emisor==tam_emp){
        transmision_iniciada_emisor = false;
        nbytes_emisor = 0;
      }
    }
  }else{ 
    //Canal en reposo
    digitalWrite(TX_PIN, 1);
  }
}
void startTransmission_emisor(){
  printf("Aviso: se iniciará la función start_transmision_emisor\n");
  transmision_iniciada_emisor = true;
}
void guardar(const char* mensaje) {
    printf("Aviso: se iniciará la función guardar\n");
    FILE *archivo = fopen("mensaje.txt", "w");
    if (archivo == nullptr) {
        printf("Error al abrir el archivo para escribir.\n");
        return;
    }
    fprintf(archivo, "%s", mensaje);
    fclose(archivo);
    printf("Mensaje guardado en 'mensaje.txt'.\n");
}
void mostrar_archivo() {
    printf("Aviso: se iniciará la función mostrar_archivo\n");
    FILE *archivo = fopen("mensaje.txt", "r");
    if (archivo == nullptr) {
        printf("El archivo 'mensaje.txt' no existe.\n");
        return;
    }
    char buffer[256];
    printf("Contenido de 'mensaje.txt':\n");
    while (fgets(buffer, sizeof(buffer), archivo) != nullptr) {
        printf("%s", buffer);
    }
    fclose(archivo);
}