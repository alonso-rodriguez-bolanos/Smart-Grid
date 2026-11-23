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


    Cliente(const Cliente&) = delete;
    Cliente& operator=(const Cliente&) = delete;


    string getNombre();
    int getTokens();
    NodoTexto* getPrimeraSubestacion();
    NodoTexto* getPrimerBeneficio();
    Cliente* getSig();


    void setNombre(string nombre);
    void setTokens(int tokens);
    void setSig(Cliente* sig);

    bool agregarSubestacion(string nombre);   // no permite repetidos
    bool tieneSubestacion(string nombre);
    bool quitarSubestacion(string nombre);
    bool renombrarSubestacion(string viejo, string nuevo);

    bool agregarBeneficio(string nombre);     // no permite repetidos
    bool tieneBeneficio(string nombre);
};
