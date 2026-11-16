#pragma once
#include "Grafo.h"
#include "ListaClientes.h"
#include "ListaBeneficios.h"
#include "ArchivoJSON.h"

// Menu con submenus por bloque (D18b). Es de Persona 1
class Menu {
private:
    Grafo& red;
    ListaClientes& clientes;
    ListaBeneficios& beneficios;
    ArchivoJSON& archivo;

    int leerEntero(int minimo, int maximo); // lectura segura de numeros (D17)
    void menuCRUD();
    void menuReportes();
    void menuConsultas();

public:
    Menu(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios, ArchivoJSON& archivo);
    void ejecutar();
};
