#include "Arco.h"

Arco::Arco(Vertice* destino, string medio, double distancia, double capacidad, double perdida, int usos) {
    this->destino = destino;
    this->medio = medio;
    this->distancia = distancia;
    this->capacidad = capacidad;
    this->perdida = perdida;
    this->usos = usos;
    this->sigArco = nullptr;
}

Vertice* Arco::getDestino() { 
    return destino; 
}
string Arco::getMedio() { 
    return medio; 
}
double Arco::getDistancia() { 
    return distancia; 
}
double Arco::getCapacidad() { 
    return capacidad; 
}
double Arco::getPerdida() { 
    return perdida; 
}
int Arco::getUsos() { 
    return usos; 
}
Arco* Arco::getSig() { 
    return sigArco; 
}

void Arco::setDestino(Vertice* destino) {
    this->destino = destino;
}
void Arco::setMedio(string medio) {
    this->medio = medio;
}
void Arco::setDistancia(double distancia) {
    this->distancia = distancia;
}
void Arco::setCapacidad(double capacidad) {
    this->capacidad = capacidad;
}
void Arco::setPerdida(double perdida) {
    this->perdida = perdida;
}
void Arco::setUsos(int usos) {
    this->usos = usos;
}
void Arco::setSig(Arco* sig) {
    this->sigArco = sig;
}
