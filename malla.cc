#include "aux.h"
#include "ply_reader.h"
#include "malla.h"
#include "math.h"

using namespace std;
using namespace cimg_library;

// *****************************************************************************
//
// Clase ObjMallaIndexada
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Función de visualización de la malla,
// puede llamar a  draw_ModoInmediato o bien a draw_ModoDiferido
void ObjMallaIndexada::draw(int modo_actual, bool modo_dibujado, int valor, string imagen)
{
	
	if (modo_actual == 5){
		if (normales_vertices.size()!=0)
			draw_iluminacion_suave(valor, imagen);
		else{
			calcular_normales();
			draw_iluminacion_suave(valor, imagen);
		}
	}

	else if (modo_actual == 4){
		if (normales_vertices.size()!=0)
			draw_iluminacion_plana(valor, imagen);
		else{
			calcular_normales();
			draw_iluminacion_plana(valor, imagen);
		}
	}
	
	else{
		if (modo_dibujado == 0)
			draw_ModoDiferido(modo_actual, imagen);
		else
			draw_ModoInmediato(modo_actual, imagen);
	}

}


// -----------------------------------------------------------------------------
// Visualización en modo inmediato con 'glDrawElements'
void ObjMallaIndexada::draw_ModoInmediato(int modo_actual, string imagen)
{
  	// habilitar uso de un array de vértices
	glEnableClientState( GL_VERTEX_ARRAY );
	// indicar el formato y la dirección de memoria del array de vértices
	// (son tuplas de 3 valores float, sin espacio entre ellas)
	glVertexPointer( 3, GL_FLOAT, 0, vertices.data() ) ;
	// visualizar, indicando: tipo de primitiva, número de índices,
	// tipo de los índices, y dirección de la tabla de índices

	if (modo_actual == 3){
		for (unsigned int i=0; triangulos.size()>i; i++){
			
			if (i%2 == 0)
				glColor3f(0.0,0.0,0.0);
			else
				glColor3f(1.0,0.3,0.3);
						
			glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangulos.data()+i );
		}
	}
	else
		glDrawElements( GL_TRIANGLES, triangulos.size()*3, GL_UNSIGNED_INT, triangulos.data() );
	
	// deshabilitar array de vértices
	glDisableClientState( GL_VERTEX_ARRAY );

	if (imagen != "")	draw_texturas(imagen);
}



GLuint CrearVBO( GLuint tipo_vbo, GLuint tamanio_bytes, GLvoid * puntero_ram )
{
	GLuint id_vbo ;
	// resultado: identificador de VBO
	glGenBuffers( 1, & id_vbo );
	// crear nuevo VBO, obtener identificador (nunca 0)
	glBindBuffer( tipo_vbo, id_vbo ); // activar el VBO usando su identificador
	// esta instrucción hace la transferencia de datos desde RAM hacia GPU
	glBufferData( tipo_vbo, tamanio_bytes, puntero_ram, GL_STATIC_DRAW );
	glBindBuffer( tipo_vbo, 0 );
	return id_vbo ;
}
// -----------------------------------------------------------------------------
// Visualización en modo diferido con 'glDrawElements' (usando VBOs)
void ObjMallaIndexada::draw_ModoDiferido(int modo_actual, string imagen)
{
	if (id_vbo_ver == 0){
		id_vbo_ver = CrearVBO(GL_ARRAY_BUFFER, 3*sizeof(float)*vertices.size(), vertices.data());
	}
	if (id_vbo_tri == 0){
		id_vbo_tri = CrearVBO(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(unsigned)*triangulos.size(), triangulos.data());
	}
	
	// especificar localización y formato de la tabla de vértices, habilitar tabla
	glBindBuffer( GL_ARRAY_BUFFER, id_vbo_ver ); 	// activar VBO de vértices
	glVertexPointer( 3, GL_FLOAT, 0, 0 );			// especifica formato y offset (=0)
	glBindBuffer( GL_ARRAY_BUFFER, 0 );				// desactivar VBO de vértices.
	glEnableClientState( GL_VERTEX_ARRAY );			// habilitar tabla de vértices

	// visualizar triángulos con glDrawElements (puntero a tabla == 0)
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri );// activar VBO de triángulos

	if (modo_actual == 3){
		for (unsigned int i=0; triangulos.size()>i; i++){
			
			if (i%2 == 0)
				glColor3f(0.0,0.0,0.0);
			else
				glColor3f(1.0,0.3,0.3);
						
			glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*3*i) );
		}
	}
	else
		glDrawElements( GL_TRIANGLES, triangulos.size()*3, GL_UNSIGNED_INT, 0 );

	// desactivar VBO de triángulos
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	// desactivar uso de array de vértices
	glDisableClientState( GL_VERTEX_ARRAY );

	if (imagen != "")	draw_texturas(imagen);
}


