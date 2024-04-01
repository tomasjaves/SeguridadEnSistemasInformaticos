#include <iostream>
#include <vector>
#include <utility>

// Colores para la consola en negrita.
#define BOLD    "\033[1m"
// Colores para la consola en colores
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// Tipo para representar un PRN (secuencia de bits)
using PRNs = std::vector<std::pair<int, int>>;

/**
 * @brief Función que almacena todos los PRNs.
 * 
 * @param pol_g1 
 * @param pol_g2 
 * @return PRNs 
 */
void ConstructPRNs(PRNs& prns) {
  prns.push_back(std::make_pair(2, 6));
  prns.push_back(std::make_pair(3, 7));
  prns.push_back(std::make_pair(4, 8));
  prns.push_back(std::make_pair(5, 9));
  prns.push_back(std::make_pair(1, 9));
  prns.push_back(std::make_pair(2, 10));
  prns.push_back(std::make_pair(1, 8));
  prns.push_back(std::make_pair(2, 9));
  prns.push_back(std::make_pair(3, 10));
  prns.push_back(std::make_pair(2, 3));
  prns.push_back(std::make_pair(3, 4));
  prns.push_back(std::make_pair(5, 6));
  prns.push_back(std::make_pair(6, 7));
  prns.push_back(std::make_pair(7, 8));
  prns.push_back(std::make_pair(8, 9));
  prns.push_back(std::make_pair(9, 10));
  prns.push_back(std::make_pair(1, 4));
  prns.push_back(std::make_pair(2, 5));
  prns.push_back(std::make_pair(3, 6));
  prns.push_back(std::make_pair(4, 7));
  prns.push_back(std::make_pair(5, 8));
  prns.push_back(std::make_pair(6, 9));
  prns.push_back(std::make_pair(1, 3));
  prns.push_back(std::make_pair(4, 6));
  prns.push_back(std::make_pair(5, 7));
  prns.push_back(std::make_pair(6, 8));
  prns.push_back(std::make_pair(7, 9));
  prns.push_back(std::make_pair(8, 10));
  prns.push_back(std::make_pair(1, 6));
  prns.push_back(std::make_pair(2, 7));
  prns.push_back(std::make_pair(3, 8));
  prns.push_back(std::make_pair(4, 9));
}

/**
 * @brief Función que construye los polinomios con todas las posiciones a 1.
 * 
 * @return std::vector<int> 
 */
std::vector<int> ConstructPol() {
  // Le damos un tamaño de 10 para que sea un polinomio de 10 bits.
  std::vector<int> polinomio(10);
  // Llenamos el polinomio con 1s.
  for (int i = 0; i < polinomio.size(); ++i) {
    polinomio[i] = 1;
  }
  return polinomio;
}

/**
 * @brief Función que se encarga del desplazamiento de G1.
 * 
 * @param vec 
 */
int DesplazamientoG1(std::vector<int>& vec) {
  // Guardamos los valores de las posiciones 3 y 10.
  int vec_3 = vec[2], vec_10 = vec[9], result = vec_10 ^ vec_3;
  // Hacemos un XOR entre ellos y lo guardamos en la primera posición.
  vec.insert(vec.begin(), result);
  vec.pop_back();
  return result;
}

/**
 * @brief Función que se encarga del desplazamiento de G2.
 * 
 * @param vec 
 */
int DesplazamientoG2(std::vector<int>& vec) {
  // Guardamos los valores de las posiciones 2, 3, 6, 8, 9 y 10.
  int vec_2 = vec[1], vec_3 = vec[2], vec_6 = vec[5], vec_8 = vec[7], vec_9 = vec[8], vec_10 = vec[9],
  // Hacemos un XOR entre ellos y lo guardamos en la primera posición.
  result = vec_10 ^ vec_9 ^ vec_8 ^ vec_6 ^ vec_3 ^ vec_2;
  vec.insert(vec.begin(), result);
  vec.pop_back();
  return result;
}

/**
 * @brief Función que muestra los bits resultantes de las operaciones.
 * 
 * @param result 
 */
