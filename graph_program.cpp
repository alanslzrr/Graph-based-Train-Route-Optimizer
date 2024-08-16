// Libraries
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <string>
using namespace std;

// Define the Edge structure
struct Edge {
    int to;
    float weight;
    Edge(int t, float w) : to(t), weight(w) {}
};

class GrafoDP {
 private:
  int V;
  vector<vector<Edge>> adjList; 
  vector<string> nombres; // Vector to store city names

 public:
  GrafoDP(int V, vector<string> nom) : V(V), adjList(V), nombres(nom) {}

  // Method to add an edge between two vertices with its respective weight
  void Agregar_Arista(int vs, int ve, float p) {
    adjList[vs].push_back(Edge(ve, p));
    adjList[ve].push_back(Edge(vs, p)); // To make it bidirectional
  }

  // Method to display the graph
  void Muestra_GrafoDP() {
    for (int i = 0; i < V; ++i) {
      cout << "VERTEX : " << i << " " << nombres[i] << "\tOut Degree : " << Grado_Out(i) << "\tIn Degree : " << Grado_In(i);
      cout << " Adjacent : ";
      for (auto& edge : adjList[i]) {
        cout << nombres[edge.to] << "(" << edge.weight << ") ";
      }
      cout << endl;
    }
  }

  // Method to get the out-degree of a vertex
  int Grado_Out(int n) {
    return adjList[n].size();
  }

  // Method to get the in-degree of a vertex
  int Grado_In(int n) {
    int grado_in = 0;
    for (auto& lista : adjList) {
      for (auto& edge : lista) {
        if (edge.to == n) {
          ++grado_in;
        }
      }
    }
    return grado_in;
  }

  // Method to find and display all possible paths between two vertices
  void Caminos(int VO, int VD);
  void Buscar_CaminosAux(int V_Actual, int VD, vector<bool>& Visto, vector<int>& Ruta, int& Peso_T);

  // Method to find and display the minimum path between two vertices using Dijkstra
  void Camino_Minimo_Dijkstra(int VO, int VD);

  // Method to find and display the minimum path between two vertices using modified BFS
  void Camino_Minimo_BFS(int VO, int VD);
};

// Recursive method to find all possible paths between two vertices
void GrafoDP::Caminos(int VO, int VD) {
    vector<bool> Visto(V, false);
    vector<int> Ruta;
    int Peso_T = 0;
    cout << "Paths from " << VO << " to " << VD << ":" << endl;
    Buscar_CaminosAux(VO, VD, Visto, Ruta, Peso_T);
}

// Auxiliary function to search for paths between two vertices
void GrafoDP::Buscar_CaminosAux(int V_Actual, int VD, vector<bool>& Visto, vector<int>& Ruta, int& Peso_T) {
    Visto[V_Actual] = true;
    Ruta.push_back(V_Actual);
    
    if (V_Actual == VD) {
        // Print the found path
        for (size_t i = 0; i < Ruta.size(); i++) {
            if (i > 0) cout << " -> ";
            cout << Ruta[i];
        }
        cout << "\tCost: " << Peso_T << endl;
    } else {
        for (const Edge& e : adjList[V_Actual]) {
            if (!Visto[e.to]) {
                int peso_original = Peso_T;
                Peso_T += e.weight;
                Buscar_CaminosAux(e.to, VD, Visto, Ruta, Peso_T);
                Peso_T = peso_original; // Restore the original weight when backtracking
            }
        }
    }
    
    Ruta.pop_back();
    Visto[V_Actual] = false;
}

// Method to find the minimum path between two vertices using Dijkstra
void GrafoDP::Camino_Minimo_Dijkstra(int VO, int VD) {
    vector<float> dist(V, numeric_limits<float>::max());
    vector<int> prev(V, -1);
    vector<bool> visitado(V, false);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;
    
    dist[VO] = 0;
    pq.push(make_pair(0, VO));
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visitado[u]) continue; // If we've already visited this node, continue with the next one.
        visitado[u] = true; // Mark as visited
        
        if (u == VD) break; // If we've reached the destination, we're done.
        
        for (const Edge& e : adjList[u]) {
            int v = e.to;
            float weight = e.weight;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    
    // Reconstruct the path
    stack<int> path;
    int u = VD;
    float total_cost = dist[VD];
    while (prev[u] != -1) {
        path.push(u);
        u = prev[u];
    }
    path.push(u); // Add the source node

    // Display the path
    cout << "Shortest path using Dijkstra: ";
    while (!path.empty()) {
        cout << path.top();
        path.pop();
        if (!path.empty()) cout << " -> ";
    }
    cout << "\nCost: " << total_cost << endl;
}

