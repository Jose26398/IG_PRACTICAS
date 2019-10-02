

#include "aux.h"     // includes de OpenGL/glut/glew, windows, y librería std de C++
#include "escena.h"
#include "malla.h" // objetos: Cubo y otros....

//**************************************************************************
// constructor de la escena (no puede usar ordenes de OpenGL)
//**************************************************************************

Escena::Escena()
{
		Front_plane = 0.1;
		Back_plane = 2000.0;
		Observer_distance = 2.0;
		Observer_angle_x  = 0.0 ;
		Observer_angle_y  = 0.0 ;

		ejes.changeAxisSize( 5000 );

		// crear los objetos de las prácticas: Mallas o Jerárquicos....
		cubo          = new Cubo();
		tetraedro     = new Tetraedro();
		ant           = new ObjPLY("plys/beethoven.ply");
		peon          = new ObjRevolucion("plys/peon.ply");
		cilindro      = new Cilindro(4,20);
		cono          = new Cono(2,20);
		esfera        = new Esfera(20,30);
		objJerarquico = new ObjJerarquico();
		cuadro        = new Cuadro();
		piramide	  = new Piramide();

		num_objetos = 10 ; // se usa al pulsar la tecla 'O' (rotar objeto actual)
}

//**************************************************************************
// inicialización de la escena (se ejecuta cuando ya se ha creado la ventana, por
// tanto sí puede ejecutar ordenes de OpenGL)
// Principalmemnte, inicializa OpenGL y la transf. de vista y proyección
//**************************************************************************

void Escena::inicializar( int UI_wx, int UI_wy )
{
	glClearColor( 1.0, 1.0, 1.0, 1.0 );// se indica cual sera el color para limpiar la ventana  (r,v,a,al)

	glEnable( GL_DEPTH_TEST );  // se habilita el z-bufer

	redimensionar( UI_wx, UI_wy );

	cout << endl << endl << "---- TECLAS UTILIZADAS ----" << endl
		 << "MOVIMIENTO: Flechas, RePag y AvPag" << endl
		 << "CAMBIAR OBJETO: O" << endl
		 << "CAMBIAR MODO DE VISUALIZACION: M" << endl
		 << "CAMBIAR MODO DE DIBUJADO: V" << endl
		 << "CONMUTAR ANIMACIONES: A (Demas de la practica 3 en la linea 365)" << endl << endl
		 << "----- PRACTICA 5 -----" << endl
		 << "ACTIVAR Y DESACTIVAR CÁMARAS: 1 y 2 (no podia poner F1 y F2 en mi portátil)" << endl
		 << "Cuando se activan las cámaras bloqueo el movimiento y elimino los ejes. Cuando se desactivan vuelvo a la posición en la que estaba "
		 <<	"antes. El código donde estan implementadas está en las siguientes líneas:" << endl
		 << "  escena.cc -> 102-113, 393-427, 547-553" << endl
		 << "MOVIMIENTO CON RATÓN: Boton derecho + movimiento y rueda del ratón" << endl
		 << "Implementado en las siguientes líneas:" << endl
		 << "  escena.cc -> 539-549" << endl
		 << "  practicas.cc -> 111-157" << endl
		 << "SELECCIONAR: Boton izquierdo del ratón" << endl
		 << "El primer objeto son las pirámides del ejemplo, las cuales se podrán seleccionar como en este. El resto de objetos también se podrán "
		 <<	"seleccionar, pero el más representativo es el segundo, que es el ObjJerarquico de la practica 3 (el resto no tienen mucho misterio)" << endl
		 << "Implementado en las siguientes líneas:" << endl
		 << "  escena.cc -> 319-332 (doble buffer), 572-600 (funciones utilizadas)" << endl
		 << "  escena.cc -> 130-305 (ya que están cambiados todos los draw)" << endl
		 << "  escena.h -> 39-47" << endl
		 << "  malla.cc -> 292-311 (método draw_seleccion), 600-706 (clase Pirámide)" << endl
		 << "-----------------------------------------" << endl << endl;
}

