#include <iostream>
#include <string>

// Función para ajustar la clave al tamaño del texto, excluyendo espacios
std::string AjustarClave(const std::string &texto, const std::string &clave) {
  std::string claveAjustada;
  int j = 0;
  for (char c : texto) {
    if (isalpha(c)) {
      claveAjustada += toupper(clave[j % clave.size()]);
      j++;
    } else {
      claveAjustada += c;
    }
  }
  return claveAjustada;
}

// Función para cifrar el texto
std::string CifrarVigenere(const std::string &texto, const std::string &clave) {
  std::string claveAjustada = AjustarClave(texto, clave);
  std::string cifrado;
  for (int i = 0; i < texto.size(); ++i) {
    if (isalpha(texto[i])) {
      cifrado += char(((toupper(texto[i]) - 'A' + toupper(claveAjustada[i]) - 'A') % 26) + 'A');
    } // else {
      // cifrado += texto[i];
    // }
  }
  return cifrado;
}

// Función para descifrar el texto
std::string DescifrarVigenere(const std::string &cifrado, const std::string &clave) {
  std::string claveAjustada = AjustarClave(cifrado, clave);
  std::string descifrado;
  for (int i = 0; i < cifrado.size(); ++i) {
    if (isalpha(cifrado[i])) {
      descifrado += char(((toupper(cifrado[i]) - toupper(claveAjustada[i]) + 26) % 26) + 'A');
    } // else {
      // descifrado += cifrado[i];
    // }
  }
  return descifrado;
}

int main() {
  std::string texto, clave;
  std::cout << "Introduce el texto a cifrar: ";
  getline(std::cin, texto);
  std::cout << "Introduce la clave: ";
  std::cin >> clave;
  std::cout << "Texto Cifrado: " << CifrarVigenere(texto, clave) << std::endl;
  std::cout << "Texto Descifrado: " << DescifrarVigenere(CifrarVigenere(texto, clave), clave) << std::endl;
  return 0;
}
