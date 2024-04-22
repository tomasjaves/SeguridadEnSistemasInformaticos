#include "../include/firma-rsa.h"

/**
 * @brief Constructor de la clase RSA.
 *
 * @param p
 * @param q
 * @param d
 * @param texto
 */
RSA::RSA(int p, int q, int e, std::string texto) {
  // Comprobamos que p y q son primos
  if (!LehmanPeralta(p) || !LehmanPeralta(q)) {
    // Si no lo son, lanzamos una excepción
    throw std::invalid_argument("p o q no son primos.");
    return;
  }
  p_ = p;
  q_ = q;
  std::cout << "Se comprueba que " << YELLOW << BOLD << "p " << RESET << "y "
            << YELLOW << BOLD << "q " << RESET << "son " << YELLOW << BOLD
            << "primos" << RESET << std::endl;
  phi_ = (p - 1) * (q - 1);
  n_ = p * q;
  // Comprobamos que d tiene inverso modular
  if (!InversoModular(e, phi_)) {
    // Si no lo tiene, lanzamos una excepción
    throw std::invalid_argument("d no tiene inverso modular.");
    return;
  }
  std::cout << "Se comprueba que " << YELLOW << BOLD << "e " << RESET << "es "
            << YELLOW << BOLD << "primo " << RESET << "con " << YELLOW << BOLD
            << "Φ(n) = " << RESET << phi_ << std::endl;
  e_ = e;
  std::cout << "Se calcula " << YELLOW << BOLD << "d " << RESET << "= " << d_
            << std::endl;
  texto_ = texto;
}

/**
 * @brief Constructor de la clase RSA.
 *
 * @param p
 * @param q
 * @param texto
 */
RSA::RSA(const int& e, const int& n, const std::string& texto) {
  valor_texto_ = std::stoi(texto);
  e_A = e;
  n_A = n;
}

/**
 * @brief Función que calcula la exponenciación rápida de un número.
 *
 * @param a
 * @param b
 * @param x
 * @param y
 * @return int
 */
int RSA::ExponenciacionRapida(long long base, unsigned int exponente, int modulo) {
  // Inicialización de la variable resultado
  int resultado = 1;
  // Se reduce la base al módulo
  base = base % modulo;
  // Si la base es 0, el resultado es 0
  if (base == 0) return 0;
  // Si el exponente es 0, el resultado es 1
  while (exponente > 0) {
    // Si el exponente es impar, se multiplica el resultado por la base y se
    // reduce al módulo
    if (exponente % 2 == 1) resultado = (resultado * base) % modulo;
    // Se divide el exponente entre 2, se eleva la base al cuadrado y se reduce al módulo
    exponente = exponente >> 1;
    base = (base * base) % modulo;
  }

  return resultado;
}

/**
 * @brief Función que verifica si un número es primo o no.
 *
 * @param prime
 * @return true
 * @return false
 */
bool RSA::LehmanPeralta(int prime) {
  // Lista de primos pequeños para la comprobación inicial rápida
  std::vector<int> short_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

  // Comprobación inicial rápida para los primeros primos
  for (int short_prime : short_primes) {
    if (prime == short_prime)
      return true;  // Si prime es igual a uno de los primos pequeños, es primo
    if (prime % short_prime == 0)
      return false;  // Si prime es divisible por alguno, no es primo
  }

  // Inicialización de la semilla de tiempo para la generación de números
  // aleatorios
  std::srand(std::time(NULL));

  // Contadores para resultados específicos de la exponenciación rápida
  unsigned int counter1 = 0, counterM1 = 0;

  // El bucle principal que realiza kIteraciones pruebas de primalidad
  for (int i = 0; i < kIteraciones; i++) {
    // Genera un número aleatorio entre 2 y prime - 2 para cada iteración para no usar 1 ni prime - 1
    // pues no aportan información
    int kRandNumber = std::rand() % (prime - 2) + 2;

    // Realizamos la exponenciación rápida con el número aleatorio generado
    int expResult = ExponenciacionRapida(kRandNumber, (prime - 1) / 2, prime);

    // Contabilizamos el resultado de la exponenciación rápida
    if (expResult == 1)
      counter1++;  // Si el resultado es 1, incrementa counter1
    // Si el resultado es -1, lo que en términos de aritmética modular es prime
    // - 1, incrementa counterM1
    else if (expResult == prime - 1)
      counterM1++;
  }

  // Si todos los números aleatorios generados dan como resultado 1 o prime-1,
  // se considera que prime no es primo según este test
  if (counter1 == kIteraciones || counterM1 == kIteraciones) return false;

  // Si todos los resultados no son iguales, el número es probablemente primo
  return true;
}

/**
 * @brief Función que calcula el algoritmo de Euclides extendido.
 *
 * @param a
 * @param m
 * @return int
 */
