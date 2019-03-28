/*
	Algoritmo de "TicTacToe" en p5.js.
	Autor original: EyderACM, hecho en C.
	Autor del port a p5.js: Carlos Chan.
*/
let tabla = [
	['_', '_', '_'],
	['_', '_', '_'],
	['_', '_', '_']
];
let canvas;
let inicioX = 0,
		inicioY = 0,
		largoCuadros = 100;
let boton1, boton2, boton3;
let menuActivo = true;
let imagenJ1, imagenJ2;
// Variable que almacena el modo de juego.
// 0 = Simular juego, 1 = Jugar contra IA, 2 = Multijugador, 3 = Ninguno
let modoDeJuego = 3;
let texto1 = "Haz clic en alguna casilla vacia";
let texto2 = "", texto3 = "";
let jugadorActual, jugadorAnterior;
// Variables de las imagenes de fondo
let backgroundJugando, backgroundMenu;
let coordenadaYTexto, tamFuente, colorRelleno1, colorRelleno2, colorRelleno3;
let juegoTerminado;

function setup() {
	noStroke();
	canvas = createCanvas(640, 480);
	// Coordenadas (x, y) desde donde iniciara a imprimirse el grid o tabla del juego
	inicioX = (width - (largoCuadros * 3)) / 2;
	inicioY = (height - (largoCuadros * 3)) / 2;
	// Cargar las imagenes de los jugadores
	imagenJ1 = loadImage('img/j1.png');
	imagenJ2 = loadImage('img/j2.png');

	// Variables para la animacion al terminarse el juego
	coordenadaYTexto = height - 25;
	tamFuente = 22;
	colorRelleno1 = 255;
	colorRelleno2 = 255;
	colorRelleno3 = 255;
	juegoTerminado = false;

	// Cargar las imagenes de background
	backgroundMenu = loadImage('img/bgMenu.png');
	backgroundJugando = loadImage('img/bgJugando.png');

	// Almacenan cual es el jugador actual y cual fue el anterior
	jugadorActual = 'x';
	jugadorAnterior = 'o';

	textSize(22);
	textAlign(CENTER);
	crearMenu();
	centrarObjetos();
}

function draw() {
	if(menuActivo){
		background(backgroundMenu);
		fill(255);
		text("EyderACM", 110, height - 20);
		text("Carlos Chan", width - 110, height - 20);
	}
	else{
		background(backgroundJugando);
		/* Imprimir el grid */
		imprimirTabla();

		fill(colorRelleno1, colorRelleno2, colorRelleno3);
		textSize(tamFuente);
		text(texto1, width / 2, coordenadaYTexto);

		fill(255);
		textSize(22);
		text(texto2, width / 2, 50);
		text(texto3, width / 2, height - 25);
	}
	//Centrar el grid y los botones
	centrarObjetos();
	// Animacion que se ejecuta cuando el juego se termina
	animacionJuegoTerminado();
}

function keyPressed(){
	// Si el juego se termino y la animacion del texto se termino
	if(juegoTerminado && coordenadaYTexto <= height / 2){
		// Si se presiono r se reinicia el juego
		if(key == 'r' || key == 'R'){
			reiniciarJuego();
		}
	}
}

// Esta variable sirve para que al dar el primer clic no se ponga una 'x' en la tabla
// al dar clic con el mouse mousePressed() se activa una o dos veces, por eso los primeros
// 2 clics no se hara nada.
let turnos = 0;
/*
	Detecta cuando se presiona el mouse, esta funcion activa todo de acuerdo al clic
*/
function mousePressed(){
	// Si el menu ya no esta activo y si ya se dieron 2 clics y si el juego no a terminado
	if(!menuActivo && turnos > 1 && !juegoTerminado){
		if(!esEmpate(tabla) && (determinaPuntuacion(tabla) != 10) && (determinaPuntuacion(tabla) != -10)){
			// Calcular a que posicion de la tabla equivalen las coordenadas del mouse (x, y)
			let columnaTabla = Math.floor((mouseX - inicioX) / largoCuadros);
			let filaTabla = Math.floor((mouseY - inicioY) / largoCuadros);

			// Validar que las coordenadas esten en el rango de la tabla
			let clicValido = comprobarClicMouse(columnaTabla, filaTabla);

			if(clicValido){
				// Comprueba que la casilla este vacia
				let jugadaValida = comprobarCasilla(columnaTabla, filaTabla);
					if(jugadaValida){
						/* -----------------------
							Si se juega contra la IA
						--------------------------*/
						if(modoDeJuego == 1){
							// Colocar la marca del jugador en la tabla
							tabla[filaTabla][columnaTabla] = jugadorActual;
							turnoMaquina(tabla);
						}
						/* -----------------------
							Si se juega contra otro jugador
						--------------------------*/
						else if(modoDeJuego == 2){
							// Colocar la marca del jugador en la tabla
							tabla[filaTabla][columnaTabla] = jugadorActual;

							// Cambiar de turno
							if(jugadorActual == 'x'){
								jugadorActual = 'o';
								jugadorAnterior = 'x';
								texto2 = "Turno de: " + jugadorActual;
							}
							else if(jugadorActual == 'o'){
								jugadorActual = 'x';
								jugadorAnterior = 'o';
								texto2 = "Turno de: " + jugadorActual;
							}
						}
						/*
						 Se verifica si se termino el juego, victoria de maquina, jugador, o empate
						*/
						if(determinaPuntuacion(tabla) == 10 || determinaPuntuacion(tabla) == -10){
							texto2 = "Fin del juego";
							texto1 = "Gano el jugador: " + jugadorAnterior;
							texto3 = "Presiona R para reiniciar";
							juegoTerminado = true;
						}
						else if(esEmpate(tabla)){
							texto1 = "Es un empate!";
							texto2 = "Fin del juego";
							texto3 = "Presiona R para reiniciar";
							juegoTerminado = true;
						}
						else{
							texto1 = "Haz clic en alguna casilla vacia";
						}
					}
			} // Fin if(clicValido)
		} //Fin if(!esEmpate)
	} // Fin if(!menuActivo)
	else{
		// Si son los primeros dos clics no se hace nada y se incrementa la variable
		turnos++;
	}
}

