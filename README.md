# 🏦 Банковская система (Thread-Safe)

## 📌 Описание проекта
Реализация потокобезопасной банковской системы с поддержкой транзакций и блокирующих итераторов. В будущем будет добавлен сервер.

## 🏗 Структура проекта
```bash
bank
├── LICENSE
├── README.md
└── solution
    ├── bank.cpp
    ├── bank.hpp
    └── bank_server.cpp
```

# 🏦 Банковская система (Thread-Safe)

## 📌 Описание проекта
Реализация потокобезопасной банковской системы с поддержкой транзакций и блокирующих итераторов.

## 🏗 Структура проекта

```
bank-system/
├── CMakeLists.txt
├── include/
│   ├── bank/
│   │   ├── ledger.hpp       # Класс банковского гроссбуха
│   │   ├── user.hpp         # Класс пользователя
│   │   ├── transaction.hpp  # Структура транзакции
│   │   └── errors.hpp       # Исключения
├── src/
│   ├── ledger.cpp
│   ├── user.cpp
│   └── main.cpp
└── tests/
    ├── test_basic.cpp
    └── test_threads.cpp
```


## 🛠 Реализованные функции

| Компонент | Методы | Описание |
|-----------|--------|----------|
| `ledger` | `get_or_create_user` | Создает/возвращает пользователя |
| `user` | `transfer` | Выполняет перевод |
|  | `balance_xts` | Возвращает баланс |
|  | `snapshot_transactions` | Атомарный снимок транзакций |
| `user_transactions_iterator` | `wait_next_transaction` | Блокирует до новой транзакции |


## 📝 Требования

- C++17
- Потокобезопасность (const-thread safe)
- Атомарные операции
- Блокирующие итераторы


Проект реализует потокобезопасную банковскую систему с поддержкой:
- Управления пользователями
- Денежных переводов
- Мониторинга транзакций в реальном времени
- Обработки ошибок перевода
