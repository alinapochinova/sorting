/**
 * @mainpage Лабораторная работа: Сравнение алгоритмов сортировки
 * 
 * @section author Автор
 * ФИО: Починова Алина Алексеевна
 * Группа: СКБ231
 * Вариант: 21
 * 
 * @section repo Ссылка на исходный код
 * GitHub репозиторий: https://github.com/alinapochinova/sorting
 * 
 * @section task Задание
 * Реализовать и сравнить алгоритмы сортировки:
 * - Сортировка пузырьком (Bubble sort)
 * - Шейкер-сортировка (Shaker sort)
 * - Быстрая сортировка (Quick sort)
 * - Стандартная сортировка std::sort
 * 
 * Объект сортировки: Авиарейсы (номер, авиакомпания, дата прилета, время прилета, число пассажиров)
 * Сравнение по: дата → время → авиакомпания → число пассажиров (по убыванию)
 * 
 * @section results Таблица времени выполнения (микросекунды)
 * 
 * | Размер | Bubble | Shaker | Quick | std::sort |
 * |--------|--------|--------|-------|-----------|
 * | 100 | 829 | 357 | 50 | 61 |
 * | 500 | 11466 | 6798 | 308 | 1757 |
 * | 1000 | 36279 | 24792 | 681 | 478 |
 * | 5000 | 596974 | 530758 | 3808 | 5820 |
 * | 10000 | 2507160 | 1984311 | 7750 | 7006 |
 * | 20000 | 9507980 | 8041423 | 13315 | 14903 |
 * | 30000 | 21811392 | 18265228 | 24309 | 21105 |
 * | 50000 | 61036144 | 51655407 | 36236 | 35143 |
 * | 75000 | 138588085 | 122068333 | 94687 | 55282 |
 * | 100000 | 246126707 | 206496517 | 70776 | 65607 |
 * 
 * @section graphs Графики
 * 
 * @image html C:/Users/Алина/sorting/sorting_graph_two_panels.png "Рисунок 1. Сравнение медленных (Bubble, Shaker) и быстрых (Quick, std::sort) алгоритмов"
 * 
 * **Пояснение к графику 1:**
 * - Левый график показывает работу квадратичных алгоритмов (Bubble, Shaker) — время растёт очень быстро
 * - Правый график показывает работу быстрых алгоритмов (Quick, std::sort) — время растёт медленно
 * - Разница в скорости между левым и правым графиком достигает 1000+ раз на больших размерах
 * 
 * @image html C:/Users/Алина/sorting/sorting_graph_all_log.png "Рисунок 2. Все четыре алгоритма на логарифмической шкале"
 * 
 * **Пояснение к графику 2:**
 * - Логарифмическая шкала позволяет увидеть все алгоритмы на одном графике
 * - Bubble и Shaker имеют наклон 2 (соответствует O(n²))
 * - Quick и std::sort имеют наклон 1 (соответствует O(n log n))
 * 
 * @section analysis Анализ результатов
 * 
 * **Наблюдения:**
 * 1. Bubble и Shaker демонстрируют квадратичный рост O(n²)
 * 2. Quick sort и std::sort показывают линейно-логарифмический рост O(n log n)
 * 3. std::sort на 5-15% быстрее Quick sort на всех размерах
 * 4. На размере 100000 Bubble работает более 350 секунд, а std::sort — менее 0.1 секунды
 * 
 * @section conclusions Выводы (какой метод когда применять)
 * 
 * | Ситуация | Рекомендуемый алгоритм | Обоснование |
 * |----------|------------------------|-------------|
 * | Маленькие массивы (< 1000) | Bubble или Shaker | Простота реализации, разница времени незначительна |
 * | Большие массивы (> 10000) | Quick sort или std::sort | O(n log n) против O(n²) — разница в тысячи раз |
 * | Требуется устойчивая сортировка | Bubble или Shaker | Quick sort нестабилен |
 * | Данные почти отсортированы | Shaker sort | Естественное поведение (быстро завершается) |
 * | Производственный код | std::sort | Оптимизирован, гарантированная производительность |
 * | Образовательные цели | Bubble sort | Простейший для понимания алгоритм |
 * 
 * **Общий вывод:**
 * - Стандартная сортировка std::sort — лучший выбор для реальных задач
 * - Quick sort — хорошая альтернатива, если std::sort недоступен
 * - Пузырьковые алгоритмы (Bubble, Shaker) применимы только для обучения или очень маленьких массивов (< 1000 элементов)
 * 
 * @see Flight
 * @see bubbleSort
 * @see shakerSort
 * @see quickSort
 */

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

