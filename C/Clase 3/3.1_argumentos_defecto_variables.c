#include <stdio.h>
#include <stdint.h>
#include <string.h>

// ##### Definiciones para hacer uso de los argumentos variables #####
#define ava(...) argumentos_variables_arreglos(                                         \
        (double []) {__VA_ARGS__},                                                      \
        sizeof((double []) { __VA_ARGS__ }) / sizeof(((double []) { __VA_ARGS__ })[0]))

#define ave(...) argumentos_variables_estructuras(                                      \
        (struct argumento_estructura) { __VA_ARGS__ })

struct argumento_estructura {
    int valor1;
    char valor2;
    double valor3;
};

enum PARTES_CAJA {
    ESI = 0b00000001,    // ESQUINA_SUPERIOR_IZQUIERDA
    PAR = 0b00000010,    // PARTE_ARRIBA
    ESD = 0b00000100,    // ESQUINA_SUPERIOR_DERECHA
    LIZ = 0b00001000,    // LADO_IZQUIERDO
    LDE = 0b00010000,    // LADO_DERECHO
    EII = 0b00100000,    // ESQUINA_INFERIOR_IZQUIERDA
    PAB = 0b01000000,    // PARTE_ABAJO
    EID = 0b10000000,    // ESQUINA_INFERIOR_DERECHA
    NO_ESI = ESI << 8,   // NO_ESQUINA_SUPERIOR_IZQUIERDA
    NO_PAR = PAR << 8,   // NO_PARTE_ARRIBA
    NO_ESD = ESD << 8,   // NO_ESQUINA_SUPERIOR_DERECHA
    NO_LIZ = LIZ << 8,   // NO_LADO_IZQUIERDO
    NO_LDE = LDE << 8,   // NO_LADO_DERECHO
    NO_EII = EII << 8,   // NO_ESQUINA_INFERIOR_IZQUIERDA
    NO_PAB = PAB << 8,   // NO_PARTE_ABAJO
    NO_EID = EID << 8,   // NO_ESQUINA_INFERIOR_DERECHA
    SPROR = 0b00000111,  // Parte de arriba
    INFOR = 0b11100000,  // Parte de abajo
    TODO = 0b11111111,   // Literalmente, todo
    NADA = TODO << 8,    // Literalmente, nada
    DEFAULT = 0          // Un cero no cambia nada
};

// ##### Prototipos de funciOn #####
int argumentos_variables_elipsis(int conta, ...);
double argumentos_variables_arreglos(double * arreglo, int conta);
void argumentos_variables_estructuras(struct argumento_estructura arg);
void argumentos_variables_bits(const char * const mensaje, int16_t secuencia_bits);

int main(void) {

    /* Argumentos variables elipsis */
    printf("%d\n", argumentos_variables_elipsis(2, 5, 10));
    printf("%d\n", argumentos_variables_elipsis(4, -5, -10, -5));
    printf("%d\n", argumentos_variables_elipsis(5, 1, 2, 3, 4, 5, 1, 2, 3, 5));

    /* Argumentos variables arreglos */
    double arreglo[] = { 15, 20, 17, 5, 1 };

    // El arreglo es creado previamente
    printf("%f\n", argumentos_variables_arreglos(arreglo, 5));
    // El arreglo es creado en la misma lInea.
    printf("%f\n", argumentos_variables_arreglos((double []) { 1, 2, 3, 4, 5 }, 5));

    // El arreglo se crea implicitamente gracias a una macro.
    printf("%f\n", ava(12, 5, 6));
    printf("%f\n", ava(-1, -2, -3, -4));
    printf("%f\n", ava(0.5, 0.25));

    /* Argumentos por defecto estructuras */
    struct argumento_estructura arg1 = { 10, 'z', 5.5 };

    // La estructura es creado previamente
    argumentos_variables_estructuras(arg1);
    // La estructura es creada en la misma lInea.
    argumentos_variables_estructuras((struct argumento_estructura) { 5, 'b', -7 });
    //
    // La estructura se crea implicitamente gracias a una macro
    ave(-1111, 'h', 9.9999);
    ave(1234);     // valor2 y valor 3 se inicializan a 0 implicitamente.
    ave(.valor2 = 'o', .valor3 = 3.141516); // Se usan las claves de acceso

    // Argumentos por defecto operadores de bits.
    argumentos_variables_bits("Hola", 0);       // <---+
                                                //     +-- Son equivalentes
    argumentos_variables_bits("Hola", DEFAULT); // <---+

    argumentos_variables_bits("Hola", TODO);
    argumentos_variables_bits("Hola", NADA);

    argumentos_variables_bits("Hola", LIZ | LDE | NO_ESI | NO_ESD | NO_EII | NO_EID);

    argumentos_variables_bits("Hola", INFOR);

    puts("");

    argumentos_variables_bits("Hola", SPROR | NO_EII | NO_EID);
    return 0;
}

