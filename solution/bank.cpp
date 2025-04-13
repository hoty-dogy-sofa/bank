#include "bank.hpp"

namespace bank {
[[nodiscard]] const std::string &user::name() const noexcept {
    return user_name;
}

[[nodiscard]] int user::balance_xts() const {
    return user_balance_xtc;
}

void user::add_to_balance(int amount) {
    user_balance_xtc += amount;
}

user_transactions_iterator::user_transactions_iterator(
    user_transactions_iterator &&other
) noexcept
    : user_(other.user_), current_number(other.current_number) {
    other.user_ = nullptr;
    other.current_number = 0;
}

[[nodiscard]] bool user::check_transaction(int amount, const user &reciever)
    const {
    if (this == &reciever) {
        const std::string message =
            "Invalid transfer from " + user_name + " to " + user_name;
        throw transfer_error(message);
    }
    if (amount <= 0) {
        const std::string message =
            "Invalid transfer from " + user_name + " to " + reciever.name();
        throw transfer_error(message);
    }
    if (user_balance_xtc < amount) {
        const std::string message =
            "Not enough funds: " + std::to_string(user_balance_xtc) +
            " XTS available, " + std::to_string(amount) + " XTS requested\n";
        throw not_enough_funds_error(message);
    }
    return true;
}

transaction user_transactions_iterator::wait_next_transaction() {
    std::unique_lock<std::mutex> lock(user_->mutex_);
    user_->cv_.wait(lock, [this]() {
        return current_number < user_->user_transaction.size();
    });
    return user_->user_transaction[current_number++];
}

void user::transfer(
    user &counterparty,
    int amount_xts,
    const std::string &comment
) {
    if (check_transaction(amount_xts, counterparty)) {
        const std::scoped_lock lock(mutex_, counterparty.mutex_);

        user_balance_xtc -= amount_xts;
        counterparty.add_to_balance(amount_xts);

        user_transaction.emplace_back(&counterparty, -amount_xts, comment);
        counterparty.user_transaction.emplace_back(this, amount_xts, comment);
        cv_.notify_all();
    }
}

[[nodiscard]] user_transactions_iterator user::monitor() const {
    const std::unique_lock<std::mutex> lock(mutex_);
    return {this, user_transaction.size()};
}

user_transactions_iterator user::snapshot_transactions(
    const std::function<void(const std::vector<transaction> &, int)> &f
) const {
    const std::unique_lock<std::mutex> lock(mutex_);
    f(user_transaction, user_balance_xtc);
    return {this, user_transaction.size()};
}

user_transactions_iterator &user_transactions_iterator::operator=(
    user_transactions_iterator &&other
) noexcept {
    user_ = other.user_;
    current_number = other.current_number;
    other.user_ = nullptr;
    other.current_number = 0;
    return *this;
}

user &ledger::get_or_create_user(const std::string &name) {
    const std::scoped_lock lock(mutex_);
    if (name_to_user.find(name) != name_to_user.end()) {
        return *name_to_user[name];
    }
    name_to_user[name] = std::make_shared<user>(name);
    return *name_to_user[name];
}

}  // namespace bank
