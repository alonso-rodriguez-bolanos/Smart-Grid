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
    void imprimirConexiones(Vertice* v);

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
    void imprimirTodasLasRutas(string origen, string destino,
                               bool altaTension, bool subterraneo, bool redLocal);
    void imprimirRutaMenorPerdida(string origen, string destino);

    // Ruta de mayor Eco-Tokens
    void consultarRutaMayorTokens(string origen, string destino, Cliente* cliente, bool horaNoPico);
    void imprimirRutaFrecuenteYUnica();
    void imprimirNodosInactivos(ListaClientes& clientes);
};