/*Hacer un juego de gato, donde la m�quina c�lcula todas las posibles soluciones, y coloca un movimiento
despu�s de determinar la mejor posibilidad de ganar, se usar� �rbol, y la computador� tendr� una estrategia para ganar*/

/*
Autor:Eyder Concha Moreno 09/Marzo/19
Entradas: Una posici�n en columnas y filas del 1 al 3
Salida: una tabla con la jugada de la IA
Procedimiento general:
-Se pregunta al jugador donde desea hacer un movimiento
-Se verifica si el lugar elegido es v�lido (no est� ocupado en la tabla)
-Se coloca en la tabla
-La IA encuentra los espacios vac�os en la tabla y coloca un movimiento en cada uno de ellos
-En cada movimiento realizado, se verifica cada uno de los siguientes posibles movimientos, para realizar esto se hace lo siguiente:
  *Se verifica si la simulaci�n a terminado, de hacerlo se revisa si el ganador es el jugador, o la m�quina
      -De ser la m�quina, el �rbol devuelve 10 puntos menos la profundidad, esto para garantizar que haga el movimiento m�s cercano a ganar
      -De ser el jugador, el �rbol devuelve 10 puntos m�ss la profundidad, esto por si es de perder, retrase su derrota lo m�ximo posible
      -De no haber terminado, retorna 0
  *Se determina si el juego ha terminado en empate, de terminar en empate se retorna un 0
  *Se determina si el turno a simular es del jugador, o de la m�quina, se realiza lo siguiente:
      -De ser de la m�quina, se toma como referencia un valor de -100, se buscan los espacios vacios, y se reemplazan con un movimiento de la m�quina.
        Este movimiento se pasa a la misma funci�n, comparando el valor que esta funci�n ha de retornar con la misma variable de la probabilidad mayor.

      -De ser del Jugador, se toma como referencia un valor de 100, se buscan los espacios vacios, y se reemplazan con un movimiento de la m�quina.
        Este movimiento se pasa a la misma funci�n, comparando el valor que esta funci�n ha de retornar con la misma variable de la probabilidad mayor.
  *Se retorna el valor encontrado por cada funcion, de esta manera recursivamente se verifican todas las posibles jugadas a realizar por la m�quina o el jugador
-El algoritmo retorna un valor, este valor representando las probabilidades de ganar en cada movimiento, siendo estas negativas (si ha de perder), o positivas (si ha de ganar)
-Se determina que espacio vac�o cuenta con la mayor probabilidad de ganar, se almacena la columna, y fila de su posicion
-Se realiza el movimiento por la maquina
-Se imprime el tablero
-Se repite hasta el fin del juego
*/

#include <stdio.h>

// Definimos para utilizar en algoritmo Minimax
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void jugar(int);
void simular();
void imprimirTabla(char[][3]);
void movimientoJugador(char[][3]);
int movimientoIdeal(char[][3], int, int, int*);
int movimientoSimulado(char[][3], int, int);
void colocarMovimiento(char[][3], int, int, char);
int determinaPuntuacion(char[][3]);
void turnoMaquina(char[][3]);
void turnoSimulado(char[][3]);
int esEmpate(char[][3]);

int main(){

    int esSimulacion = 1; 
    printf("\nIngresa 0 si deseas jugar, o 1 si deseas simular juegos\n");        
    scanf("%d", &esSimulacion);
    jugar(esSimulacion);    
    
}

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

// Esta funcion coloca el movimiento realizado por el jugador
void colocarMovimiento(char tabla[][3], int columna, int fila, char movimiento){
    tabla[columna][fila] = movimiento;
}

///////////////////////* Juego Simulado *///////////////////////////////

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
                tabla[columnas][filas] = '_';
            }
        }
    }
    // Se coloca el movimiento en el espacio con la mayor probabilidad de ganar  
    printf("\nEsta es la jugada ideal!\n");
    colocarMovimiento(tabla, columnaIdeal, filaIdeal, 'o');
}

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







