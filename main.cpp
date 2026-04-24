#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>

using namespace std;
using namespace chrono;


class Flight {
private:
    string flightNumber;
    string airline;
    string arrivalDate;
    string arrivalTime;
    int passengers;
    
public:
    Flight() : flightNumber(""), airline(""), arrivalDate(""), arrivalTime(""), passengers(0) {}
    
    Flight(const string& fn, const string& al, const string& date, 
           const string& time, int pass)
        : flightNumber(fn), airline(al), arrivalDate(date), 
          arrivalTime(time), passengers(pass) {}
    
    // Геттеры
    string getFlightNumber() const { return flightNumber; }
    string getAirline() const { return airline; }
    string getArrivalDate() const { return arrivalDate; }
    string getArrivalTime() const { return arrivalTime; }
    int getPassengers() const { return passengers; }
    
    // Операторы сравнения
    bool operator<(const Flight& other) const {
        if (arrivalDate != other.arrivalDate)
            return arrivalDate < other.arrivalDate;
        if (arrivalTime != other.arrivalTime)
            return arrivalTime < other.arrivalTime;
        if (airline != other.airline)
            return airline < other.airline;
        return passengers > other.passengers; // по убыванию
    }
    
    bool operator>(const Flight& other) const {
        return other < *this;
    }
    
    // Для вывода в CSV (без заголовка)
    string toCSV() const {
        return flightNumber + "," + airline + "," + arrivalDate + "," + arrivalTime + "," + to_string(passengers);
    }
    
    friend ostream& operator<<(ostream& os, const Flight& f) {
        os << f.flightNumber << "," << f.airline << "," << f.arrivalDate << "," 
           << f.arrivalTime << "," << f.passengers;
        return os;
    }
    
    // Чтение из CSV
    friend istream& operator>>(istream& is, Flight& f) {
        string line;
        if (!getline(is, line)) return is;
        stringstream ss(line);
        getline(ss, f.flightNumber, ',');
        getline(ss, f.airline, ',');
        getline(ss, f.arrivalDate, ',');
        getline(ss, f.arrivalTime, ',');
        string passStr;
        getline(ss, passStr, ',');
        f.passengers = stoi(passStr);
        return is;
    }
};

template<typename T>
void bubbleSort(vector<T>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (size_t j = n - 1; j > i; j--) {
            if (arr[j - 1] > arr[j]) {
                swap(arr[j - 1], arr[j]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

template<typename T>
void shakerSort(vector<T>& arr) {
    size_t n = arr.size();
    size_t left = 0;
    size_t right = n - 1;
    bool swapped = true;
    
    while (left < right && swapped) {
        swapped = false;
        for (size_t i = right; i > left; i--) {
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        left++;
        for (size_t i = left; i <= right; i++) {
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        right--;
    }
}

template<typename T>
int partition(vector<T>& arr, int low, int high) {
    int mid = low + (high - low) / 2;
    T pivot = arr[mid];
    int i = low;
    int j = high;
    while (i <= j) {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    return i;
}

template<typename T>
void quickSortRecursive(vector<T>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSortRecursive(arr, low, pivotIndex - 1);
        quickSortRecursive(arr, pivotIndex, high);
    }
}

template<typename T>
void quickSort(vector<T>& arr) {
    if (arr.size() <= 1) return;
    quickSortRecursive(arr, 0, arr.size() - 1);
}

template<typename T>
bool isSorted(const vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

// Загрузка из CSV
vector<Flight> loadFromCSV(const string& filename) {
    vector<Flight> flights;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return flights;
    }
    
    string line;
    getline(file, line); // пропускаем заголовок
    
    Flight f;
    while (file >> f) {
        flights.push_back(f);
    }
    
    file.close();
    return flights;
}

// Сохранение результата в файл
void saveResultToFile(const vector<Flight>& arr, const string& filename, const string& sortName) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return;
    }
    
    file << "=== " << sortName << " ===\n";
    for (const auto& f : arr) {
        file << f.toCSV() << "\n";
    }
    file << "\n";
    file.close();
}

// Замер времени
template<typename SortFunc>
long long measureTime(vector<Flight> arr, SortFunc sortFunc, 
                      const string& sortName, const string& resultFilename) {
    
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start).count();
    
    if (!isSorted(arr)) {
        cerr << "ОШИБКА: " << sortName << " не отсортировала массив!" << endl;
    } else {
        cout << "  " << sortName << ": " << duration << " мкс (OK)" << endl;
        saveResultToFile(arr, resultFilename, sortName);
    }
    
    return duration;
}

int main() {
    system("mkdir sorted_results 2> nul");

    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 20000, 30000, 50000, 75000, 100000};
    
    // Открываем CSV для результатов замеров времени
    ofstream timingFile("timing_results.csv");
    timingFile << "Размер,Bubble,Shaker,Quick,std_sort\n";
    
    for (size_t size : sizes) {
        cout << "\n--- Размер массива: " << size << " ---" << endl;
        
        // Загружаем данные из CSV
        string filename = "data/flights_" + to_string(size) + ".csv";
        vector<Flight> original = loadFromCSV(filename);
        
        if (original.empty()) {
            cerr << "Ошибка: не удалось загрузить данные для размера " << size << endl;
            continue;
        }
        
        cout << "Загружено " << original.size() << " записей" << endl;
        
        // Файл для сохранения результатов сортировки для этого размера
        string resultFilename = "sorted_results/sorted_results_" + to_string(size) + ".txt";
        
        // Очищаем файл результатов (создаем заново)
        ofstream clearFile(resultFilename);
        clearFile << "Результаты сортировки для размера " << size << "\n";
        clearFile.close();
        
        // Создаем копии
        vector<Flight> arrBubble = original;
        vector<Flight> arrShaker = original;
        vector<Flight> arrQuick = original;
        vector<Flight> arrStd = original;
        
        // Замеряем
        long long timeBubble = measureTime(arrBubble, bubbleSort<Flight>, 
                                           "Сортировка пузырьком (Bubble)", resultFilename);
        long long timeShaker = measureTime(arrShaker, shakerSort<Flight>, 
                                           "Шейкер-сортировка (Shaker)", resultFilename);
        long long timeQuick = measureTime(arrQuick, quickSort<Flight>, 
                                          "Быстрая сортировка (Quick)", resultFilename);
        
        // std::sort
        auto start = high_resolution_clock::now();
        sort(arrStd.begin(), arrStd.end());
        auto end = high_resolution_clock::now();
        long long timeStd = duration_cast<microseconds>(end - start).count();
        
        if (isSorted(arrStd)) {
            cout << "  std::sort: " << timeStd << " мкс (OK)" << endl;
            saveResultToFile(arrStd, resultFilename, "std::sort");
        }
        
        // Записываем в CSV для графиков
        timingFile << size << "," << timeBubble << "," << timeShaker << "," 
                   << timeQuick << "," << timeStd << "\n";
    }
    
    timingFile.close();
       
    return 0;
}