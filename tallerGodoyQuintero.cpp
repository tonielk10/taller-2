#include <iostream>
#include <fstream>
#include <string>
#include "color.h"

using namespace std;

// Estructura que representa una arista en el grafo
struct Arista {
    int destino; // Nodo destino de la arista
    int peso;    // Peso de la arista
    Arista* next; // Puntero a la siguiente arista
};

// Estructura que representa un vértice en el grafo
struct Node_vertice {
    int value;       // Valor del vértice
    int peso;        // Peso del vértice (no utilizado actualmente)
    Node_vertice* next; // Puntero al siguiente vértice
    Arista* down;    // Puntero a la lista de aristas del vértice
};

// Crea un nuevo vértice con el valor especificado
Node_vertice* crearNodo(int valor) {
    Node_vertice* nuevo = new Node_vertice;
    nuevo->value = valor;
    nuevo->next = NULL;
    nuevo->down = NULL;
    return nuevo;
}

// Busca un vértice en el grafo por su valor
Node_vertice* buscarNodo(Node_vertice* cabeza, int valor) {
    Node_vertice* actual = cabeza;
    while (actual != NULL) {
        if (actual->value == valor) return actual;
        actual = actual->next;
    }
    return NULL; 
}

// Agrega una arista entre dos vértices en el grafo
void agregarArista(Node_vertice** cabeza, int origen, int destino, int peso) {
    Node_vertice* nodoOrigen = buscarNodo(*cabeza, origen);
    if (nodoOrigen == NULL) {
        nodoOrigen = crearNodo(origen);
        nodoOrigen->next = *cabeza;
        *cabeza = nodoOrigen;
    }

    Node_vertice* nodoDestino = buscarNodo(*cabeza, destino);
    if (nodoDestino == NULL) {
        nodoDestino = crearNodo(destino);
        nodoDestino->next = *cabeza;
        *cabeza = nodoDestino;
    }

    Arista* arista1 = new Arista{destino, peso, nodoOrigen->down};
    nodoOrigen->down = arista1;

    Arista* arista2 = new Arista{origen, peso, nodoDestino->down};
    nodoDestino->down = arista2;
}

// Lee el grafo desde un archivo de texto
Node_vertice* leerArchivo() {
    // Intenta diferentes rutas para el archivo
    ifstream archivo("estructura.txt");
    if (!archivo.is_open()) {
        archivo.open("./estructura.txt");
    }
    if (!archivo.is_open()) {
        archivo.open("/Users/enr.g/Documents/UCAB/Taller_2/taller-2/estructura.txt");
    }
    
    if (!archivo.is_open()) {
        colorP(12, "Error al abrir el archivo estructura.txt");
        colorP(14, "Asegúrate de que el archivo esté en el directorio del programa");
        return NULL;
    }

    int total_nodos; // Número total de nodos (no utilizado actualmente)
    archivo >> total_nodos;

    Node_vertice* cabeza = NULL;
    int origen, destino, peso;

    while (archivo >> origen >> destino >> peso) {
        agregarArista(&cabeza, origen, destino, peso);
    }

    archivo.close();
    colorP(2, "Archivo leído correctamente");
    return cabeza;
}

// Imprime el grafo en formato de lista de adyacencia
void imprimirGrafo(Node_vertice* cabeza) {
    Node_vertice* actualNodo = cabeza;
    colorP(6, "\n=== GRAFO IMPLEMENTADO CON LISTA DE ADYACENCIA ===");
    colorP(7, "(Vértice conectado, Peso)\n");
    
    while (actualNodo != NULL) {
        colorPrint(3, "Nodo ");
        colorPrint(1, to_string(actualNodo->value));
        colorPrint(7, " -> ");
        
        Arista* actualArista = actualNodo->down;
        while (actualArista != NULL) {
            colorPrint(10, " ( ");
            colorPrint(4, to_string(actualArista->destino));
            colorPrint(10, " , ");
            colorPrint(14, to_string(actualArista->peso));
            colorPrint(10, " ) ");
            actualArista = actualArista->next;
        }
        cout << endl;
        actualNodo = actualNodo->next;
    }
}

// Libera la memoria utilizada por el grafo
void liberarGrafo(Node_vertice* cabeza) {
    Node_vertice* actualNodo = cabeza;
    while (actualNodo != NULL) {
        Arista* actualArista = actualNodo->down;
        while (actualArista != NULL) {
            Arista* temp = actualArista;
            actualArista = actualArista->next;
            delete temp;
        }
        Node_vertice* tempNodo = actualNodo;
        actualNodo = actualNodo->next;
        delete tempNodo;
    }
}

// Cuenta el número de vértices en el grafo
int contarVertices(Node_vertice* grafo) {
    int count = 0;
    Node_vertice* actual = grafo;
    while (actual != NULL) {
        count++;
        actual = actual->next;
    }
    return count;
}

