# Datos de prueba, formato de los JSON y resultados esperados

Persona 2 · versión del 16 de noviembre de 2026, con el formato de los JSON ajustado al enunciado el 20 de noviembre.
Este documento sirve para tres cosas: que Persona 1 sepa qué recibe la carga, probar cada consulta
contra un resultado calculado a mano y armar después el **análisis de resultados**.

## 1. Formato de los archivos

Los tres archivos siguen el formato del enunciado. Los nombres de los campos están en un solo lugar:
las constantes `K_...` al inicio de `src/ArchivoJSON.cpp`.

**red_electrica.json**: las regiones agrupan los puntos de interconexión; al cargarlos se aplanan en vértices con la región como texto.
```json
{
  "regiones": [ { "nombre": "Norte", "puntos_interconexion": [ { "nombre": "Arenal", "tipo": "generador" } ] } ],
  "lineas_transmision": [
    { "origen": { "region": "Norte", "punto": "Arenal" },
      "destino": { "region": "Norte", "punto": "Liberia" },
      "medio": "alta_tension", "distancia_km": 60.0, "capacidad_max": 500.0,
      "perdida_porcentaje": 2.0, "usos": 3 }
  ]
}
```
`capacidad_max` y `usos` son los dos campos que se agregan al ejemplo del enunciado. Son opcionales: si una
línea no los trae, se usan 100 y 0, así el programa también lee un archivo con el formato exacto del enunciado.
**clientes.json** (igual al enunciado; si faltan las dos listas se toman como vacías)
```json
{ "clientes": [ { "nombre": "Ana_Mora", "tokens": 120,
                  "subestaciones_conectadas": ["SanJose"], "beneficios": ["Descuento_5"] } ] }
```
**beneficios.json**
```json
{ "beneficios": [ { "nombre": "Descuento_5", "costo": 50 } ] }
```
`usos`, `tokens` y `costo` tienen que ser enteros (`120`, no `120.0`); si no, el archivo se considera mal formado.

## 2. Reglas de carga implementadas

| Situación | Resultado |
|---|---|
| `red_electrica.json` no existe o tiene un error de sintaxis | Mensaje y el programa termina |
| `clientes.json` o `beneficios.json` no existen | Aviso y se arranca con la lista vacía |
| `clientes.json` o `beneficios.json` tienen un error de sintaxis | Mensaje y el programa termina |
| Una línea no trae `capacidad_max` o `usos` | Se usan 100 y 0; aviso solo por la capacidad |
| Falta un campo obligatorio o tiene el tipo equivocado (por ejemplo, `"tokens": "cien"`) | Se trata como archivo mal formado: el programa termina e indica el elemento |
| Dato con valor inválido en un JSON correcto (pérdida 150, lazo, nombre repetido…) | El método de Persona 1 lo rechaza e imprime el motivo; la carga avisa que lo omitió y sigue |
| Cliente con una subestación que no existe | Aviso y se ignora esa subestación; el cliente sí se carga |
| Beneficio canjeado que ya no está en el catálogo | Se conserva |

Orden de carga: red (todos los vértices y después todos los arcos) → beneficios → clientes.
El guardado escribe primero en `archivo.json.tmp` y luego reemplaza el original, así un fallo a medias no daña los datos.

### Archivos para probar estas reglas (`data/pruebas/`)
Para cada prueba se copia el archivo sobre el de `data/` con el nombre correcto (guardar antes los originales).

| Archivo | Reemplaza a | Qué se espera |
|---|---|---|
| `red_formato_enunciado.json` | red_electrica.json | El ejemplo exacto del enunciado (sin `capacidad_max` ni `usos`): carga 2 nodos y 1 línea, con un aviso por la capacidad |
| `red_mal_formada.json` | red_electrica.json | Error de sintaxis y el programa termina |
| `red_datos_invalidos.json` | red_electrica.json | Carga 2 nodos (Arenal y Liberia) y 1 línea (Arenal → Liberia alta_tension); rechaza nodo repetido, tipo inválido, nombre vacío, arco repetido con el mismo medio, pérdida 150, lazo, medio inválido, distancia 0, capacidad negativa y destino inexistente |
| `clientes_subestacion_inexistente.json` | clientes.json | Aviso por la subestación inexistente `Monteverde`; rechaza el cliente repetido y los tokens negativos |
| `clientes_formato_incorrecto.json` | clientes.json | `tokens` es texto: el programa termina indicando "el cliente 1" |
| (borrar clientes.json) | — | Arranca sin clientes y al salir crea el archivo |

## 3. La red de prueba

17 vértices, 22 líneas y 10 clientes (los mínimos son 15, 20 y 10).

| Región | Generador | Subestación | Subestación local |
|---|---|---|---|
| Norte | Arenal, Miravalles | Liberia | Canas |
| Centro | — | SanJose, Heredia, Alajuela | Escazu, Tibas |
| Este | Cachi, Reventazon | Cartago | — |
| Pacífico | — | Puntarenas | Jaco, Esparza |
| Sur | — | PerezZeledon | Golfito |

