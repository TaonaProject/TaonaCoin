// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017-2019 The Raven Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef TAONA_QT_TAONAADDRESSVALIDATOR_H
#define TAONA_QT_TAONAADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class TaonaAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit TaonaAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Taona address widget validator, checks for a valid taona address.
 */
class TaonaAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit TaonaAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // TAONA_QT_TAONAADDRESSVALIDATOR_H
