#include "regexpvalidator.h"

namespace RTPTechGroup {
namespace ModelerIde {

RegExpValidator::RegExpValidator(QObject *parent)
    :QRegExpValidator(parent)
{

}

RegExpValidator::RegExpValidator(const QRegExp &rx, QObject *parent)
    :QRegExpValidator(rx, parent)
{

}

QValidator::State RegExpValidator::validate(QString &input, qint32 &pos) const
{
    QValidator::State state = QRegExpValidator::validate(input, pos);
    emit stateChanged(state);

    return state;
}

}}