Puntaje por arco = tabla[medio] − pérdida − 0,05 × distancia (alta_tension 10 · subterraneo 8 · red_local 5).

| # | Línea | Medio | km | Pérdida % | Usos | Puntaje |
|---|---|---|---|---|---|---|
| 1 | Arenal → Liberia | alta_tension | 60 | 2,0 | 3 | 5,00 |
| 2 | Arenal → Canas | alta_tension | 45 | 1,5 | 1 | 6,25 |
| 3 | Miravalles → Liberia | alta_tension | 30 | 1,2 | 2 | 7,30 |
| 4 | Miravalles → Canas | subterraneo | 25 | 1,0 | 0 | 5,75 |
| 5 | Liberia → Canas | red_local | 20 | 0,8 | 1 | 3,20 |
| 6 | Liberia → Puntarenas | red_local | 120 | 9,0 | 0 | **−10,00** |
| 7 | Canas → Alajuela | alta_tension | 80 | 2,5 | 4 | 3,50 |
| 8 | Puntarenas → Esparza | red_local | 30 | 3,4 | 0 | **0,10** |
| 9 | Puntarenas → Jaco | red_local | 40 | 2,0 | 1 | 1,00 |
| 10 | Cachi → SanJose | alta_tension | 30 | 1,5 | **6** | 7,00 |
| 11 | Cachi → SanJose | subterraneo | 32 | 0,9 | 2 | 5,50 |
| 12 | Cachi → Cartago | alta_tension | 20 | 1,0 | 1 | 8,00 |
| 13 | Alajuela → Heredia | subterraneo | 15 | 0,7 | 2 | 6,55 |
| 14 | Alajuela → SanJose | red_local | 18 | 0,5 | 1 | 3,60 |
| 15 | Heredia → SanJose | subterraneo | 12 | 0,6 | 3 | 6,80 |
| 16 | Heredia → Tibas | red_local | 8 | 0,4 | 0 | 4,20 |
| 17 | SanJose → Escazu | red_local | 10 | 0,5 | 2 | 4,00 |
| 18 | SanJose → Tibas | red_local | 6 | 0,3 | 0 | 4,40 |
| 19 | Reventazon → Cartago | alta_tension | 35 | 1,8 | 5 | 6,45 |
| 20 | Cartago → SanJose | alta_tension | 25 | 1,2 | 2 | 7,55 |
| 21 | Cartago → PerezZeledon | alta_tension | 110 | 4,5 | 0 | 0,00 |
| 22 | PerezZeledon → Golfito | red_local | 90 | 6,0 | 0 | −5,50 |

## 4. Dónde está cada caso obligatorio (sección 6 de la división)

| Caso | Dónde | Demuestra |
|---|---|---|
| Nodo inalcanzable | Desde Arenal no se llega a Miravalles, Cachi, Reventazon, Cartago, PerezZeledon ni Golfito. Golfito no tiene salidas | Grafo dirigido; nodos no alcanzables en BFS y DFS |
| Subestaciones sin clientes, de ambos tipos | PerezZeledon (subestacion), Esparza y Golfito (subestacion_local) | Nodos inactivos |
| Dos arcos entre el mismo par con distinto medio | Cachi → SanJose (alta_tension y subterraneo) | Arcos repetidos solo con medio distinto; borrar un arco pidiendo el medio |
| Varias rutas X→Y y una de un solo medio | Cachi → SanJose (3 rutas, dos de solo alta_tension); Alajuela → SanJose (una solo subterraneo y otra solo red_local) | Filtro por medio en todas las rutas; búsqueda de la ruta de más Eco-Tokens |
| Ruta cuyo total da ≤ 0 | Liberia → Puntarenas (−10). Puntarenas → Esparza da 0,10, que al redondear queda en 0 | Rutas que no otorgan tokens |
| Dijkstra y Eco-Tokens eligen distinto | Alajuela → SanJose y Arenal → SanJose (ver sección 5) | C2 vs. C3 |
| usos: máximo claro, empate y varios en 1 | Máximo: Cachi → SanJose alta_tension (6). Empate: al aceptar en C3 la ruta Reventazon → SanJose, Reventazon → Cartago pasa de 5 a 6. Con usos 1: líneas 2, 5, 9, 12 y 14 | Empates y ruta única (C5) |
| Cliente sin historial | Bruno_Solis (0 tokens y listas vacías) | El reporte omite clientes sin historial |
| Saldo para unos beneficios y no para otros | Ana_Mora (120): solo puede canjear Descuento_10 | El canje muestra solo lo que el cliente puede pagar |
| Beneficio que ya no está en el catálogo | Jorge_Arias conserva Plan_antiguo | El cliente conserva beneficios borrados del catálogo |
| Cliente con 0 tokens pero con historial (sí sale en el reporte) | Fabian_Castro | Tener 0 tokens no es lo mismo que no tener historial |

## 5. Resultados esperados (calculados a mano y verificados con un script)

