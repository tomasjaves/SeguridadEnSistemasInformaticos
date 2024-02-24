#include <iostream>
#include <vector>
#include <stdexcept>
#include <bitset>

// Colores para salida por pantalla.
#define MAGENTA "\033[35m" 
#define YELLOW  "\033[33m"
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define RED     "\033[31m"

// Declaramos la longitud máxima de los bits de los números a multiplicar.
const int N = 8;
const std::vector<int> AES_1B = {0, 0, 0, 1, 1, 0, 1, 1};
const std::vector<int> SNOW_3G_A9 = {1, 0, 1, 0, 1, 0, 0, 1};

/**
 * @brief Función que convierte un número entero a su representación binaria con longitud 8.
 * 
 * @param n 
 * @return std::vector<int> 
 */
std::vector<int> ConvertBinary(std::string n) {
  // Verificación de que n está en el rango de 0 a 255
  if (std::stoi(n) < 0 || std::stoi(n) > 255) {
    // Retorna un vector vacío como indicador de error
    return {};
  }
  
  std::bitset<8> result(std::stoi(n, nullptr, 16));
  std::vector<int> binary;
  for (int i = N-1; i >= 0; i--) {
    binary.push_back(result[i]);
  }
  return binary;
}

/**
 * @brief Función que realiza la multiplicación de dos números en el campo finito de Galois.
 * 
 * @param a 
 * @param b 
 * @return std::vector<int> 
 */
std::vector<int> Multiplicacion(std::vector<int> first_operand, std::vector<int> second_operand, bool algoritmo_aes) {
  // Comprobamos que a y b tengan la misma longitud.
  if (first_operand.size() != N || second_operand.size() != N || first_operand.size() != second_operand.size()) {
    throw std::invalid_argument("Los vectores a y b deben tener longitud N.");
  }
  std::vector<std::vector<int>> result;
  // Recorremos el segundo operando.
  for (int i = 0; i < N; ++i) {
    // Si el bit es 1.
    if (second_operand[i] == 1) {
      // Creamos un vector temporal.
      std::vector<int> temp(N);
      // Copiamos el primer operando en el vector temporal.
      for (int j = 0; j < N; j++) {
        temp[j] = first_operand[j];
      }
      int pos;
      // Nos quedamos con la posición del bit que es 1.
      for (int j = 0; j < N; j++) {
        if (j == i) {
          pos = 7 - j;
        }
      }
      // Realizamos el desplazamiento y el XOR con AES_1B iterando pos veces.
      for (int j = 0; j < pos; j++) {
        // Si el bit más significativo es 0, desplazamos. Si es uno, desplazamos y hacemos XOR con AES_1B.
        if (temp[0] == 0) {
          for (int k = 0; k < N - 1; k++) {
            temp[k] = temp[k + 1];
          }
          temp[N - 1] = 0;
        } else {
          for (int k = 0; k < N - 1; k++) {
            temp[k] = temp[k + 1];
          }
          temp[N - 1] = 0;
          if (algoritmo_aes == true) {
            for (int k = 0; k < N; k++) {
              temp[k] = temp[k] ^ AES_1B[k];
            }
          } else {
            for (int k = 0; k < N; k++) {
              temp[k] = temp[k] ^ SNOW_3G_A9[k];
            }
          }
        }
      }
      // Guardamos el resultado en un vector.
      result.push_back(temp);
    }
  }
  
  // Hacemos el XOR de todos los resultados.
  for (int i = 1; i < result.size(); ++i) {
    for (int j = 0; j < N; ++j) {
      result[0][j] = result[i][j] ^ result[0][j];
    }
  }
  return result[0];
}

void PrintOutput(std::vector<int> result, std::vector<int> first_operand, std::vector<int> second_operand, bool algoritmo_aes) {
  std::cout << GREEN << BOLD << "\nResultado de la multiplicación: \n" << RESET;
  std::cout << BOLD << "Primer byte: " << RESET;
  for (int i = 0; i < N; i++) {
    std::cout << first_operand[i];
  }
  std::cout << std::endl;
  std::cout << BOLD << "Segundo byte: " << RESET;
  for (int i = 0; i < N; i++) {
    std::cout << second_operand[i];
  }
  std::cout << std::endl;
  std::cout << BOLD << "Byte Algoritmo: " << RESET;
  if (algoritmo_aes == true) {
    for (int i = 0; i < N; i++) {
      std::cout << RED << BOLD << AES_1B[i];
    }
  } else {
    for (int i = 0; i < N; i++) {
      std::cout << RED << BOLD << SNOW_3G_A9[i];
    }
  }
  std::cout << RESET << std::endl;
  std::cout << BOLD << "Multiplicación: " << RESET;
  for (int i = 0; i < N; i++) {
    std::cout << result[i];
  }
  std::cout << std::endl << std::endl;
}

int main() {
  std::string algoritmo, kOperando1, kOperando2;
  bool algoritmo_aes = false;
  std::cout << CYAN << BOLD << "\n\t\tMultiplicación AES y SNOW3G" << RESET << std::endl << std::endl; 
  std::cout << GREEN << BOLD << "Introduce los bytes a multiplicar y el algoritmo a utilizar." << RESET << std::endl;
  std::cout << BOLD << "Primer byte: " << RESET;
  std::cin >> kOperando1;
  std::cout << BOLD << "Segundo byte: " << RESET;
  std::cin >> kOperando2;
  std::cout << BOLD << "Algoritmo: " << RESET;
  std::cin >> algoritmo;
  std::vector<int> first_operand = ConvertBinary(kOperando1);
  std::vector<int> second_operand = ConvertBinary(kOperando2);
  // Si el algoritmo es AES.
  if (algoritmo == "AES") {
    algoritmo_aes = true;
    std::vector<int> result = Multiplicacion(first_operand, second_operand, algoritmo_aes);
    if (result.empty()) throw std::invalid_argument("Los vectores a y b deben tener longitud N.");
    PrintOutput(result, first_operand, second_operand, algoritmo_aes);
  } else if (algoritmo == "SNOW3G") { // Si el algoritmo es SNOW3G.
    std::vector<int> result = Multiplicacion(first_operand, second_operand, algoritmo_aes);
    if (result.empty()) throw std::invalid_argument("Los vectores a y b deben tener longitud N.");
    PrintOutput(result, first_operand, second_operand, algoritmo_aes);
  } else { // Si el algoritmo no es AES ni SNOW3G.
    std::cout << "Algoritmo no soportado." << std::endl;
  }
  return 0;
}