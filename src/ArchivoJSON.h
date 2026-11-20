#pragma once
#include <string>
#include "Grafo.h"
#include "ListaClientes.h"
#include "ListaBeneficios.h"
using namespace std;

// Lectura y escritura de los 3 archivos. Es de Persona 2
// json.hpp se incluye SOLO en ArchivoJSON.cpp: este .h no lo expone,
// asi Persona 1 nunca depende de la biblioteca JSON
class ArchivoJSON {
private:
    string rutaRed;
    string rutaClientes;
    string rutaBeneficios;

    // Carga por archivo. Devuelven false si el programa debe terminar
    bool cargarRed(Grafo& red);
    bool cargarBeneficios(ListaBeneficios& beneficios);
    bool cargarClientes(ListaClientes& clientes, Grafo& red);

    // Guardado por archivo. Devuelven false si no se pudo escribir
    bool guardarRed(Grafo& red);
    bool guardarBeneficios(ListaBeneficios& beneficios);
    bool guardarClientes(ListaClientes& clientes);

public:
    ArchivoJSON(string rutaRed, string rutaClientes, string rutaBeneficios);

    // ===== CONTRATO: lo usa main al iniciar =====
    // Reglas de carga acordadas:
    //   red_electrica.json no existe o esta mal formado      -> false (el programa termina)
    //   clientes.json o beneficios.json no existen           -> lista vacia y sigue
    //   clientes.json o beneficios.json mal formados         -> false (el programa termina)
    //   dato invalido en un JSON correcto (ej. perdida 150)  -> lo rechaza el metodo de
    //                                                           Persona 1 con su aviso y se sigue
    //   linea sin capacidad_max o sin usos                   -> 100 y 0 por defecto (aviso solo por capacidad)
    bool cargarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios);

    // ===== CONTRATO: lo usan main al salir y Menu bajo demanda =====
    // Escribe los 3 archivos. Devuelve false si alguno fallo
    bool guardarTodo(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios);
};
