#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>

using namespace std;

int main() {
    uint16_t rlz1 = 0x5B5D;  // 15-битный регистр 
    uint64_t rlz2[2] = {0x2D952AAAAAA, 0x36D6AA};  // 106-битный регистр 

    const char* inputFileName = "lb.txt";     
    const char* outputFileName = "lb_sh.txt";

    ifstream in(inputFileName, ios::binary);
    ofstream out(outputFileName, ios::binary);

    if (!in || !out) {
        cerr << "Ошибка открытия файлов!" << endl;
        return 1;
    }

    // Переменные для обработки данных
    uint64_t dataBlock;
    uint64_t gamma;
    uint8_t bit_gamma;
    uint8_t buffer;

    // Обработка файла блоками по 8 байт
    while (in.read(reinterpret_cast<char*>(&dataBlock), sizeof(uint64_t))) {
        gamma = 0;

        // Генерация 64-битной гаммы
        for (int i = 0; i < 64; i++) {
            gamma <<= 1;

            // Вычисление бита для rlz1 
            bit_gamma = ((rlz1 >> 14) ^ (rlz1 >> 3)) & 0x1;
            rlz1 = (rlz1 << 1) | bit_gamma;

            // Вычисление бита для rlz2 
            uint8_t bit2 = ((rlz2[1] >> 41) ^ (rlz2[0] >> 14)) & 0x1;

            // Сдвиг 106-битного регистра
            buffer = (rlz2[0] >> 63) & 0x1;
            rlz2[0] = (rlz2[0] << 1) | (rlz2[1] >> 41);
            rlz2[1] = (rlz2[1] << 1) | bit2;

            // Комбинирование битов
            gamma |= (bit_gamma ^ bit2);
        }

        // Маскирование данных
        uint64_t maskedBlock = dataBlock ^ gamma;
        out.write(reinterpret_cast<char*>(&maskedBlock), sizeof(uint64_t));
    }

    // Обработка последнего блока (если размер файла не кратен 8 байтам)
    streamsize remainingBytes = in.gcount();
    if (remainingBytes > 0) {
        uint64_t lastBlock = 0;
        memcpy(&lastBlock, reinterpret_cast<char*>(&dataBlock), remainingBytes);

        gamma = 0;
        // Повторная генерация гаммы для последнего блока
        for (int i = 0; i < 64; i++) {
            gamma <<= 1;
            bit_gamma = ((rlz1 >> 14) ^ (rlz1 >> 3)) & 0x1;
            rlz1 = (rlz1 << 1) | bit_gamma;

            uint8_t bit2 = ((rlz2[1] >> 41) ^ (rlz2[0] >> 14)) & 0x1;
            buffer = (rlz2[0] >> 63) & 0x1;
            rlz2[0] = (rlz2[0] << 1) | (rlz2[1] >> 41);
            rlz2[1] = (rlz2[1] << 1) | bit2;

            gamma |= (bit_gamma ^ bit2);
        }

        // Маскирование последнего блока
        uint64_t lastMasked = lastBlock ^ gamma;
        out.write(reinterpret_cast<char*>(&lastMasked), remainingBytes);
    }

    // Закрытие файлов
    in.close();
    out.close();

    cout << "Файл зашифрован" << endl;
    return 0;
}
