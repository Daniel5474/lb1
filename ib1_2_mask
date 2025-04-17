#include <iostream>
#include <fstream>
using namespace std;

int main() {
    long long int mask; // Результат маскирования одного блока (64 бита)
    long long int data; // Один блок исходных данных (64 бита)

    // Рекуррентная линия задержки 1 (РЛЗ1)
    unsigned short rlz1 = 0x5B5D;

    // Рекуррентная линия задержки 2 (РЛЗ2)
    unsigned long long rlz2[2] = {
        0x2D952AAAAAA,  
        0x36D6AA         
    };

    unsigned long long gamma = 0; // 64-битная псевдослучайная последовательность (ПСП)
    int bit_gamma = 0;            // Один бит ПСП, вычисляемый на каждом такте
    int buffer = 0;               // Для переноса бита между частями rlz2 при сдвиге

    const char* inputFileName = "lb.txt";     
    const char* outputFileName = "lb_sh.txt";

    // Открытие файлов в бинарном режиме
    ifstream in(inputFileName, ios::binary);
    ofstream out(outputFileName, ios::binary);

    if (!in || !out) {
        cerr << "Ошибка открытия файлов.\n";
        return 1;
    }

    // Поблочное чтение и шифрование по 64 бита
    while (in.read((char*)&data, sizeof(long long int))) {
        gamma = 0; // Обнуляем ПСП на каждый блок

        // Генерация 64-битной ПСП
        for (int i = 0; i < 64; i++) {
            gamma <<= 1; // Освобождаем место для нового бита

            // Вычисление ПСП из РЛЗ1 
            int bit1 = ((rlz1 >> 14) ^ (rlz1 >> 3)) & 0x1;

            // Вычисление ПСП из РЛЗ2
            int bit2 = ((rlz2[1] >> 41) ^ (rlz2[0] >> 14)) & 0x1;

            // Комбинирование ПСП (бит гамма)
            bit_gamma = bit1 ^ bit2; 
            gamma |= bit_gamma;      

            // Обновление состояния РЛЗ1
            rlz1 <<= 1;     
            rlz1 |= bit1;   

            // Обновление состояния РЛЗ2
            buffer = (rlz2[1] >> 41) & 0x1; // Сохраняем старший бит rlz2[1] перед сдвигом
            rlz2[1] <<= 1; // Сдвиг старшей части
            if (rlz2[0] & (1ULL << 63)) rlz2[1] |= 1;
            rlz2[0] <<= 1;
            rlz2[0] |= bit2; 
        }

        // Маскирование данных и запись в выходной файл
        mask = data ^ gamma;
        out.write((char*)&mask, sizeof(long long int));
    }

    in.close();
    out.close();

    cout << "Файл успешно зашифрован и сохранён как: " << outputFileName << "\n";
    return 0;
}
