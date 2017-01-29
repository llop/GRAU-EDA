#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef vector<int> IV;
typedef vector<IV> G;
typedef vector<bool> BV;
typedef queue<int> IQ;


G g;     // grafo: lista de adyacencias
BV vis;  // array de visitados


void bfs(int x) {
  // añadir el nodo raiz a la cola
  IQ que;
  que.push(x);
  vis[x] = true;
  // explorar los nodos en el orden en que se han ido descubriendo
  while (!que.empty()) {
    x = que.front();
    que.pop();
    // Tratar nodo (imprimir)
    cout << x << endl;
    for (int y : g[x]) {
      // encolar nodos no visitados
      if (!vis[y]) {
	vis[y] = true;
	que.push(y);
      }
    }
  }
}

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  //memset(vis, 0, sizeof vis);  // inicializar visitados a false
  g = G(101);
  vis = BV(101, false);
  g[0].push_back(1);
  g[0].push_back(2);
  g[1].push_back(3);
  g[2].push_back(3);
  g[3].push_back(4);
  
  bfs(0);
  
  return 0;
}

