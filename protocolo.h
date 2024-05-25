#ifndef TAREA_1_PROTOCOLO_H
#define TAREA_1_PROTOCOLO_H
#define BYTE unsigned char // Se define el tipo de byte
#define LARGO_DATO 31
#define LARGO_FRAME 35

//  Se define la estructura del grupo 6 según el protocolo ya revisado por el profesor
struct grupo6{
    BYTE cmd;   // 4 bits
    BYTE lng;   // 5 bits
    BYTE data[LARGO_DATO];   // 31 bytes
    BYTE fcs;   // 9 bits
    BYTE frame[LARGO_FRAME];
};

#endif //TAREA_1_PROTOCOLO_H