Redondeo: medio hacia arriba, alejándose del cero (`std::round`). Persona 1 debe usar el mismo para que coincidan.

### B1 · BFS desde Arenal
Orden: Arenal, Liberia, Canas, Puntarenas, Alajuela, Esparza, Jaco, Heredia, SanJose, Tibas, Escazu
Árbol: Arenal→Liberia, Arenal→Canas, Liberia→Puntarenas, Canas→Alajuela, Puntarenas→Esparza, Puntarenas→Jaco, Alajuela→Heredia, Alajuela→SanJose, Heredia→Tibas, SanJose→Escazu
No alcanzables: Miravalles, Cachi, Reventazon, Cartago, PerezZeledon, Golfito

### B2 · DFS desde Arenal
Orden: Arenal, Liberia, Canas, Alajuela, Heredia, SanJose, Escazu, Tibas, Puntarenas, Esparza, Jaco
Árbol: Arenal→Liberia, Liberia→Canas, Canas→Alajuela, Alajuela→Heredia, Heredia→SanJose, SanJose→Escazu, SanJose→Tibas, Liberia→Puntarenas, Puntarenas→Esparza, Puntarenas→Jaco
No alcanzables: los mismos que en el BFS

### B4 · Reporte de clientes
Salen 9 clientes; Bruno_Solis no aparece.

### B5 · Nodos inactivos
Esparza, PerezZeledon, Golfito (los generadores no cuentan)

### C1 · Todas las rutas con filtro de medios
| Origen → destino | Medios | Rutas |
|---|---|---|
| Cachi → SanJose | alta_tension | 2 (directa; por Cartago) |
| Cachi → SanJose | subterraneo | 1 (directa) |
| Cachi → SanJose | alta_tension + subterraneo | 3 |
| Canas → SanJose | alta_tension | 0 → mensaje "no hay rutas" |
| Canas → SanJose | alta_tension + subterraneo | 1 (por Alajuela y Heredia) |
| Canas → SanJose | alta_tension + red_local | 1 (por Alajuela) |

### C2 · Dijkstra contra C3 · Eco-Tokens
| Origen → destino | Ruta | Pérdida % | Puntaje | Tokens normal | Tokens hora no pico |
|---|---|---|---|---|---|
| Alajuela → SanJose | directa (red_local) | **0,50 ← Dijkstra** | 3,60 | 4 | 5 |
| Alajuela → SanJose | por Heredia (subterraneo) | 1,30 | **13,35 ← Eco** | 13 | 20 |
| Arenal → SanJose | Liberia, Canas, Alajuela, Heredia | 6,60 | **25,05 ← Eco** | 25 | 38 |
| Arenal → SanJose | Liberia, Canas, Alajuela | 5,80 | 15,30 | 15 | 23 |
| Arenal → SanJose | Canas, Alajuela, Heredia | 5,30 | 23,10 | 23 | 35 |
| Arenal → SanJose | Canas, Alajuela | **4,50 ← Dijkstra** | 13,35 | 13 | 20 |
| Cachi → SanJose | directa alta_tension | 1,50 | 7,00 | 7 | 11 |
| Cachi → SanJose | directa subterraneo | **0,90 ← Dijkstra** | 5,50 | 6 | 8 |
| Cachi → SanJose | por Cartago | 2,20 | **15,55 ← Eco** | 16 | 23 |
| Reventazon → SanJose | por Cartago (única) | 3,00 | 14,00 | 14 | 21 |
| Liberia → Puntarenas | directa (única) | 9,00 | −10,00 | no otorga tokens | no otorga tokens |
| Puntarenas → Esparza | directa (única) | 3,40 | 0,10 | 0 → no otorga | 0,15 → 0 → no otorga |
| Puntarenas → Jaco | directa (única) | 2,00 | 1,00 | 1 | 1,5 → 2 |

**Al aceptar en C3** (por ejemplo, Elena_Rojas con Arenal → SanJose en hora normal): tokens 260 → 285. Se agregan Liberia, Heredia y SanJose a sus subestaciones (Canas y Alajuela ya estaban; Arenal es generador y no se agrega). Cada línea de la ruta suma 1 a sus usos.

### C4 · Canje
| Cliente | Tokens | Beneficios que se le deben mostrar |
|---|---|---|
| Ana_Mora | 120 | Descuento_10 |
| Diego_Vargas | 35 | ninguno |
| Irene_Campos | 1000 | Mes_gratis |
| Carla_Jimenez | 450 | Descuento_5, Bono_carga_vehiculo, Panel_solar_parcial |

### C5 · Ruta frecuente / única (con los datos iniciales)
Frecuente: Cachi → SanJose alta_tension (6 usos).
Usadas una sola vez: Arenal → Canas, Liberia → Canas, Puntarenas → Jaco, Cachi → Cartago, Alajuela → SanJose.
Empate: después de aceptar Reventazon → SanJose en C3, Cachi → SanJose y Reventazon → Cartago quedan ambas con 6.
