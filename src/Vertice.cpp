#include "Vertice.h"

Vertice::Vertice(string nombre, string tipo, string region) {
    this->nombre = nombre;
    this->tipo = tipo;
    this->region = region;
    this->visitado = false;
    this->sigVertice = nullptr;
    this->primerArco = nullptr;
}

string Vertice::getNombre() {
    return nombre;
}

string Vertice::getTipo() {
    return tipo;
}

string Vertice::getRegion() {
    return region;
}

bool Vertice::isVisitado() {
    return visitado;
}

Vertice* Vertice::getSig() {
    return sigVertice;
}

Arco* Vertice::getPrimerArco() {
    return primerArco;
}

void Vertice::setNombre(string nombre) {
    this->nombre = nombre;
}

void Vertice::setTipo(string tipo) {
    this->tipo = tipo;
}

void Vertice::setRegion(string region) {
    this->region = region;
}

void Vertice::setVisitado(bool visitado) {
    this->visitado = visitado;
}

void Vertice::setSig(Vertice* sig) {
    this->sigVertice = sig;
}

void Vertice::setPrimerArco(Arco* arco) {
    this->primerArco = arco;
}