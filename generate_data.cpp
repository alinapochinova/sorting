/**
 * @file generate_data.cpp
 * @brief Генератор тестовых данных для лабораторной работы
 * 
 * @author [Ваше имя]
 * @student_id [Номер студенческого]
 * @group [Номер группы]
 * @date 2026
 * @version 1.0
 * 
 * @details Создаёт CSV-файлы с информацией об авиарейсах
 *          для последующего тестирования алгоритмов сортировки.
 * 
 * @section usage Использование
 * Запустите программу без аргументов:
 * @code
 * ./generate_data.exe
 * @endcode
 * 
 * @section output Выходные данные
 * Файлы сохраняются в папку data/ с именами flights_{size}.csv
 * 
 * @see main.cpp
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>

using namespace std;

/**
 * @struct FlightData
 * @brief Вспомогательная структура для генерации данных
 * @details Используется только в процессе генерации, не участвует в сортировке
 */

struct FlightData {
    string flightNumber;   ///< Номер рейса
    string airline;        ///< Авиакомпания
    string arrivalDate;    ///< Дата прилета (YYYY-MM-DD)
    string arrivalTime;    ///< Время прилета (HH:MM:SS)
    int passengers;        ///< Число пассажиров (0-300)
};

/**
 * @brief Список авиакомпаний для генерации
 */

const vector<string> AIRLINES = {
    "Aeroflot", "S7 Airlines", "Rossiya", "Pobeda",
    "Ural Airlines", "Utair", "Nordwind", "Red Wings"
};

/**
 * @brief Префиксы для номеров рейсов
 */

const vector<string> PREFIXES = {"SU", "S7", "FV", "DP", "U6", "UT", "N4", "WZ"};

/**
 * @brief Генератор случайных чисел (Mersenne Twister)
 */

random_device rd;
mt19937 gen(rd());

/**
 * @brief Генерация случайной даты
 * @return Строка в формате YYYY-MM-DD
 * @details Диапазон: 2024-01-01 до 2025-12-28
 */

string randomDate() {
    uniform_int_distribution<int> yearDist(2024, 2025);
    uniform_int_distribution<int> monthDist(1, 12);
    uniform_int_distribution<int> dayDist(1, 28); // упрощённо
    
    stringstream ss;
    ss << yearDist(gen) << "-"
       << setw(2) << setfill('0') << monthDist(gen) << "-"
       << setw(2) << setfill('0') << dayDist(gen);
    return ss.str();
}

/**
 * @brief Генерация случайного времени
 * @return Строка в формате HH:MM:SS
 * @details Диапазон: 00:00:00 до 23:59:59
 */

string randomTime() {
    uniform_int_distribution<int> hourDist(0, 23);
    uniform_int_distribution<int> minuteDist(0, 59);
    uniform_int_distribution<int> secondDist(0, 59);
    
    stringstream ss;
    ss << setw(2) << setfill('0') << hourDist(gen) << ":"
       << setw(2) << setfill('0') << minuteDist(gen) << ":"
       << setw(2) << setfill('0') << secondDist(gen);
    return ss.str();
}

/**
 * @brief Генерация случайного номера рейса
 * @return Строка вида "SU1234"
 */

string randomFlightNumber() {
    uniform_int_distribution<size_t> prefixDist(0, PREFIXES.size() - 1);
    uniform_int_distribution<int> numDist(100, 9999);
    return PREFIXES[prefixDist(gen)] + to_string(numDist(gen));
}

/**
 * @brief Генерация случайной авиакомпании
 * @return Название из списка AIRLINES
 */

string randomAirline() {
    uniform_int_distribution<size_t> dist(0, AIRLINES.size() - 1);
    return AIRLINES[dist(gen)];
}

/**
 * @brief Генерация случайного числа пассажиров
 * @return Число от 0 до 300
 */

int randomPassengers() {
    uniform_int_distribution<int> dist(0, 300);
    return dist(gen);
}

/**
 * @brief Генерация одного случайного рейса
 * @return FlightData со случайными полями
 */

FlightData generateFlight() {
    FlightData flight;
    flight.flightNumber = randomFlightNumber();
    flight.airline = randomAirline();
    flight.arrivalDate = randomDate();
    flight.arrivalTime = randomTime();
    flight.passengers = randomPassengers();
    return flight;
}

/**
 * @brief Сохранение массива рейсов в CSV-файл
 * @param flights Массив данных
 * @param filename Имя выходного файла
 * 
 * @details Создаёт файл с заголовком и данными.
 *          Если файл существует, перезаписывает его.
 */

void saveToCSV(const vector<FlightData>& flights, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось создать файл " << filename << endl;
        return;
    }
    
    file << "Номер рейса,Авиакомпания,Дата прилета,Время прилета,Число пассажиров\n";
    
    for (const auto& f : flights) {
        file << f.flightNumber << ","
             << f.airline << ","
             << f.arrivalDate << ","
             << f.arrivalTime << ","
             << f.passengers << "\n";
    }
    
    file.close();
    
}

/**
 * @brief Главная функция генератора данных
 * @return 0 при успешном выполнении
 * 
 * @details Последовательно генерирует файлы для размеров:
 *          100, 500, 1000, 5000, 10000, 20000, 30000, 50000, 75000, 100000
 * 
 * @post Все файлы сохраняются в папку data/
 * 
 * @see main.cpp
 */

int main() {
    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 20000, 30000, 50000, 75000, 100000};
    
    system("mkdir data 2> nul"); // Windows
    
    for (size_t size : sizes) {          
        vector<FlightData> flights;
        flights.reserve(size);
        
        for (size_t i = 0; i < size; i++) {
            flights.push_back(generateFlight());
        }
        
        string filename = "data/flights_" + to_string(size) + ".csv";
        saveToCSV(flights, filename);
    }
    
    return 0;
}