int MAX=9999999;
// Algoritmo de Prim para encontrar el Árbol Mínimo Cobertor (AMC)
void primAMC(Node_vertice* grafo) {
    if (grafo == NULL) {
        colorP(12, "El grafo está vacío");
        return;
    }

    int numVertices = contarVertices(grafo);

    if (numVertices == 0) {
        colorP(12, "No hay vértices en el grafo");
        return;
    }

    bool enAMC[numVertices]; 
    for (int i = 0; i < numVertices; i++) {
        enAMC[i] = false;  
    }
    int pesoMinimo[numVertices]; 
    int verticePadre[numVertices]; 

    for (int i = 0; i < numVertices; i++) {
        pesoMinimo[i] = MAX;  
    }
    pesoMinimo[0] = 0;
    verticePadre[0] = -1; 

    for (int count = 0; count < numVertices - 1; count++) {
        int minPeso=MAX;
        int u = -1;
        
        for (int v = 0; v < numVertices; v++) {
            if (!enAMC[v] && pesoMinimo[v] < minPeso) {
                minPeso = pesoMinimo[v];
                u = v;
            }
        }

        if (u == -1) {
            colorP(12, "El grafo no es conexo, no se puede completar el AMC.");
            return;
        }

        enAMC[u] = true;

        Node_vertice* verticeU = buscarNodo(grafo, u);
        Arista* arista = verticeU->down;
        
        while (arista != NULL) {
            int v = arista->destino; 
            int peso = arista->peso;

            if (!enAMC[v] && peso < pesoMinimo[v]) {
                verticePadre[v] = u;
                pesoMinimo[v] = peso;
            }
            arista = arista->next;
        }
    }

    colorP(5, "\n=== ALGORITMO DE PRIM ===");
    colorP(6, "Árbol Mínimo Cobertor (AMC):");
    colorP(7, "Arista (Rama) \tPeso");
    
    int pesoTotal = 0;
    for (int i = 1; i < numVertices; i++) {
        colorPrint(10, "  ( ");
        colorPrint(4, to_string(verticePadre[i]));
        colorPrint(7, " - ");
        colorPrint(4, to_string(i));
        colorPrint(10, " )\t ");
        colorP(14, to_string(pesoMinimo[i]));
        pesoTotal += pesoMinimo[i];
    }
    
    colorPrint(2, "\nPeso total del Árbol Mínimo Cobertor: ");
    colorP(1, to_string(pesoTotal));
}

// Función para imprimir el camino más corto desde el vértice indicado hasta el vértice destinatario
void imprimirCamino(int destino, int verticePadre[], int origen) {
    if (destino == origen) {
        colorPrint(4, to_string(origen));
        return;
    }
    if (verticePadre[destino] == -1) {
        colorPrint(12, "No hay camino");
        return;
    }
    imprimirCamino(verticePadre[destino], verticePadre, origen);
    colorPrint(7, " -> ");
    colorPrint(4, to_string(destino));
}