// **************************************************************************
// Funcion que dibuja el objeto activo actual, usando su método 'draw'
// (llamada desde Escena::dibujar)
// ***************************************************************************

void Escena::dibujar_objeto_actual()
{
	using namespace std ;
	 
	// (1) configurar OpenGL para el modo actual (puntos/lineas/sólido)
	//    llamar glPolygonMode, glColor... (y alguna cosas más), según dicho modo
	glColor3f(0.0,0.0,0.0);
	glPointSize(4);
	glLineWidth(2);

	switch( modo_actual )
	{
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);  
			break ;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);  
			break ;
		case 2:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		case 3:
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			break;
		case 4:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 5:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		default:
			cout << "draw_object: el número de modo actual (" << modo_actual << ") es incorrecto." << endl ;
			break ;
	}


	// Practica 5 -- Seleccionar cámara
	switch( num_camara )
	{
		case 1:
			moverCamara(1);
			break;
		case 2:
			moverCamara(2);
			break;
		default:
			break;
	}


	// (2) dibujar el objeto actual usando método 'draw' del objeto asociado al
	// valor entero en 'objeto_actual'

	switch( objeto_actual )
	{
		case 0:
			num_objetos_sel = 5;
			if ( piramide != nullptr ){
				if (seleccion_dibujada){
					piramide->draw_piramides(modo_actual, modo_dibujado, valor, "", 255, 155, 0, obj_sel) ;
					glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
					glLineWidth(1.5);
					piramide->draw_piramides(modo_actual, modo_dibujado, valor, "", 0, 0, 0, obj_sel) ;
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					piramide->draw_piramides_seleccion() ;
				}
			}
			break;
		case 1:
			num_objetos_sel = 20;
			if ( objJerarquico != nullptr ){
				if (seleccion_dibujada)
					objJerarquico->draw(modo_actual, true, valor, true, obj_sel) ;
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					objJerarquico->draw(modo_actual, true, valor, false, obj_sel) ;
				}
			}
			break;
		case 2:
			num_objetos_sel = 1;
			if ( cubo != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)  cubo->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						cubo->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						cubo->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					cubo->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 3:
			num_objetos_sel = 1;
			if ( tetraedro != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	tetraedro->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						tetraedro->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						tetraedro->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					tetraedro->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 4:
			num_objetos_sel = 1;
			if ( cilindro != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	cilindro->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						cilindro->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						cilindro->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					cilindro->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 5:
			num_objetos_sel = 1;
			if ( cono != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	cono->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						cono->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						cono->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					cono->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 6:
			num_objetos_sel = 1;
			if ( peon != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	peon->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						peon->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						peon->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					peon->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 7:
			num_objetos_sel = 1;
			if ( ant != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	ant->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						ant->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						ant->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					ant->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 8:
			num_objetos_sel = 1;
			if ( cuadro != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	cuadro->draw(modo_actual, modo_dibujado, valor, "./imagen.png") ;
					else{
						cuadro->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						cuadro->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					cuadro->draw_seleccion(r, g, b) ;
				}
			}
			break;
		case 9:
			num_objetos_sel = 1;
			if ( esfera != nullptr ){
				if (seleccion_dibujada){
					if (obj_sel[0] == false)	esfera->draw(modo_actual, modo_dibujado, valor, "") ;
					else{
						esfera->draw_seleccion(255, 155, 0) ;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						glLineWidth(1.5);
						esfera->draw_seleccion(0, 0, 0) ;
					}
				}
				else{
					glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
					esfera->draw_seleccion(r, g, b) ;
				}
			}
			break;
		
		default:
			cout << "draw_object: el número de objeto actual (" << objeto_actual << ") es incorrecto." << endl ;
			break ;
	}
}


// **************************************************************************
//
// función de dibujo de la escena: limpia ventana, fija cámara, dibuja ejes,
// y dibuja los objetos
//
// **************************************************************************

void Escena::dibujar()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Limpiar la pantalla
	change_observer();
	ejes.draw();
	dibujar_objeto_actual();
	seleccion_dibujada = false;

	glDrawBuffer(GL_BACK);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Limpiar la pantalla
	change_observer();
	dibujar_objeto_actual();
	seleccion_dibujada = true;
	glDrawBuffer(GL_FRONT);

}

//**************************************************************************
//
// función que se invoca cuando se pulsa una tecla
// Devuelve true si se ha pulsado la tecla para terminar el programa (Q),
// devuelve false en otro caso.
//
//**************************************************************************

bool Escena::teclaPulsada( unsigned char tecla, int x, int y )
{
	using namespace std ;
	cout << "Tecla pulsada: '" << tecla << "'" << endl;

	if (tecla == 'z' && objeto_actual == 4){
		objJerarquico->decrementaParamAct();
	}
	 
	else{
		switch( toupper(tecla) )
		{
		 	// salir
			case 'Q' :
				return true ;
				break;
			
			// activar siguiente objeto
			case 'O' :
				objeto_actual = (objeto_actual+1) % num_objetos ;
				seleccion_dibujada = true; animacion_activa = false;
				cout << "objeto actual == " << objeto_actual << endl ;
				break;

			// modo
			case 'M' :
				modo_actual = (modo_actual+1) % 6 ;
				cout << "modo actual == " << modo_actual << endl ;
				break;
			
			// dibujado
			case 'V' :
				modo_dibujado = (modo_dibujado+1) % 2 ;
				if (modo_dibujado == 0)   cout << "modo dibujado == diferido" << endl ;
				else      cout << "modo dibujado == inmediato" << endl ;
				break ;

			case 'P' :
				if (objeto_actual == 1) objJerarquico->siguienteParametro();
				break;
			
			case 'A' :
				if (objeto_actual == 1) conmutarAnimaciones();
				break;
			
			case 'Z' :
				if (objeto_actual == 1) objJerarquico->incrementaParamAct();
				break;
			
			case '<' :
				if (objeto_actual == 1) objJerarquico->acelerar();
				break;
			
			case '>' :
				if (objeto_actual == 1) objJerarquico->decelerar();
				break;
			
			case 'R' :
				valor = (valor+1)%1000;
				break;
			

			//*************************************************************************************
			// PRACTICA 5 -- Cámaras
			// Se guarda la posición que había antes de ponerla para recuperarla cuando la quitamos
			//*************************************************************************************
			case '1' :
				if (num_camara != 1){
					num_camara = 1;
					if (Observer_distance != 0) od_aux = Observer_distance;
					if (Observer_angle_x  != 0) ox_aux = Observer_angle_x;
					if (Observer_angle_y  != 0) oy_aux = Observer_angle_y;
					Observer_distance = Observer_angle_x = Observer_angle_y = 0;
				}
				else{
					num_camara = 0;
					Observer_distance = od_aux;
					Observer_angle_x  = ox_aux;
					Observer_angle_y  = oy_aux;
				}
				break;

			case '2' :
				if (num_camara != 2){
					num_camara = 2;
					if (Observer_distance != 0) od_aux = Observer_distance;
					if (Observer_angle_x  != 0) ox_aux = Observer_angle_x;
					if (Observer_angle_y  != 0) oy_aux = Observer_angle_y;
					Observer_distance = Observer_angle_x = Observer_angle_y = 0;
				}
				else{
					num_camara = 0;
					Observer_distance = od_aux;
					Observer_angle_x  = ox_aux;
					Observer_angle_y  = oy_aux;
				}
				break;
		}

	}
	return false ;
}
//**************************************************************************

void Escena::teclaEspecial( int Tecla1, int x, int y )
{
	if (num_camara == 0){
		switch ( Tecla1 )
		{
			case GLUT_KEY_LEFT:
				Observer_angle_y-- ;
				break;
			case GLUT_KEY_RIGHT:
				Observer_angle_y++ ;
				break;
			case GLUT_KEY_UP:
				Observer_angle_x-- ;
				break;
			case GLUT_KEY_DOWN:
				Observer_angle_x++ ;
				break;
			case GLUT_KEY_PAGE_UP:
				Observer_distance *= 1.1 ;
				break;
			case GLUT_KEY_PAGE_DOWN:
				Observer_distance /= 1.1 ;
				break;
		}
	}

	//std::cout << Observer_distance << std::endl;
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//
// ratio_xy : relacción de aspecto del viewport ( == ancho(X) / alto(Y) )
//
//***************************************************************************

void Escena::change_projection( const float ratio_xy )
{
	 glMatrixMode( GL_PROJECTION );
	 glLoadIdentity();
	 const float wy = 0.84*Front_plane,
				 wx = ratio_xy*wy ;
	 glFrustum( -wx, +wx, -wy, +wy, Front_plane, Back_plane );
}
//**************************************************************************
// Funcion que se invoca cuando cambia el tamaño de la ventana
//***************************************************************************

void Escena::redimensionar( int newWidth, int newHeight )
{
	 Width = newWidth;
	 Height = newHeight;
	 change_projection( float(Width)/float(Height) );
	 glViewport( 0, 0, Width, Height );
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void Escena::change_observer()
{
	 // posicion del observador
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 glTranslatef( 0.0, 0.0, -Observer_distance );
	 glRotatef( Observer_angle_x, 1.0 ,0.0, 0.0 );
	 glRotatef( Observer_angle_y, 0.0, 1.0, 0.0 );
}



/*************************************************************************/

void Escena::conmutarAnimaciones()
{
	if (!animacion_activa){
		animacion_activa = true;
		objJerarquico->inicioAnimaciones();
		glutIdleFunc( funcion_desocupado );
	}
	else {
		animacion_activa = false;
		glutIdleFunc( nullptr );
	}
}

void Escena::mgeDesocupado(){
	objJerarquico->actualizarEstado();
	glutPostRedisplay();
}



//*************************************************************************/
//
//	PRACTICA 5
//
//*************************************************************************/

void Escena::getCamara (GLfloat *x, GLfloat *y){
	*x = Observer_angle_y;
	*y = Observer_angle_x;
}

void Escena::setCamara (GLfloat x, GLfloat y){
	if (num_camara == 0){
		Observer_angle_x = y;
		Observer_angle_y = x;
	}
}

void Escena::moverCamara(int num_camara){
	if (num_camara == 1){
		gluLookAt(0.0, 0, 4,   0.0, 0, 0.0,    0.0, 1.0, 0.0);
	}
	if (num_camara == 2){
		gluLookAt(0.0, 4, 0,   0.0, 0, 0.0,    0.0, 0.0, 1.0);
	}


}


void Escena::pick(int x, int y)
{
	GLint viewport[4];
	float pixel[3];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadBuffer(GL_BACK);
	glReadPixels(x, viewport[3]-y, 1, 1, GL_RGB, GL_FLOAT, (GLfloat *) &pixel[0]);
	printf(" valor x %d, valor y %d, color %f, %f, %f \n",x,y,pixel[0],pixel[1],pixel[2]);

	procesar_color(pixel);
	glutPostRedisplay();
}

void Escena::procesar_color(float color[3]){
	int col = 255*color[0];	

	for (int i = 0; i < num_objetos_sel; ++i)
	{
		if (obj_sel[i] && col == 50+i){
			obj_sel[i] = false;
		}

		else if (!obj_sel[i] && col == 50+i){
			obj_sel[i] = true;
		}
	}
}
