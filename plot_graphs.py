import pandas as pd
import matplotlib.pyplot as plt

# Читаем данные
df = pd.read_csv('timing_results.csv')
sizes = df['Размер']

# Переводим микросекунды в миллисекунды
bubble = df['Bubble'] / 1000
shaker = df['Shaker'] / 1000
quick = df['Quick'] / 1000
std_sort = df['std_sort'] / 1000

print("Данные для графиков:")
print(df.to_string(index=False))
print("\n" + "="*50 + "\n")

# ========== ГРАФИК 1 + 2: Два графика на одной картинке ==========
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

# Левый график: медленные алгоритмы (Bubble, Shaker)
ax1.plot(sizes, bubble, 'o-', label='Bubble sort', color='red', linewidth=2, markersize=6)
ax1.plot(sizes, shaker, 's-', label='Shaker sort', color='orange', linewidth=2, markersize=6)
ax1.set_xlabel('Размер массива (количество элементов)', fontsize=11)
ax1.set_ylabel('Время сортировки (миллисекунды)', fontsize=11)
ax1.set_title('Медленные алгоритмы (Bubble, Shaker)', fontsize=12)
ax1.legend(fontsize=10)
ax1.grid(True, alpha=0.3)

# Правый график: быстрые алгоритмы (Quick, std::sort)
ax2.plot(sizes, quick, '^-', label='Quick sort', color='green', linewidth=2, markersize=6)
ax2.plot(sizes, std_sort, 'd-', label='std::sort', color='blue', linewidth=2, markersize=6)
ax2.set_xlabel('Размер массива (количество элементов)', fontsize=11)
ax2.set_ylabel('Время сортировки (миллисекунды)', fontsize=11)
ax2.set_title('Быстрые алгоритмы (Quick, std::sort)', fontsize=12)
ax2.legend(fontsize=10)
ax2.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('sorting_graph_two_panels.png', dpi=150, bbox_inches='tight')
print("Сохранён график 1: sorting_graph_two_panels.png (два графика на одной картинке)")
plt.show()

# ========== ГРАФИК 3: Все алгоритмы с логарифмической шкалой (отдельно) ==========
plt.figure(figsize=(10, 6))

plt.plot(sizes, bubble, 'o-', label='Bubble sort', color='red', linewidth=2, markersize=6)
plt.plot(sizes, shaker, 's-', label='Shaker sort', color='orange', linewidth=2, markersize=6)
plt.plot(sizes, quick, '^-', label='Quick sort', color='green', linewidth=2, markersize=6)
plt.plot(sizes, std_sort, 'd-', label='std::sort', color='blue', linewidth=2, markersize=6)

# Включаем логарифмическую шкалу по обеим осям
plt.yscale('log')
plt.xscale('log')

plt.xlabel('Размер массива (логарифмическая шкала)', fontsize=12)
plt.ylabel('Время сортировки (миллисекунды) - логарифмическая шкала', fontsize=12)
plt.title('Все алгоритмы сортировки (логарифмическая шкала)', fontsize=14)
plt.legend(fontsize=12)
plt.grid(True, alpha=0.3, which='both')

plt.savefig('sorting_graph_all_log.png', dpi=150, bbox_inches='tight')
print("Сохранён график 2: sorting_graph_all_log.png (логарифмическая шкала)")
plt.show()

print("\nГотово! Сохранены файлы:")
print("  1. sorting_graph_two_panels.png")
print("  2. sorting_graph_all_log.png")