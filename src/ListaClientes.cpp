#include "ListaClientes.h"
#include <iostream>

using namespace std;

ListaClientes::ListaClientes() {
    primero = nullptr;
}

ListaClientes::~ListaClientes() {
    Cliente* actual = primero;
    while (actual != nullptr) {
        Cliente* siguiente = actual->getSig(); // se guarda antes de borrar
        delete actual;
        actual = siguiente;
    }
}

bool ListaClientes::insertarCliente(string nombre, int tokens) {
    if (nombre.empty()){
        cout << "Nombre vacio" << endl;
        return false;
    }

    if (tokens < 0){
        cout << nombre << " tiene tokens negativos" << endl;
        return false;
    }

    if (buscarCliente(nombre) != nullptr){
        cout << nombre << ": Cliente repetido" << endl;
        return false;
    }

    Cliente* nuevo = new Cliente(nombre, tokens);

    if(primero == nullptr){
        primero = nuevo;
    }
    else{
        Cliente* actual = primero;
        while (actual->getSig() != nullptr) {
            actual = actual->getSig();
        }
        actual->setSig(nuevo);
    }
    return true;
}

bool ListaClientes::agregarSubestacionACliente(string nombreCliente, string nombreSubestacion) {
    Cliente* cliente = buscarCliente(nombreCliente);

    if (cliente == nullptr){
        cout << "No existe el cliente con nombre: " << nombreCliente << endl;
        return false;
    }

    return cliente->agregarSubestacion(nombreSubestacion);
}

bool ListaClientes::agregarBeneficioACliente(string nombreCliente, string nombreBeneficio) {
    Cliente* cliente = buscarCliente(nombreCliente);

    if (cliente == nullptr){
        cout << "No existe el cliente con nombre: " << nombreCliente << endl;
        return false;
    }

    return cliente->agregarBeneficio(nombreBeneficio);
}

Cliente* ListaClientes::getPrimero() {
    return primero;
}

Cliente* ListaClientes::buscarCliente(string nombre) {
    Cliente* actual = primero;
    while (actual != nullptr) {
        if (actual->getNombre() == nombre) {
            return actual;
        }
        actual = actual->getSig();
    }
    return nullptr;
}

bool ListaClientes::eliminarCliente(string nombre) {
    Cliente* anterior = nullptr;
    Cliente* actual = primero;

    while (actual != nullptr) {
        if (actual->getNombre() == nombre) {
            if (anterior == nullptr) {
                primero = actual->getSig();
            } else {
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

bool ListaClientes::asignarTokens(string nombre, int cantidad) {
    Cliente* cliente = buscarCliente(nombre);
    if (cliente == nullptr) {
        cout << "No existe el cliente con nombre: " << nombre << endl;
        return false;
    }

    int nuevoSaldo = cliente->getTokens() + cantidad;
    if (nuevoSaldo < 0) {
        cout << "No se puede asignar tokens. El saldo no puede ser negativo." << endl;
        return false;
    }

    cliente->setTokens(nuevoSaldo);
    return true;
}

void ListaClientes::quitarSubestacionDeTodos(string nombre) {
    Cliente* actual = primero;
    while (actual != nullptr) {
        actual->quitarSubestacion(nombre); // si no la tiene devuelve false y se ignora
        actual = actual->getSig();
    }
}

void ListaClientes::renombrarSubestacionEnTodos(string viejo, string nuevo) {
    Cliente* actual = primero;
    while (actual != nullptr) {
        actual->renombrarSubestacion(viejo, nuevo); // Cliente valida vacio y repetido
        actual = actual->getSig();
    }
}

void ListaClientes::imprimirReporte() {}

bool ListaClientes::canjearBeneficio(string nombreCliente, ListaBeneficios& catalogo) {return false;}