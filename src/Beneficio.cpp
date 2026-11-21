#include "Beneficio.h"

Beneficio::Beneficio(string nombre, int costo) {
    this->nombre = nombre;
    this->costo = costo;
}

string Beneficio::getNombre() {
    return nombre;
}

int Beneficio::getCosto() {
    return costo;
}

Beneficio* Beneficio::getSig() {
    return sig;
}

void Beneficio::setNombre(string nombre) {
    this->nombre = nombre;
}

void Beneficio::setCosto(int costo) {
    this->costo = costo;
}

void Beneficio::setSig(Beneficio* sig) {
    this->sig = sig;
}
