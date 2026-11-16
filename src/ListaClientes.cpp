#include "ListaClientes.h"

ListaClientes::ListaClientes() {
    primero = nullptr;
}

ListaClientes::~ListaClientes() {
    // pendiente Persona 1
}

bool ListaClientes::insertarCliente(string nombre, int tokens) {
    return true; // pendiente
}

bool ListaClientes::agregarSubestacionACliente(string nombreCliente, string nombreSubestacion) {
    return true; // pendiente
}

bool ListaClientes::agregarBeneficioACliente(string nombreCliente, string nombreBeneficio) {
    return true; // pendiente
}

Cliente* ListaClientes::getPrimero() {
    return primero;
}

Cliente* ListaClientes::buscarCliente(string nombre) {
    return nullptr; // pendiente
}

bool ListaClientes::eliminarCliente(string nombre) {
    return false; // pendiente
}

bool ListaClientes::asignarTokens(string nombre, int cantidad) {
    return false; // pendiente
}

void ListaClientes::quitarSubestacionDeTodos(string nombre) {}
void ListaClientes::renombrarSubestacionEnTodos(string viejo, string nuevo) {}
void ListaClientes::imprimirReporte() {}

bool ListaClientes::canjearBeneficio(string nombreCliente, ListaBeneficios& catalogo) {
    return false; // pendiente
}
