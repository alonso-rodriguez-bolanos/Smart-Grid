#include "Cliente.h"

Cliente::Cliente(string nombre, int tokens) {
    // pendiente Persona 1
}

Cliente::~Cliente() {
    // pendiente Persona 1
}

string Cliente::getNombre() { return ""; }
int Cliente::getTokens() { return 0; }
NodoTexto* Cliente::getPrimeraSubestacion() { return nullptr; }
NodoTexto* Cliente::getPrimerBeneficio() { return nullptr; }
Cliente* Cliente::getSig() { return nullptr; }

void Cliente::setNombre(string nombre) {}
void Cliente::setTokens(int tokens) {}
void Cliente::setSig(Cliente* sig) {}

bool Cliente::agregarSubestacion(string nombre) { return true; }
bool Cliente::tieneSubestacion(string nombre) { return false; }
bool Cliente::quitarSubestacion(string nombre) { return false; }
bool Cliente::renombrarSubestacion(string viejo, string nuevo) { return false; }

bool Cliente::agregarBeneficio(string nombre) { return true; }
bool Cliente::tieneBeneficio(string nombre) { return false; }