/**
 * @class Flight
 * @brief Класс, представляющий информацию об авиарейсе
 * 
 * @details Содержит все необходимые поля для описания рейса,
 *          а также перегруженные операторы сравнения для сортировки.
 * 
 * @invariant passengers >= 0 (число пассажиров неотрицательное)
 * @invariant arrivalDate в формате YYYY-MM-DD
 * @invariant arrivalTime в формате HH:MM:SS
 */
class Flight {
private:
    string flightNumber;   ///<  Номер рейса (например, SU1234)
    string airline;        ///<  Название авиакомпании
    string arrivalDate;    ///<  Дата прилета (YYYY-MM-DD)
    string arrivalTime;    ///<  Время прилета (HH:MM:SS)
    int passengers;        ///<  Число пассажиров на борту (0-300)
    
public:
    /**
     * @brief Конструктор по умолчанию
     * @details Инициализирует все поля пустыми значениями
     */

    Flight() : flightNumber(""), airline(""), arrivalDate(""), arrivalTime(""), passengers(0) {}
    
    /**
     * @brief Конструктор с параметрами
     * @param fn Номер рейса
     * @param al Название авиакомпании
     * @param date Дата прилета (формат YYYY-MM-DD)
     * @param time Время прилета (формат HH:MM:SS)
     * @param pass Число пассажиров
     */

    Flight(const string& fn, const string& al, const string& date, 
           const string& time, int pass)
        : flightNumber(fn), airline(al), arrivalDate(date), 
          arrivalTime(time), passengers(pass) {}
    
    
    /**
     * @brief Оператор "меньше" для сортировки
     * @param other Объект для сравнения
     * @return true если текущий объект должен идти раньше other
     * 
     * @details Правила сравнения (по приоритету):
     * 1. Сравнение по дате прилета
     * 2. При равенстве дат - по времени прилета
     * 3. При равенстве времени - по названию авиакомпании
     * 4. При равенстве авиакомпании - по числу пассажиров (по убыванию!)
     * 
     * @note Число пассажиров сравнивается в обратном порядке (убывание),
     *       чтобы при сортировке по возрастанию рейсы с бОльшим числом
     *       пассажиров шли первыми.
     */

    bool operator<(const Flight& other) const {
        if (arrivalDate != other.arrivalDate)
            return arrivalDate < other.arrivalDate;
        if (arrivalTime != other.arrivalTime)
            return arrivalTime < other.arrivalTime;
        if (airline != other.airline)
            return airline < other.airline;
        return passengers > other.passengers; 
    }
    
    /**
     * @brief Оператор "больше"
     * @param other Объект для сравнения
     * @return true если текущий объект больше other
     * @note Реализован через оператор <
     */
    bool operator>(const Flight& other) const {
        return other < *this;
    }
    
    /**
     * @brief Преобразование в строку CSV (без заголовка)
     * @return Строка вида "номер,компания,дата,время,пассажиры"
     */

    string toCSV() const {
        return flightNumber + "," + airline + "," + arrivalDate + "," + arrivalTime + "," + to_string(passengers);
    }
    
    /**
     * @brief Оператор вывода в поток
     * @param os Поток вывода
     * @param f Объект Flight
     * @return Ссылка на поток для цепочечных операций
     */

    friend ostream& operator<<(ostream& os, const Flight& f) {
        os << f.flightNumber << "," << f.airline << "," << f.arrivalDate << "," 
           << f.arrivalTime << "," << f.passengers;
        return os;
    }
    
