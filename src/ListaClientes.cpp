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

void ListaClientes::imprimirTextos(NodoTexto* actual) {
    if (actual == nullptr) { // lista vacia
        cout << "sin registros" << endl;
        return;
    }

    bool esPrimero = true;
    while (actual != nullptr) {
        if (!esPrimero) {
            cout << ", "; // coma antes de todos menos el primero
        }
        cout << actual->getNombre();
        esPrimero = false;
        actual = actual->getSig(); // avanza al siguiente nodo
    }
    cout << endl;
}

void ListaClientes::imprimirReporte() {
    cout << "========== Reporte de clientes ==========" << endl;
    int impresos = 0;
    Cliente* actual = primero;

    while (actual != nullptr) {
        if (actual->getTokens() != 0 || actual->getPrimeraSubestacion() != nullptr || actual->getPrimerBeneficio() != nullptr) { // tiene historial
            cout << actual->getNombre() << endl;
            cout << "  Tokens: " << actual->getTokens() << endl;
            cout << "  Subestaciones: ";
            imprimirTextos(actual->getPrimeraSubestacion());
            cout << "  Beneficios: ";
            imprimirTextos(actual->getPrimerBeneficio());
            cout << endl; // linea en blanco entre clientes
            impresos++;
        }
        actual = actual->getSig();
    }

    if (impresos == 0) {
        cout << "No hay clientes con historial" << endl;
    } 
    else {
        cout << "Clientes con historial: " << impresos << endl;
    }
}

void ListaClientes::imprimirCliente(Cliente* cliente) {
    cout << cliente->getNombre() << endl;
    cout << "  Tokens: " << cliente->getTokens() << endl;
    cout << "  Subestaciones: ";
    imprimirTextos(cliente->getPrimeraSubestacion());
    cout << "  Beneficios: ";
    imprimirTextos(cliente->getPrimerBeneficio());
}

bool ListaClientes::canjearBeneficio(string nombreCliente, ListaBeneficios& catalogo) {
    Cliente* cliente = buscarCliente(nombreCliente);
    if (cliente == nullptr) {
        cout << "No existe el cliente con nombre: " << nombreCliente << endl;
        return false;
    }

    // Mostrar los beneficios disponibles para el cliente
    cout << "Beneficios disponibles para " << cliente->getNombre() << " (" << cliente->getTokens() << " tokens):" << endl;
    int cantidad = 0;
    Beneficio* beneficio = catalogo.getPrimero();
    while (beneficio != nullptr) {
        if (beneficio->getCosto() <= cliente->getTokens() && !cliente->tieneBeneficio(beneficio->getNombre())) { // le alcanza y no lo tiene
            cantidad++;
            cout << cantidad << ". " << beneficio->getNombre() << " | costo: " << beneficio->getCosto() << " tokens" << endl;
        }
        beneficio = beneficio->getSig();
    }

    if (cantidad == 0) {
        cout << "No hay beneficios disponibles para " << cliente->getNombre() << endl;
        return false;
    }

    // Elegir
    cout << "0. Cancelar" << endl;
    cout << "Seleccione el beneficio a canjear: ";
    int opcion;
    while (!(cin >> opcion) || opcion < 0 || opcion > cantidad) {
        cout << "Valor invalido. Escriba un numero entre 0 y " << cantidad << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore(1000, '\n'); // limpia el Enter

    if (opcion == 0) {
        cout << "Canje cancelado." << endl;
        return false;
    }

    // Llegar al elegido
    int contador = 0;
    beneficio = catalogo.getPrimero();
    while (beneficio != nullptr) {
        if (beneficio->getCosto() <= cliente->getTokens() && !cliente->tieneBeneficio(beneficio->getNombre())) { // mismo filtro
            contador++;
            if (contador == opcion) {
                break; // beneficio queda apuntando al elegido
            }
        }
        beneficio = beneficio->getSig();
    }

    // Aplicar
    if (!cliente->agregarBeneficio(beneficio->getNombre())) {
        return false;
    }
    int antes = cliente->getTokens();
    cliente->setTokens(antes - beneficio->getCosto());
    cout << "Canjeado: " << beneficio->getNombre() << ". Tokens: " << antes << " -> " << cliente->getTokens() << endl;
    return true;
}