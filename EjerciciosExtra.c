#include <stdio.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
/*Hacer un juego de gato, donde la máquina cálcula todas las posibles soluciones, y coloca un movimiento
después de determinar la mejor posibilidad de ganar, se usará árbol, y la computadorá tendrá una estrategia para ganar*/

/*
Autor:Eyder Concha Moreno 09/Marzo/19
Entradas: Una posición en columnas y filas del 1 al 3
Salida: una tabla con la jugada de la IA
Procedimiento general:
-Se pregunta al jugador donde desea hacer un movimiento
-Se verifica si el lugar elegido es válido (no está ocupado en la tabla)
-Se coloca en la tabla
-La IA encuentra los espacios vacíos en la tabla y coloca un movimiento en cada uno de ellos
-En cada movimiento realizado, se verifica cada uno de los siguientes posibles movimientos, para realizar esto se hace lo siguiente:
  *Se verifica si la simulación a terminado, de hacerlo se revisa si el ganador es el jugador, o la máquina
      -De ser la máquina, el árbol devuelve 10 puntos menos la profundidad, esto para garantizar que haga el movimiento más cercano a ganar
      -De ser el jugador, el árbol devuelve 10 puntos máss la profundidad, esto por si es de perder, retrase su derrota lo máximo posible
      -De no haber terminado, retorna 0
  *Se determina si el juego ha terminado en empate, de terminar en empate se retorna un 0
  *Se determina si el turno a simular es del jugador, o de la máquina, se realiza lo siguiente:
      -De ser de la máquina, se toma como referencia un valor de -100, se buscan los espacios vacios, y se reemplazan con un movimiento de la máquina.
        Este movimiento se pasa a la misma función, comparando el valor que esta función ha de retornar con la misma variable de la probabilidad mayor.

      -De ser del Jugador, se toma como referencia un valor de 100, se buscan los espacios vacios, y se reemplazan con un movimiento de la máquina.
        Este movimiento se pasa a la misma función, comparando el valor que esta función ha de retornar con la misma variable de la probabilidad mayor.
  *Se retorna el valor encontrado por cada funcion, de esta manera recursivamente se verifican todas las posibles jugadas a realizar por la máquina o el jugador
-El algoritmo retorna un valor, este valor representando las probabilidades de ganar en cada movimiento, siendo estas negativas (si ha de perder), o positivas (si ha de ganar)
-Se determina que espacio vacío cuenta con la mayor probabilidad de ganar, se almacena la columna, y fila de su posicion
-Se realiza el movimiento por la maquina
-Se imprime el tablero
-Se repite hasta el fin del juego
*/

void imprimirTabla(char[][3]);
void movimientoJugador(char[][3]);
int movimientoIdeal(char[][3], int, int);
void colocarMovimiento(char[][3], int, int, char);
int determinaPuntuacion(char[][3]);
void turnoMaquina(char[][3]);
int esEmpate(char[][3]);

int main(){

    // Definimos el espacio de juego como una tabla de 3x3
    char tabla[3][3] = {{'_', '_', '_'}, {'_', '_', '_'}, {'_', '_', '_'}};
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
            turnoMaquina(tabla);
            imprimirTabla(tabla);
        }
    }

    // Verificamos de forma termino el juego, victoria de maquina, jugador, o empate
    if(esEmpate(tabla)){
        printf("Hemos empatado!");
    }else if(determinaPuntuacion(tabla) == 10){
        printf("He ganado!");
    }else{
        printf("Felicidades, has ganado!");
    }
}

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


void movimientoJugador(char tabla[][3]){
    int movimientoColumna = 0, movimientoFila = 0, esValido = 0;

    // Determinamos si el movimiento del jugador es válido, esto para validar los datos ingresados
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

// Esta funcion coloca el movimiento realizado por el jugador
void colocarMovimiento(char tabla[][3], int columna, int fila, char movimiento){
    tabla[columna][fila] = movimiento;
}

void turnoMaquina(char tabla[][3]){
    int maximaPuntuacion = -100;
    int filaIdeal;
    int columnaIdeal;
    for (int filas = 0; filas < 3; filas++){
        for (int columnas = 0; columnas < 3; columnas++){
            if(tabla[columnas][filas] == '_'){
                tabla[columnas][filas] = 'o';
                if(movimientoIdeal(tabla, 0, 0) > maximaPuntuacion){
                    maximaPuntuacion = movimientoIdeal(tabla, 0, 0);
                    filaIdeal = filas;
                    columnaIdeal = columnas;
                }
                tabla[columnas][filas] = '_';
            }
        }
    }
    colocarMovimiento(tabla, columnaIdeal, filaIdeal, 'o');
}

int movimientoIdeal(char tabla[][3], int esTurnoMaquina, int profundidad){
    int puntuacion = determinaPuntuacion(tabla);
    if(puntuacion == 10){
        return (puntuacion-profundidad);
    }

    if(puntuacion == -10){
        return(puntuacion+profundidad);
    }

    if(esEmpate(tabla)){
        return 0;
    }
    if(esTurnoMaquina){
        int mejor = -100;
        for(int filas = 0; filas < 3; filas++){
            for(int columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    tabla[columnas][filas] = 'o';
                    mejor = MAX(movimientoIdeal(tabla, !esTurnoMaquina, profundidad+1), mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    }else {
        int mejor = 100;
        for(int filas = 0; filas < 3; filas++){
            for(int columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    tabla[columnas][filas] = 'x';
                    mejor = MIN(movimientoIdeal(tabla, !esTurnoMaquina, profundidad+1), mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    }
}

int esEmpate(char tabla[][3]){
    int empataron = 1;
    for(int filas = 0; filas < 3; filas++){
        for(int columnas = 0; columnas < 3; columnas++){
            if(tabla[columnas][filas] == '_'){
                empataron = 0;
            }
        }
    }
    return empataron;
}

int determinaPuntuacion(char tabla[][3]) {
    int puntuacion = 0;
    // Comprobamos de izquierda a derecha
    for(int fila = 0; fila < 3; fila++){
        if((tabla[0][fila] == tabla[1][fila]) && (tabla[1][fila] ==  tabla[2][fila])){
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
            if(tabla[columna][0] == 'o'){
                puntuacion = 10;
            }else if(tabla[columna][0] == 'x'){
                puntuacion = -10;
            }
        }
    }

    // Comprobamos diagonales
    if((tabla[0][0] == tabla[1][1] && tabla[1][1] == tabla[2][2]) || (tabla[0][2] == tabla [1][1] && tabla[1][1] == tabla[2][0])){
        if(tabla[1][1] == 'o'){
            puntuacion = 10;
        }else if(tabla[1][1] == 'x'){
            puntuacion = -10;
        }
    }
    return puntuacion;
}







