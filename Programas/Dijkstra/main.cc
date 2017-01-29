#include <iostream>
#include <limits>
#include <vector>
#include <queue>
using namespace std;


typedef vector<int> IV;
typedef pair<int, int> IP;
typedef vector<IP> IPV;
typedef vector<IPV> G;
typedef priority_queue<IP, IPV, greater<IP>> IQ;


G g;     // grafo: lista de adyacencias
IV dis;  // array de distancias


void dijkstra(int v) {
  // distancia al nodo origen = 0; infinito para los demas
  dis = IV(g.size(), numeric_limits<int>::max());
  dis[v] = 0;
  // añadir a la cola el primer par (distancia a nodo, nodo)
  // la distancia debe ser el primer elemento del par, ya que
  // priority_queue ordena los pair segun el primer elemento, y luego el 2º
  IQ que;
  que.push(IP(0, v));
  // tratar sucesivamente los nodos a menor distancia
  while (!que.empty()) {
    const IP &p = que.top();
    int x = p.second;  // nodo que vamos a tratar
    int d = p.first;   // distancia a este nodo
    que.pop();
    // trataremos cada nodo solo una vez:
    // otras apariciones de este en la cola se habran añadido antes
    // y estaran a mayor distancia
    if (d <= dis[x]) {
      // examinar todas la aristas que salen del nodo 'x'
      for (const IP &a : g[x]) {
	int y = a.second;  // nodo vecino
	int c = a.first;   // distancia desde el nodo 'x' hasta el nodo vecino
	// si la distancia que hemos sacado de la cola + distancia al nodo vecino
	// iguala o mejora la aproximación que tenemos guardada en el array 'dis',
	// establecerla y encolar el par (nueva distancia mejorada, nodo vecino)
	if (dis[y] > d + c) {
	  dis[y] = d + c;
	  que.push(IP(dis[y], y));
	}
      }
    }
  }
}


int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  g = G(6);
  g[0].push_back(IP(7, 1));
  g[0].push_back(IP(14, 5));
  g[0].push_back(IP(9, 2));
  g[1].push_back(IP(10, 2));
  g[1].push_back(IP(15, 3));
  g[2].push_back(IP(2, 5));
  g[2].push_back(IP(11, 3));
  g[3].push_back(IP(6, 4));
  g[5].push_back(IP(9, 4));
  
  dijkstra(0);
  for (int i = 0; i < (int)g.size(); ++i)
    cout << i << ' ' << dis[i] << endl;
  
  return 0;
}

