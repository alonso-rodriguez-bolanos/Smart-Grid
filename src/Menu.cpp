#include "Menu.h"

Menu::Menu(Grafo& red, ListaClientes& clientes, ListaBeneficios& beneficios, ArchivoJSON& archivo)
    : red(red), clientes(clientes), beneficios(beneficios), archivo(archivo) {
}

int Menu::leerEntero(int minimo, int maximo) {
    return minimo; // pendiente Persona 1
}

void Menu::menuCRUD() {}
void Menu::menuReportes() {}
void Menu::menuConsultas() {}

void Menu::ejecutar() {
    // pendiente Persona 1
}
