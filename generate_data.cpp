/**
 * Генератор тестовых данных для лабораторной работы
 * Создает CSV-файлы с информацией об авиарейсах
 * 
 * Запуск: ./generate_data.exe
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>

using namespace std;

// Структура для хранения данных рейса (только для генерации)
struct FlightData {
    string flightNumber;
    string airline;
    string arrivalDate;
    string arrivalTime;
    int passengers;
};

// Список авиакомпаний
const vector<string> AIRLINES = {
    "Aeroflot",
    "S7 Airlines", 
    "Rossiya",
    "Pobeda",
    "Ural Airlines",
    "Utair",
    "Nordwind",
    "Red Wings"
};

// Префиксы для номеров рейсов
const vector<string> PREFIXES = {"SU", "S7", "FV", "DP", "U6", "UT", "N4", "WZ"};

// Генератор случайных чисел
random_device rd;
mt19937 gen(rd());

// Генерация случайной даты в формате YYYY-MM-DD
string randomDate() {
    uniform_int_distribution<int> yearDist(2024, 2025);
    uniform_int_distribution<int> monthDist(1, 12);
    uniform_int_distribution<int> dayDist(1, 28); // упрощенно, все месяцы по 28 дней
    
    stringstream ss;
    ss << yearDist(gen) << "-"
       << setw(2) << setfill('0') << monthDist(gen) << "-"
       << setw(2) << setfill('0') << dayDist(gen);
    return ss.str();
}

// Генерация случайного времени в формате HH:MM:SS
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

// Генерация случайного номера рейса
string randomFlightNumber() {
    uniform_int_distribution<size_t> prefixDist(0, PREFIXES.size() - 1);
    uniform_int_distribution<int> numDist(100, 9999);
    return PREFIXES[prefixDist(gen)] + to_string(numDist(gen));
}

// Генерация случайной авиакомпании
string randomAirline() {
    uniform_int_distribution<size_t> dist(0, AIRLINES.size() - 1);
    return AIRLINES[dist(gen)];
}

// Генерация случайного числа пассажиров
int randomPassengers() {
    uniform_int_distribution<int> dist(0, 300);
    return dist(gen);
}

// Генерация одного рейса
FlightData generateFlight() {
    FlightData flight;
    flight.flightNumber = randomFlightNumber();
    flight.airline = randomAirline();
    flight.arrivalDate = randomDate();
    flight.arrivalTime = randomTime();
    flight.passengers = randomPassengers();
    return flight;
}

// Сохранение массива в CSV-файл
void saveToCSV(const vector<FlightData>& flights, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось создать файл " << filename << endl;
        return;
    }
    
    // Заголовок
    file << "Номер рейса,Авиакомпания,Дата прилета,Время прилета,Число пассажиров\n";
    
    // Данные
    for (const auto& f : flights) {
        file << f.flightNumber << ","
             << f.airline << ","
             << f.arrivalDate << ","
             << f.arrivalTime << ","
             << f.passengers << "\n";
    }
    
    file.close();
    cout << "Создан файл: " << filename << " (" << flights.size() << " записей)" << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "Генератор тестовых данных" << endl;
    cout << "========================================" << endl;
    
    // Размеры массивов для генерации
    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 20000, 30000, 50000, 75000, 100000};
    
    // Создаем папку data, если её нет
    system("mkdir data 2> nul"); // Windows
    // system("mkdir -p data"); // для Linux/Mac
    
    // Генерируем файлы для каждого размера
    for (size_t size : sizes) {
        cout << "\nГенерация " << size << " записей..." << endl;
        
        vector<FlightData> flights;
        flights.reserve(size);
        
        for (size_t i = 0; i < size; i++) {
            flights.push_back(generateFlight());
        }
        
        string filename = "data/flights_" + to_string(size) + ".csv";
        saveToCSV(flights, filename);
    }
    
    cout << "\n========================================" << endl;
    cout << "Генерация завершена!" << endl;
    cout << "Файлы сохранены в папку data/" << endl;
    cout << "========================================" << endl;
    
    return 0;
}