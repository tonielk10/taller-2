#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Arista;

struct Vertice {
    int valor;
    Vertice* siguiente;
    Arista* lista_adyacencias;
};

struct Arista {
    Vertice* destino;
    Arista* siguiente;
    int peso;
};

Vertice* crearVertice(int valor) {
    Vertice* nuevo = new Vertice;
    nuevo->valor = valor;
    nuevo->siguiente = NULL;
    nuevo->lista_adyacencias = NULL;
    return nuevo;
}

Arista* crearArista(Vertice* destino, int peso) {
    Arista* nueva = new Arista;
    nueva->destino = destino;
    nueva->siguiente = NULL;
    nueva->peso = peso;
    return nueva;
}

void agregarVertice(Vertice*& grafo, int valor) {
    Vertice* nuevo = crearVertice(valor);
    if (grafo == NULL) {
        grafo = nuevo;
    } else {
        Vertice* actual = grafo;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

Vertice* buscarVertice(Vertice* grafo, int valor) {
    Vertice* actual = grafo;
    while (actual != NULL) {
        if (actual->valor == valor) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}

void agregarArista(Vertice* grafo, int origen, int destino, int peso) {
    Vertice* vertOrigen = buscarVertice(grafo, origen);
    Vertice* vertDestino = buscarVertice(grafo, destino);
    
    if (vertOrigen == NULL || vertDestino == NULL) {
        cout << "Uno o ambos vertices no existen" << endl;
        return;
    }
    
    // Agregar arista de origen a destino
    Arista* nuevaArista = crearArista(vertDestino, peso);
    if (vertOrigen->lista_adyacencias == NULL) {
        vertOrigen->lista_adyacencias = nuevaArista;
    } else {
        Arista* actual = vertOrigen->lista_adyacencias;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevaArista;
    }
    
    // Como es no dirigido, agregamos también la arista inversa
    nuevaArista = crearArista(vertOrigen, peso);
    if (vertDestino->lista_adyacencias == NULL) {
        vertDestino->lista_adyacencias = nuevaArista;
    } else {
        Arista* actual = vertDestino->lista_adyacencias;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevaArista;
    }
}


void imprimirGrafo(Vertice* red) {
    cout << "\nGrafo:\n";
    cout << "Nodo\tConexiones (Destino, Peso)\n";
    cout << "-----------------------------------\n";
    Vertice* nodoActual = red;
    while (nodoActual != NULL) {
        cout << nodoActual->valor << "\t";
        Arista* conexionActual = nodoActual->lista_adyacencias;
        while (conexionActual != NULL) {
            cout << "(" << conexionActual->destino->valor << ", " << conexionActual->peso << ") ";
            conexionActual = conexionActual->siguiente;
        }
        cout << endl;
        nodoActual = nodoActual->siguiente;
    }
}

Vertice* leerGrafoDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    Vertice* grafo = nullptr;
    
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombreArchivo << endl;
        return nullptr;
    }
    
    string linea;
    int numVertices = 0;
    
    // Leer primera línea (número de vértices)
    if (getline(archivo, linea)) {
        istringstream iss(linea);
        if (!(iss >> numVertices)) {
            cerr << "Error al leer número de vertices" << endl;
            archivo.close();
            return nullptr;
        }
        
        // Crear todos los vértices desde 0 hasta numVertices-1
        for (int i = 0; i < numVertices; ++i) {
            agregarVertice(grafo, i);
        }
    }
    
    // Leer las aristas
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        istringstream iss(linea);
        int origen, destino, peso;
        
        if (!(iss >> origen >> destino >> peso)) {
            cerr << "Error al leer línea: " << linea << endl;
            continue;
        }
        
        // Validar rangos de vértices
        if (origen < 0 || origen >= numVertices || destino < 0 || destino >= numVertices) {
            cerr << "Error: Vertice fuera de rango en línea: " << linea << endl;
            continue;
        }
        
        agregarArista(grafo, origen, destino, peso);
    }
    
    archivo.close();
    return grafo;
}

// Función para contar vértices
int contarVertices(Vertice* grafo) {
    int count = 0;
    Vertice* actual = grafo;
    while (actual != NULL) {
        count++;
        actual = actual->siguiente;
    }
    return count;
}