// -----------------------------------------------------------------------------
// Visualización en con iluminacion plana (usando normales de las caras)
void ObjMallaIndexada::draw_iluminacion_plana(int valor, string imagen)
{
	//Cyan plastic
	GLfloat mat_ambient[]  = { 0.0,		   0.1, 	   0.06,	   1.0 };
	GLfloat mat_diffuse[]  = { 0.0, 	   0.50980392, 0.50980392, 1.0 };
	GLfloat mat_specular[] = { 0.50196078, 0.50196078, 0.50196078, 1.0 };
	GLfloat shine[] = { 32.0 };


	float posicion_luz_x	  = 6*cos( 0.01*(2.0*M_PI*valor) );
    float posicion_luz_z 	  = 6*sin( 0.01*(2.0*M_PI*valor) );
	GLfloat posicion_luz_0[]  = { 0.0, 6.0, 5.0, 0.0 };
	GLfloat posicion_luz_1[]  = { posicion_luz_x, 0.5, posicion_luz_z, 1.0 };
	GLfloat luz_difusa_1[]	  = { 1.0, 0.0, 0.8, 0.0 };
	GLfloat luz_especular_1[] = { 1.0, 0.0, 0.8, 0.0 };
	
	glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz_0);
	glLightfv(GL_LIGHT1, GL_POSITION, posicion_luz_1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  luz_difusa_1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular_1);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

    glVertexPointer( 3, GL_FLOAT, 0, vertices.data() );
	glNormalPointer( GL_FLOAT, 0, normales_caras.data() );

	// Para añadir las luces a las texturas, descomentar las siguientes líneas
	//if (imagen != ""){ draw_texturas(imagen); }

	//else{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

		glBegin(GL_TRIANGLES);
	    for (int i = 0; i<triangulos.size(); i++) {
	        glNormal3fv(normales_caras[i]);
	        glVertex3fv(vertices[triangulos[i](0)]);
	        glVertex3fv(vertices[triangulos[i](1)]);
	        glVertex3fv(vertices[triangulos[i](2)]);
	    }
	    glEnd();
	//}

	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);
}



// -----------------------------------------------------------------------------
// Visualización en con iluminacion suave (usando normales de los vertices)
void ObjMallaIndexada::draw_iluminacion_suave(int valor, string imagen)
{
	// Polished silver
	GLfloat mat_ambient[]  = { 0.23125,  0.23125,  0.23125,  1.0 };
	GLfloat mat_diffuse[]  = { 0.2775,   0.2775,   0.2775,   1.0 };
	GLfloat mat_specular[] = { 0.773911, 0.773911, 0.773911, 1.0 };
	GLfloat shine[] = { 89.6 };


	float posicion_luz_x	  = 6*cos( 0.01*(2.0*M_PI*valor) );
    float posicion_luz_z	  = 6*sin( 0.01*(2.0*M_PI*valor) );
	GLfloat posicion_luz_0[]  = { 0.0, 6.0, 5.0, 0.0 };							// DIRECCIONAL - inf
	GLfloat posicion_luz_1[]  = { posicion_luz_x, 0.5, posicion_luz_z, 1.0 };	// POSICIONAL
	GLfloat luz_difusa_1[]	  = { 1.0, 0.0, 1.0, 0.0 };
	GLfloat luz_especular_1[] = { 1.0, 0.0, 1.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, posicion_luz_0);
	glLightfv(GL_LIGHT1, GL_POSITION, posicion_luz_1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  luz_difusa_1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, luz_especular_1);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

    glVertexPointer( 3, GL_FLOAT, 0, vertices.data() );
	glNormalPointer( GL_FLOAT, 0, normales_vertices.data() );

	// Para añadir las luces a las texturas, descomentar las siguientes líneas
	// if (imagen != ""){ draw_texturas(imagen); }

	//else{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);

		glDrawElements( GL_TRIANGLES, triangulos.size()*3, GL_UNSIGNED_INT, triangulos.data() ) ;
	//}

	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
}