// Checa si las coordenadas del mouse estan en el rango de casillas de la matriz 3x3
function comprobarClicMouse(columna, fila){
	let valido = false;
	if(fila >= 0 && fila <= 2){
		if(columna >= 0 && columna <= 2){
			valido = true;
		}
	}
	return valido;
}

// Checa que la casilla donde dio clic el jugador este vacia
function comprobarCasilla(columna, fila){
	let valido = false;
	if(tabla[fila][columna] == '_'){
		valido = true;
	}
	return valido;
}

// Imprime el grid del juego
function imprimirTabla(){
	if(!menuActivo){
		let filas, columnas;
		let x = inicioX,
				y = inicioY;
		// Las lineas de fondo
		fill(0);
		rect(x, y, largoCuadros * 3 + 10, largoCuadros * 3 + 10);
		// La matriz
		for(filas = 0; filas < 3; filas++){
			for(columnas = 0; columnas < 3; columnas++){
				if(tabla[filas][columnas] == 'o'){
					image(imagenJ2, x, y);
				}
				else if (tabla[filas][columnas] == 'x') {
					image(imagenJ1, x, y);
				}
				else if(tabla[filas][columnas] == '_'){
					fill(255);
					rect(x, y, largoCuadros, largoCuadros);
				}
				x += largoCuadros + 5;
			}
			x = inicioX;
			y += largoCuadros + 5;
		}
	}
}

/*
	Crea el menu de inicio
*/
function crearMenu(){
	menuActivo = true;
	boton1 = createButton('Jugar contra IA');
	boton2 = createButton('Jugar multijugador');
	boton3 = createButton('Simular juego');
	// Lo que sucede al hacer clic
	boton1.mousePressed(jugarIA);
	boton2.mousePressed(jugarMultiJugador);
	boton3.mousePressed(jugarSimulado);
}

/*
	Desaparece el menu de inicio
*/
function destruirMenu(){
	menuActivo = false;
	boton1.remove();
	boton2.remove();
	boton3.remove();
}

/*
	Invocar a los modos de juego
*/
function jugarIA(){
	destruirMenu();
	modoDeJuego = 1;
	texto2 = "Bienvenido a gato master 2000";
	// La funcion esta en el archivo TicTacToe.js
	//jugar(modoDeJuego);
}
function jugarMultiJugador(){
	destruirMenu();
	modoDeJuego = 2;
	texto2 = "Turno de: " + jugadorActual;
}
function jugarSimulado(){
	destruirMenu();
	modoDeJuego = 0;
	// La funcion esta en el archivo TicTacToe.js
	juegoTerminado = true;
	coordenadaYTexto = height / 2;
	texto1 = "";
	texto2 = "MODO NO DISPONIBLE";
	texto3 = "Presione R para regresar";
	//jugar(modoDeJuego);
}

/*
	Actualiza las posiciones del canvas y los botones
*/
function centrarObjetos(){
	// La cantidad de pixeles de desface en la coordenada (y) de los botones
	// con respecto al centro (y) del canvas
	let desfaceY = 60;

	// Centrar el canvas en la ventana
	canvas.position((windowWidth - width)/2, (windowHeight - height)/2);

	if (menuActivo) {
	// Variables que almacenan la coordenada central (x, y) del canvas
	let centroCanvasX = canvas.x + (width / 2),
			centroCanvasY = canvas.y + (height / 2);

/*
	Las coordenadas (y) de los botones se basan en la coordenada del boton central (boton2).
	Las coordenadas (x) de los botones se calculan en base a su ancho dividido entre 2 y dicho
	valor se le resta al centroX del canvas, de esa manera se centran horizontalmente.
	Boton de en medio, boton de arriba, boton de abajo.
*/
	boton2.position(centroCanvasX - (boton2.width / 2), centroCanvasY - (boton2.height / 2) + desfaceY);
	boton1.position(centroCanvasX - (boton1.width / 2), boton2.y - boton1.height - 15);
	boton3.position(centroCanvasX - (boton3.width / 2), boton2.y + boton1.height + 15);
	}
}

/*
	Animacion del texto cuando el juego se termina
*/
function animacionJuegoTerminado(){
	// Reduce la coordenada (y) del texto, aumenta el alto de la fuente
	// y cambia el color de relleno
	if(juegoTerminado && coordenadaYTexto > height / 2){
		coordenadaYTexto -= 3;
		tamFuente += .5;
		colorRelleno2 -= 0.5;
		colorRelleno3 -= 3;
	}
}

/*
	Funciones para reiniciar el juego
*/
function reiniciarJuego(){
	juegoTerminado = false;
	modoDeJuego = 3;
	coordenadaYTexto = height - 25;
	tamFuente = 22;
	colorRelleno1 = 255;
	colorRelleno2 = 255;
	colorRelleno3 = 255;
	texto1 = "Haz clic en alguna casilla vacia";
	texto2 = "";
	texto3 = "";
	turnos = 0;
	jugadorActual = 'x';
	jugadorAnterior = 'o';
	reiniciarMatriz();
	crearMenu();
}

// Llena la matriz con '_'
function reiniciarMatriz(){
	for(let filas = 0; filas < 3; filas++){
		for(let columnas = 0; columnas < 3; columnas++){
			tabla[filas][columnas] = '_';
		}
	}
}
