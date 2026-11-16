#pragma once
#include <string>
#include "NodoTexto.h"
using namespace std;

class Cliente {
private:
    string nombre;
    int tokens;
    NodoTexto* primeraSubestacion; // subestaciones_conectadas
    NodoTexto* primerBeneficio;    // beneficios canjeados
    Cliente* sig;

public:
    Cliente(string nombre, int tokens);
    ~Cliente(); // libera sus dos sublistas

    // No se permite copiar la estructura: siempre se pasa con &
    Cliente(const Cliente&) = delete;
    Cliente& operator=(const Cliente&) = delete;

    // ===== CONTRATO: los usa Persona 2 para guardar =====
    string getNombre();
    int getTokens();
    NodoTexto* getPrimeraSubestacion();
    NodoTexto* getPrimerBeneficio();
    Cliente* getSig();

    // ===== Solo Persona 1 =====
    void setNombre(string nombre);
    void setTokens(int tokens);
    void setSig(Cliente* sig);

    bool agregarSubestacion(string nombre);   // no permite repetidos
    bool tieneSubestacion(string nombre);
    bool quitarSubestacion(string nombre);
    bool renombrarSubestacion(string viejo, string nuevo);

    bool agregarBeneficio(string nombre);     // no permite repetidos (D12a)
    bool tieneBeneficio(string nombre);
};
