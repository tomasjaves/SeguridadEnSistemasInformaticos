#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include <string>
#include <cmath>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define ITALIC "\033[3m"
#define UNDERLINE "\033[4m"
#define PURPLE "\033[95m"

/**
 * @brief Función que calcula el algoritmo de Euclides extendido.
 *
 * @param a
 * @param m
 * @return int
 */
int McdExtendido(int a, int b, int &x, int &y) {
  // Caso base. Si b es 0, devolvemos a y x=1, y=0
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  // Llamada recursiva. Calculamos el mcd de b y a%b porque mcd(a,b) = mcd(b, a%b)
  int x1, y1;
  int mcd = McdExtendido(b, a % b, x1, y1);
  // Actualizamos x e y. X conserva el valor de y1 y y conserva el valor de x1 - (a/b)*y1
  x = y1;
  y = x1 - (a / b) * y1;
  return mcd;
}

/**
 * @brief Función que calcula el inverso modular de un número.
 *
 * @param a
 * @param m
 * @return int
 */
int ModInverse(int d, int phi) {
  int x, y;
  // Calculamos el mcd de d y phi y guardamos el resultado en mcd.
  int mcd = McdExtendido(d, phi, x, y);
  // Si el mcd no es 1, no tiene inverso modular
  if (mcd != 1) {
    return -1;
  }
  // Si el mcd es 1, calculamos el inverso modular.
  return (x % phi + phi) % phi;
}

/**
 * @brief Función que realiza el cálculo de los puntos de la curva
 * 
 * @param p 
 * @param a 
 * @param b 
 * @param points 
 */
void CalculatePoints(const int& p, const int& a, const int& b, std::vector<std::pair<int, int>>& points) {
  // Recorremos los valores de x (0 a p-1)
  for (int x = 0; x < p; ++x) {
    // Calculamos el valor de x^3 + ax + b (lado derecho de la ecuación de la curva).
    int equals_right_side = (x*x*x + a*x + b) % p;
    // Recorremos los valores de y (0 a p-1)
    for (int y = 0; y < p; ++y) {
      // Si y^2 es igual al valor calculado anteriormente, guardamos el punto (x, y).
      if ((y*y) % p == equals_right_side) {
        points.push_back(std::make_pair(x, y));
      }
    }
  }
}

/**
 * @brief Función que realiza el cálculo de un punto doble
 * 
 * @param p 
 * @param a 
 * @param x 
 * @param y 
 * @return std::pair<int, int> 
 */