int RSA::McdExtendido(int a, int b, int &x, int &y) {
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
bool RSA::InversoModular(int e, int phi) {
  // Calculamos el inverso modular de e y phi
  int x, y;
  int mcd = McdExtendido(e, phi, x, y);
  // Si el mcd no es 1, no tiene inverso modular
  if (mcd != 1) {
    return false;
  }
  // Si el mcd es 1, calculamos el inverso modular.
  d_ = (x % phi + phi) % phi;
  return true;
}

/**
 * @brief Función que asigna el tamaño del bloque.
 *
 */
void RSA::SetBlockSize() {
  int i = 0;
  // Mientras la base elevada a i sea menor que n, incrementamos i
  while (std::pow(kBase, i) < n_) {
    ++i;
  }

  // El tamaño del bloque es i - 1 porque el bucle se detiene cuando la base
  // elevada a i es mayor que n y queremos el tamaño del bloque anterior
  blockSize_ = i - 1;

  std::cout << "Como " << YELLOW << BOLD << "n " << RESET << "= " << n_
            << ", se divide el texto en " << YELLOW << BOLD << "bloques de "
            << RESET << blockSize_ << YELLOW << BOLD << " caracteres" << RESET
            << std::endl;
}

/**
 * @brief Función que convierte el texto a decimal.
 *
 */
void RSA::ToDecimal() {
  // Asignamos el tamaño del bloque
  SetBlockSize();
  // Inicializamos las variables necesarias
  int temp = 0;
  int valor, count = blockSize_ - 1;

  // Bucle que recorre el texto y lo convierte a decimal
  for (size_t i = 0; i < texto_.size(); ++i) {
    // Se resta el valor de 'A' para obtener el valor de la letra en el alfabeto
    valor = texto_[i] - 'A';
    // Se multiplica el valor por la base elevada a la posición en el bloque
    temp += valor * std::pow(kBase, count);
    --count;
    // Si se ha completado un bloque, se añade al vector y se reinicia el contador
    if (count == -1) {
      texto_decimal_.push_back(temp);
      temp = 0;
      count = blockSize_ - 1;
    }
  }

  // Si el último bloque no está completo, se rellena con X (numero 23)
  if (count != blockSize_ - 1) {
    while (count >= 0) {
      temp += ('X' - 'A') * std::pow(kBase, count);
      --count;
    }
    texto_decimal_.push_back(temp);
  }
}

/**
 * @brief Función que elimina los espacios del texto.
 *
 */
void RSA::EraseSpaces() {
  texto_.erase(std::remove(texto_.begin(), texto_.end(), ' '), texto_.end());
}

/**
 * @brief Función que cifra el texto.
 *
 */
void RSA::Encrypt() {
  // Se pasa cada bloque a decimal para poder firmar, obteniendo x
  for (int x : texto_decimal_) {
    // Se calcula el texto cifrado con la fórmula c = x^d mod n
    int c = ExponenciacionRapida(x, d_, n_);
    // Se añade el texto cifrado al vector de texto cifrado
    texto_cifrado_.push_back(c);
  }
}

/**
 * @brief Función que decodifica el texto cifrado.
 *
 */
void RSA::Decode(const int& c) {
  // Asignamos una letra del alfabeto a cada número del texto cifrado
  std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for (char letter : alphabet) {
    // Si el texto original es igual al valor de la letra en el alfabeto, se añade al texto decodificado
    if (c == letter - 'A') {
      texto_descifrado_ += letter;
      break;
    }
  }
}

/**
 * @brief Función que cifra el texto.
 *
 */
void RSA::Ink() {
  // Eliminamos los espacios del texto
  EraseSpaces();
  // Convertimos el texto a decimal
  ToDecimal();
  
  std::cout << "Se pasa cada bloque a" << BOLD << YELLOW << " decimal " << RESET
            << "para poder" << BOLD << YELLOW << " firmar " << RESET << "obteniendo ";
  // Se muestra el texto en decimal
  for (int x : texto_decimal_) {
    std::cout << x << ", ";
  }
  std::cout << std::endl;
  // Se cifra el texto
  Encrypt();
  std::cout << "Se calcula en" << BOLD << YELLOW << " decimal " << RESET 
            << "el texto" << BOLD << YELLOW << " cifrado" << RESET << ": ";
  // Se muestra el texto cifrado en decimal
  for (int x : texto_cifrado_) {
    std::cout << x << ", ";
  }
  std::cout << std::endl;
  // Inicializamos el vector de texto cifrado
  texto_cifrado_.clear();
}

/**
 * @brief Función que verifica la firma.
 *
 */
void RSA::Verify() {
  // Se calcula la potencia valor_texto_ elevado a e_A (mod n_A)
  int c = ExponenciacionRapida(valor_texto_, e_A, n_A);
  std::cout << "Se calcula la" << BOLD << YELLOW << " potencia " << RESET
            << valor_texto_ << "^" << e_A << " (mod " << n_A << ") " << "obteniendo " << c << std::endl;
  // Se decodifica obteniendo el valor original
  Decode(c);
  std::cout << "Se" << BOLD << YELLOW << " decodifica " << RESET << "obteniendo el" 
            << BOLD << YELLOW << " texto original" << RESET << ": " << texto_descifrado_ << std::endl;
}