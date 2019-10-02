// #############################################################################
//
// Informática Gráfica (Grado Informática)
//
// Archivo: GrafoParam.h
// -- declaraciones de clase para el objeto jerárquico de la práctica 3
//
// #############################################################################

#ifndef GRAFOPARAM_H_INCLUDED
#define GRAFOPARAM_H_INCLUDED

#include "malla.h" // añadir .h de cualquier objetos malla indexada usados....

constexpr int num_parametros = 8 ; // número de parámetros o grados de libertad
                                   // de este modelo

class GrafoParam
{
   public:

   // crea mallas indexadas (nodos terminales del grafo)
   GrafoParam();

   // función principal de visualización
   void draw( const int p_modo_vis, const bool p_usar_diferido, int p_valor,
              bool p_sel_dibujada, bool p_obj_sel[]); //ModoVis

   // actualizar valor efectivo de un parámetro (a partir de su valor no acotado)
   void actualizarValorEfe( const unsigned iparam, const float valor_na );

   // devuelve el número de parámetros
   unsigned numParametros() { return num_parametros ; }

   private:

   void cuerpo();
   void pierna_arriba();
   void pierna_abajo();
   void brazo();
   void cabeza();


   // objetos tipo malla indexada (nodos terminales)

   Esfera * esfera         = nullptr ;
   Semiesfera * semiesfera = nullptr;
   Cono * cono             = nullptr ;
   Cubo * base             = nullptr;
   Cilindro * cilindro     = nullptr;

   // parámetros de la llamada actual (o última) a 'draw'
   int  modo_vis ;      // modo de visualización //ModoVis
   bool usar_diferido ; // modo de envío (true -> diferido, false -> inmediato)
   int  valor ;
   bool sel_dibujada, obj_sel[255];


   // valores efectivos de los parámetros (angulos, distancias, factores de
   // escala, etc.....) calculados a partir de los valores no acotados

   float altura_base = 0.2,
         angulo_hombro_dch = 90.0, angulo_hombro_izq = -90.0, 
         angulo_pelvis_dch = 70.0, angulo_pelvis_izq = -70.0,
         angulo_rodilla_dch = 45.0, angulo_rodilla_izq = 20.0,
         altura_personaje, angulo_personaje,
         posicion_personaje_x, posicion_personaje_y;

   void seleccionar_modo(int modo_actual);

} ;

#endif