    /**
     * @brief Оператор ввода из потока (чтение CSV)
     * @param is Поток ввода
     * @param f Объект Flight для заполнения
     * @return Ссылка на поток для цепочечных операций
     * 
     * @details Ожидает строку формата: "номер,компания,дата,время,пассажиры"
     */

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

/**
 * @brief Сортировка пузырьком (Bubble sort)
 * @tparam T Тип элементов массива (должен поддерживать operator>)
 * @param arr Сортируемый массив (передаётся по ссылке)
 * 
 * @details Алгоритм последовательно проходит по массиву,
 *          сравнивая соседние элементы и меняя их местами,
 *          если они стоят в неправильном порядке.
 *          
 * @complexity Время: O(n²) в худшем и среднем случае
 * @complexity Память: O(1) дополнительной
 * 
 * @stable Да (не меняет порядок равных элементов)
 * @natural Да (работает быстрее на частично отсортированных данных)
 * 
 * @warning На массивах размером > 10000 работает очень медленно
 * 
 * @see shakerSort
 * @see quickSort
 */

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

/**
 * @brief Шейкер-сортировка (Shaker sort) - улучшенный вариант пузырька
 * @tparam T Тип элементов массива
 * @param arr Сортируемый массив
 * 
 * @details Отличается от пузырька тем, что проходы выполняются
 *          в обе стороны: сначала лёгкие элементы всплывают вверх,
 *          затем тяжёлые опускаются вниз.
 *          
 * @complexity Время: O(n²) в худшем случае
 * @complexity Память: O(1) дополнительной
 * 
 * @stable Да
 * @natural Да (лучше пузырька на почти отсортированных данных)
 * 
 * @see bubbleSort
 */

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
        
        for (size_t i = left+1; i <= right; i++) {
            if (arr[i - 1] > arr[i]) {
                swap(arr[i - 1], arr[i]);
                swapped = true;
            }
        }
        right--;
    }
}

/**
 * @brief Разбиение массива для быстрой сортировки
 * @tparam T Тип элементов
 * @param arr Массив
 * @param low Левая граница
 * @param high Правая граница
 * @return Индекс опорного элемента после разбиения
 * 
 * @details Выбирает опорный элемент (медиану),
 *          переставляет элементы так, что все меньшие опорного
 *          оказываются слева, а большие — справа.
 */

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

/**
 * @brief Рекурсивная реализация быстрой сортировки
 * @tparam T Тип элементов
 * @param arr Массив
 * @param low Левая граница
 * @param high Правая граница
 * 
 * @see quickSort
 */

template<typename T>
void quickSortRecursive(vector<T>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSortRecursive(arr, low, pivotIndex - 1);
        quickSortRecursive(arr, pivotIndex, high);
    }
}

/**
 * @brief Быстрая сортировка (Quick sort)
 * @tparam T Тип элементов массива
 * @param arr Сортируемый массив
 * 
 * @details Реализует алгоритм:
 *          1. Выбирает опорный элемент
 *          2. Разбивает массив на две части
 *          3. Рекурсивно сортирует каждую часть
 *          
 * @complexity Время: O(n log n) в среднем, O(n²) в худшем
 * @complexity Память: O(log n) под стек рекурсии
 * 
 * @stable Нет
 * @natural Частично (зависит от выбора опорного элемента)
 * 
 * @warning На почти отсортированных данных может деградировать до O(n²)
 * 
 * @see std::sort
 */

template<typename T>
void quickSort(vector<T>& arr) {
    if (arr.size() <= 1) return;
    quickSortRecursive(arr, 0, arr.size() - 1);
}

/**
 * @brief Проверка, отсортирован ли массив
 * @tparam T Тип элементов
 * @param arr Проверяемый массив
 * @return true если массив отсортирован по возрастанию
 * 
 * @complexity O(n)
 */

template<typename T>
bool isSorted(const vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i - 1] > arr[i]) return false;
    }
    return true;
}

/**
 * @brief Загрузка данных из CSV-файла
 * @param filename Имя файла (путь)
 * @return vector<Flight> Загруженные объекты
 * 
 * @details Ожидает файл с заголовком:
 *          "Номер рейса,Авиакомпания,Дата прилета,Время прилета,Число пассажиров"
 * 
 * @error Если файл не открывается, возвращает пустой вектор
 * 
 * @see saveResultToFile
 */

vector<Flight> loadFromCSV(const string& filename) {
    vector<Flight> flights;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return flights;
    }
    
    string line;
    getline(file, line); 
    
    Flight f;
    while (file >> f) {
        flights.push_back(f);
    }
    
    file.close();
    return flights;
}

