#include <iostream>
#include <vector>
#include <iomanip>

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define RED     "\033[31m"

// SCaja es un vector que contiene los valores de la caja de sustitución utilizada en la operación de sustitución de bytes
const unsigned char SCaja[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// RCon es un vector que contiene los valores de la constante RCon utilizada en la expansión de clave
const unsigned char RCon[10] = {
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

/**
 * @brief Función que realiza la operación de sustitución de bytes
 * 
 * @param cipher_text 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>> SubBytes(std::vector<std::vector<unsigned char>>& cipher_text) {
  // Realizamos la sustitución de cada byte de la matriz utilizando la SCaja
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      cipher_text[i][j] = SCaja[cipher_text[i][j]];
    }
  }
  return cipher_text;
}

/**
 * @brief Función que realiza la operación de expansión de clave
 * 
 * @param clave 
 * @param it
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>> ExtendClave(std::vector<std::vector<unsigned char>> clave, int it) {
  std::vector<std::vector<unsigned char>> claveExpandida = clave; // Copia de la clave original para expansión
  std::vector<unsigned char> tempColumn(4);

  // Paso 1: RotWord y SubWord para la última columna
  // RotWord
  for (int i = 0; i < 4; ++i) {
    tempColumn[i] = claveExpandida[i][3]; // Selecciona la última columna para la rotación
  }
  unsigned char temp = tempColumn[0]; // Guarda el primer elemento para la rotación
  for (int i = 0; i < 3; ++i) {
    tempColumn[i] = tempColumn[i + 1]; // Desplaza los elementos hacia arriba
  }
  tempColumn[3] = temp; // Mueve el primer elemento guardado a la última posición

  // Hacemos una sustitución utilizando la SCaja
  for (int i = 0; i < 4; ++i) {
    tempColumn[i] = SCaja[tempColumn[i]];
  }

  // Paso 2: Rcon XOR para el primer byte de la columna resultante
  tempColumn[0] ^= RCon[it]; // Solo el primer byte hace XOR con RCon

  // Paso 3: XOR sucesivo con las columnas de la clave actual
  for (int i = 0; i < 4; ++i) {
    claveExpandida[i][0] ^= tempColumn[i]; // XOR con la primera columna
  }
  // Realiza XOR para las columnas restantes
  for (int col = 1; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      claveExpandida[row][col] ^= claveExpandida[row][col - 1]; // XOR con la columna previamente generada
    }
  }

  return claveExpandida;
}


/**
 * @brief Función que realiza la operación de desplazamiento de filas
 * 
 * @param cipher_text 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>> ShiftRows(std::vector<std::vector<unsigned char>>& cipher_text) {
  // Realizamos el desplazamiento de las filas empezando desde la segunda
  for (int i = 1; i < 4; i++) {
    // Recorremos las columnas de la matriz
    for (int j = 0; j < i; j++) {
      // Variable temporal para guardar el primer byte de la fila
      unsigned char temp = cipher_text[i][0];
      // Realizamos el desplazamiento a la izquierda de la fila
      for (int k = 0; k < 3; k++) {
        cipher_text[i][k] = cipher_text[i][k + 1];
      }
      // Asignamos el primer byte al final de la fila
      cipher_text[i][3] = temp;
    }
  }
  return cipher_text;
}


/**
 * @brief Función que realiza la operación de mezcla de columnas
 * 
 * @param cipher_text 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>> MixColumns(std::vector<std::vector<unsigned char>>& state) {
  // Matriz resultante
  std::vector<std::vector<unsigned char>> result(4, std::vector<unsigned char>(4));
  // Creamos dos vectores para realizar la operación de mezcla de columnas y un byte auxiliar
  unsigned char a[4], b[4], h;
  // Recorremos las filas de la matriz
  for (int i = 0; i < 4; i++) {
    // Recorremos las columnas de la matriz
    for (int j = 0; j < 4; j++) {
       // Copiamos el estado actual en 'a'
      a[j] = state[j][i];
      // Si state[j][i] es mayor o igual a 0x80, entonces el bit más significativo es 1, por lo que h = 0x80
      h = state[j][i] & 0x80; // 0x80 es 10000000 en binario
      // Realizamos el desplazamiento a la izquierda de state[j][i] y lo guardamos en 'b'
      b[j] = state[j][i] << 1; 
      if (h == 0x80) {
        // Si el bit más significativo de 'a[j]' era 1, aplicamos XOR con 0x1b
        b[j] ^= 0x1b; // 0x1b es el polinomio irreducible x^8 + x^4 + x^3 + x + 1
      }
    }
    // Asignamos a cada posición de la matriz resultante el resultado de la operación de mezcla de columnas
    result[0][i] = b[0] ^ a[1] ^ b[1] ^ a[2] ^ a[3];
    result[1][i] = a[0] ^ b[1] ^ a[2] ^ b[2] ^ a[3];
    result[2][i] = a[0] ^ a[1] ^ b[2] ^ a[3] ^ b[3];
    result[3][i] = a[0] ^ b[0] ^ a[1] ^ a[2] ^ b[3];
  }
  return result;
}

/**
 * @brief Función que realiza la operación de adición de clave
 * 
 * @param cipher_text 
 * @param clave 
 * @return std::vector<std::vector<unsigned char>> 
 */
std::vector<std::vector<unsigned char>> AddRoundKey(const std::vector<std::vector<unsigned char>>& cipher_text, const std::vector<std::vector<unsigned char>>& clave) {
  // Matriz resultante
  std::vector<std::vector<unsigned char>> result(4, std::vector<unsigned char>(4));
  // Recorremos las filas de la matriz
  for (int i = 0; i < 4; i++) {
    // Recorremos las columnas de la matriz
    for (int j = 0; j < 4; j++) {
      // Asignamos byte por byte el resultado de la operación XOR entre el bloque de texto original y la clave
      result[i][j] = cipher_text[i][j] ^ clave[i][j];
    }
  }
  return result;
}

/**
 * @brief Función que realiza el cifrado de Rijndael
 * 
 * @param cipher_text 
 * @param clave 
 * @return std::vector<std::vector<unsigned char>>
 */
std::vector<std::vector<unsigned char>> Rijndael(const std::vector<std::vector<unsigned char>>& cipher_text, std::vector<std::vector<unsigned char>>& clave) {
  // Imprime los valores iniciales de clave y bloque de texto original
  std::cout << BOLD << "Clave: ";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)clave[j][i] << " ";
    }
  }
  std::cout << std::endl;
  std::cout << "Bloque de Texto Original: ";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)cipher_text[j][i] << " ";
    }
  }
  std::cout << RESET << std::endl << std::endl;
  // Mostramos el formato de salida de cada ronda donde se muestra la subclave utilizada y el bloque de texto cifrado
  std::cout << BOLD << RED << "R0 (Subclave = " << RESET;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)clave[j][i] << " " << RESET;
    }
  }
  std::cout << BOLD << RED << ") = " << RESET;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)cipher_text[j][i] << " " << RESET;
    }
  }
  std::cout << std::endl;
  // Realizamos la primera ronda y expandimos la clave
  std::vector<std::vector<unsigned char>> result = AddRoundKey(cipher_text, clave);
  // Realizamos las siguientes 9 rondas
  for (int i = 1; i < 10; i++) {
    // Expandimos la clave
    clave = ExtendClave(clave, i-1);
    // Mostramos el formato de salida de cada ronda donde se muestra la subclave utilizada y el bloque de texto cifrado
    std::cout << BOLD << RED << "R" << i << " (Subclave = " << RESET;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)clave[j][i] << " " << RESET;
      }
    }
    // Realizamos las operaciones de cada ronda
    result = SubBytes(result);
    result = ShiftRows(result);
    result = MixColumns(result);
    result = AddRoundKey(result, clave);
    std::cout << BOLD << RED << ") = " << RESET;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)result[j][i] << " " << RESET;
      }
    }
    std::cout << std::endl;
  }
  // Realizamos la última ronda, ya no se realiza la operación de mezcla de columnas pero si todas las demás
  result = SubBytes(result);
  result = ShiftRows(result);
  clave = ExtendClave(clave, 9);
  // Mostramos el formato de salida de cada ronda donde se muestra la subclave utilizada y el bloque de texto cifrado
  std::cout << BOLD << RED << "R10 (Subclave = " << RESET;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)clave[j][i] << " " << RESET;
    }
  }
  result = AddRoundKey(result, clave);
  std::cout << BOLD << RED << ") = " << RESET;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)result[j][i] << " ";
    }
  }
  std::cout << RESET << std::endl << std::endl;
  // Mostramos el formato de salida del bloque de texto cifrado final del algoritmo y lo devolvemos
  std::cout << BOLD << GREEN << "Bloque de Texto Cifrado: " << RESET;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << BOLD << std::hex << std::setw(2) << std::setfill('0') << (int)result[j][i] << " " << RESET;
    }
  }
  std::cout << std::endl;
  return result;
}


int main() {
  // Creamos la clave y el bloque de texto original
  std::vector<std::vector<unsigned char>> clave = {
    {0xFF, 0xBB, 0x08, 0x06},
    {0xEE, 0xAA, 0x08, 0x06},
    {0xDD, 0x09, 0x07, 0x05},
    {0xCC, 0x09, 0x07, 0x05}
  };
  std::vector<std::vector<unsigned char>> texto_cifrado = {
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF}
  };
  // Realizamos el cifrado de Rijndael
  std::cout << CYAN << BOLD << "\n\t\t\t\t\t\tCifrado de Rijndael" << RESET << std::endl << std::endl;
  std::vector<std::vector<unsigned char>> resultado = Rijndael(texto_cifrado, clave);
  return 0;
}