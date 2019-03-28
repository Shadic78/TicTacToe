/**
 * @file TicTacToe.c
 * @author Eyder Concha, port: Carlos Chan
 * @date 23 Marzo 2019
 * @brief Un juego de gato, con IA creada utilizando recursividad
 *
 */

/*
	Funciones originales adaptadas a js
*/
function MIN(x, y){
	let retorno = 0;
	if (x < y) {
		retorno = x;
	}
	else {
		retorno = y;
	}
	return retorno;
}

function MAX(x, y){
	let retorno = 0;
	if (x > y) {
		retorno = x;
	}
	else {
		retorno = y;
	}
	return retorno;
}

//--------------------------------------------------------------------------

let simulaciones = 0
function turnoMaquina(tabla){
    // Determinamos una puntuacion, esta sera reemplazada por los movimientos probados por la maquina
    let maximaPuntuacion = -100, simulacionesTotales = 0, filaIdeal, columnaIdeal;
    for (let filas = 0; filas < 3; filas++){
        for (let columnas = 0; columnas < 3; columnas++){
            if(tabla[columnas][filas] == '_'){
                // Buscamos los espacios vacios, los reemplazamos con un movimiento de la maquina
                tabla[columnas][filas] = 'o';
                // Comprobamos si la probabilidad de ganar de ese movimiento es mayor a la maxima probabilidad, de serlo, almacenamos la fila y columna
                if(movimientoIdeal(tabla, 0, 0) > maximaPuntuacion){
                    simulaciones = 0;
                    maximaPuntuacion = movimientoIdeal(tabla, 0, 0);
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
    console.log("Se simularon " + simulacionesTotales + " jugadas");
		if(simulacionesTotales > 0){
			colocarMovimiento(tabla, columnaIdeal, filaIdeal, 'o');
		}
}

/*!
    La funcion coloca un movimiento en un espacio de la tabla.
    Recibe la tabla, la columna, la fila y el caracter a colocar.
    @param tabla El espacio de juego a imprimir
    @param columna La columna donde el movimiento debará ir
    @param fila La fila donde el movimiento debará ir
    @param movimiento El caracter donde el movmiento deberá ir
*/
function colocarMovimiento(tabla, columna, fila, movimiento){
    tabla[columna][fila] = movimiento;
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
function movimientoIdeal(tabla, esTurnoMaquina, profundidad){
    // Determinamos si el juego ha terminado, si gana la maquina, determinamos 10 puntos, si gana el jugador, -10
    let puntuacion = determinaPuntuacion(tabla);

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
        let mejor = -100;
        for(let filas = 0; filas < 3; filas++){
            for(let columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    // Probamos un movimiento de la maquina
                    tabla[columnas][filas] = 'o';
                    simulaciones = simulaciones + 1;
                    // Determinamos la mayor probabilidad entre el movimiento encontrado, y el actual
                    mejor = MAX(movimientoIdeal(tabla, !esTurnoMaquina, profundidad+1, simulaciones), mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    // Si el turno a simular es del jugador
    }
		else{
        // Definimos el mejor resultado posible como 100, ya que la maquina al perder, debera retornar un resultado negativo
        let mejor = 100;
        for(let filas = 0; filas < 3; filas++){
            for(let columnas = 0; columnas < 3; columnas++){
                if(tabla[columnas][filas] == '_'){
                    // Simulamos un movimiento del jugador
                    tabla[columnas][filas] = 'x';
                    simulaciones = simulaciones + 1;
                    // Determinamos cual es la menor probabilidad, el movimiento actual, o el siguiente
                    mejor = MIN(movimientoIdeal(tabla, !esTurnoMaquina, profundidad+1, simulaciones), mejor);
                    tabla[columnas][filas] = '_';
                }
            }
        }
        return mejor;
    }
}
//--------------------------------------------------------------------------

/*!
    La función encargada de determinar si el juego ha terminado en empate.
    De estar lleno el espacio de juego, retorna una valor binario con valor de 1, de otra forma la retorna con 0.
    @param tabla La tabla donde se han de evaluar las posiciones
    @return Un entero representando si es empate o no
*/
function esEmpate(tabla){
    let empataron = true;
    // Determinamos si la tabla y sus movimientos resultan un empate
    for(let filas = 0; filas < 3; filas++){
        for(let columnas = 0; columnas < 3; columnas++){
            // De no encontrar ningun espacio vacio se retornara un empate (cabe resaltar que esta funcion se utilizaza con otra comparacion de ganador)
            if(tabla[columnas][filas] == '_'){
                empataron = false;
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
function determinaPuntuacion(tabla) {
    let puntuacion = 0;
    // Comprobamos de izquierda a derecha
    for(let fila = 0; fila < 3; fila++){
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
    for(let columna = 0; columna < 3; columna++){
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