// Algoritmo de Dijkstra para encontrar el camino más corto desde un vértice origen
void djikstra (Node_vertice* grafo, int verticeOrigen) {
    if (grafo == NULL) {
        colorP(12, "El grafo está vacío");
        return;
    }

    int numVertices = contarVertices(grafo);

    if (numVertices == 0) {
        colorP(12, "No hay vértices en el grafo");
        return;
    }

    if (buscarNodo(grafo, verticeOrigen) == NULL) {
        colorP(12, "El vértice origen " + to_string(verticeOrigen) + " no existe en el grafo");
        return;
    }

    bool procesado[numVertices];
    for (int i = 0; i < numVertices; i++) {
        procesado[i] = false;
    }

    int distancia[numVertices];
    int verticePadre[numVertices];

    for (int i = 0; i < numVertices; i++) {
        distancia[i] = MAX;
        verticePadre[i] = -1;
    }
    
    distancia[verticeOrigen] = 0;

    for (int count = 0; count < numVertices; count++) {
        int minDistancia = MAX;
        int u = -1;
        
        for (int v = 0; v < numVertices; v++) {
            if (!procesado[v] && distancia[v] < minDistancia) {
                minDistancia = distancia[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        procesado[u] = true;

        Node_vertice* verticeU = buscarNodo(grafo, u);
        Arista* arista = verticeU->down;
        
        while (arista != NULL) {
            int v = arista->destino;
            int peso = arista->peso;

            if (!procesado[v] && distancia[u] != MAX && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                verticePadre[v] = u;
            }
            arista = arista->next;
        }
    }

    colorP(6, "\n=== CAMINOS MÁS CORTOS ===");
    colorPrint(7, "Partiendo desde el vértice ");
    colorP(4, to_string(verticeOrigen));
    colorP(7, "Destino\tDistancia\tCamino");
    
    for (int i = 0; i < numVertices; i++) {
        colorPrint(4, to_string(i));
        colorPrint(7, "\t");
        if (distancia[i] == MAX) {
            colorP(12, "∞\t\tNo se puede acceder.");
        } else {
            colorPrint(14, to_string(distancia[i]));
            colorPrint(7, "\t\t");
            imprimirCamino(i, verticePadre, verticeOrigen);
            cout << endl;
        }
    }
}

int obtenerIndice(Node_vertice* grafo, int vertice) {
    int indice = 0;
    Node_vertice* actual = grafo;
    while (actual != NULL) {
        if (actual->value == vertice) { 
            return indice;
        }
        actual = actual->next;
        indice++;
    }
    return -1;
}

bool colorDisponible(Node_vertice* vertice, int colores[], int color, Node_vertice* grafo) {
    Arista* arista = vertice->down;
    while (arista != NULL) {
        int idxVecino = obtenerIndice(grafo, arista->destino);
        if (idxVecino != -1 && colores[idxVecino] == color) {
            return false;
        }
        arista = arista->next;
    }
    return true;
}

void colorearGrafo(Node_vertice* grafo, int numVertices) {
    int* colores = new int[numVertices];
    const char* nombresColores[] = {
        "Rojo", "Verde", "Azul", "Amarillo", "Naranja",
        "Morado", "Cyan", "Rosa", "Gris", "Marrón"
    };

    int codigosColores[] = {
        1,  // Rojo brillante
        2,  // Verde brillante
        4,  // Azul brillante
        3,  // Amarillo brillante
        6,  // Naranja (Cyan)
        5,  // Morado brillante
        11, // Cyan
        13, // Rosa (Magenta)
        8,  // Gris
        14  // Marrón (Amarillo)
    };

    for (int i = 0; i < numVertices; i++) {
        colores[i] = -1;
    }

    Node_vertice* actual = grafo;
    while (actual != NULL) {
        int indice = obtenerIndice(grafo, actual->value);

        for (int c = 0; c < numVertices; c++) {
            if (colorDisponible(actual, colores, c, grafo)) {
                colores[indice] = c;
                break;
            }
        }

        actual = actual->next;
    }

    colorP(5, "\n=== COLORACIÓN DEL GRAFO ===");
    actual = grafo;
    while (actual != NULL) {
        int indice = obtenerIndice(grafo, actual->value);
        int color = colores[indice];
        colorPrint(7, "Vértice ");
        colorPrint(4, to_string(actual->value));
        colorPrint(7, " -> ");
        colorP(codigosColores[color], nombresColores[color]);
        actual = actual->next;
    }

    delete[] colores;
}

int main(){
    int verticeInicio, op = 0;
    Node_vertice* grafo = leerArchivo();  
    
    if (grafo != NULL) {
        int numVertices = contarVertices(grafo);
        
        // Título principal con colores
        colorP(6, "\n╔════════════════════════════════════════════════════════════════╗");
        colorP(6, "║            TALLER #2 - TEORÍA DE GRAFOS                       ║");
        colorP(6, "║        Realizado por: Enrique Godoy y Jose Quintero            ║");
        colorP(6, "╚════════════════════════════════════════════════════════════════╝");

        while (op != 5){
            colorP(3, "\n┌─────────────── MENÚ PRINCIPAL ───────────────┐");
            colorP(7, "│ 1. Imprimir Grafo                            │");
            colorP(7, "│ 2. Ver Árbol Mínimo Cobertor (Prim)          │");
            colorP(7, "│ 3. Camino de Costo Mínimo (Dijkstra)         │");
            colorP(7, "│ 4. Coloración del Grafo (Algoritmo Voraz)    │");
            colorP(7, "│ 5. Cerrar Programa                           │");
            colorP(3, "└───────────────────────────────────────────────┘");
            
            colorPrint(14, "\nIndique el número de operación que desea realizar: ");
            cin >> op;
            
            switch (op){
                case 1:
                    imprimirGrafo(grafo);
                    break;

                case 2:
                    primAMC(grafo);
                    break;

                case 3:
                    colorP(5, "\n=== ALGORITMO DE DIJKSTRA ===");
                    do{
                        colorPrint(7, "\nIndique el vértice desde donde desea conocer el camino de menor costo: ");
                        colorPrint(6, "\nVértices disponibles: ( 0 - ");
                        colorPrint(6, to_string(numVertices-1));
                        colorPrint(6, " ): ");
                        cin >> verticeInicio;
                        
                        if (verticeInicio < 0 || verticeInicio >= numVertices){
                            colorP(12, "ERROR: El vértice debe estar entre el rango indicado");
                            colorPrint(14, "Rango válido: ( 0 - ");
                            colorPrint(14, to_string(numVertices-1));
                            colorP(14, " )");
                            colorP(14, "Inténtelo nuevamente...");
                        }
                    } while (verticeInicio < 0 || verticeInicio >= numVertices);
                    djikstra(grafo, verticeInicio);
                    break;

                case 4:
                    colorP(5, "\n=== ALGORITMO VORAZ DE COLORACIÓN ===");
                    colorearGrafo(grafo, numVertices);
                    break;

                case 5:
                    colorP(14, "\nLiberando memoria...");
                    liberarGrafo(grafo);
                    colorP(2, "Memoria liberada correctamente");
                    colorP(6, "¡Gracias por usar el programa!");
                    colorP(7, "Cerrando...");
                    break;

                default:
                    colorP(12, "Opción no válida. Por favor, seleccione una opción del 1 al 5.");
                    break;
            }
        }  
    } else {
        colorP(12, "No se pudo cargar el grafo. Verifique el archivo 'estructura.txt'");
    }
    
    return 0;
}
