#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define ITALIC "\033[3m"

const int kIteraciones = 100;
const int kBase = 26; // Base del alfabeto sin Ñ

class RSA {
 public:
  RSA(int, int, int, std::string);

  bool LehmanPeralta(int);
  int ExponenciacionRapida(long long, unsigned int, int);
  int McdExtendido(int, int, int&, int&);
  bool InversoModular(int, int);

  void SetBlockSize();
  void ToDecimal();
  void EraseSpaces();

  void Encrypt();

 private:
  int p_, q_, d_, phi_, e_, n_, blockSize_;
  std::string texto_;
  std::vector<int> texto_decimal_, texto_cifrado_;
};