#include <iostream>
#include <cstring>
#include <vector>
#include <stack>
using namespace std;

typedef vector<int> IV;
typedef vector<IV> G;
typedef vector<bool> BV;
typedef stack<int> IS;


G g;     // grafo: lista de adyacencias
BV vis;  // array de visitados


void dfs(int x) {
  vis[x] = true;
  // Tratar nodo: en este ejemplo tan solo imprime
  cout << x << endl;
  for (int y : g[x]) {
    if (!vis[y]) {
      dfs(y);
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
  
  dfs(0);
  
  return 0;
}

