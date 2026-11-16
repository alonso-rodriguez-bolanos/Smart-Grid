#pragma once
#include <string>
using namespace std;

class Arco; // aviso de que Arco existe porque Vertice y Arco se apuntan entre si

// Punto de interconexion de la red. La region es solo un texto (D1)
class Vertice {
private:
    string nombre;
    string tipo;          // generador subestacion o subestacion_local
    string region;
    bool visitado;        // marca para BFS DFS rutas y Dijkstra
    Vertice* sigVertice;  // siguiente en la lista de vertices
    Arco* primerArco;     // primera linea que sale de este vertice

public:
    Vertice(string nombre, string tipo, string region);

    string getNombre();
    string getTipo();
    string getRegion();
    bool isVisitado();
    Vertice* getSig();
    Arco* getPrimerArco();

    void setNombre(string nombre);
    void setTipo(string tipo);
    void setRegion(string region);
    void setVisitado(bool visitado);
    void setSig(Vertice* sig);
    void setPrimerArco(Arco* arco);
};
