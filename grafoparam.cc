// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.cc
// -- implementación del objeto jerárquico de la práctica 3
//
// #############################################################################


#include "grafoparam.h"

// -----------------------------------------------------------------------------
// constructor: crea mallas indexadas en los nodos terminales del grafo

GrafoParam::GrafoParam()
{
	esfera = new Esfera( 10, 20 );
	semiesfera = new Semiesfera( 10, 20 );
	cono   = new Cono( 4, 16 );
	base   = new Cubo();
	cilindro = new Cilindro(3, 14);
}
// -----------------------------------------------------------------------------
// actualizar valor efectivo de un parámetro (a partir de su valor no acotado)
void GrafoParam::actualizarValorEfe( const unsigned iparam, const float valor_na )
{
	assert( iparam < num_parametros );

	using namespace std ;
	//cout << "GrafoParam::actualizarValorEfe( " << iparam << ", " << valor_na << " )" << endl ;

	switch( iparam )
	{
		case 0:
			angulo_hombro_izq = 90*cos( 0.8*(2.0*M_PI*valor_na) );
			break ;
		case 1:
			angulo_hombro_dch = -(90*cos( 0.8*(2.0*M_PI*valor_na) ));
			break ;
		case 2:
			altura_personaje = 0.1*cos( 0.8*(2.0*M_PI*valor_na) );
			break ;
		case 3:
			angulo_pelvis_izq = 70*cos( 0.8*(2.0*M_PI*valor_na) );

			break ;
		case 4:
			angulo_pelvis_dch = -(70*cos( 0.8*(2.0*M_PI*valor_na) ));
			break ;
		case 5:
			if (angulo_pelvis_izq > 0)
				angulo_rodilla_izq = 90*cos( 0.8*(2.0*M_PI*valor_na) );
			else
				angulo_rodilla_izq = 0;
			break ;
		case 6:
			if (angulo_pelvis_dch > 0)
				angulo_rodilla_dch = -(90*cos( 0.8*(2.0*M_PI*valor_na) ));
			else
				angulo_rodilla_dch = 0;
		case 7:
			posicion_personaje_x = 6*cos( 0.2*(2.0*M_PI*valor_na) );
			posicion_personaje_y = 6*sin( 0.2*(2.0*M_PI*valor_na) );
			angulo_personaje = -72*valor_na;
			break ;
	}
}

// -----------------------------------------------------------------------------
// visualización del objeto Jerárquico con OpenGL,
// mediante llamadas a los métodos 'draw' de los sub-objetos

