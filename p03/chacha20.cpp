#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>

#include <ctime>

/**
 * @brief Función que genera un nonce aleatorio.
 * 
 * srand() con time(nullptr) como argumento asegura que cada 
 * ejecución del programa use una semilla diferente para rand()
 * 
 * Cada llamada a rand() % 256 genera un nuevo número aleatorio en el rango de 0 a 255
 * utilizando la secuencia aleatoria determinada por la última semilla establecida con srand()
 * 
 * @return std::vector<uint8_t> 
 */
std::vector<uint8_t> generateRandomNonce() {
  std::vector<uint8_t> nonce(12); // 96 bits = 12 bytes
  // Time(nullptr) -> indica tiempo actual
  // Uso de static cast para convertir el tiempo a unsigned int, pues srand necesita ese tipo de datos.
  srand(static_cast<unsigned int>(time(nullptr)));

  // Recorremos los bytes del nonce
  for (int i = 0; i < 12; ++i) {
    // Asignamos a cada byte el numero aleatorio generado entre 0 y 255.
    nonce[i] = static_cast<uint8_t>(rand() % 256);
  }

  return nonce;
}

/**
 * @brief Función que realiza la operación de rotación a la izquierda.
 * 
 * @param value 
 * @param shift 
 * @return uint32_t 
 */
uint32_t rotl(uint32_t value, uint32_t shift) {
  // Movemos los bits a la izquierda y rellenamos con los bits que se desplazaron.
  return (value << shift) | (value >> (32 - shift));
}

/**
 * @brief Función que realiza la operación QR de ChaCha20.
 * 
 * @param a 
 * @param b 
 * @param c 
 * @param d 
 */
void quarterRound(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d) {
  // ARX: Add Rotate Xor.
  a += b; d ^= a; d = rotl(d, 16);
  c += d; b ^= c; b = rotl(b, 12);
  a += b; d ^= a; d = rotl(d, 8);
  c += d; b ^= c; b = rotl(b, 7);
}

/**
 * @brief Función que inicializa el estado de ChaCha20.
 * 
 * @param key 
 * @param counter 
 * @param nonce 
 * @return std::vector<uint32_t> 
 */
std::vector<uint32_t> initializeState(const std::vector<uint8_t>& key, uint32_t counter, const std::vector<uint8_t>& nonce, const std::vector<uint32_t>& constants) {
  // Estado de ChaCha20.
  std::vector<uint32_t> state(16);

  // Rellenamos las primeras 4 palabras con las constantes.
  for (int i = 0; i < 4; ++i) {
    state[i] = constants[i];
  }

  // Rellenamos las siguientes 8 palabras con la clave en little-endian.
  for (int i = 0; i < 8; ++i) {
    state[i + 4] = key[i * 4] | (key[i * 4 + 1] << 8) | (key[i * 4 + 2] << 16) | (key[i * 4 + 3] << 24);
  }

  // Rellenamos la palabra 12 con el contador.
  state[12] = counter;

  // Rellenamos la palabra 13, 14 y 15 con el nonce en little-endian.
  for (int i = 0; i < 3; ++i) {
    state[i + 13] = nonce[i * 4] | (nonce[i * 4 + 1] << 8) | (nonce[i * 4 + 2] << 16) | (nonce[i * 4 + 3] << 24);
  }

  return state;
}

/**
 * @brief Función que realiza las 20 iteraciones de ChaCha20.
 * 
 * @param state 
 */
void chacha20Rounds(std::vector<uint32_t>& state) {
  // 20 iteraciones = 10 iteraciones de 2 QRs (par e impar)
  for (int i = 0; i < 10; ++i) {
    // Iteraciones pares: columnas.
    quarterRound(state[0], state[4], state[8], state[12]);
    quarterRound(state[1], state[5], state[9], state[13]);
    quarterRound(state[2], state[6], state[10], state[14]);
    quarterRound(state[3], state[7], state[11], state[15]);
  
    // Iteraciones impares: diagonales.
    quarterRound(state[0], state[5], state[10], state[15]);
    quarterRound(state[1], state[6], state[11], state[12]);
    quarterRound(state[2], state[7], state[8], state[13]);
    quarterRound(state[3], state[4], state[9], state[14]);
  }
}

/**
 * @brief Función que imprime el estado de ChaCha20.
 * 
 * @param text 
 * @param state 
 */
