#include <iostream>
#include <string>

/**
 * @brief Función para ajustar la clave al tamaño del texto
 * Ejemplo: texto = "MENSAJESECRETO", clave = "CLAVE", claveAjustada = "CLAVECLAVECLAV"
 * 
 * @param texto 
 * @param clave 
 * @return std::string 
 */
std::string AjustarClave(const std::string &texto, const std::string &clave) {
  std::string claveAjustada;
  int j = 0;
  // Recorremos el texto carácter a carácter
  for (char c : texto) {
    // Si es una letra, la añadimos a la clave ajustada y avanzamos el índice de la clave
    if (isalpha(c)) {
      // Añadimos la letra en mayúsculas para evitar problemas con el cifrado, además
      // usamos el operador módulo para repetir la clave cíclicamente.
      claveAjustada += toupper(clave[j % clave.size()]);
      j++;
    } else {
      // Si no es una letra, añadimos el carácter al texto sin modificar
      claveAjustada += c;
    }
  }
  return claveAjustada;
}

/**
 * @brief Función para formatear el texto en bloques del tamaño de la clave
 * Ejemplo: texto = "MENSAJESECRETO", clave = "CLAVE", textoFormateado = "MENSA JESEC RETO"
 * 
 * @param texto 
 * @param tamanoBloque 
 * @return std::string 
 */
std::string FormatearEnBloques(const std::string &texto, int tamanoBloque) {
  std::string textoFormateado;
  int contador = 0;
  // Recorremos el texto carácter a carácter
  for (char c : texto) {
    // Si es una letra, la añadimos al texto formateado y avanzamos el contador
    if (isalpha(c)) {
      // Añadimos un espacio cada vez que el contador sea múltiplo del tamaño del bloque
      if (contador > 0 && contador % tamanoBloque == 0) {
        textoFormateado += ' ';
      }
      textoFormateado += c;
      contador++;
    }
  }
  return textoFormateado;
}

/**
 * @brief Función para cifrar el texto usando el cifrado de Vigenere.
 * 
 * Cifrado de Vigenere:
 *   1. Convertimos el carácter a mayúsculas para evitar problemas con el cifrado
 *   2. Restamos el valor ASCII de la letra 'A' para obtener un valor entre 0 y 25
 *   3. Sumamos el valor de la letra de la clave ajustada
 *   4. Aplicamos el operador módulo para obtener un valor entre 0 y 25 (si la suma supera 'Z' (25), se vuelve al inicio del alfabeto)
 *   5. Sumamos el valor ASCII de la letra 'A' para convertir el número de vuelta en un carácter alfabético 
 *   6. Convertimos el valor ASCII a carácter
 * 
 * @param texto 
 * @param clave 
 * @return std::string 
 */
std::string CifrarVigenere(const std::string &texto, const std::string &clave) {
  // Ajustamos la clave al tamaño del texto
  std::string claveAjustada = AjustarClave(texto, clave);
  std::string cifrado;
  // Recorremos el texto carácter a carácter
  for (int i = 0; i < texto.size(); ++i) {
    // Si es una letra, la ciframos y la añadimos al texto cifrado
    if (isalpha(texto[i])) {
      // Explicado en la definición de la función
      cifrado += char(((toupper(texto[i]) - 'A' + toupper(claveAjustada[i]) - 'A') % 26) + 'A');
    }
  }
  // Formateamos el texto cifrado en bloques del tamaño de la clave
  return FormatearEnBloques(cifrado, clave.size());
}

/**
 * @brief Función MODIFICACI
 * 
 * @param texto 
 * @param clave 
 * @return std::string 
 */
std::string CifradoModificacion(std::string texto, std::string clave) {
  std::string cifrado = "", claveAjustada = AjustarClave(texto, clave);
  for (int i = 0; i < texto.size(); ++i) {
    if (isalpha(texto[i])) {
      int valor_texto = toupper(texto[i]) - 'A';
      int valor_clave = toupper(claveAjustada[i]) - 'A';
      int resta_valores = valor_clave - valor_texto;
      if (resta_valores < 0) resta_valores += 26;
      cifrado += char((resta_valores % 26) + 'A');
    }
  }
  return cifrado;
}

/**
 * @brief Función para descifrar el texto usando el cifrado de Vigenere.
 * 
 * Descifrado de Vigenere:
 *  1. Convertimos el carácter a mayúsculas para evitar problemas con el cifrado
 *  2. Restamos el carácter de la clave ajustada al carácter del texto cifrado para obtener el valor original
 *  3. Puesto que el número puede ser negativo, sumamos 26 para obtener un valor positivo
 *  4. Aplicamos el operador módulo para obtener un valor entre 0 y 25 (si la resta supera 'Z' (25), se vuelve al inicio del alfabeto)
 *  5. Sumamos el valor ASCII de la letra 'A' para convertir el número de vuelta en un carácter alfabético
 *  6. Convertimos el valor ASCII a carácter
 * 
 * @param cifrado 
 * @param clave 
 * @return std::string 
 */
std::string DescifrarVigenere(const std::string &cifrado, const std::string &clave) {
  // Ajustamos la clave al tamaño del texto
  std::string claveAjustada = AjustarClave(cifrado, clave);
  std::string descifrado;
  // Recorremos el texto cifrado carácter a carácter
  for (int i = 0; i < cifrado.size(); ++i) {
    // Si es una letra, la desciframos y la añadimos al texto descifrado
    if (isalpha(cifrado[i])) {
      // Explicado en la definición de la función
      descifrado += char(((toupper(cifrado[i]) - toupper(claveAjustada[i]) + 26) % 26) + 'A');
    }
  }
  // Formateamos el texto descifrado en bloques del tamaño de la clave
  return FormatearEnBloques(descifrado, clave.size());
}

/**
 * @brief Función principal
 * 
 * @return int 
 */
int main() {
  std::string texto, clave;
  int opcion;
  std::cout << "Introduce el texto: ";
  getline(std::cin, texto);
  std::cout << "Introduce la clave: ";
  std::cin >> clave;
  std::cout << "Introduce la opcion: \n";
  std::cout << "1. Cifrado Vigenere\n";
  std::cout << "2. Descifrado Vigenere\n";
  std::cout << "3. Cifrado Modificacion\n";
  std::cout << "4. Salir Programa\n";
  std::cin >> opcion;
  switch (opcion)
  {
  case 1:
    std::cout << "Texto Cifrado: " << CifrarVigenere(texto, clave) << std::endl;
    break;
  case 2:
    std::cout << "Texto Descifrado: " << DescifrarVigenere(texto, clave) << std::endl;
    break;
  case 3:
    std::cout << "Cifrado Modificación: " << CifradoModificacion(texto, clave) << std::endl;
    break;
  case 4:
    std::cout << "Saliendo del programa..." << std::endl;
    break;
  default:
    std::cout << "Opcion no valida" << std::endl;
    break;
  }
  // std::cout << "Texto Cifrado: " << CifrarVigenere(texto, clave) << std::endl;
  // std::cout << "Texto Descifrado: " << DescifrarVigenere(CifrarVigenere(texto, clave), clave) << std::endl;
  // std::cout << "Cifrado Modificación: " << CifradoModificacion(clave, texto) << std::endl;
  return 0;
}