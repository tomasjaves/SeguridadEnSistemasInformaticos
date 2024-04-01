#include <iostream>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define ITALIC  "\033[3m"

/**
 * @brief Función que realiza la lectura de los valores de p, g, xA y xB
 * 
 * @param p 
 * @param g 
 * @param xA 
 * @param xB 
 */
void ReadData(int &p, int &g, int &xA, int &xB, int &xC) {
  std::cout << "Ingrese el valor de p: ";
  std::cin >> p;
  std::cout << "Ingrese el valor de g: ";
  std::cin >> g;
  std::cout << "Ingrese el valor de xA: ";
  std::cin >> xA;
  std::cout << "Ingrese el valor de xB: ";
  std::cin >> xB;
  std::cout << "Ingrese el valor de xC: ";
  std::cin >> xC;
  std::cout << std::endl;
}

/**
 * @brief Función que realiza el cálculo de yA y yB
 * 
 * @param p 
 * @param g 
 * @param xA 
 * @param xB 
 * @param yA 
 * @param yB 
 * @param kA 
 * @param kB 
 */
int FastExponentiation(int p, int g, int xA) {
  int y = 1;
  while (xA > 0) {
    if (xA % 2 == 0) {
      xA /= 2;
      g = (g * g) % p;
    } else {
      xA -= 1;
      y = (y * g) % p;
    }
  }
  return y;
}

/**
 * @brief Función que muestra los resultados de p, g, xA, xB, yA, yB, kA y kB
 * 
 * @param p 
 * @param g 
 * @param xA 
 * @param xB 
 * @param yA 
 * @param yB 
 * @param k
 */
void MostrarResultados(const int& p, const int& g, const int& xA, const int& xB, const int& xC, const int& yA, const int& yB, const int& k) {
  std::cout << BOLD << CYAN << "p =  " << RESET <<  p << BOLD << CYAN <<", g = " << RESET << g << BOLD << CYAN << ", xA = " << RESET << xA << BOLD << CYAN << ", xB = " << RESET << xB << BOLD << CYAN << ", xC = " << RESET << xC << BOLD << CYAN << ", yA = " << RESET << yA << BOLD << CYAN << ", yB = " << RESET << yB << BOLD << CYAN << ", k = " << RESET << k << std::endl;
}

/**
 * @brief Función que realiza el cálculo de kA y kB
 * 
 * @param p 
 * @param g 
 * @param xA 
 * @param xB 
 * @param yA 
 * @param yB 
 * @param kA 
 * @param kB 
 */
int DiffieHellman(int& p, int& g, int& xA, int& xB, int &xC, int& yA) {
  int result, result2;
  yA = FastExponentiation(p, g, xA);
  result = FastExponentiation(p, yA, xB);
  result2 = FastExponentiation(p, result, xC);
  MostrarResultados(p, g, xA, xB, xC, yA, result, result2);
  return result;
}

/**
 * @brief Función para calcular los ejemplos de prueba
 * 
 * @return int 
 */
void Test() {
  int p = 23, g = 5, xA = 6, xB = 8, xC = 10, yA, k;
  k = DiffieHellman(p, g, xA, xB, xC, yA);
  k = DiffieHellman(p, g, xB, xC, xA, yA);
  k = DiffieHellman(p, g, xC, xA, xB, yA);
}

int main() {
  std::cout << std::endl;
  std::cout << BOLD << RED << "\tINTERCAMBIO DE CLAVES DE DIFFIE-HELLMAN " << RESET << std::endl << std::endl;
  std::cout << ITALIC << "¿Le gustaría ingresar los valores de p, g, xA y xB? (s/n): " << RESET;
  char opcion;
  std::cin >> opcion;
  if (opcion == 's') {
    int p, g, xA, xB, xC, yA, yB, yC, k;
    ReadData(p, g, xA, xB, xC);
    k = DiffieHellman(p, g, xA, xB, xC, yA);
    k = DiffieHellman(p, g, xB, xC, xA, yB);
    k = DiffieHellman(p, g, xC, xA, xB, yC);
  } else if (opcion == 'n') {
    std::cout << std::endl;
    std::cout << BOLD << RED << "EJEMPLOS DE PRUEBA" << RESET << std::endl << std::endl;
    std::cout << BOLD << YELLOW << "Resultados: " << RESET << std::endl;
    Test();
  } else {
    std::cout << "Opción no válida." << std::endl;
  }
  std::cout << std::endl;
  return 0;
}