#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

typedef vector<int> IV;
typedef pair<int, int> IP;
typedef vector<IP> IPV;
typedef vector<IPV> G;

// struct que representa una arista ()
struct E {
  int w;     // peso de la arista
  int x, y;  // vertices de la arista
  
  // operador menor-que
  // se necesita para la priority queue, que debe ordenar por el peso 'w'
  inline friend bool operator<(const E &a, const E &b) {
    return a.w > b.w;
  }
};
typedef priority_queue<E> EQ;


G g;     // grafo: lista de adyacencias
IV par;  // array de padres del MST


void prim() {
  // inicializar el array de padres
  // al principio, ningun vertice tiene padre (indicado con -1)
  par = IV(g.size(), -1);
  // inicializar la cola de prioridad
  // añadimos una arista "fantasma" que une el vertice origen consigo mismo
  // asi conseguimos que este vertice origen sea la raiz del MST
  EQ que;
  que.push({ 0, 0, 0 });
  while (!que.empty()) {
    // la arista que sacamos es la de menor peso de todas las que hemos examinado
    const E &e = que.top();
    int x = e.x;
    int y = e.y;
    que.pop();
    if (par[y] == -1) {
      // si el vertice 'y' no tenia padre, se lo asignamos ahora
      // luego añadimos a la cola todas las aristas que conectan con 'y'
      par[y] = x;
      for (const IP &a : g[y]) {
	int w = a.first;
	int z = a.second;
	que.push({ w, y, z });
      }
    }
  }
}

inline void add_edge(int x, int y, int w) {
  g[x].push_back(IP(w, y));
  g[y].push_back(IP(w, x));
}

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  // test case 1:
  g = G(9);
  add_edge(0, 1, 4);
  add_edge(0, 7, 8);
  add_edge(1, 2, 8);
  add_edge(1, 7, 11);
  add_edge(2, 3, 7);
  add_edge(2, 8, 2);
  add_edge(2, 5, 4);
  add_edge(3, 4, 9);
  add_edge(3, 5, 14);
  add_edge(4, 5, 10);
  add_edge(5, 6, 2);
  add_edge(6, 7, 1);
  add_edge(6, 8, 6);
  add_edge(7, 8, 7);
  
  
  /*
  // test case 2:
  g = G(4);
  add_edge(0, 1, 1);
  add_edge(0, 3, 1);
  add_edge(1, 3, 2);
  add_edge(2, 3, 3);
  */
  
  prim();
  
  for (int i = 1; i < (int)g.size(); ++i) {
    cout << par[i] << ' ' << i << endl;
  }
    
  return 0;
}














