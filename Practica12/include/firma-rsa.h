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
#define MAGENTA "\033[35m"
#define YELLOW "\033[33m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"

const int kIteraciones = 100;
const int kBase = 26; // Base del alfabeto sin Ñ

class RSA {
 public:
  RSA(int, int, int, std::string);
  RSA(const int&, const int&, const std::string&);

  bool LehmanPeralta(int);
  int ExponenciacionRapida(long long, unsigned int, int);
  int McdExtendido(int, int, int&, int&);
  bool InversoModular(int, int);

  void SetBlockSize();
  void ToDecimal();
  void EraseSpaces();

  void Encrypt();
  void Decode(const int&);

  void Ink();
  void Verify();

 private:
  int p_, q_, d_, phi_, e_, n_, blockSize_;
  int e_A, n_A, valor_texto_;
  std::string texto_, texto_descifrado_;
  std::vector<int> texto_decimal_, texto_cifrado_;
};