void GrafoParam::draw( const int p_modo_vis, const bool p_usar_diferido, int p_valor,
					   bool p_sel_dibujada, bool p_obj_sel[]) //ModoVis
{
	// asegurarnos que está inicializado ok

	assert( cono     != nullptr );
	assert( esfera   != nullptr );

	// guardar parametros de visualización para esta llamada a 'draw'
	// (modo de visualización, modo de envío)

	modo_vis      = p_modo_vis ;
	usar_diferido = p_usar_diferido ;
	valor         = p_valor ;
	sel_dibujada  = p_sel_dibujada ;

	for (int i = 0; i < 255; ++i)
		obj_sel[i] = p_obj_sel[i];

	// dibujar objetos
	glPushMatrix();
		glTranslatef( posicion_personaje_x,altura_personaje,posicion_personaje_y);
		glRotatef( angulo_personaje, 0.0, 1.0, 0.0);

		// cuerpo
		glPushMatrix();
			glColor3f( 0.8, 0.8, 0.8 );
			cuerpo();
		glPopMatrix();

		// cabeza
		glPushMatrix();
			glTranslatef( 0.0, 2.0, -0.1 );
			cabeza();
		glPopMatrix();

		// pierna arriba
		glPushMatrix();
			glTranslatef( -0.2, -0.15, 0.0 );
			glRotatef( angulo_pelvis_dch, 1,0,0 );
			glTranslatef( 0.0, -0.1, 0.0 );
			pierna_arriba();
		glPopMatrix();
		glPushMatrix();
			glTranslatef( 0.2, -0.15, 0.0 );
			glRotatef( angulo_pelvis_izq, 1,0,0 );
			glTranslatef( 0.0, -0.1, 0.0 );
			pierna_arriba();
		glPopMatrix();
		
		// pierna abajo
		glPushMatrix();
			glTranslatef( 0, -0.6*sin(M_PI*abs(angulo_pelvis_dch-90)/180), -0.5*cos(M_PI*abs(angulo_pelvis_dch-90)/180) );
			glTranslatef( -0.2, -0.1+(-0.5*sin(M_PI*abs(angulo_rodilla_dch-90)/180)),
									  -0.05+(-0.5*cos(M_PI*abs(angulo_rodilla_dch-90)/180)) );
			glRotatef( angulo_rodilla_dch, 1,0,0 );
			glTranslatef( 0, 0.9 ,0.05);
			pierna_abajo();
		glPopMatrix();
		glPushMatrix();
			glTranslatef( 0, -0.6*sin(M_PI*abs(angulo_pelvis_izq-90)/180), -0.5*cos(M_PI*abs(angulo_pelvis_izq-90)/180) );
			glTranslatef(0.2, -0.1+(-0.5*sin(M_PI*abs(angulo_rodilla_izq-90)/180)),
									-0.05+(-0.5*cos(M_PI*abs(angulo_rodilla_izq-90)/180)) );
			glRotatef( angulo_rodilla_izq, 1,0,0 );
			glTranslatef( 0, 0.9, 0.05);
			pierna_abajo();
		glPopMatrix();

		// brazos
		glPushMatrix();
			glTranslatef( 0.2, 1.3, 0.0 );
			glRotatef(angulo_hombro_izq,1,0,0);
			glRotatef(25, 0.0, 0.0, 1.0);
			brazo();
		glPopMatrix();
		glPushMatrix();
			glTranslatef( -0.2, 1.3, 0.0 );
			glRotatef(angulo_hombro_dch,1,0,0);
			glRotatef(-25, 0.0, 0.0, 1.0);
			brazo();
		glPopMatrix();

	glPopMatrix();
		
		// base
	glPushMatrix();
		glColor3f( 0.1, 0.6, 0.1 );
		glTranslatef( 0.0, -1.3, 0.0 );
		glScalef( 20, 0.2, 20 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[0] == false)  base->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				base->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				base->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	base->draw_seleccion(50,50,50) ;}
		
	glPopMatrix();
}



void GrafoParam::cuerpo()
{
	glColor3f( 0.8, 0.8, 0.0 );
	glPushMatrix();
		glScalef( 0.48, 1.4, 0.48 );
		glTranslatef( 0.0, 0.25, 0.0 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[1] == false)  cono->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				cono->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				cono->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	cono->draw_seleccion(50+1, 50+1, 50+1) ;}
	glPopMatrix();

	glColor3f( 0.9, 0.0, 0.0 );
	glPushMatrix( );
		glScalef( 0.5, 0.5, 0.5);
		glTranslatef( 0.0, 0.5+altura_base, 0.0 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[2] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+2, 50+2, 50+2) ;}
	glPopMatrix();
}

void GrafoParam::pierna_arriba()
{
	glColor3f( 0.9, 0.0, 0.0 );
	glPushMatrix( );
		glScalef( 0.15, 0.25, 0.15);
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[3] == false)  semiesfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				semiesfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				semiesfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	semiesfera->draw_seleccion(50+3, 50+3, 50+3) ;}
	glPopMatrix();

	glColor3f( 0.5, 0.7, 1.0 );
	glPushMatrix();
		glTranslatef( 0, -0.4, 0);
		glScalef( 0.2, 0.5, 0.2 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[4] == false)  cilindro->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				cilindro->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				cilindro->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	cilindro->draw_seleccion(50+4, 50+4, 50+4) ;}
	glPopMatrix();
}

void GrafoParam::pierna_abajo()
{
	glColor3f( 0.5, 0.7, 1.0 );
	glPushMatrix( );
		glTranslatef( 0, -0.4, 0);
		glScalef( 0.1, 0.1, 0.1);
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[5] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+5, 50+5, 50+5) ;}
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 0, -0.8, 0);
		glScalef( 0.2, 0.4, 0.2 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[6] == false)  cilindro->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				cilindro->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				cilindro->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	cilindro->draw_seleccion(50+6, 50+6, 50+6) ;}
	glPopMatrix();

	glColor3f( 0.9,0.9,0.9 );
	glPushMatrix();
		glTranslatef( 0, -0.85, 0.1);
		glScalef( 0.15, 0.15, 0.3 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[7] == false)  semiesfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				semiesfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				semiesfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	semiesfera->draw_seleccion(50+7, 50+7, 50+7) ;}
	glPopMatrix();
}

