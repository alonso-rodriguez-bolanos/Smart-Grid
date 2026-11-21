#include "NodoTexto.h"

NodoTexto::NodoTexto(string nombre) {
    this->nombre = nombre;
    this->sig = nullptr; // al crearse no apunta a nadie
}

string NodoTexto::getNombre() {
    return nombre;
}

NodoTexto* NodoTexto::getSig() {
    return sig;
}

void NodoTexto::setNombre(string nombre) {
    this->nombre = nombre;
}

void NodoTexto::setSig(NodoTexto* sig) {
    this->sig = sig;
}