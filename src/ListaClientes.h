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

    // ===== CONTRATO: los usa Persona 2 para cargar =====
    // Si rechazan un dato imprimen el motivo y devuelven false
    bool insertarCliente(string nombre, int tokens);
    bool agregarSubestacionACliente(string nombreCliente, string nombreSubestacion);
    bool agregarBeneficioACliente(string nombreCliente, string nombreBeneficio);

    // ===== CONTRATO: lo usa Persona 2 para guardar =====
    Cliente* getPrimero();

    // ===== Solo Persona 1 =====
    Cliente* buscarCliente(string nombre);
    bool eliminarCliente(string nombre);
    bool asignarTokens(string nombre, int cantidad);   // puede ser negativa pero el saldo no baja de 0

    void quitarSubestacionDeTodos(string nombre);      // al borrar un vertice (D5a)
    void renombrarSubestacionEnTodos(string viejo, string nuevo); // al renombrar un vertice (D5b)

    void imprimirReporte(); // B4 omite clientes sin historial
    bool canjearBeneficio(string nombreCliente, ListaBeneficios& catalogo); // C4
};