// -----------------------------------------------------------------------------
// Dibuja una textura en un objeto con los texVertices ya calculados
void ObjMallaIndexada::draw_texturas(string imagen)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(1.0,1.0,1.0);
	glEnable(GL_TEXTURE_2D);
   
	cargarTextura(imagen);

    glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);

	glBindTexture(GL_TEXTURE_2D, textura_id);

	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices.data());

	glDrawElements( GL_TRIANGLES, triangulos.size()*3, GL_UNSIGNED_INT, triangulos.data() ) ;

   	glDisableClientState(GL_VERTEX_ARRAY);
    glEnableClientState (GL_TEXTURE_COORD_ARRAY_EXT);
   
	glDisable(GL_TEXTURE_2D);

	glutSwapBuffers();
	glutMainLoop();
	glDeleteTextures(1, &textura_id);

}

void ObjMallaIndexada::draw_seleccion(int r, int g, int b)
{
  	// habilitar uso de un array de vértices
	glEnableClientState( GL_VERTEX_ARRAY );
	// indicar el formato y la dirección de memoria del array de vértices
	// (son tuplas de 3 valores float, sin espacio entre ellas)
	glVertexPointer( 3, GL_FLOAT, 0, vertices.data() ) ;
	// visualizar, indicando: tipo de primitiva, número de índices,
	// tipo de los índices, y dirección de la tabla de índices
			
	float ra = r/255.0, ga = g/255.0, ba = b/255.0;
	glColor3f(ra,ga,ba);
	glDrawElements( GL_TRIANGLES, triangulos.size()*3, GL_UNSIGNED_INT, triangulos.data() );
	
	// deshabilitar array de vértices
	glDisableClientState( GL_VERTEX_ARRAY );
}




// *****************************************************************************
//
// Clase Cubo (práctica 1)
//
// *****************************************************************************
Cubo::Cubo()
{

   // inicializar la tabla de vértices
   vertices =  {  { -0.5, -0.5, -0.5 }, // 0
                  { -0.5, -0.5, +0.5 }, // 1
                  { -0.5, +0.5, -0.5 }, // 2
                  { -0.5, +0.5, +0.5 }, // 3
                  { +0.5, -0.5, -0.5 }, // 4
                  { +0.5, -0.5, +0.5 }, // 5
                  { +0.5, +0.5, -0.5 }, // 6
                  { +0.5, +0.5, +0.5 }, // 7
                  { -0.5, -0.5, -0.5 }, // 8  -> 0'
                  { -0.5, -0.5, -0.5 }, // 9  -> 0''
                  { -0.5, +0.5, -0.5 }, // 10 -> 2'
                  { -0.5, +0.5, -0.5 }, // 11 -> 2''
                  { +0.5, +0.5, -0.5 }, // 12 -> 6'
                  { +0.5, -0.5, -0.5 }  // 13 -> 4'
               };

   // inicializar la tabla de caras o triángulos:
   // (es importante en cada cara ordenar los vértices en sentido contrario
   //  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

   triangulos = { { 0, 2, 4 }, { 4, 2, 6 },
                  { 1, 5, 3 }, { 3, 5, 7 },
                  { 1, 3, 8 }, { 8, 3, 10 },
                  { 5, 4, 7 }, { 7, 4, 6 },
                  { 1, 9, 5 }, { 5, 9, 13 },
                  { 3, 7, 11 },{ 11, 7, 12 }
                } ;

   texVertices = {  { 1.0, 0.5 }, 	//0
   					{ 0.25, 0.5 }, 	//1
   					{ 1.0, 0.25 }, 	//2
   					{ 0.25, 0.25 }, //3
   					{ 0.75, 0.5 }, 	//4
   					{ 0.5, 0.5 }, 	//5
   					{ 0.75, 0.25 }, //6 
   					{ 0.5, 0.25 }, 	//7
   					{ 0, 0.5 },  	//0'
   					{ 0.25, 0.75 }, //0''
   					{ 0, 0.25 },  	//2'
   					{ 0.25, 0 }, 	//2''
   					{ 0.5, 0 },	  	//6'
   					{ 0.5, 0.75 } 	//4'
				 };
}


