#include "last5transactions.h"

last5Transactions::last5Transactions()
{

}

QString last5Transactions::getIdaccount() const
{
    return idaccount;
}

void last5Transactions::setIdaccount(const QString &value)
{
    idaccount= value;
}

QString last5Transactions::getTransactions() const
{
    return transactions;
}

void last5Transactions::setTransactions(const QString &value)
{
    transactions = value;
}

QString last5Transactions::getAmount() const
{
    return amount;
}

void last5Transactions::setAmount(const QString &value)
{
    amount = value;
}

QString last5Transactions::getDate() const
{
    return date;
}

void last5Transactions::setDate(const QString &value)
{
    date = value;
}
