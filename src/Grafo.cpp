#include "Grafo.h"

Grafo::Grafo() {
    primerVertice = nullptr;
}

Grafo::~Grafo() {
    // pendiente Persona 1
}

bool Grafo::insertarVertice(string nombre, string tipo, string region) {
    return true; // pendiente
}

bool Grafo::insertarArco(string origen, string destino, string medio,
                         double distancia, double capacidad, double perdida, int usos) {
    return true; // pendiente
}

bool Grafo::existeVertice(string nombre) {
    return true; // pendiente
}

Vertice* Grafo::getPrimerVertice() {
    return primerVertice;
}

Vertice* Grafo::buscarVertice(string nombre) {
    return nullptr; // pendiente
}

bool Grafo::modificarVertice(string nombreActual, string nuevoNombre, string nuevoTipo, string nuevaRegion) {
    return false; // pendiente
}

bool Grafo::eliminarVertice(string nombre) {
    return false; // pendiente
}

bool Grafo::modificarArco(string origen, string destino, string medio,
                          double distancia, double capacidad, double perdida) {
    return false; // pendiente
}

bool Grafo::eliminarArco(string origen, string destino, string medio) {
    return false; // pendiente
}

void Grafo::reiniciarVisitados() {}
void Grafo::imprimirBFS(string inicio) {}
void Grafo::imprimirDFS(string inicio) {}
void Grafo::imprimirTodasLasRutas(string origen, string destino,
                                  bool altaTension, bool subterraneo, bool redLocal) {}
void Grafo::imprimirRutaMenorPerdida(string origen, string destino) {}
void Grafo::consultarRutaMayorTokens(string origen, string destino, Cliente* cliente, bool horaNoPico) {}
void Grafo::imprimirRutaFrecuenteYUnica() {}
void Grafo::imprimirNodosInactivos(ListaClientes& clientes) {}
