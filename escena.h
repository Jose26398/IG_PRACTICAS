#ifndef _ESCENA_H
#define _ESCENA_H

#include "ejes.h"
#include "malla.h"
#include "jerarquico.h"

class Escena
{

	private:

	Ejes ejes;

	// variables que definen la posicion de la camara en coordenadas polares
	GLfloat Observer_distance, od_aux;
	GLfloat Observer_angle_x,  ox_aux;
	GLfloat Observer_angle_y,  oy_aux;

	// variables que controlan la ventana y la transformacion de perspectiva
	GLfloat Width, Height, Front_plane, Back_plane;

	void clear_window();
	void dibujar_objeto_actual();

	// Transformación de cámara
	void change_projection( const float ratio_xy );
	void change_observer();

	int objeto_actual = 0 , // objeto actual (el que se visualiza)
		num_objetos   = 0 , // número de objetos (actualizado al crear los objetos en el constructor)
		modo_actual   = 3 , // modo en el que se visualiza el objeto actual
		modo_dibujado = 1 ,
		valor	   	  = 0 ;

	bool modo_ajedrez 		= false,
		 animacion_activa   = false;

	// Practica 5
	int 	r   = 50,
			g   = 50,
			b   = 50,
			num_objetos_sel = 0,
			num_camara		= 0;

	bool 	obj_sel[255],
		 	seleccion_dibujada = true;



	 // Objetos de la escena
	Cubo * cubo           = nullptr ; // es importante inicializarlo a 'nullptr'
	Tetraedro * tetraedro = nullptr;
	ObjPLY * ant          = nullptr;
	ObjRevolucion * peon  = nullptr;
	Cilindro * cilindro   = nullptr;
	Cono * cono           = nullptr;
	Esfera * esfera       = nullptr;
	Cuadro * cuadro       = nullptr;
	ObjJerarquico * objJerarquico = nullptr;
	Piramide * piramide   = nullptr;


	public:

	Escena();
	void inicializar( int UI_window_width, int UI_window_height );
	void redimensionar( int newWidth, int newHeight ) ;

	// Dibujar
	void dibujar() ;

	// Interacción con la escena
	bool teclaPulsada( unsigned char Tecla1, int x, int y ) ;
	void teclaEspecial( int Tecla1, int x, int y );
	void conmutarAnimaciones();
	void mgeDesocupado();

	// Practica 5
	void setCamara(GLfloat x, GLfloat y);
	void getCamara(GLfloat *x, GLfloat *y);
	void pick(int x, int y);
	void procesar_color(float color[3]);
	void moverCamara(int num_camara);
};

#endif
