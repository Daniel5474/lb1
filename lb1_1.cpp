#include <iostream>
#include <cstdint>

int main() {
    uint16_t rlz1 = 0x5B5D;  // 15-битный регистр 
    uint64_t rlz2_1 = 0x2D952AAAAAA;  // Младшие 64 бита 106-битного регистра
    uint64_t rlz2_2 = 0x36D6AA;       // Старшие 42 бита 

    int count, count_1 = 0, count_2 = 0;
    std::cout << "Количество бит в последовательности: ";
    std::cin >> count;

    for(int i = 1; i <= count; i++) {
        // Вычисление бита для rlz1
        uint16_t bit1 = ((rlz1 >> 14) ^ (rlz1 >> 3)) & 0x01;
        rlz1 = (rlz1 << 1) | bit1;

        // Вычисление бита для rlz2
        uint64_t bit2 = ((rlz2_2 >> 41) ^ (rlz2_1 >> 14)) & 0x01;
        
        // Сдвиг rlz2 (106 бит)
        uint64_t carry = rlz2_1 >> 63;
        rlz2_1 = (rlz2_1 << 1) | (rlz2_2 >> 41);
        rlz2_2 = (rlz2_2 << 1) | bit2;

        // Комбинирование битов
        uint16_t res_bit = bit1 ^ bit2;
        std::cout << res_bit;

    }

    std::cout << std::endl;

    return 0;
}

