#pragma once
#include <string>
#include "Grafo.h"
#include "ListaClientes.h"
#include "ListaBeneficios.h"
using namespace std;

// Lectura y escritura de los 3 archivos. Es de Persona 2
class ArchivoJSON {
private:
    string rutaRed;
    string rutaClientes;
    string rutaBeneficios;

public:
    ArchivoJSON(string rutaRed, string rutaClientes, string rutaBeneficios);

    // Devuelve false si el programa debe terminar (C1-A y R2-C)
    bool cargarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios);

    // Escribe los 3 archivos (al salir y bajo demanda)
    bool guardarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios);
};