// *****************************************************************************
//
// Clase Tetraedro (práctica 1)
//
// *****************************************************************************
Tetraedro::Tetraedro()
{
   // inicializar la tabla de vértices
   vertices =  {  { -0.5, -0.5, -0.5 }, // 0
                  { 0, -0.5, +0.5 },	 // 1
                  { +0.5, -0.5, -0.5 }, // 2
                  { 0, +0.5, 0 }		 // 3
               };

   // inicializar la tabla de caras o triángulos:
   // (es importante en cada cara ordenar los vértices en sentido contrario
   //  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

   triangulos = { { 0, 1, 3 }, { 1, 2, 3 },
                  { 2, 0, 3 }, { 0, 2, 1 }
                } ;
}


// *****************************************************************************
//
// Clase ObjPLY (práctica 2)
//
// *****************************************************************************
ObjPLY::ObjPLY( const std::string & nombre_archivo )
{
   // leer la lista de caras y vértices
   ply::read( nombre_archivo, vertices, triangulos );
}


// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************


// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

ObjRevolucion::ObjRevolucion( const std::string & nombre_ply_perfil )
{
	std::vector<Tupla3f> vertices_aux;
	ply::read_vertices( nombre_ply_perfil, vertices_aux );
	
	crearMalla(vertices_aux, 50, true, true);
}

void ObjRevolucion::crearMalla( const std::vector<Tupla3f> & perfil_original, const int num_instancias_perf,
								bool tapa_inferior, bool tapa_superior )
{
	Tupla3f v;
	float ang;

	for (int i=0; i < num_instancias_perf; i++)
	{
		ang = (2*M_PI*i)/num_instancias_perf;
		for (int j=0; j < perfil_original.size(); j++)
		{
			v(X) = perfil_original[j](X) * cos(ang);
			v(Y) = perfil_original[j](Y);
			v(Z) = perfil_original[j](X) * sin(ang);
			vertices.push_back(v);
		}
	}


	int a, b, cont;
	for (int i=0; i < num_instancias_perf; i++)
	{
		for (int j=0; j < perfil_original.size()-1; j++)
		{
			a = (perfil_original.size() * i) + j;
			b = (perfil_original.size() * ((i+1) % num_instancias_perf)) + j;
			triangulos.push_back( Tupla3i(b,a,b+1) );
			triangulos.push_back( Tupla3i(a,a+1,b+1) );
		}
	}


	if (tapa_inferior){
		cont = 0;
		vertices.push_back( Tupla3f(0,perfil_original.front()(Y),0) );
	    for(int i=0; i<num_instancias_perf; i++)
	    {
	        if (i == num_instancias_perf-1)
	            triangulos.push_back( Tupla3i(cont, 0, vertices.size()-1) );
	        else{
	            triangulos.push_back( Tupla3i(cont, cont+perfil_original.size(), vertices.size()-1) );
	            cont += perfil_original.size();
	        }
		}
	}

	if (tapa_superior){
		cont = perfil_original.size()-1;
		vertices.push_back( Tupla3f(0,perfil_original.back()(Y),0) );
	    for (int i=0; i<num_instancias_perf; i++)
	    {
	        if (i == num_instancias_perf-1)
	            triangulos.push_back( Tupla3i(cont, vertices.size()-1, perfil_original.size()-1) );
	        else{
	            triangulos.push_back( Tupla3i(cont, vertices.size()-1, cont+perfil_original.size()) );
	            cont += perfil_original.size();
	        }
		}
	}

}

// *****************************************************************************
//
// Clase Cilindro (práctica 2)
//
// *****************************************************************************
Cilindro::Cilindro( const int num_vert_perfil, const int num_instancias_perf )
{
	std::vector<Tupla3f> vertices;
	float div = 0.0;
	for (float i=0; i<num_vert_perfil; i++){
		div = i / (num_vert_perfil-1);
		vertices.push_back( Tupla3f(0.5, div, 0.0) );
	}
	
	crearMalla(vertices, num_instancias_perf, true, true);
}