std::pair<int, int> DoublePoint(const int& p, const int& a, const std::pair<int, int>& P) {
  int x1 = P.first;
  int y1 = P.second;

  // Caso especial: Si y1 es 0, retorna el punto en el infinito.
  if (y1 == 0) return {0, 0};

  // Calculamos el inverso modular de 2*y1
  int inv = ModInverse(2 * y1, p);
  if (inv == -1) {
    std::cerr << "Error calculando el inverso modular." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Calculamos lambda según la fórmula
  int lambda = (3 * x1 * x1 + a) % p;
  lambda = (lambda * inv) % p;
  if (lambda < 0) lambda += p;

  // Calculamos x3 según la fórmula
  int x3 = (lambda * lambda - 2 * x1) % p;
  if (x3 < 0) x3 += p;

  // Calculamos y3 según la fórmula
  int y3 = (lambda * (x1 - x3) - y1) % p;
  if (y3 < 0) y3 += p;

  return {x3, y3};
}


/**
 * @brief Función que realiza el cálculo de la suma de dos puntos
 * 
 * @param p 
 * @param a 
 * @param P 
 * @param Q 
 * @return std::pair<int, int> 
 */
std::pair<int, int> AddPoints(const int& p, const int& a, const std::pair<int, int>& P, const std::pair<int, int>& Q) {
  // Si los puntos son iguales, se realiza el cálculo de un punto doble.
  if (P == Q) {
    return DoublePoint(p, a, P);
  }

  int x1 = P.first, y1 = P.second;
  int x2 = Q.first, y2 = Q.second;
  int lambda;

  // Caso de puntos inversos o punto en el infinito
  if ((x1 == x2) && (y1 == p - y2)) {
    return std::make_pair(0, 0); // Representación convencional del punto en el infinito.
  }

  int dx = x2 - x1, dy = y2 - y1;
  if (dx < 0) dx += p;
  if (dy < 0) dy += p;

  // Calcula el inverso modular de dx.
  int dx_inv = ModInverse(dx, p);
  // Si el inverso modular no existe, se produce un error.
  if (dx_inv == -1) {
    std::cerr << "Error calculando el inverso modular." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Calculamos lambda según la fórmula.
  lambda = (dy * dx_inv) % p;
  if (lambda < 0) lambda += p;

  // Calculamos la coordenada x del punto resultante.
  int x3 = (lambda * lambda - x1 - x2) % p;
  if (x3 < 0) x3 += p;

  // Calculamos la coordenada y del punto resultante.
  int y3 = (lambda * (x1 - x3) - y1) % p;
  if (y3 < 0) y3 += p;

  return std::make_pair(x3, y3);
}

/**
 * @brief Función que realiza la multiplicación escalar de un punto
 * 
 * @param k 
 * @param P 
 * @param p 
 * @param a 
 * @return std::pair<int, int> 
 */
std::pair<int, int> ScalarMultiplication(int k, const std::pair<int, int>& P, const int& p, const int& a) {
  // Usamos como valor inicial el punto en el infinito.
  std::pair<int, int> result = {0, 0};
  // Representamos el punto a sumar como addend. Inicialmente es P.
  std::pair<int, int> addend = P;

  // Mientras aún queden bits en k para procesar
  while (k > 0) {
    // Si el bit menos significativo de k es 1, añadimos addend al resultado
    if (k & 1) {
      // Si result es el punto en el infinito, simplemente copiamos addend
      // De lo contrario, sumamos addend al resultado actual. Esto se hace para evitar
      // que se realice una suma con el punto en el infinito, lo que daría como resultado el punto en el infinito.
      result = (result.first == 0 && result.second == 0) ? addend : AddPoints(p, a, result, addend);
    }
    // Preparamos addend para la siguiente iteración duplicándolo
    addend = DoublePoint(p, a, addend);
    // Desplazamos k a la derecha para procesar el siguiente bit
    k >>= 1;
  }
  return result;
}

/**
 * @brief Función que busca un punto por su coordenada x
 * 
 * @param x 
 * @param points 
 * @return std::pair<int, int> 
 */
std::pair<int, int> SearchPointByX(int x, const std::vector<std::pair<int, int>>& points) {
  // Inicializamos el punto encontrado como (-1, -1).
  std::pair<int, int> point_found = {-1, -1};
  // Mientras el valor del punto encontrado sea (-1, -1), buscamos el punto con la coordenada x.
  while (point_found.first == -1 && point_found.second == -1) {
    // Recorremos los puntos de la curva y comparamos la coordenada x.
    for (const std::pair<int, int>& point : points) {
      // Si la coordenada x del punto es igual a x, guardamos el punto y salimos del bucle.
      if (point.first == x) {
        point_found = point;
        break;
      }
    }
    // Si no se ha encontrado el punto, incrementamos x en 1 y volvemos a buscar.
    ++x;
  }
  return point_found;
}

/**
 * @brief Función que muestra los puntos de la curva
 * 
 * @param points 
 */
void ShowPoints(const std::vector<std::pair<int, int>>& points) {
  std::cout << std::endl;
  std::cout << ITALIC << "Puntos de la curva: " << RESET;
  // Recorremos los puntos de la curva y los mostramos.
  for (const auto& point : points) {
    if (point == points.back()) {
      std::cout << "(" << point.first << "," << point.second << ")";
    } else {
      std::cout << "(" << point.first << "," << point.second << "),";
    }
  }
  std::cout << std::endl << std::endl;
}

/**
 * @brief Función que realiza el cifrado de un mensaje
 * 
 * @param p 
 * @param a 
 * @param b 
 * @param G 
 * @param da 
 * @param db 
 * @param original_message 
 */
void Encrypt(const int& p, const int& a, const int& b, const std::pair<int, int>& G, const int& da, const int& db, const std::string& original_message) {
  std::vector<std::pair<int, int>> points;
  // Calculamos los puntos de la curva.
  CalculatePoints(p, a, b, points);
  // Mostramos los puntos de la curva.
  ShowPoints(points);
  // Calculamos la clave pública de B (dbG).
  std::pair<int, int> dbG = ScalarMultiplication(db, G, p, a);
  std::cout << UNDERLINE << "Clave pública de B: punto dbG=(" << RESET << dbG.first << "," << dbG.second << ")" << std::endl;
  
  // Calculamos la clave pública de A (daG) - aunque no es necesario para el cifrado, se muestra aquí para completitud.
  std::pair<int, int> daG = ScalarMultiplication(da, G, p, a);
  std::cout << UNDERLINE << "Clave pública de A: punto daG=(" << RESET << daG.first << "," << daG.second << ")" << std::endl;
  
  // Calculamos la clave secreta de A (da(dbG)).
  std::pair<int, int> secret_key_A = ScalarMultiplication(da, dbG, p, a);
  std::cout << UNDERLINE << "Clave secreta de A: punto da(dbG)=(" << RESET << secret_key_A.first << "," << secret_key_A.second << ")" << std::endl;

  // Calculamos la clave secreta de B (db(daG)).
  std::pair<int, int> secret_key_B = ScalarMultiplication(db, daG, p, a);
  std::cout << UNDERLINE << "Clave secreta de B: punto db(daG)=(" << RESET << secret_key_B.first << "," << secret_key_B.second << ")" << std::endl;

  // Calculamos M como potencia de 2 más cercana al mensaje original.
  int M = 1;
  int message = std::stoi(original_message);
  while (M < message) {
    M *= 2;
  }
  std::cout << UNDERLINE << "M:" << RESET << " " << M << std::endl;
  // Calculamos h como el cociente (quotient) de M y p.
  int h = p / M;
  std::cout << UNDERLINE << "h:" << RESET << " " << h << std::endl;
  // Calculamos Qm como el punto de la curva cuya coordenada x es el mensaje original codificado multiplicado por h.
  std::cout << UNDERLINE << "Mensaje original codificado como punto Qm =" << RESET << " ";
  int aux = message * h;
  // Encotramos el punto Qm en la curva. Si no existe, se busca el siguiente punto (+1).
  std::pair<int, int> Qm = SearchPointByX(aux, points);
  std::cout << "(" << Qm.first << "," << Qm.second << ")" << std::endl << std::endl;
  std::cout << ITALIC << "Mensaje cifrado y clave pública enviados de A a B: " << RESET;
  // Calculamos el mensaje cifrado como la suma de Qm y la clave secreta de A.
  std::pair<int, int> encrypted_message = AddPoints(p, a, Qm, secret_key_A);
  // El mensaje cifrado será la suma calculada anteriormente y la clave pública de A.
  std::cout << "{(" << encrypted_message.first << "," << encrypted_message.second << "),(" << daG.first << "," << daG.second << ")}" << std::endl;
}

/**
 * @brief Función que lee los datos de entrada.
 * Se ejecuta cuando el usuario selecciona la opción 1.
 * 
 * @param p 
 * @param a 
 * @param b 
 * @param G 
 * @param da 
 * @param db 
 * @param original_message 
 */
void ReadData(int& p, int& a, int& b, std::pair<int, int>& G, int& da, int& db, std::string& original_message) {
  std::cout << "Ingrese el valor de p: ";
  std::cin >> p;
  std::cout << "Ingrese el valor de a: ";
  std::cin >> a;
  std::cout << "Ingrese el valor de b: ";
  std::cin >> b;
  std::cout << "Ingrese el valor de G: ";
  std::cin >> G.first >> G.second;
  std::cout << "Ingrese el valor de da: ";
  std::cin >> da;
  std::cout << "Ingrese el valor de db: ";
  std::cin >> db;
  std::cout << "Ingrese el mensaje original: ";
  std::cin >> original_message;
}

/**
 * @brief Función que muestra el menú de opciones.
 * 
 */
void MostrarMenu() {
  std::cout << std::endl;
  std::cout << BOLD << PURPLE << "ALGORITMOS DIFFIE-HELLMAN y ELGAMAL ELÍPTICOS" << RESET << std::endl << std::endl;
  std::cout << "Seleccione una opción:" << std::endl;
  std::cout << "1. Introducir valores manualmente." << std::endl;
  std::cout << "2. Ejemplo del PDF." << std::endl;
  std::cout << "3. Salir." << std::endl;
  std::cout << std::endl;
  std::cout << BOLD << PURPLE << "Opción: " << RESET;
}

/**
 * @brief Función principal
 * 
 * @return int 
 */
int main() {
  int option;
  MostrarMenu();
  std::cin >> option;
  switch (option) {
    case 1: {
      int p, a, b, da, db;
      std::pair<int, int> G;
      std::string original_message;
      ReadData(p, a, b, G, da, db, original_message);
      Encrypt(p, a, b, G, da, db, original_message);
      break;
    }
    case 2: {
      int p = 11, a = 1, b = 1, da = 3, db = 2;
      std::pair<int, int> G = {3, 8};
      std::string original_message = "3";
      Encrypt(p, a, b, G, da, db, original_message);
      break;
    }
    case 3:
      std::cout << "Saliendo..." << std::endl;
      break;
    default:
      std::cerr << "Opción no válida." << std::endl;
      break;
  }
  return 0;
}