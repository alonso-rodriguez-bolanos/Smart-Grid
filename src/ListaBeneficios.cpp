#include "ListaBeneficios.h"

ListaBeneficios::ListaBeneficios() {
    primero = nullptr;
}

ListaBeneficios::~ListaBeneficios() {
    // pendiente Persona 1
}

bool ListaBeneficios::insertarBeneficio(string nombre, int costo) {
    return true; // pendiente
}

Beneficio* ListaBeneficios::getPrimero() {
    return primero;
}

Beneficio* ListaBeneficios::buscarBeneficio(string nombre) {
    return nullptr; // pendiente
}

bool ListaBeneficios::modificarBeneficio(string nombreActual, string nuevoNombre, int nuevoCosto) {
    return false; // pendiente
}

bool ListaBeneficios::eliminarBeneficio(string nombre) {
    return false; // pendiente
}

void ListaBeneficios::imprimirRecursivo() {}
void ListaBeneficios::imprimirDesde(Beneficio* actual) {}
