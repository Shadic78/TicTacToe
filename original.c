/**
 * @file EjerciciosExtra.c
 * @author Eyder Concha
 * @date 21 Marzo 2019
 * @brief Un juego de gato, con IA creada utilizando recursividad
 *
 */

#include <stdio.h>

// Definimos para utilizar en algoritmo Minimax
//!Operación a utilizar para determinar resultado mínimo
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
//!Operación a utilizar para determinar resultado máximo
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

//funciones
//!La función que comienza el juego
void jugar(int);
//!La función que comienza la modalidad de "simulación"
void simular();
//!La función que imprime la tabla de juego
void imprimirTabla(char[][3]);
//!La función que valida y realiza el movimiento del jugador
void movimientoJugador(char[][3]);
//!La función que determina el mejor movimiento a realizar
int movimientoIdeal(char[][3], int, int, int*);
//!La función que determina el mejor movimiento en la simulacion
int movimientoSimulado(char[][3], int, int);
//!La función que coloca un movimiento
void colocarMovimiento(char[][3], int, int, char);
//!La función que determina si se ha ganado o perdido la partida en algun movimiento
int determinaPuntuacion(char[][3]);
//!La función que determina si el juego ha terminado en empate
int esEmpate(char[][3]);
//!La función que determina el mejor movimiento a realizar en la siguiente posición por la máquina
void turnoMaquina(char[][3]);
//!La función que determina el mejor movimiento a realizar en la siguiente posición por la máquina en el juego simulado
void turnoSimulado(char[][3]);

int main(){

    int esSimulacion = 1;
    printf("\nIngresa 0 si deseas jugar, o 1 si deseas simular juegos\n");
    scanf("%d", &esSimulacion);
    jugar(esSimulacion);

}

