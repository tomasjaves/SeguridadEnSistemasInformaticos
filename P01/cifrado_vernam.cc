/*
    Cifrado:
111011111001011100011111
    Clave:
101111001101100001010011
    Descifrado:
010100110100111101001100

*/

#include <iostream>
#include <string>
#include <bitset>

std::string CifradoVerman(const std::string& mensaje, const std::string& clave) {
  std::string cifrado_verman = "";
  for (int i = 0; i < mensaje.length(); i++) {
    cifrado_verman += mensaje[i] == clave[i] ? '0' : '1';
  }
  return cifrado_verman;
}

// IBM
std::string ConvertirBinario(std::string mensaje) {
  std::string binario = "";
  for(int i = 0; i < mensaje.size(); ++i) {
    binario += std::bitset<8>(mensaje[i]).to_string();
  }
  return binario;
}

std::string CifradoASCII(std::string cifrado) {
  std::string ascii = "";
  for (int i = 0; i < cifrado.size(); i += 8) {
    std::string letra = "";
    int j = 0;
    while (letra.size() != 8) letra += cifrado[i + j++];
    char numero = std::bitset<8>(letra).to_ulong();
    ascii += numero;
  }
  return ascii;
}

int main() {
  std::string mensaje, clave;
  std::cout << "Mensaje: ";
  std::cin >> mensaje;
  std::cout << "Clave: ";
  std::cin >> clave;
  mensaje = ConvertirBinario(mensaje);
  if (mensaje.size() != clave.size()) {
    std::cout << "Mensaje y clave no tienen misma longitud" << std::endl;
    return 1;
  }
  std::cout << "Cifrado: " << CifradoVerman(mensaje, clave) << std::endl;
  std::cout << "Cifrado ASCII: " << CifradoASCII(CifradoVerman(mensaje,clave)) << std::endl;
  return 0;
}