// *****************************************************************************
//
// Clase Cono (práctica 2)
//
// *****************************************************************************
Cono::Cono( const int num_vert_perfil, const int num_instancias_perf )
{
	std::vector<Tupla3f> vertices;
	float div = 0.0;
	for (float i=0; i<num_vert_perfil; i++){
		div = i / (num_vert_perfil-1);
		vertices.push_back( Tupla3f(1-div, div, 0.0) );
	}
	
	crearMalla(vertices, num_instancias_perf, true, false);
}

// *****************************************************************************
//
// Clase Esfera (práctica 2)
//
// *****************************************************************************
Esfera::Esfera( const int num_vert_perfil, const int num_instancias_perf )
{
	std::vector<Tupla3f> vertices;
	float div = 0.0, ang = 0.0;
	for (float i=0; i<=num_vert_perfil; i++){
		ang = i*(180/num_vert_perfil)+270;
		vertices.push_back( Tupla3f( cos(ang*M_PI/180), sin(ang*M_PI/180), 0.0) );
	}
	
	crearMalla(vertices, num_instancias_perf, false, false);
	
	for(float i=0; i<=num_instancias_perf; i++){
        float x = 1-(i/num_instancias_perf);

        for(float j=0; j<=num_vert_perfil; j++){
            float y = 1-(j/num_vert_perfil);
            texVertices.push_back(Tupla2f(x, y));
        }
    }

}


// *****************************************************************************
//
// Clase Semiesfera
//
// *****************************************************************************
Semiesfera::Semiesfera( const int num_vert_perfil, const int num_instancias_perf )
{
	std::vector<Tupla3f> vertices;
	float div = 0.0, ang = 0.0;
	for (float i=num_vert_perfil/2; i<=num_vert_perfil; i++){
		ang = i*(180/num_vert_perfil)+270;
		vertices.push_back( Tupla3f( cos(ang*M_PI/180), sin(ang*M_PI/180), 0.0) );
	}
	
	crearMalla(vertices, num_instancias_perf, true, false);
}



// *****************************************************************************
//
// Clase Cuadro
//
// *****************************************************************************
Cuadro::Cuadro()
{
	// inicializar la tabla de vértices
	vertices =  { { -1.0, 0.0, +0.5 },	// 0
                  { -1.0, 0.0, -0.5 },	// 1
                  { +1.0, 0.0, +0.5 },	// 2
                  { +1.0, 0.0, -0.5 }	// 3
               };

	// inicializar la tabla de caras o triángulos:
	// (es importante en cada cara ordenar los vértices en sentido contrario
	//  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

	triangulos = { { 0, 2, 1 }, { 1, 2, 3 } };

	texVertices = { { 0.0, 0.0 },
					{ 0.0, 1.0 },
					{ 1.0, 0.0 },
					{ 1.0, 1.0 }
				 };
}



// *****************************************************************************
//
// PRACTICA 5 -- Clase Piramide
//
// *****************************************************************************
Piramide::Piramide()
{
	// inicializar la tabla de vértices
	vertices =  { { -0.5, 0.0, +0.5 },	// 0
                  { -0.5, 0.0, -0.5 },	// 1
                  { +0.5, 0.0, +0.5 },	// 2
                  { +0.5, 0.0, -0.5 },	// 3
                  { +0.0, 0.8, +0.0 }	// 4
               };

	// inicializar la tabla de caras o triángulos:
	// (es importante en cada cara ordenar los vértices en sentido contrario
	//  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

	triangulos = { { 1, 0, 4 }, { 0, 2, 4 },
				   { 2, 3, 4 }, { 3, 1, 4 },
				   { 0, 1, 3 }, { 0, 3, 2 }
				 };
}

