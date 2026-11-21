#pragma once
#include "Grafo.h"
#include "ListaClientes.h"
#include "ListaBeneficios.h"
#include "ArchivoJSON.h"

// Menu con submenus por bloque
class Menu {
private:
    Grafo& red;
    ListaClientes& clientes;
    ListaBeneficios& beneficios;
    ArchivoJSON& archivo;

    // Lectura segura desde el teclado
    int leerEntero(int minimo, int maximo);
    double leerDecimal();
    string leerTexto(string mensaje);

    // Eligen por numero para evitar errores de escritura
    string elegirTipo();
    string elegirMedio();

    void menuCRUD();
    void menuReportes();
    void menuConsultas();

public:
    Menu(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios, ArchivoJSON& archivo);
    void ejecutar();
};
