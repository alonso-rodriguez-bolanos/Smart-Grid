#include "Arco.h"

Arco::Arco(Vertice* destino, string medio, double distancia, double capacidad, double perdida, int usos) {
    // pendiente Persona 1
}

Vertice* Arco::getDestino() { return nullptr; }
string Arco::getMedio() { return ""; }
double Arco::getDistancia() { return 0; }
double Arco::getCapacidad() { return 0; }
double Arco::getPerdida() { return 0; }
int Arco::getUsos() { return 0; }
Arco* Arco::getSig() { return nullptr; }

void Arco::setDestino(Vertice* destino) {}
void Arco::setMedio(string medio) {}
void Arco::setDistancia(double distancia) {}
void Arco::setCapacidad(double capacidad) {}
void Arco::setPerdida(double perdida) {}
void Arco::setUsos(int usos) {}
void Arco::setSig(Arco* sig) {}
