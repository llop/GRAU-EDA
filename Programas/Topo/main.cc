#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
using namespace std;


typedef vector<int> IV;
typedef vector<IV> G;
typedef queue<int> IQ;


int n;   // numero de vertices
G g;     // grafo: lista de adyacencias
IV deg;  // array de grados
//string clo[101];  // prendas


void topo() {
  // contar el grado de cada vertices
  for (int i = 0; i < (int)g.size(); ++i) {
    for (int x : g[i]) {
      ++deg[x];
    }
  }
  // encolar aquellos vertices de grado 0
  IQ que;
  for (int i = 0; i < (int)g.size(); ++i) {
    if (!deg[i]) que.push(i);
  }
  while (!que.empty()) {
    int x = que.front();
    que.pop();
    //cout << clo[x] << endl;
    cout << x << endl;
    for (int y : g[x]) {
      --deg[y];
      if (!deg[y]) que.push(y);
    }
  }
}

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  n = 6;
  g = G(n);
  deg = IV(n, 0);
  
  /*
  // Pollo curry con arroz
  // Ordenacion topologica
  clo[0] = "Freir tacos pollo";
  clo[1] = "Cocer verduras con salsa";
  clo[2] = "Cocer arroz";
  clo[3] = "Trocear pollo";
  clo[4] = "Trocear cebolla, zanahoria y ajo";
  clo[5] = "Batir verduras con salsa";
  clo[6] = "Mezclar tacos pollo, limon y curry";
  clo[7] = "Pochar verduras";
  clo[8] = "Juntar pollo y salsa con arroz";
  clo[9] = "Cocer pollo con salsa";
  */
  /*
  // prcedencias
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  g[].push_back();
  */
  //memset(deg, 0, sizeof deg);  // inicializar visitados a false
  
  
  topo();
  
  return 0;
}

