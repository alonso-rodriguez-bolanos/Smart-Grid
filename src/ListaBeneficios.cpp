#include "ListaBeneficios.h"
#include <iostream>
using namespace std;

ListaBeneficios::ListaBeneficios() {
    primero = nullptr;
}

ListaBeneficios::~ListaBeneficios() {
    Beneficio* actual = primero;
    while (actual != nullptr) {
        Beneficio* siguiente = actual->getSig(); // se guarda antes de borrar
        delete actual;
        actual = siguiente;
    }
}

bool ListaBeneficios::insertarBeneficio(string nombre, int costo)  {  
    if (nombre.empty()){
        cout << "Nombre vacio" << endl;
        return false;
    }

    if (costo <= 0){ 
        cout << nombre << " debe tener un costo mayor que 0" << endl;
        return false;
    }

    if (buscarBeneficio(nombre) != nullptr){
        cout << nombre << ": Beneficio repetido" << endl;
        return false;
    }

    Beneficio* nuevo = new Beneficio(nombre, costo);

    if(primero == nullptr){
        primero = nuevo;
    }
    else{
        Beneficio* actual = primero;
        while (actual->getSig() != nullptr) {
            actual = actual->getSig();
        }
        actual->setSig(nuevo);
    }
    return true;
}

Beneficio* ListaBeneficios::getPrimero() {
    return primero;
}

Beneficio* ListaBeneficios::buscarBeneficio(string nombre) {
    Beneficio* actual = primero;
    while (actual != nullptr) {
        if (actual->getNombre() == nombre) {
            return actual;
        }
        actual = actual->getSig();
    }
    return nullptr;
}

bool ListaBeneficios::modificarBeneficio(string nombreActual, string nuevoNombre, int nuevoCosto) {
    Beneficio* beneficio = buscarBeneficio(nombreActual);
    if (beneficio == nullptr) {
        cout << nombreActual << ": Beneficio no encontrado" << endl;
        return false;
    }

    if (nuevoNombre.empty()) {
        cout << "Nombre vacio" << endl;
        return false;
    }

    if (nuevoCosto <= 0) {
        cout << nuevoNombre << " debe tener un costo mayor que 0" << endl;
        return false;
    }

    if (buscarBeneficio(nuevoNombre) != nullptr && nuevoNombre != nombreActual) {
        cout << nuevoNombre << ": Beneficio repetido" << endl;
        return false;
    }

    beneficio->setNombre(nuevoNombre);
    beneficio->setCosto(nuevoCosto);
    return true;
}

bool ListaBeneficios::eliminarBeneficio(string nombre) {
    Beneficio* beneficio = buscarBeneficio(nombre);
    if (beneficio == nullptr) {
        cout << nombre << ": Beneficio no encontrado" << endl;
        return false;
    }

    if (primero == beneficio) {
        primero = beneficio->getSig();
    } 
    else {
        Beneficio* actual = primero;
        while (actual->getSig() != beneficio) {
            actual = actual->getSig();
        }
        actual->setSig(beneficio->getSig());
    }

    delete beneficio;
    return true;
}

void ListaBeneficios::imprimirRecursivo() {
    if (primero == nullptr) {
        cout << "No hay beneficios en el catalogo" << endl;
        return;
    }
    cout << "===== Catalogo de beneficios =====" << endl;
    imprimirDesde(primero); // arranca la recursion desde el primero
}

void ListaBeneficios::imprimirDesde(Beneficio* actual) {
    if (actual == nullptr) { // se acabo la lista
        return;
    }
    cout << "- " << actual->getNombre() << " | costo: " << actual->getCosto() << " tokens." << endl;
    imprimirDesde(actual->getSig()); // recursividad
}