#ifndef BANK_HPP
#define BANK_HPP

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace bank {

inline static const int INITIAL_BALANCE = 100;

class transfer_error : public std::runtime_error {
public:
    explicit transfer_error(const std::string &message)
        : std::runtime_error(message) {
    }
};

class not_enough_funds_error : public transfer_error {
public:
    explicit not_enough_funds_error(const std::string &msg)
        : transfer_error(msg) {
    }
};

class user;
class user_transactions_iterator;

// NOLINTBEGIN(misc-non-private-member-variables-in-classes,-warnings-as-errors)
class transaction {
public:
    const user *const counterparty;
    const int balance_delta_xts;
    const std::string comment;

    transaction(const user *cp, int delta, std::string cmt)
        : counterparty(cp), balance_delta_xts(delta), comment(std::move(cmt)) {
    }
};

// NOLINTEND(misc-non-private-member-variables-in-classes,-warnings-as-errors)

class user {
private:
    std::string user_name;
    int user_balance_xtc = {INITIAL_BALANCE};
    std::vector<transaction> user_transaction = {transaction(
        nullptr,
        INITIAL_BALANCE,
        "Initial deposit for " + user_name
    )};
    [[nodiscard]] bool check_transaction(int amount, const user &reciever)
        const;
    mutable std::condition_variable cv_;
    mutable std::mutex mutex_;

public:
    friend class user_transactions_iterator;

    explicit user(std::string name_of_user)
        : user_name(std::move(name_of_user)){};

    void add_to_balance(int amount);
    [[nodiscard]] const std::string &name() const noexcept;
    [[nodiscard]] int balance_xts() const;
    void
    transfer(user &counterparty, int amount_xts, const std::string &comment);
    user_transactions_iterator snapshot_transactions(
        const std::function<void(const std::vector<transaction> &, int)> &f
    ) const;

    [[nodiscard]] user_transactions_iterator monitor() const;
};

class user_transactions_iterator {
private:
    const user *user_;
    size_t current_number;

public:
    user_transactions_iterator(const user *user_x, size_t number)
        : user_(user_x), current_number(number) {
    }

    user_transactions_iterator(const user_transactions_iterator &other
    ) = default;

    user_transactions_iterator &operator=(
        const user_transactions_iterator &other
    ) = default;

    user_transactions_iterator(user_transactions_iterator &&other) noexcept;

    user_transactions_iterator &operator=(user_transactions_iterator &&other
    ) noexcept;

    ~user_transactions_iterator() = default;

    transaction wait_next_transaction();
};

class ledger {
private:
    std::unordered_map<std::string, std::shared_ptr<user>> name_to_user;
    mutable std::mutex mutex_;

public:
    ledger() = default;
    user &get_or_create_user(const std::string &name);
};

}  // namespace bank

#endif