/*!
    La funcion que inicializa el juego. Recibe un valor binario.
    Pregunta al usuario por la casilla donde desea jugar, valida el movimiento, y lo coloca como simulacion si el valor binario es 1
    o como juego si el valor binario es 0.
    @param esSimulacion Un valor binario que representa la modalidad a jugar
*/
void jugar(int esSimulacion){
    // Definimos el espacio de juego como una tabla de 3x3
    char tabla[3][3];
    if(!esSimulacion){
        char tabla[3][3] = {{'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};
    }else {
        char tabla[3][3] = {{'x', '_', '_'}, {'_', '_', 'x'}, {'o', '_', 'o'}};
    }

    printf("Bienvenido a gato master 1000 \n");
    imprimirTabla(tabla);
    //Verificamos que el juego no haya terminado
    while(!esEmpate(tabla) && (determinaPuntuacion(tabla) != 10) && (determinaPuntuacion(tabla) != -10)){
        printf("\n selecciona la casilla donde deseas jugar");
        movimientoJugador(tabla);
        imprimirTabla(tabla);
        printf("\nEstoy pensando...\n");
        //De no haber terminado, la maquina realiza un movimiento
        if(!esEmpate(tabla) && (determinaPuntuacion(tabla) != 10) && (determinaPuntuacion(tabla) != -10)){
            esSimulacion ? turnoSimulado(tabla) : turnoMaquina(tabla);
            imprimirTabla(tabla);
        }
    }

    // Verificamos de forma termino el juego, victoria de maquina, jugador, o empate
    if(esEmpate(tabla)){
        printf("\nHemos empatado!\n");
    }else if(determinaPuntuacion(tabla) == 10){
        printf("\nHe ganado! Perdedor");
    }else{
        printf("\nFelicidades, has ganado!");
    }
}

/*!
    La funcion que Imprime la tabla. Recibe una matriz.
    Imprime la tabla de juego, con sus respectivos valores
    @param tabla El espacio de juego a imprimir
*/
void imprimirTabla(char tabla[][3]){
    int columnas = 0;
    int filas = 0;
    for( filas = 0; filas < 3 ; filas++){
        for(columnas = 0 ; columnas < 3; columnas++){
            if(columnas > 0){
                // Imprimimos el grid vertical de la tabla
                printf(" | ");
            }
            printf(" %c", tabla[columnas][filas]);
        }
        if(filas < 2){
        // Imprimimos el grid horizontal de la tabla
         printf("\n ------------ \n");
        }
    }
    printf("\n");
}

/*!
    La funcion que Imprime la tabla. Recibe una matriz.
    Imprime la tabla de juego, con sus respectivos valores
    @param tabla El espacio de juego a imprimir
*/
void movimientoJugador(char tabla[][3]){
    int movimientoColumna = 0, movimientoFila = 0, esValido = 0;

    // Determinamos si el movimiento del jugador es v�lido, esto para validar los datos ingresados
    while(!esValido){
        printf("\n Ingresa su posicion en columna ");
        scanf(" %i", &movimientoColumna);
        movimientoColumna--;
        printf("\n Ingresa su posicion en fila ");
        scanf(" %i", &movimientoFila);
        // Pasamos datos ingresados a datos validos para los arreglos (de 3 a 2, de 1 a 0, etc)
        movimientoFila--;
        // Verificamos que el espacio seleccionado este dentro de los limites, y no tenga ningun movimiento ya seleccionado
        if(tabla[movimientoColumna][movimientoFila] != 'o' && tabla[movimientoColumna][movimientoFila] != 'x' && movimientoColumna < 3 && movimientoFila < 3 && movimientoFila >= 0 && movimientoColumna >= 0){
            esValido = 1;
        }else{
            printf("Posicion incorrecta, intenta de nuevo");
        }
    }
    // Colocamos el movimiento en el espacio seleccionado
    colocarMovimiento(tabla, movimientoColumna, movimientoFila, 'x');
}

/*!
    La funcion coloca un movimiento en un espacio de la tabla.
    Recibe la tabla, la columna, la fila y el caracter a colocar.
    @param tabla El espacio de juego a imprimir
    @param columna La columna donde el movimiento debará ir
    @param fila La fila donde el movimiento debará ir
    @param movimiento El caracter donde el movmiento deberá ir
*/
void colocarMovimiento(char tabla[][3], int columna, int fila, char movimiento){
    tabla[columna][fila] = movimiento;
}

///////////////////////* Juego Simulado *///////////////////////////////

/*!
    La función encargada de determinar el mejor movimiento en una jugada simulada.
    Determina la puntuacion a partir de un movimiento de la maquina,y elige la jugada con el mejor movimiento.
    Coloca el mejor movimiento en la posición donde se encontró
    @param tabla La tabla donde se han de evaluar los movimientos
*/
void turnoSimulado(char tabla[][3]){
    // Determinamos una puntuacion, esta sera reemplazada por los movimientos probados por la maquina
    int maximaPuntuacion = -100, filaIdeal, columnaIdeal, puntuacion = 0;
    for (int filas = 0; filas < 3; filas++){
        for (int columnas = 0; columnas < 3; columnas++){
            if(tabla[columnas][filas] == '_'){
                // Buscamos los espacios vacios, los reemplazamos con un movimiento de la maquina
                tabla[columnas][filas] = 'o';
                // Comprobamos si la probabilidad de ganar de ese movimiento es mayor a la maxima probabilidad, de serlo, almacenamos la fila y columna
                printf("\nSimulando movimientos a partir de columna %d fila %d\n", (columnas+1), (filas+1));
                imprimirTabla(tabla);
                puntuacion = movimientoSimulado(tabla, 0, 0);
                if(puntuacion > maximaPuntuacion){
                    maximaPuntuacion = puntuacion;
                    filaIdeal = filas;
                    columnaIdeal = columnas;
                }
                // Volvemos a vaciar el espacio, para probar otro movimiento
                printf("\nSimulación a partir de columna %d fila %d ha terminado\n", (columnas+1), (filas+1));
                tabla[columnas][filas] = '_';
            }
        }
    }
    // Se coloca el movimiento en el espacio con la mayor probabilidad de ganar
    printf("\nEsta es la jugada ideal!\n");
    colocarMovimiento(tabla, columnaIdeal, filaIdeal, 'o');
}

/*!
    La función encargada de determinar la puntuacion apartir de un movimiento en juego simulado.
    Determina si el juego ha terminado, si ha ganado la maquina, el usuario, o terminó en empate.
    Retorna una puntuación según el resultado final, el cual se ve afectado por la profundidad de la jugada.
    Verifica si es turno de la maquina, de ser turno de la máquina simula un movimiento del jugador y lo imprime
    buscando la mayor puntuación, la cual es retornada.
    Si es turno del jugador, simula un movimiento de la máquina, lo imprime y busca la menor puntuación.
    @param tabla La tabla donde se han de evaluar los movimientos
    @param esTurnoMaquina Valor binario que representa el turno de la maquina o del jugador
    @param profundidad Determina la profundidad del movimiento a simular
    @return retorna un entero con la puntuación del movimiento
*/
int movimientoSimulado(char tabla[][3], int esTurnoMaquina, int profundidad){
    // Determinamos si el juego ha terminado, si gana la maquina, determinamos 10 puntos, si gana el jugador, -10
    int puntuacion = determinaPuntuacion(tabla);

    // De haber ganado la maquina, retornamos la puntuacion, menos la profunidad, esto para ganar en la menor cantidad de pasos posibles
    if(puntuacion == 10){
        printf("En esta jugada gana la maquina!\n");
        return (puntuacion-profundidad);
    }

    // De haber el jugador, retornamos la puntuacion mas la profundidad, esto por si la maquina ha de perder, hacerlo en la mayor cantidad de pasos posibles
    if(puntuacion == -10){
        printf("En esta jugada gana el jugador!\n");
        return(puntuacion+profundidad);
    }

    //De ser empate retornamos 0
    if(esEmpate(tabla)){
        printf("En esta jugada termina en empate!\n");
        return 0;
    }

    // Si el turno a simular es de la maquina
    if(esTurnoMaquina){
        // Definimos el mejor resultado posible como -100, ya que la maquina al ganar, debera retornar un resultado positivo
        int mejor = -100;
        for(int filas = 0; filas < 3; filas++){
            for(int columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    // Probamos un movimiento de la maquina
                    tabla[columnas][filas] = 'o';
                    printf("\nSimulando jugada de maquina de %d de profundidad\n", profundidad);
                    // Determinamos la mayor probabilidad entre el movimiento encontrado, y el actual
                    imprimirTabla(tabla);
                    int puntuacionMovSimulado = movimientoSimulado(tabla, !esTurnoMaquina, profundidad+1);
                    mejor = MAX(puntuacionMovSimulado, mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    // Si el turno a simular es del jugador
    }else {
        // Definimos el mejor resultado posible como 100, ya que la maquina al perder, debera retornar un resultado negativo
        int mejor = 100;
        for(int filas = 0; filas < 3; filas++){
            for(int columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    // Simulamos un movimiento del jugador
                    tabla[columnas][filas] = 'x';
                    // Determinamos cual es la menor probabilidad, el movimiento actual, o el siguiente
                    printf("\nSimulando jugada de jugador de %d de profundidad\n", profundidad);
                    imprimirTabla(tabla);
                    int puntuacionMovSimulado = movimientoSimulado(tabla, !esTurnoMaquina, profundidad+1);
                    mejor = MIN(puntuacionMovSimulado, mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    }
}

///////////////////////////////////////////////////////////////////////////////////7


/*!
    La función encargada de determinar el mejor movimiento en una jugada.
    Determina la puntuacion a partir de un movimiento de la maquina, y elige la jugada con el mejor movimiento.
    Coloca el mejor movimiento en la posición donde se encontró
    Imprime las simulaciones totales realizadas.
    @param tabla La tabla donde se han de evaluar los movimientos
*/
void turnoMaquina(char tabla[][3]){
    // Determinamos una puntuacion, esta sera reemplazada por los movimientos probados por la maquina
    int maximaPuntuacion = -100, simulaciones = 0, simulacionesTotales = 0, filaIdeal, columnaIdeal;
    for (int filas = 0; filas < 3; filas++){
        for (int columnas = 0; columnas < 3; columnas++){
            if(tabla[columnas][filas] == '_'){
                // Buscamos los espacios vacios, los reemplazamos con un movimiento de la maquina
                tabla[columnas][filas] = 'o';
                // Comprobamos si la probabilidad de ganar de ese movimiento es mayor a la maxima probabilidad, de serlo, almacenamos la fila y columna
                if(movimientoIdeal(tabla, 0, 0, &simulaciones) > maximaPuntuacion){
                    simulaciones = 0;
                    maximaPuntuacion = movimientoIdeal(tabla, 0, 0, &simulaciones);
                    filaIdeal = filas;
                    columnaIdeal = columnas;
                }
                simulacionesTotales++;
                simulacionesTotales += simulaciones;
                // Volvemos a vaciar el espacio, para probar otro movimiento
                tabla[columnas][filas] = '_';
            }
        }
    }
    // Se coloca el movimiento en el espacio con la mayor probabilidad de ganar
    printf("\nSe simularon %d jugadas\n", simulacionesTotales);
    colocarMovimiento(tabla, columnaIdeal, filaIdeal, 'o');
}

/*!
    La función encargada de determinar la puntuacion apartir de un movimiento
    Determina si el juego ha terminado, si ha ganado la maquina, el usuario, o terminó en empate.
    Retorna una puntuación según el resultado final, el cual se ve afectado por la profundidad de la jugada.
    Verifica si es turno de la maquina, de ser turno de la máquina simula un movimiento del jugador
    buscando la mayor puntuación, la cual es retornada.
    Si es turno del jugador, simula un movimiento de la máquina y busca la menor puntuación.
    Al simular un turno, se le agrega uno a la variable "simulaciones", la cual determina la cantidad de jugadas simuladas
    @param tabla La tabla donde se han de evaluar los movimientos
    @param esTurnoMaquina Valor binario que representa el turno de la maquina o del jugador
    @param profundidad Determina la profundidad del movimiento a simular
    @param simulaciones Determina el total de las jugadas simuladas
    @return Un entero con la puntuación del movimiento
*/
int movimientoIdeal(char tabla[][3], int esTurnoMaquina, int profundidad, int* simulaciones){
    // Determinamos si el juego ha terminado, si gana la maquina, determinamos 10 puntos, si gana el jugador, -10
    int puntuacion = determinaPuntuacion(tabla);

    // De haber ganado la maquina, retornamos la puntuacion, menos la profunidad, esto para ganar en la menor cantidad de pasos posibles
    if(puntuacion == 10){
        return (puntuacion-profundidad);
    }

    // De haber el jugador, retornamos la puntuacion mas la profundidad, esto por si la maquina ha de perder, hacerlo en la mayor cantidad de pasos posibles
    if(puntuacion == -10){
        return(puntuacion+profundidad);
    }

    //De ser empate retornamos 0
    if(esEmpate(tabla)){
        return 0;
    }

    // Si el turno a simular es de la maquina
    if(esTurnoMaquina){
        // Definimos el mejor resultado posible como -100, ya que la maquina al ganar, debera retornar un resultado positivo
        int mejor = -100;
        for(int filas = 0; filas < 3; filas++){
            for(int columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    // Probamos un movimiento de la maquina
                    tabla[columnas][filas] = 'o';
                    *simulaciones = *simulaciones + 1;
                    // Determinamos la mayor probabilidad entre el movimiento encontrado, y el actual
                    mejor = MAX(movimientoIdeal(tabla, !esTurnoMaquina, profundidad+1, simulaciones), mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    // Si el turno a simular es del jugador
    }else {
        // Definimos el mejor resultado posible como 100, ya que la maquina al perder, debera retornar un resultado negativo
        int mejor = 100;
        for(int filas = 0; filas < 3; filas++){
            for(int columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    // Simulamos un movimiento del jugador
                    tabla[columnas][filas] = 'x';
                    *simulaciones = *simulaciones + 1;
                    // Determinamos cual es la menor probabilidad, el movimiento actual, o el siguiente
                    mejor = MIN(movimientoIdeal(tabla, !esTurnoMaquina, profundidad+1, simulaciones), mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    }
}

/*!
    La función encargada de determinar si el juego ha terminado en empate.
    De estar lleno el espacio de juego, retorna una valor binario con valor de 1, de otra forma la retorna con 0.
    @param tabla La tabla donde se han de evaluar las posiciones
    @return Un entero representando si es empate o no
*/
int esEmpate(char tabla[][3]){
    int empataron = 1;
    // Determinamos si la tabla y sus movimientos resultan un empate
    for(int filas = 0; filas < 3; filas++){
        for(int columnas = 0; columnas < 3; columnas++){
            // De no encontrar ningun espacio vacio se retornara un empate (cabe resaltar que esta funcion se utilizaza con otra comparacion de ganador)
            if(tabla[columnas][filas] == '_'){
                empataron = 0;
            }
        }
    }
    return empataron;
}

/*!
    La función encargada de determinar si el juego ha terminado en una victoria para la máquina o el jugador.
    De estar llena una fila, columna o diagonal con los mismos valores, determina si son "o" o "x", de ser
    "o" implica que el ganador esla maquina, por lo que retorna una puntuación positiva, de ser "x" implica que el jugador ganó,
    por lo tanto retorna una puntuación negativa.
    @param tabla La tabla donde se han de evaluar las posiciones
    @return un entero representando la puntuación del juego
*/
int determinaPuntuacion(char tabla[][3]) {
    int puntuacion = 0;
    // Comprobamos de izquierda a derecha
    for(int fila = 0; fila < 3; fila++){
        if((tabla[0][fila] == tabla[1][fila]) && (tabla[1][fila] ==  tabla[2][fila])){
            // Verificamos si el ganador es el jugador o la maquina
            if(tabla[0][fila] == 'o'){
                puntuacion = 10;
            }else if(tabla[0][fila] == 'x'){
                puntuacion = -10;
            }
        }
    }

    // Comprobamos de arriba abajo
    for(int columna = 0; columna < 3; columna++){
        if(tabla[columna][0] == tabla[columna][1] && tabla[columna][1] == tabla[columna][2]){
            // Verificamos si el ganador es el jugador o la maquina
            if(tabla[columna][0] == 'o'){
                puntuacion = 10;
            }else if(tabla[columna][0] == 'x'){
                puntuacion = -10;
            }
        }
    }

    // Comprobamos diagonales
    if((tabla[0][0] == tabla[1][1] && tabla[1][1] == tabla[2][2]) || (tabla[0][2] == tabla [1][1] && tabla[1][1] == tabla[2][0])){
        // Verificamos si el ganador es el jugador o la maquina
        if(tabla[1][1] == 'o'){
            puntuacion = 10;
        }else if(tabla[1][1] == 'x'){
            puntuacion = -10;
        }
    }
    // Retornamos una puntuacion representando quien gano
    return puntuacion;
}
