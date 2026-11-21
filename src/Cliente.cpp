#include "Cliente.h"
#include <iostream>
using namespace std;

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

bool Cliente::agregarSubestacion(string nombre) {
    if (nombre.empty()) {
        cout << "En el cliente: " << this->nombre << ". La subestacion esta vacio" << endl;
        return false; // nombre invalido
    }
    if (tieneSubestacion(nombre)) {
        cout << "En el cliente: " << this->nombre << ". Ya existe la subestacion: " << nombre << endl;
        return false; // no permite repetidos
    }

    NodoTexto* nuevaSubestacion = new NodoTexto(nombre);
    NodoTexto* actual = primeraSubestacion;

    if (actual == nullptr) {
        primeraSubestacion = nuevaSubestacion; // lista vacia
    } 
    else {
        while (actual->getSig() != nullptr) {
            actual = actual->getSig();
        }
        actual->setSig(nuevaSubestacion);
    }
    return true;
}

bool Cliente::tieneSubestacion(string nombre) {
    NodoTexto* actual = primeraSubestacion;
    while (actual != nullptr) {
        if (actual->getNombre() == nombre) {
            return true; // ya existe
        }
        actual = actual->getSig();
    }
    return false; // no existe
}

bool Cliente::quitarSubestacion(string nombre) {
    if (nombre.empty()) {
        return false; // nombre invalido
    }

    NodoTexto* anterior = nullptr;
    NodoTexto* actual = primeraSubestacion;

    while (actual != nullptr) {
        if (actual->getNombre() == nombre){
            if (anterior == nullptr){
                primeraSubestacion = actual->getSig();
            }
            else{
                anterior->setSig(actual->getSig());
            }
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->getSig();
    }
    return false;
}

bool Cliente::renombrarSubestacion(string viejo, string nuevo) {
    if (viejo.empty() || nuevo.empty()) {
        return false;
    }

    if (tieneSubestacion(nuevo)){
        return false;
    }

    NodoTexto* nodo = primeraSubestacion;

    while(nodo != nullptr){
        if (nodo->getNombre() == viejo){
            nodo->setNombre(nuevo);
            return true;
        }
        nodo = nodo->getSig();
    }
    return false;
}

bool Cliente::agregarBeneficio(string nombre) {
    if (nombre.empty()) {
        cout << "En el cliente: " << this->nombre << ". El beneficio esta vacio" << endl;
        return false; // nombre invalido
    }
    if (tieneBeneficio(nombre)) {
        cout << "En el cliente: " << this->nombre << ". Ya existe el beneficio: " << nombre << endl;
        return false; // no permite repetidos
    }

    NodoTexto* nuevoBeneficio = new NodoTexto(nombre);
    NodoTexto* actual = primerBeneficio;

    if (actual == nullptr) {
        primerBeneficio = nuevoBeneficio; // lista vacia
    } 
    else {
        while (actual->getSig() != nullptr) {
            actual = actual->getSig();
        }
        actual->setSig(nuevoBeneficio);
    }
    return true;
}

bool Cliente::tieneBeneficio(string nombre) {
    NodoTexto* actual = primerBeneficio;
    while (actual != nullptr) {
        if (actual->getNombre() == nombre) {
            return true; // ya existe
        }
        actual = actual->getSig();
    }
    return false; // no existe
}