void MostrarCA(std::vector<int> pol_g1, std::vector<int> pol_g2, int bitRealimentacion1, int bitRealimentacion2) {
  // Mostramos los bits resultantes de las operaciones del primer polinomio junto con el bit de realimentación.
  for (int i = 0; i < pol_g1.size(); ++i) {
    std::cout << pol_g1[i] << " ";
  }
  std::cout << GREEN << BOLD << "| Bit de realimentación: " << RESET << bitRealimentacion1 << "\t ||  ";
  // Mostramos los bits resultantes de las operaciones del segundo polinomio junto con el bit de realimentación.
  for (int i = 0; i < pol_g2.size(); ++i) {
    std::cout << pol_g2[i] << " ";
  }
  std::cout << GREEN << BOLD << "| Bit de realimentación: " << RESET << bitRealimentacion2 << std::endl;
}

/**
 * @brief Función que genera C/A CODE.
 * 
 * @param pol_g1 
 * @param pol_g2 
 * @return std::vector<int> 
 */
std::vector<int> GenerateCA(std::vector<int> pol_g1, std::vector<int> pol_g2, std::pair<int, int> prn, int longitud) {
  // Construimos los polinomios.
  pol_g1 = ConstructPol();
  pol_g2 = ConstructPol();
  // Vector que almacena los resultados.
  std::vector<int> result, result_g1, result_g2;
  // SE MOSTRARÁN LOS BITS DE REALIMENTACIÓN DE G1 Y G2 JUNTO CON LOS POLINOMIOS DESPLAZADOS.
  std::cout << MAGENTA << BOLD << "\t\t  LFSR1 \t\t\t\t\t       LFSR2" << RESET << std::endl;
  // Bucle que se encarga de generar la secuencia de bits de tamaño "longitud".
  for (int i = 0; i < longitud; ++i) {
    // Guardamos el último valor del polinomio de realimentación.
    int back_g1 = pol_g1[pol_g1.size()-1];
    // Guardamos el valor de la operación XOR entre los valores de las posiciones que nos indica el PRN.
    // int back_g2 = pol_g2[prn.first-1] ^ pol_g2[prn.second-1];
    int back_g2 = pol_g2[pol_g2.size()-1];
    // Mostramos los bits resultantes de las operaciones utilizando las funciones de desplazamiento que se encargan de realizar las operaciones necesarias.
    MostrarCA(pol_g1, pol_g2, DesplazamientoG1(pol_g1), DesplazamientoG2(pol_g2));
    // Guardamos el resultado de la operación XOR entre los valores guardados.
    result.push_back(back_g1 ^ back_g2);
  }
  return result;
}

/**
 * @brief Función que muestra el resultado de la secuencia generada.
 * 
 * @param result 
 */
void MostrarResultado(std::vector<int> result) {
  std::cout << "\n";
  std::cout << YELLOW << BOLD << "Secuencia generada:" << RESET << std::endl;
  for (int i = 0; i < result.size(); ++i) {
    std::cout << result[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  // Inicializamos las variables necesarias.
  PRNs prns;
  int n, longitud;
  std::vector<int> pol_g1, pol_g2, result;
  // Construimos los PRNs.
  ConstructPRNs(prns);
  // std::cout << BOLD << "¿Qué PNR desea generar? (1-32): " << RESET;
  // std::cin >> n;
  std::cout << BOLD << "¿Qué longitud desea?: " << RESET;
  std::cin >> longitud;
  // Verificamos que el valor ingresado sea válido. Si no lo es, escogemos el PRN 1 por defecto.
  if (n < 1 || n > prns.size()) {
    std::cout << std::endl;
    std::cout << BLUE << BOLD <<"Se ha escogido un valor inválido. Por defecto escogeremos el PRN 1." << RESET;
    n = 1;
  }
  std::cout << std::endl;
  std::cout << BLUE << BOLD << "Generando C/A CODE para PRN " << n << " (TAPS(" << prns[n-1].first << ", " << prns[n-1].second << "))." << RESET << std::endl << std::endl;
  // Generamos el C/A CODE.
  result = GenerateCA(pol_g1, pol_g2, prns[n-1], longitud);
  // Mostramos el resultado.
  MostrarResultado(result);
  return 0;
}