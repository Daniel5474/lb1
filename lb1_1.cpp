#include <iostream>
#include <fstream>
using namespace std;

int main() {
    // Рекуррентная линия задержки 1 (РЛЗ1)
    unsigned short rlz1 = 0x5B5D;

    int bit_gamma = 0;  
    int buffer = 0;     

    // Рекуррентная линия задержки 2 (РЛЗ2)
    unsigned long long rlz2[2] = {
        0x2D952AAAAAA,  
        0x36D6AA        
    };

    unsigned long long gamma = 0; // Результирующая выходная последовательность (64 бита)

    // Генерируем 64 бита ПСП
    for (int i = 0; i < 64; i++) {
        gamma = gamma << 1;  // Освобождаем младший бит

        // Вычисляем бит обратной связи для РЛЗ1
        int bit1 = ((rlz1 >> 14) ^ (rlz1 >> 3)) & 0x1;

        // Вычисляем бит обратной связи для РЛЗ2
        int bit2 = (((rlz2[1] >> 41) ^ (rlz2[0] >> 14)) & 0x1);

        bit_gamma = bit1 ^ bit2;
        gamma |= bit_gamma;  // Записываем бит в младший разряд gamma

        // Обновляем состояния регистров (РЛЗ1 и РЛЗ2) 

        // Сдвигаем РЛЗ1 на 1 бит влево и вставляем бит обратной связи в младший разряд
        rlz1 <<= 1;
        rlz1 |= bit1;

        // Сдвиг РЛЗ2
        buffer = (rlz2[1] >> 41) & 0x1;

        rlz2[1] <<= 1;

        if (rlz2[0] & (1ULL << 63)) rlz2[1] |= 1;

        rlz2[0] <<= 1;
        rlz2[0] |= bit2;
    }

    cout << hex << gamma << "\n";
    return 0;
}
