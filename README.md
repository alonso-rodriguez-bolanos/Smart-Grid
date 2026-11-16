# Smart Grid · IC2001 Estructuras de Datos · Segundo Proyecto

Sistema de optimización y distribución para una red eléctrica inteligente.
Entrega: 24 de noviembre de 2026, antes de las 11:59 p. m.

## Estructura

```
src/    código fuente (un .h y un .cpp por clase)
data/   red_electrica.json, clientes.json, beneficios.json
docs/   documentación
```

## Compilar y ejecutar (desde la carpeta raíz)

```
g++ -std=c++17 src/*.cpp -o smartgrid
./smartgrid
```

## Quién edita qué

| Archivos | Responsable |
|---|---|
| Vertice, Arco, Grafo, NodoTexto, Cliente, ListaClientes, Beneficio, ListaBeneficios, Menu, main.cpp | Persona 1 (lógica) |
| ArchivoJSON, json.hpp, data/*.json, docs/ | Persona 2 (datos y documentación) |

## Reglas

- Los métodos marcados como **CONTRATO** en los `.h` no se cambian sin avisar al otro.
- `pull` antes de empezar a trabajar y `push` al terminar, al menos una vez al día.
- Nunca subir código que no compile.
- Registrar en docs/ las fuentes y prompts usados (lo exige el enunciado).
- Unión del código: sábado 21. Código congelado: martes 24 a las 12:00.

## Notas

- `json.hpp` va dentro de `src/` (así el comando de compilación lo encuentra).
- Se ejecuta **desde la carpeta raíz**, porque las rutas son `data/...`. Si el IDE corre desde otra carpeta, configurar su "working directory".
- Nombres sin tildes en los datos y en lo que se escribe en la consola (la comparación es exacta).
- Los métodos de insertar imprimen el motivo cuando rechazan un dato y devuelven `false`; la carga solo sigue con el siguiente.
- La carga revisa `existeVertice()` antes de `agregarSubestacionACliente()` y avisa si no existe.

## Pendiente de confirmar

- Respuesta de la profesora: nlohmann/json, beneficios.json, campos capacidad_max y usos.
- Quién hace C5 (ruta frecuente / única) y B5 (nodos inactivos).
- Cola del BFS: std::queue o cola propia.
