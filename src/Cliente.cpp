#include "Cliente.h"

Cliente::Cliente(string nombre, int tokens) {
    this->nombre = nombre;
    this->tokens = tokens; // la validacion va en insertarCliente
    this->primeraSubestacion = nullptr; // sublistas vacias
    this->primerBeneficio = nullptr;
    this->sig = nullptr;
}

Cliente::~Cliente() {
    // libera la sublista de subestaciones
    NodoTexto* actual = primeraSubestacion;
    while (actual != nullptr) {
        NodoTexto* siguiente = actual->getSig(); // se guarda antes de borrar
        delete actual;
        actual = siguiente;
    }

    // libera la sublista de beneficios
    actual = primerBeneficio;
    while (actual != nullptr) {
        NodoTexto* siguiente = actual->getSig();
        delete actual;
        actual = siguiente;
    }
}

string Cliente::getNombre() {
    return nombre;
}

int Cliente::getTokens() {
    return tokens;
}

NodoTexto* Cliente::getPrimeraSubestacion() {
    return primeraSubestacion;
}

NodoTexto* Cliente::getPrimerBeneficio() {
    return primerBeneficio;
}

Cliente* Cliente::getSig() {
    return sig;
}

void Cliente::setNombre(string nombre) {
    this->nombre = nombre;
}

void Cliente::setTokens(int tokens) {
    this->tokens = tokens;
}

void Cliente::setSig(Cliente* sig) {
    this->sig = sig;
}

bool Cliente::agregarSubestacion(string nombre) { return true; }
bool Cliente::tieneSubestacion(string nombre) { return false; }
bool Cliente::quitarSubestacion(string nombre) { return false; }
bool Cliente::renombrarSubestacion(string viejo, string nuevo) { return false; }

bool Cliente::agregarBeneficio(string nombre) { return true; }
bool Cliente::tieneBeneficio(string nombre) { return false; }