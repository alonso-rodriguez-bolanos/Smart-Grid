#include "Vertice.h"

Vertice::Vertice(string nombre, string tipo, string region) {
    // pendiente Persona 1
}

string Vertice::getNombre() { return ""; }
string Vertice::getTipo() { return ""; }
string Vertice::getRegion() { return ""; }
bool Vertice::isVisitado() { return false; }
Vertice* Vertice::getSig() { return nullptr; }
Arco* Vertice::getPrimerArco() { return nullptr; }

void Vertice::setNombre(string nombre) {}
void Vertice::setTipo(string tipo) {}
void Vertice::setRegion(string region) {}
void Vertice::setVisitado(bool visitado) {}
void Vertice::setSig(Vertice* sig) {}
void Vertice::setPrimerArco(Arco* arco) {}
