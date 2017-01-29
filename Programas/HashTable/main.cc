#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

typedef unordered_map<string, int> SM;
typedef pair<string, int> SP;
typedef SM::iterator SMI;
typedef vector<SMI> IV;

SM urls;
IV its;

void ins(string url) {
  if (urls.find(url) == urls.end()) {
    SMI it = urls.insert(urls.end(), SP(url, its.size()));
    its.push_back(it);
  }
}

void rem(string url) {
  SMI it = urls.find(url);
  if (it != urls.end()) {
    int p = it->second;
    its[p] = its.back();
    urls[its[p]->first] = p;
    urls.erase(url);
    its.pop_back();
  }
}

int random_int(int a, int b) {
  return a + rand() % (b - a + 1);
}

string random_url() {
  int p = random_int(0, its.size() - 1);
  return its[p]->first;
}

int main() {
  // esta funcion accelera el cin/cout
  ios_base::sync_with_stdio(0);
  
  ins("adios");
  ins("gbye");
  ins("hola");
  ins("hola");
  ins("hola");
  cout << its.size() << endl;
  cout << random_url() << endl;
  rem("hola");
  rem("adios");
  cout << urls.size() << ' ' << its.size() << endl;
  cout << random_url() << endl;
  
  return 0;
}

