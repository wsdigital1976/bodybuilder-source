// Copyright (c) 2011-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BODYBUILDER_QT_BODYBUILDERADDRESSVALIDATOR_H
#define BODYBUILDER_QT_BODYBUILDERADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BodybuilderAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BodybuilderAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Bodybuilder address widget validator, checks for a valid bodybuilder address.
 */
class BodybuilderAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BodybuilderAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // BODYBUILDER_QT_BODYBUILDERADDRESSVALIDATOR_H
