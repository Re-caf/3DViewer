#ifndef OBSERVER_HH
#define OBSERVER_HH

#include <algorithm>
#include <vector>

namespace s21 {
/// @brief Интерфейс наблюдателя
class IObserver {
 public:
  /// @brief Обновление наблюдателя
  virtual void Update() = 0;
  virtual ~IObserver() = default;
};

/// @brief Интерфейс наблюдаемого объекта
class IObservable {
 protected:
  std::vector<IObserver*> observers;

 public:
  /// @brief Добавление наблюдателя
  void AddObserver(IObserver* observer) { observers.push_back(observer); }
  /// @brief Удаление наблюдателя
  void RemoveObserver(IObserver* observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }
  /// @brief Уведомление наблюдателей
  void NotifyObservers() {
    for (IObserver* observer : observers) {
      observer->Update();
    }
  }
  /// @brief Деструктор
  virtual ~IObservable() = default;
};
}  // namespace s21
#endif  // OBSERVER_HH