void Piramide::draw_piramides(int modo_actual, bool modo_dibujado, int valor, string imagen,
							  int r, int g, int b, bool obj_sel[])
{
  	glPushMatrix();
		glTranslatef(0.0,-0.4,0.0);

		glPushMatrix(); 
		glTranslatef(0.0,0.8,0.0);
		if (obj_sel[0])
			this->draw_seleccion(r,g,b) ;
		else{
			glColor3f(0.0,0.0,0.0);
			this->draw(modo_actual, modo_dibujado, valor, imagen);
		}
		glPopMatrix();

		glPushMatrix(); 
		glTranslatef(-0.5,0.0,0.5);
		if (obj_sel[1])
			this->draw_seleccion(r,g,b) ;
		else{
			glColor3f(0.0,0.0,0.0);
			this->draw(modo_actual, modo_dibujado, valor, imagen);
		}
		glPopMatrix();

		glPushMatrix(); 
		glTranslatef(0.5,0.0,0.5);
		if (obj_sel[2])
			this->draw_seleccion(r,g,b) ;
		else{
			glColor3f(0.0,0.0,0.0);
			this->draw(modo_actual, modo_dibujado, valor, imagen);
		}
		glPopMatrix();

		glPushMatrix(); 
		glTranslatef(-0.5,0.0,-0.5);
		if (obj_sel[3])
			this->draw_seleccion(r,g,b) ;
		else{
			glColor3f(0.0,0.0,0.0);
			this->draw(modo_actual, modo_dibujado, valor, imagen);
		}
		glPopMatrix(); 

		glPushMatrix(); 
		glTranslatef(0.5,0.0,-0.5);
		if (obj_sel[4])
			this->draw_seleccion(r,g,b) ;
		else{
			glColor3f(0.0,0.0,0.0);
			this->draw(modo_actual, modo_dibujado, valor, imagen);
		}
		glPopMatrix();
	glPopMatrix();
}

void Piramide::draw_piramides_seleccion()
{
  	glPushMatrix();
		glTranslatef(0.0,-0.4,0.0);

		glPushMatrix(); 
		glTranslatef(0.0,0.8,0.0);
		this->draw_seleccion(50, 50, 50);
		glPopMatrix();

		glPushMatrix(); 
		glTranslatef(-0.5,0.0,0.5);
		this->draw_seleccion(50+1, 50+1, 50+1);
		glPopMatrix();

		glPushMatrix(); 
		glTranslatef(0.5,0.0,0.5);
		this->draw_seleccion(50+2, 50+2, 50+2);
		glPopMatrix();

		glPushMatrix(); 
		glTranslatef(-0.5,0.0,-0.5);
		this->draw_seleccion(50+3, 50+3, 50+3);
		glPopMatrix(); 

		glPushMatrix(); 
		glTranslatef(0.5,0.0,-0.5);
		this->draw_seleccion(50+4, 50+4, 50+4);
		glPopMatrix(); 

	glPopMatrix();
}






// *****************************************************************************
//
// Métodos auxiliares (utilizados por alguna de las funciones anteriores)
//
// *****************************************************************************


void ObjMallaIndexada::calcular_normales()
{
	Tupla3f a, b;

	for (int x=0; x < vertices.size(); x++)
		normales_vertices.push_back( Tupla3f(0.0,0.0,0.0) );
	
	for (int i = 0; i < triangulos.size(); i++){
		a = vertices[triangulos[i](1)] - vertices[triangulos[i](0)];
		b = vertices[triangulos[i](2)] - vertices[triangulos[i](1)];

		normales_caras.push_back( ( a.cross(b) ) );

		normales_vertices[triangulos[i](0)] = normales_vertices[triangulos[i](0)] + normales_caras[i];
		normales_vertices[triangulos[i](1)] = normales_vertices[triangulos[i](1)] + normales_caras[i];
		normales_vertices[triangulos[i](2)] = normales_vertices[triangulos[i](2)] + normales_caras[i];
	}

}


void ObjMallaIndexada::cargarTextura(string imagen)
{
   std::vector<unsigned char> data;

   CImg<unsigned char> logo;
   logo.load(imagen.c_str());

   // empaquetamos bien los datos
   for (long y = 0; y < logo.height(); y ++)
      for (long x = 0; x < logo.width(); x ++)
      {
      	unsigned char *r = logo.data(x, y, 0, 0);
      	unsigned char *g = logo.data(x, y, 0, 1);
      	unsigned char *b = logo.data(x, y, 0, 2);
      	data.push_back(*r);
      	data.push_back(*g);
      	data.push_back(*b);
      }

   glGenTextures(1, &textura_id);
   glBindTexture(GL_TEXTURE_2D, textura_id);

   glActiveTexture(GL_TEXTURE0);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, logo.width(), logo.height(),
				0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);

   glBindTexture(GL_TEXTURE_2D, 0);
}