/** Argumentos variables por medio de elipsis
 *
 *  Ventajas:
 *      -   Relativamente facil de usar los argumentos, hay
 *          que usar apuntadores o las macros de stdarg.h
 *      -   Relativamente facil de llamar en el programa.
 *
 *  Desventajas:
 *      -   No se puede usar comprobaciOn de tipos.
 *      -   Puede resultar dificil usar argumentos de distintos tipos
 *          pues se requiere decidir como hacer uso del Ultimo argumento
 *          fijo para identificar los tipos de la lista recivida.
 */

int argumentos_variables_elipsis(int conta, ...) {
    int suma = 0;
    int * dir = &conta + 1;

    for (int i = 0; i < conta; i++) {
        suma += *dir;
        dir++;
    }

    return suma;

}

/** Argumentos variables por medio de arreglos
 *
 *  Ventajas:
 *      -   Mayor seguridad de tipos (hay warnings cuando no se cumplen).
 *      -   Fácil de usar en el programa si se combina con macros.
 *
 *  Desventajas:
 *      -   Las macros para facilitar su uso resultan bastante complejas.
 *      -   No se puede combinar varios tipos de datos.
 */

double argumentos_variables_arreglos(double * arreglo, int conta) {
    double suma = 0;

    for (int i = 0; i < conta; i++) {
        suma += arreglo[i];
    }

    return suma;
}

/** Argumentos por defecto usando estructuras
 *
 *  Ventajas:
 *      -   Se pueden combinar varios tipos de dAtos.
 *      -   Se puede usar la notacion de estructuras mandar
 *          argumentos en desorden.
 *
 *  Desventajas:
 *      -   Nuevamente se requiere de macros complicadas para hacer
 *          las llamadas mAs agradables a la vista.
 *      -   Puede confundir a los muchos desconocedores de la sintaxis
 *          de inicializaciOn de estructuras en desorden.
 */

void argumentos_variables_estructuras(struct argumento_estructura arg) {
    putchar('\n');
    printf("El argumento entero es: %d\n", arg.valor1);
    printf("El argumento carácter es: %c\n", arg.valor2);
    printf("El argumento doble es: %f\n\n", arg.valor3);
}

/** Argumentos por defecto usando operaciones de bits
 *
 *  Ventajas:
 *      -   Ocupa muy poco espacio para representar una gran cantidad
 *          de argumentos.
 *      -   Es sencillo hacer un conjunto de constantes asociadas a
 *          los significados de cada bit (en comparaciOn con las
 *          macros de los otros mEtodos).
 *
 *  Desventajas:
 *      -   Los argumentos solo pueden ser verdadero o falso.
 *      -   La lOgica binario es poco conocida entre programadores
 *          novatos, por lo que la gente puede confundirse con tu
 *          cOdigo.
 *      -   Como el tamanio de los enteros cambia segUn la arquitectura,
 *          puede que un tipo sea suficiente para todos los arguemntos en
 *          una computadora y no sea suficiente en otra.
 *          Para solventar esto hay que usar enteros de tamanio fijo
 *          proporcionados por la cabecera stdint.h
 */

// int16_t es un entero de 16 bits definido en stdint.h, la cual
// garantiza que en todas las arquitecturas que sigan el estandar
// c99 dicho entero mida lo mismo.
void argumentos_variables_bits(const char * const mensaje, int16_t secuencia_bits) {
    int32_t longitud = strlen(mensaje);
    int8_t banderas = 0b10100101;       // Esta es la secuencia por defecto
                                        // representa las esquinas

    banderas &= ~(secuencia_bits >> 8);  // Se deshabilitan las partes de acuerdo a la secuencia
    banderas |= secuencia_bits;          // Se habilitan las partes de acuerdo a la secuencia

    // Esquina superior izquierda
    if ( banderas & ESI ) putchar('+');
    else
    if ( banderas & SPROR ) putchar(' ');

    // Parte de arriba
    for (int i = 0; i < longitud + 2; i++) {
        if ( banderas & PAR )
                putchar('-');
        else if ( banderas & SPROR )
            putchar(' ');
    }

    // Esquina superior derecha
    if ( banderas & ESD ) putchar('+');
    else
    if ( banderas & SPROR ) putchar(' ');

    // Salto de lInea si hay algo en la parte de arriba
    if ( banderas & SPROR ) putchar('\n');

    // Lado izquierdo
    if ( banderas & LIZ ) putchar('|');
    else                  putchar(' ');

    // Se imprime la cadena
    printf(" %s ", mensaje);

    // Lado derecho
    if ( banderas & LIZ ) putchar('|');
    else                  putchar(' ');

    putchar('\n');

    // Esquina inferior izquierda
    if ( banderas & EII ) putchar('+');
    else
    if ( banderas & INFOR ) putchar(' ');

    // Parte de abajo
    for (int i = 0; i < longitud + 2; i++) {
        if ( banderas & PAB )
            putchar('-');
        else if ( banderas & INFOR )
            putchar(' ');
    }

    // Esquina inferior derecha
    if ( banderas & EID ) putchar('+');
    else
    if ( banderas & INFOR ) putchar(' ');

    putchar('\n');
}