void printState(const std::string& text, const std::vector<uint32_t>& state) {
  std::cout << text << "=" << std::endl;
  const int& num = 4;
  
  // Imprimir el estado en bloques de 4 palabras.
  for (int i = 0; i < state.size(); ++i) {
    if (i % num == 0 && i != 0) {
      std::cout << std::endl;
    }
    // Imprimir en hexadecimal, rellenando con ceros los 8 dígitos.
    std::cout << std::hex << std::setfill('0') << std::setw(8) << state[i] << " ";
  }
  std::cout << std::endl << std::endl;
}

/**
 * @brief Función que cifra un mensaje usando ChaCha20.
 * 
 * @param plaintext 
 * @param key 
 * @param counter 
 * @param nonce 
 * @return std::vector<uint8_t> 
 */
std::vector<uint8_t> chacha20Encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key, uint32_t counter, const std::vector<uint8_t>& nonce, const std::vector<uint32_t> constants) {
  // Inicializamos el estado.
  auto state = initializeState(key, counter, nonce, constants);

  printState("Estado inicial", state);

  auto originalState = state;

  // Realizamos las 20 iteraciones, state que inicialmente contiene el estado inicial, termina con el estado final.
  chacha20Rounds(state);

  printState("Estado final tras las 20 iteraciones", state);

  // Sumamos el estado inicial al estado final.
  for (int i = 0; i < 16; ++i) {
    state[i] += originalState[i];
  }

  printState("Estado de salida del generador", state);
  
  // Inicializamos un vector para almacenar el keystream de 64 bytes.
  std::vector<uint8_t> keystream(64);
  
  // Iteramos sobre las 16 palabras de 32 bits del estado de ChaCha20.
  for (int i = 0; i < 16; ++i) {
    // Divide cada palabra de 32 bits en 4 bytes.
    for (int j = 0; j < 4; ++j) {
      // Extrae cada byte de la palabra y lo almacena en el keystream.
      // Desplaza los bits a la derecha para posicionar el byte deseado al final y aplica una máscara para aislarlo.
      keystream[i * 4 + j] = (state[i] >> (8 * j)) & 0xFF;
    }
  }
  
  // Inicializamos un vector para almacenar el texto cifrado con longitud igual al texto plano.
  std::vector<uint8_t> ciphertext(plaintext.size());
  
  // Ciframos el texto plano aplicando XOR con el keystream.
  for (size_t i = 0; i < plaintext.size(); ++i) {
    ciphertext[i] = plaintext[i] ^ keystream[i % 64]; // i % 64 para repetir el keystream si es necesario.
  }
  
  return ciphertext;
}

int main() {
  // Constantes de ChaCha20.
  const std::vector<uint32_t> constants = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};

  // Inicializamos la clave de 256 bits en forma de 8 palabras en hexadecimal.
  std::vector<uint8_t> key = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
  };

  // Inicializamos un contador de 32 bits en forma de 1 palabra en hexadecimal.
  uint32_t counter = 0x00000001;

  // Inicializamos un nonce aleatorio de 96 bits en forma de 3 palabras en hexadecimal.
  std::vector<uint8_t> nonce = generateRandomNonce();

  std::cout << "Nonce generado: ";
  for (const uint8_t& n : nonce) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(n);
  }
  std::cout << std::endl << std::endl;

  // Inicializamos el mensaje a cifrar.
  std::string message = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it";
  // Convertimos el mensaje a un vector de 8 bytes.
  std::vector<uint8_t> plaintext(message.begin(), message.end());

  // Realizamos el cifrado y mostramos el resultado en hexadecimal.
  auto ciphertext = chacha20Encrypt(plaintext, key, counter, nonce, constants);
  std::cout << "Cifrado: ";
  for (const uint8_t& c : ciphertext) {
    std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)c;
  }
  std::cout << std::endl << std::endl;

  // Realizamos el descifrado y mostramos el resultado en texto plano.
  auto decrypted = chacha20Encrypt(ciphertext, key, counter, nonce, constants);
  // Convertimos el vector de bytes a string.
  std::string decryptedStr(decrypted.begin(), decrypted.end());
  std::cout << "Descifrado: " << decryptedStr << std::endl;
  std::cout << std::endl;

  return 0;
}
