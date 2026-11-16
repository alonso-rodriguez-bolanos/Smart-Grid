#pragma once
#include <string>
using namespace std;

class Vertice; // aviso de que Vertice existe

// Linea de transmision que sale de un vertice
class Arco {
private:
    Vertice* destino;   // puntero al vertice al que llega
    string medio;       // alta_tension subterraneo o red_local
    double distancia;   // distancia_km
    double capacidad;   // capacidad_max
    double perdida;     // perdida_porcentaje
    int usos;           // veces que se acepto en C3
    Arco* sigArco;      // siguiente linea del mismo vertice origen

public:
    Arco(Vertice* destino, string medio, double distancia, double capacidad, double perdida, int usos);

    Vertice* getDestino();
    string getMedio();
    double getDistancia();
    double getCapacidad();
    double getPerdida();
    int getUsos();
    Arco* getSig();

    void setDestino(Vertice* destino);
    void setMedio(string medio);
    void setDistancia(double distancia);
    void setCapacidad(double capacidad);
    void setPerdida(double perdida);
    void setUsos(int usos);
    void setSig(Arco* sig);
};
