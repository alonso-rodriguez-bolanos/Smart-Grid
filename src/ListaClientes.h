#pragma once
#include <string>
#include "Cliente.h"
#include "ListaBeneficios.h"
using namespace std;

class ListaClientes {
private:
    Cliente* primero;

public:
    ListaClientes();
    ~ListaClientes(); // libera todos los clientes

    // No se permite copiar la estructura: siempre se pasa con &
    ListaClientes(const ListaClientes&) = delete;
    ListaClientes& operator=(const ListaClientes&) = delete;

    // Si rechazan un dato imprimen el motivo y devuelven false
    bool insertarCliente(string nombre, int tokens);
    bool agregarSubestacionACliente(string nombreCliente, string nombreSubestacion);
    bool agregarBeneficioACliente(string nombreCliente, string nombreBeneficio);

    Cliente* getPrimero();

    Cliente* buscarCliente(string nombre);
    bool eliminarCliente(string nombre);
    bool asignarTokens(string nombre, int cantidad);   // puede ser negativa pero el saldo no baja de 0

    void quitarSubestacionDeTodos(string nombre);      // al borrar un vertice
    void renombrarSubestacionEnTodos(string viejo, string nuevo); // al renombrar un vertice

    void imprimirReporte(); // omite clientes sin historial
    bool canjearBeneficio(string nombreCliente, ListaBeneficios& catalogo);
};
