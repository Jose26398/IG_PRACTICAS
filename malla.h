// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: ObjMallaIndexada.h
// -- declaraciones de clase ObjMallaIndexada (mallas indexadas) y derivados
//
// #############################################################################

#ifndef OBJETOS_H_INCLUDED
#define OBJETOS_H_INCLUDED

#include "aux.h"
#include "CImg.h"

using namespace std;

// *****************************************************************************
//
// clase para objetos 3D (mallas indexadas)
//
// *****************************************************************************

class ObjMallaIndexada
{
   public:

   // dibuja el objeto en modo inmediato
   void draw_ModoInmediato(int modo_actual, string imagen);

   // dibuja el objeto en modo diferido (usando VBOs)
   void draw_ModoDiferido(int modo_actual, string imagen);

   // función que redibuja el objeto
   // está función llama a 'draw_MI' (modo inmediato)
   // o bien a 'draw_MD' (modo diferido, VBOs)
   void draw(int modo_actual, bool modo_dibujado, int valor, string imagen) ;

   void draw_iluminacion_plana(int valor, string imagen);
   void draw_iluminacion_suave(int valor, string imagen);

   void draw_texturas(string imagen);
   void cargarTextura(string imagen);

   void draw_seleccion(int r, int g, int b);
	
   GLuint textura_id;
   std::vector<Tupla2f> texVertices;

   protected:

   void calcular_normales() ; // calcula tabla de normales de vértices (práctica 3)

   std::vector<Tupla3f> vertices ;   // tabla de coordenadas de vértices (una tupla por vértice, con tres floats)
   std::vector<Tupla3i> triangulos ; // una terna de 3 enteros por cada cara o triángulo
   GLuint id_vbo_ver=0, id_vbo_tri=0;

   std::vector<Tupla3f> normales_vertices;
   std::vector<Tupla3f> normales_caras;
} ;

// *****************************************************************************
//
// clases derivadas de ObjMallaIndexada (definen constructores específicos)
//
// *****************************************************************************

// *****************************************************************************
// Cubo con centro en el origen y lado unidad
// (tiene 9 vertices y 6 caras)

class Cubo : public ObjMallaIndexada
{
   public:
   Cubo() ;

} ;

// *****************************************************************************
// Tetraedo con centro en el origen y lado unidad
// (tiene 4 vertices y 4 caras)

class Tetraedro : public ObjMallaIndexada
{
   public:
   Tetraedro() ;

} ;

// *****************************************************************************
// objeto leído de un archivo PLY

class ObjPLY : public ObjMallaIndexada
{
   public:
      ObjPLY( const std::string & nombre_archivo );

} ;

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)
class ObjRevolucion : public ObjMallaIndexada
{
   public:
      ObjRevolucion( const std::string & nombre_ply_perfil );
      ObjRevolucion(){}

   protected:
      void crearMalla( const std::vector<Tupla3f> & perfil_original, const int num_instancias_perf,
                       bool tapa_inferior, bool tapa_superior ) ;
} ;

// *****************************************************************************
// Cilindro
class Cilindro : public ObjRevolucion
{
   public:
   Cilindro( const int num_vert_perfil, const int num_instancias_perf );
} ;

// *****************************************************************************
// Cono
class Cono : public ObjRevolucion
{
   public:
   Cono( const int num_vert_perfil, const int num_instancias_perf );
} ;

// *****************************************************************************
// Esfera
class Esfera : public ObjRevolucion
{
   public:
   Esfera( const int num_vert_perfil, const int num_instancias_perf );
} ;

// *****************************************************************************
// Semiesfera
class Semiesfera : public ObjRevolucion
{
   public:
   Semiesfera( const int num_vert_perfil, const int num_instancias_perf );
} ;

// *****************************************************************************
// Cuadro
class Cuadro : public ObjMallaIndexada
{
	public:
	Cuadro();
} ;

// *****************************************************************************
// Piramide
class Piramide : public ObjMallaIndexada
{
   public:
   Piramide();
   void draw_piramides(int modo_actual, bool modo_dibujado, int valor, string imagen, int r, int g, int b, bool obj_sel[]);
   void draw_piramides_seleccion();
} ;


#endif