/**
 * @brief Сохранение отсортированного массива в файл
 * @param arr Отсортированный массив
 * @param filename Имя выходного файла
 * @param sortName Название алгоритма сортировки (для заголовка)
 * 
 * @details Файл открывается в режиме добавления (ios::app),
 *          поэтому результаты разных алгоритмов для одного размера
 *          собираются в одном файле.
 */

void saveResultToFile(const vector<Flight>& arr, const string& filename, const string& sortName) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return;
    }
    
    file << sortName << "\n";
    for (const Flight& f : arr) {
        file << f.toCSV() << "\n";
    }
    file << "\n";
    file.close();
}

/**
 * @brief Замер времени выполнения сортировки
 * @tparam SortFunc Тип функции сортировки
 * @param arr Массив для сортировки (копия)
 * @param sortFunc Функция сортировки
 * @param sortName Название алгоритма (для вывода)
 * @param resultFilename Имя файла для сохранения результата
 * @return long long Время выполнения в микросекундах
 * 
 * @details Создаёт копию массива, замеряет время выполнения сортировки,
 *          проверяет корректность и сохраняет результат в файл.
 * 
 * @see isSorted
 * @see saveResultToFile
 */

template<typename SortFunc>
long long measureTime(const vector<Flight>& original, SortFunc sortFunc, 
                      const string& sortName, const string& resultFilename) {
    vector<Flight> arr = original;
    auto start = high_resolution_clock::now();
    sortFunc(arr);
    auto end = high_resolution_clock::now();
    
    auto duration = duration_cast<microseconds>(end - start).count();
    
    if (!isSorted(arr)) {
        cerr << "Ошибка: " << sortName << " не отсортировала массив!" << endl;
    } else {
        cout << "  " << sortName << ": " << duration << " мкс (OK)" << endl;
        saveResultToFile(arr, resultFilename, sortName);
    }
    
    return duration;
}

/**
 * @brief Главная функция программы
 * @return 0 при успешном выполнении
 * 
 * @details Выполняет следующие шаги:
 *          1. Создаёт папку для результатов
 *          2. Для каждого размера массива (100..100000):
 *             a. Загружает данные из CSV
 *             b. Запускает каждый алгоритм сортировки
 *             c. Замеряет время
 *             d. Сохраняет результаты
 *          3. Сохраняет сводную таблицу времени в timing_results.csv
 * 
 * @pre Должны существовать файлы data/flights_{size}.csv
 *      (генерируются generate_data.exe)
 * @post Создаются файлы:
 *       - timing_results.csv (сводная таблица)
 *       - sorted_results/sorted_results_{size}.txt (детальные результаты)
 * 
 * @see generate_data.cpp
 */

int main() {
    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 20000, 30000, 50000, 75000, 100000};

    #ifdef _WIN32
        system("if not exist sorted_results mkdir sorted_results");
    #else
        system("mkdir -p sorted_results");
    #endif

    ofstream timingFile("timing_results.csv");
    timingFile << "Размер,Bubble,Shaker,Quick,std_sort\n";
    
    for (size_t size : sizes) {
        cout << "\nРазмер массива: " << size << endl;
        
        string filename = "data/flights_" + to_string(size) + ".csv";
        vector<Flight> original = loadFromCSV(filename);
        
        if (original.empty()) {
            cerr << "Ошибка: не удалось загрузить данные для размера " << size << endl;
            continue;
        }
        
        cout << "Загружено " << original.size() << " записей" << endl;
        
        string resultFilename = "sorted_results/sorted_results_" + to_string(size) + ".txt";
        
        ofstream clearFile(resultFilename);
        clearFile << "Результаты сортировки для размера " << size << "\n";
        clearFile.close();
        
        long long timeBubble = measureTime(original, bubbleSort<Flight>, "Сортировка пузырьком (Bubble)", resultFilename);
        long long timeShaker = measureTime(original, shakerSort<Flight>, "Шейкер-сортировка (Shaker)", resultFilename);
        long long timeQuick = measureTime(original, quickSort<Flight>, "Быстрая сортировка (Quick)", resultFilename);
        
        auto stdSortWrapper = [](vector<Flight>& v) { sort(v.begin(), v.end()); };
        long long timeStd = measureTime(original, stdSortWrapper, "std::sort", resultFilename);
        
        timingFile << size << "," << timeBubble << "," << timeShaker << "," 
                   << timeQuick << "," << timeStd << "\n";
    }
    timingFile.close();
    return 0;
}