#pragma once
#include <string>
#include "Vertice.h"
#include "Arco.h"
#include "ListaClientes.h"
using namespace std;

// Multilista: lista de vertices y cada uno con su lista de arcos
class Grafo {
private:
    Vertice* primerVertice;

    // Revisa distancia, capacidad y perdida
    bool valoresArcoValidos(double distancia, double capacidad, double perdida);

    // Imprime el vertice y sus conexiones en una linea
    void imprimirConexiones(Vertice* v);

    // Imprime los vertices que quedaron sin visitar despues de un recorrido
    void imprimirNoAlcanzables();

    // Recursion del recorrido en profundidad
    void dfsRecursivo(Vertice* actual);

    // Dice si un arco pasa el filtro de medios
    bool medioPermitido(string medio, bool altaTension, bool subterraneo, bool redLocal);
    // Cuantos vertices hay para saber el tamano del arreglo de la ruta
    int contarVertices();
    // Backtracking: avanza nodo por nodo y al llegar al destino
    // imprime la ruta o si puntuar es true guarda la de mayor puntaje
    void buscarRutas(Vertice* inicio, Vertice* actual, Vertice* destino, bool altaTension, bool subterraneo, bool redLocal, Arco** ruta, int largo, int& cantidad, bool puntuar, Arco** mejorRuta, int& mejorLargo, double& mejorPuntaje);
    // Puntaje Eco-Tokens de una linea
    double puntajeArco(Arco* arco);
    // Imprime una ruta ya encontrada
    void imprimirRuta(Vertice* inicio, Arco** ruta, int largo, int numero);
    // ayuda en el algoritmo de Dijkstra para encontrar la posicion de un vertice en el arreglo
    int posicionDe(Vertice** vertices, int n, Vertice* buscado);

public:
    Grafo();
    ~Grafo(); // libera todos los vertices y arcos

    // No se permite copiar la estructura: siempre se pasa con &
    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    // Si rechazan un dato imprimen el motivo y devuelven false
    bool insertarVertice(string nombre, string tipo, string region);
    bool insertarArco(string origen, string destino, string medio, double distancia, double capacidad, double perdida, int usos);
    bool existeVertice(string nombre); // la carga lo revisa ANTES de agregarSubestacionACliente

    // Para guardar
    Vertice* getPrimerVertice();


    Vertice* buscarVertice(string nombre);
    bool modificarVertice(string nombreActual, string nuevoNombre, string nuevoTipo, string nuevaRegion);
    bool eliminarVertice(string nombre);
    bool modificarArco(string origen, string destino, string medio, double distancia, double capacidad, double perdida);
    bool eliminarArco(string origen, string destino, string medio);

    void reiniciarVisitados();
    void imprimirBFS(string inicio);
    void imprimirDFS(string inicio);
    void imprimirTodasLasRutas(string origen, string destino, bool altaTension, bool subterraneo, bool redLocal);
    void imprimirRutaMenorPerdida(string origen, string destino);

    // Ruta de mayor Eco-Tokens
    void consultarRutaMayorTokens(string origen, string destino, Cliente* cliente, bool horaNoPico);
    void imprimirRutaFrecuenteYUnica();
    void imprimirNodosInactivos(ListaClientes& clientes);
};