int MAX=999999999;
void calcularPRIM(Vertice* red) {
    if (red == NULL) {
        cout << "El grafo está vacío" << endl;
        return;
    }

    int totalNodos = contarVertices(red);
    if (totalNodos == 0) {
        cout << "No hay nodos en el grafo" << endl;
        return;
    }

    bool incluidoEnMST[totalNodos];
    int pesoMinimo[totalNodos];
    int nodoPadre[totalNodos];

    for (int i = 0; i < totalNodos; i++) {
        incluidoEnMST[i] = false;
        pesoMinimo[i] = MAX;
    }
    pesoMinimo[0] = 0;
    nodoPadre[0] = -1;

    for (int count = 0; count < totalNodos - 1; count++) {
        int pesoMin = MAX;
        int u = -1;
        for (int v = 0; v < totalNodos; v++) {
            if (!incluidoEnMST[v] && pesoMinimo[v] < pesoMin) {
                pesoMin = pesoMinimo[v];
                u = v;
            }
        }

        if (u == -1) {
            cout << "El grafo no es conexo, no se puede completar el algoritmo Prim." << endl;
            return;
        }

        incluidoEnMST[u] = true;
        Vertice* nodoU = buscarVertice(red, u);
        Arista* conexion = nodoU->lista_adyacencias;
        while (conexion != NULL) {
            int v = conexion->destino->valor;
            int peso = conexion->peso;
            if (!incluidoEnMST[v] && peso < pesoMinimo[v]) {
                nodoPadre[v] = u;
                pesoMinimo[v] = peso;
            }
            conexion = conexion->siguiente;
        }
    }

    cout << "\nAlgoritmo Prim:\n";
    cout << "Conexion\tPeso\n";
    cout << "-------------------\n";
    int pesoTotal = 0;
    for (int i = 1; i < totalNodos; i++) {
        cout << nodoPadre[i] << " - " << i << "\t" << pesoMinimo[i] << endl;
        pesoTotal += pesoMinimo[i];
    }
    cout << "\nPeso total del Prim: " << pesoTotal << endl;
}

// Función para imprimir el camino más corto desde el vértice origen hasta el destino
void imprimirCamino(int destino, int verticePadre[], int origen) {
    if (destino == origen) {
        cout << origen;
        return;
    }
    if (verticePadre[destino] == -1) {
        cout << "No hay camino";
        return;
    }
    imprimirCamino(verticePadre[destino], verticePadre, origen);
    cout << " -> " << destino;
}

// Algoritmo de Dijkstra para encontrar el camino más corto desde un vértice origen
void encontrarCaminosCortos(Vertice* red, int nodoOrigen) {
    if (red == NULL) {
        cout << "El grafo está vacío" << endl;
        return;
    }

    int totalNodos = contarVertices(red);
    if (totalNodos == 0) {
        cout << "No hay nodos en el grafo" << endl;
        return;
    }

    if (buscarVertice(red, nodoOrigen) == NULL) {
        cout << "El nodo origen " << nodoOrigen << " no existe en el grafo" << endl;
        return;
    }

    bool procesado[totalNodos];
    int distancia[totalNodos];
    int nodoPadre[totalNodos];

    for (int i = 0; i < totalNodos; i++) {
        procesado[i] = false;
        distancia[i] = MAX;
        nodoPadre[i] = -1;
    }
    distancia[nodoOrigen] = 0;

    for (int count = 0; count < totalNodos; count++) {
        int distanciaMin = MAX;
        int u = -1;
        for (int v = 0; v < totalNodos; v++) {
            if (!procesado[v] && distancia[v] < distanciaMin) {
                distanciaMin = distancia[v];
                u = v;
            }
        }

        if (u == -1) {
            break;
        }

        procesado[u] = true;
        Vertice* nodoU = buscarVertice(red, u);
        Arista* conexion = nodoU->lista_adyacencias;
        while (conexion != NULL) {
            int v = conexion->destino->valor;
            int peso = conexion->peso;
            if (!procesado[v] && distancia[u] != MAX && distancia[u] + peso < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                nodoPadre[v] = u;
            }
            conexion = conexion->siguiente;
        }
    }

    cout << "\nCaminos más cortos desde el nodo " << nodoOrigen << ":\n";
    cout << "Destino\tDistancia\tCamino\n";
    cout << "-----------------------------------\n";
    for (int i = 0; i < totalNodos; i++) {
        cout << i << "\t";
        if (distancia[i] == MAX) {
            cout << "∞\t\tNo accesible";
        } else {
            cout << distancia[i] << "\t\t";
            imprimirCamino(i, nodoPadre, nodoOrigen);
        }
        cout << endl;
    }
}


