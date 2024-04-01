#include <iostream>
#include <sstream>
#include "../include/rsa.h"

/**
 * @brief Función que comprueba si el texto introducido está compuesto únicamente por letras mayúsculas.
 * 
 * @param texto 
 */
void ComprobarAlfabeto(std::string texto) {
  for (char c : texto) {
    if (c < 'A' || c > 'Z') {
      std::cerr << "El texto debe estar compuesto únicamente por letras mayúsculas." << std::endl;
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * @brief Función que muestra el menú de opciones.
 * 
 */
void MostrarMenu() {
  std::cout << std::endl;
  std::cout << BOLD << RED << "\tCIFRADO RSA" << RESET << std::endl << std::endl;
  std::cout << "Seleccione una opción:" << std::endl;
  std::cout << "1. Introducir valores manualmente." << std::endl;
  std::cout << "2. Ejemplos del PDF." << std::endl;
  std::cout << "3. Salir." << std::endl;
}

int main() {
  int opcion;
  do {
    MostrarMenu();
    std::cin >> opcion;
    switch (opcion) {
      case 1: {
        std::string inputLine;
        int p, q, d;
        std::string texto;

        std::cout << "Introduzca el valor de p: ";
        std::cin >> p;

        std::cout << "Introduzca el valor de q: ";
        std::cin >> q;

        std::cout << "Introduzca el valor de d: ";
        std::cin >> d;

        std::cout << "Introduzca el texto a cifrar: ";
        std::cin.ignore();
        std::getline(std::cin, texto);

        RSA rsa(p, q, d, texto);
        rsa.Encrypt();
        break;
      }
      case 2: {
        std::cout << std::endl;
        std::cout << BOLD << ITALIC << "Ejemplo 1:" << RESET << std::endl;
        RSA rsa(421, 7, 1619, "MANDA DINEROS");
        rsa.Encrypt();
        std::cout << std::endl;
        std::cout << BOLD << ITALIC << "Ejemplo 2:" << RESET << std::endl;
        RSA rsa2(2347, 347, 5, "AMIGO MIO");
        rsa2.Encrypt();
        return 0;
      }
      case 3:
        std::cout << "Saliendo..." << std::endl;
        break;
      default:
        std::cout << "Opción no válida." << std::endl;
        break;
    }
  } while (opcion != 3);
  return 0;
}