// Method to find the minimum path between two vertices using modified BFS
void GrafoDP::Camino_Minimo_BFS(int VO, int VD) {
    vector<float> dist(V, numeric_limits<float>::max());
    vector<int> prev(V, -1);
    queue<int> q;
    
    dist[VO] = 0;
    q.push(VO);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (const Edge& e : adjList[u]) {
            int v = e.to;
            float weight = e.weight;
            
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                q.push(v);
            }
        }
    }

    stack<int> camino;
    int nodo_actual = VD;
    if (prev[nodo_actual] == -1) {
        cout << "There is no path from " << VO << " to " << VD << endl;
        return;
    }

    while (nodo_actual != -1) {
        camino.push(nodo_actual);
        nodo_actual = prev[nodo_actual];
    }
    
    cout << "Shortest path using modified BFS: ";
    while (!camino.empty()) {
        int nodo = camino.top();
        camino.pop();
        cout << nodo;
        if (!camino.empty()) cout << " -> ";
    }
    cout << "\nCost: " << dist[VD] << endl;
}

int main() {
    // Define the city names corresponding to each index
    vector<string> nombres = {
        "A Coruña", "Santiago", "Padrón", "Vila García de A.", "Pontevedra", 
        "Vigo", "Sanxenxo", "Poio", "Forcarei", "A Estrada", "Noia", 
        "Carballino", "Orense", "Lalin"
    };
    
    // Initialize the graph with the correct number of nodes and city names
    GrafoDP GND(14, nombres); 
    
    // Add edges according to the provided map
    GND.Agregar_Arista(0, 1, 22);
    GND.Agregar_Arista(1, 2, 8);
    GND.Agregar_Arista(1, 9, 15);
    GND.Agregar_Arista(1, 10, 20);
    GND.Agregar_Arista(1, 13, 35);
    GND.Agregar_Arista(2, 3, 3);
    GND.Agregar_Arista(3, 4, 15);
    GND.Agregar_Arista(4, 5, 25);
    GND.Agregar_Arista(4, 7, 8);
    GND.Agregar_Arista(4, 8, 17);
    GND.Agregar_Arista(6, 7, 15);
    GND.Agregar_Arista(8, 9, 8);
    GND.Agregar_Arista(8, 11, 15);
    GND.Agregar_Arista(11, 12, 10);
    GND.Agregar_Arista(12,13,15);

    // Display the graph.
    cout << "**** WEIGHTED DIRECTED GRAPH ****" << endl;
    GND.Muestra_GrafoDP();
    cout << endl;

    // Ask the user to enter the source and destination vertices.
    int vi, vf;
    cout << "SOURCE VERTEX: ";
    cin >> vi;
    cout << "DESTINATION VERTEX: ";
    cin >> vf;
    cout << endl;

    // Find and display all possible paths from source to destination.
    cout << "--- PATHS BETWEEN VERTICES [" << vi << ", " << vf << "]" << endl;
    GND.Caminos(vi, vf);
    cout << endl;

    // Find and display the minimum path from source to destination using Dijkstra.
    cout << "--- MINIMUM PATH BETWEEN VERTICES [" << vi << ", " << vf << "] - DIJKSTRA" << endl;
    GND.Camino_Minimo_Dijkstra(vi, vf);
    cout << endl;

    // Find and display the minimum path from source to destination using modified BFS.
    cout << "--- MINIMUM PATH BETWEEN VERTICES [" << vi << ", " << vf << "] - MODIFIED BFS" << endl;
    GND.Camino_Minimo_BFS(vi, vf);
    
    return 0;
}