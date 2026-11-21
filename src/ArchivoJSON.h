#pragma once
#include <string>
#include "Grafo.h"
#include "ListaClientes.h"
#include "ListaBeneficios.h"
using namespace std;

// json.hpp se incluye SOLO en ArchivoJSON.cpp: este .h no lo expone,
class ArchivoJSON {
private:
    string rutaRed;
    string rutaClientes;
    string rutaBeneficios;

    // Carga por archivo.
    bool cargarRed(Grafo& red);
    bool cargarBeneficios(ListaBeneficios& beneficios);
    bool cargarClientes(ListaClientes& clientes, Grafo& red);

    // Guardado por archivo.
    bool guardarRed(Grafo& red);
    bool guardarBeneficios(ListaBeneficios& beneficios);
    bool guardarClientes(ListaClientes& clientes);

public:
    ArchivoJSON(string rutaRed, string rutaClientes, string rutaBeneficios);


    bool cargarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios);


    bool guardarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios);
};