// Función auxiliar para obtener el índice de un vértice en la lista
int obtenerIndiceVertice(Vertice* grafo, Vertice* vertice) {
    int indice = 0;
    Vertice* actual = grafo;
    while (actual != NULL) {
        if (actual == vertice) {
            return indice;
        }
        actual = actual->siguiente;
        indice++;
    }
    return -1; // No encontrado
}

// Función para verificar si un color está disponible para un vértice
bool colorDisponible(Vertice* vertice, int colores[], int color, Vertice* grafo) {
    Arista* arista = vertice->lista_adyacencias;
    while (arista != NULL) {
        int indiceVecino = obtenerIndiceVertice(grafo, arista->destino);
        if (indiceVecino != -1 && colores[indiceVecino] == color) {
            return false;
        }
        arista = arista->siguiente;
    }
    return true;
}

// Algoritmo de coloración voraz mejorado
void colorearGrafo(Vertice* grafo, int colores[], int numVertices) {
    if (grafo == NULL || numVertices <= 0) return;
    
    // Inicializar todos los colores a -1 (no coloreado)
    for (int i = 0; i < numVertices; i++) {
        colores[i] = -1;
    }
    
    // Asignar primer color al primer vértice
    colores[0] = 0;
    
    // Colorear los vértices restantes
    Vertice* actual = grafo->siguiente;
    int indice = 1;
    
    while (actual != NULL && indice < numVertices) {
        // Encontrar el color disponible más bajo
        int colorCr = 0;
        while (true) {
            if (colorDisponible(actual, colores, colorCr, grafo)) {
                colores[indice] = colorCr;
                break;
            }
            colorCr++;
        }
        actual = actual->siguiente;
        indice++;
    }
}

// Modificar la función mostrarGrafoColoreado para imprimir en formato tabular
void mostrarGrafoColoreado(Vertice* grafo, int colores[], int numVertices) {
    const char* nombresColores[] = {"Verde", "Rojo", "Azul", "Amarillo", "Morado", "Naranja"};
    cout << "\nGrafo coloreado:\n";
    cout << "Vertice\tColor\n";
    cout << "-------------------\n";
    Vertice* actualVert = grafo;
    int indice = 0;
    while (actualVert != NULL && indice < numVertices) {
        cout << actualVert->valor << "\t";
        if (colores[indice] >= 0 && colores[indice] < 6) {
            cout << nombresColores[colores[indice]];
        } else {
            cout << "Color " << colores[indice];
        }
        cout << endl;
        actualVert = actualVert->siguiente;
        indice++;
    }
}

int main() {
    Vertice* grafo = NULL;
    int verticeindicado; 
    cout << "Leyendo grafo desde archivo..." << endl;
    grafo = leerGrafoDesdeArchivo("/Users/enr.g/Documents/Taller_2/taller-2/estructura.txt");
    
   
    // Mostrar grafo
    imprimirGrafo(grafo);

    //PRIM
    calcularPRIM(grafo);

    // Coloración del grafo
    int numVertices = contarVertices(grafo);
    int colores[numVertices];
    colorearGrafo(grafo, colores, numVertices);
    mostrarGrafoColoreado(grafo, colores, numVertices);

    cout << "Ingrese de que vertice quiere conseguir el camino mas corto en el algoritmo de Dijkstra (0-9): ";
    cin >> verticeindicado;
    // DIJKSTRA - Encontrar caminos más cortos desde el vértice 0
    encontrarCaminosCortos(grafo, verticeindicado);

    return 0;
}