#include <iostream>
#include <vector>
#include <limits>
using namespace std;

typedef pair<int, int> IP;
typedef vector<IP> IPV;
typedef vector<IPV> G;
typedef vector<int> IV;

int n, m;  // numero de vertex i numero d'arestes
int s, t;  // vertex origen i vertex desti
G g;       // el graf: llista d'adjacencies
IV dis;    // vector de distancies
IV par;    // vector de pares

// retorna true si no hi ha cicles negatius
bool bellman_ford() {
  // en cada iteracio mirem, per totes les arestes del graf, 
  // si hem trobat una distancia menor que la que tenim
  for (int i = 1; i < n; ++i) {
    for (int x = 0; x < n; ++x) {
      // important! assegurar-se que la distància al vèrtex 'x' s'hagi establert
      // si 'x' no té pare, la distància serà 'numeric_limits<int>::max()', 
      // i sumar-li la distància al vèrtex 'y' no té sentit (apart que pot donar overflow)
      if (par[x] != -1) {
	for (const IP &a : g[x]) {
	  int y = a.first;
	  int d = a.second;
	  if (dis[y] > dis[x] + d) {
	    dis[y] = dis[x] + d;
	    par[y] = x;
	  }
	}
      }
    }
  }
  // per acabar mirem si hi ha algun cicle negatiu
  for (int x = 0; x < n; ++x) {
    if (par[x] != -1) {
      for (const IP &a : g[x]) {
	int y = a.first;
	int d = a.second;
	if (dis[y] > dis[x] + d) {
	  return false;
	}
      }
    }
  }
  return true;
}

// funció per imprimir el camí de vèrtex
void print(int x) {
  if (par[x] == x) {
    cout << x;
  } else {
    print(par[x]);
    cout << ' ' << x;
  }
}

void print_sol() {
  if (par[t] != -1) {
    // si el vèrtex destí té un pare, és que hi podem arribar
    // imprimir la distància i el camí
    cout << "Distància: " << dis[t] << endl;
    cout << "Camí: ";
    print(t);
    cout << endl;
  } else {
    // no es pot arribar al vèrtex destí
    cout << "No hi ha camí des de " << s << " fins " << t << endl;
  }
}

int main() {
  // llegir num vertex i arestes
  while (cin >> n >> m) {
    // inicialitza estructures dades
    g = G(n);
    dis = IV(n, numeric_limits<int>::max());  // distancies inicialment a infinit
    par = IV(n, -1);                          // pares no establerts
    // llegir arestes
    for (int i = 0; i < m; ++i) {
      int x, y, d;                 // vèrtex origen, vèrtex destí, i distància
      cin >> x >> y >> d;
      g[x].push_back(IP(y, d));
    }
    // llegir origen i destí
    cin >> s >> t;
    dis[s] = 0;     // distancia al vèrtex inicial és 0
    par[s] = s;     // pare del vèrtex inicial és ell mateix
    // executar bellman-ford
    bool sol = bellman_ford();
    // si no hi ha cicles negatius, 'sol' serà true
    if (sol) {
      print_sol();
    } else {
      // quan hi ha un cicle negatiu, el podriem recorrer infinites vegades
      // i la distancia al destí no pararia de decrementar
      cout << "Cicle negatiu trobat: no existeix camí més curt!" << endl;
    }
    cout << endl;
  }
  return 0;
}