void GrafoParam::brazo()
{
	glColor3f( 0.8, 0.8, 0.0 );
	glPushMatrix();
		glTranslatef( 0,-0.15,0);
		glScalef( 0.1, 0.3, 0.1 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[8] == false)  semiesfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				semiesfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				semiesfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	semiesfera->draw_seleccion(50+8, 50+8, 50+8) ;}
	glPopMatrix();

	glColor3f( 0.5, 0.7, 1.0 );
	glPushMatrix();
		glTranslatef( 0.0, -0.65, 0.0);
		glScalef( 0.1, 0.5, 0.1 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[9] == false)  cilindro->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				cilindro->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				cilindro->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	cilindro->draw_seleccion(50+9, 50+9, 50+9) ;}
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 0.0, -0.65, 0.0);
		glScalef( 0.07, 0.07, 0.07 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[10] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+10, 50+10, 50+10) ;}
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 0.0, -1.05, 0.4);
		glRotatef( -45, 1, 0, 0 );
		glScalef( 0.1, 0.5, 0.1 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[11] == false)  cilindro->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				cilindro->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				cilindro->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	cilindro->draw_seleccion(50+11, 50+11, 50+11) ;}
	glPopMatrix();

	glPushMatrix();
		glColor3f( 0.9,0.9,0.9 );
		glTranslatef( 0.0, -1.05, 0.4);
		glRotatef( -45, 1, 0, 0 );
		glScalef( 0.1, 0.2, 0.1 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[12] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+12, 50+12, 50+12) ;}
	glPopMatrix();
}

void GrafoParam::cabeza()
{
	// cabeza
	glColor3f( 0.5, 0.7, 1.0 );
	glPushMatrix();
		glScalef( 0.5, 0.5, 0.5 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[13] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+13, 50+13, 50+13) ;}
	glPopMatrix();

	// nariz
	glColor3f( 0,0,0 );
	glPushMatrix();
		glTranslatef( 0.0, -0.2, 0.5);
		glScalef( 0.15, 0.15, 0.15 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[14] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+14, 50+14, 50+14) ;}
	glPopMatrix();

	// ojos
	glPushMatrix();
		glTranslatef( -0.2, 0.0, 0.52);
		glScalef( 0.05, 0.05, 0.05 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[15] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+15, 50+15, 50+15) ;}
	glPopMatrix();
	glPushMatrix();
		glTranslatef( 0.2, 0.0, 0.52);
		glScalef( 0.05, 0.05, 0.05 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[16] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+16, 50+16, 50+16) ;}
	glPopMatrix();

	glColor3f( 0.9,0.9,0.9 );
	glPushMatrix();
		glTranslatef( -0.2, 0.0, 0.4);
		glRotatef( 15, 1,0,0 );
		glScalef( 0.15, 0.2, 0.15 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[17] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+17, 50+17, 50+17) ;}
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 0.2, 0.0, 0.4);
		glRotatef( 15, 1,0,0 );
		glScalef( 0.15, 0.2, 0.15 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[18] == false)  esfera->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				esfera->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				esfera->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	esfera->draw_seleccion(50+18, 50+18, 50+18) ;}
	glPopMatrix();
	
	// gorro
	glColor3f( 0.9, 0.0, 0.0);
	glPushMatrix();
		glTranslatef( 0.0, 0.4, -0.1 );
		glRotatef( -15, 1.0, 0.0, 0.0 );
		glScalef( 0.3, 0.3, 0.3 );
		if (sel_dibujada){
			seleccionar_modo(modo_vis);
            if (obj_sel[19] == false)  cilindro->draw(modo_vis, usar_diferido, valor, "") ;
			else{
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
				cilindro->draw_seleccion(255, 155, 0) ;
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1.5);
				cilindro->draw_seleccion(0, 0, 0) ;
			}
		}
		else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	cilindro->draw_seleccion(50+19, 50+19, 50+19) ;}
	glPopMatrix();
}



void GrafoParam::seleccionar_modo(int modo_vis){
	switch( modo_vis )
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
			cout << "draw_object: el número de modo actual (" << modo_vis << ") es incorrecto." << endl ;
			break ;
	}
}