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
