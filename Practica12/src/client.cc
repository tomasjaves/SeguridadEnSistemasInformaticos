#include <iostream>
#include <sstream>
#include "../include/firma-rsa.h"

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
  std::cout << ITALIC << "Seleccione una opción:" << RESET << std::endl;
  std::cout << BOLD << MAGENTA << "[0]" << RESET << " Salir" << std::endl;
  std::cout << BOLD << MAGENTA << "[1]" << RESET << " Firmar" << std::endl;
  std::cout << BOLD << MAGENTA << "[2]" << RESET << " Verificar" << std::endl;
  std::cout << BOLD << MAGENTA << "[3]" << RESET << " Ejemplo PDF" << std::endl << std::endl;

  std::cout << UNDERLINE << "Opción" << RESET << ": ";
}

int main() {
  int opcion;
  std::cout << std::endl;
  std::cout << BOLD << RED << "     FIRMA RSA" << RESET << std::endl;
  do {
    MostrarMenu();
    std::cin >> opcion;
    std::cout << std::endl;
    switch (opcion) {
      case 0:
        std::cout << std::endl;
        std::cout << BOLD << GREEN << "Saliendo..." << std::endl << std::endl;
        break;
      case 1: {
        int p, q, e;
        std::string texto;

        std::cout << UNDERLINE << "Introduzca el valor de p" << RESET << ": ";
        std::cin >> p;

        std::cout << UNDERLINE << "Introduzca el valor de q" << RESET << ": ";
        std::cin >> q;

        std::cout << UNDERLINE << "Introduzca el valor de e" << RESET << ": ";
        std::cin >> e;

        std::cout << UNDERLINE << "Introduzca el texto a firmar" << RESET << ": ";
        std::cin.ignore();
        std::getline(std::cin, texto);
        std::cout << std::endl;

        RSA rsa(p, q, e, texto);
        rsa.Ink();
        break;
      }
      case 2: {
        int e, n;
        std::string texto;

        std::cout << UNDERLINE << "Introduzca el valor de e" << RESET << ": ";
        std::cin >> e;

        std::cout << UNDERLINE << "Introduzca el valor de n" << RESET << ": ";
        std::cin >> n;

        std::cout << UNDERLINE << "Introduzca el texto firmado a verificar" << RESET << ": ";
        std::cin.ignore();
        std::getline(std::cin, texto);
        std::cout << std::endl;

        RSA rsa(e, n, texto);
        rsa.Verify();
        break;
      }
      case 3: {
        RSA rsa(421, 7, 1619, "MANDA DINEROS");
        rsa.Ink();

        std::cout << std::endl;

        RSA rsa2(7, 39, "11");
        rsa2.Verify();
        break;
      }
      default:
        std::cout << BOLD << RED << "Opción no válida." << std::endl;
        break;
    }
  } while (opcion != 0);
  return 0;
}