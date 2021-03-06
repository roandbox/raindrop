#ifndef TEXTCLIPBOARDITEM_H
#define TEXTCLIPBOARDITEM_H

#include <QPlainTextEdit>

#include <iclipboarditem.h>

namespace RTPTechGroup {
namespace SqlEditor {

//! Класс элемента стека буфера обмена редактора sql запросов

class TextClipboardItem: public QObject, public IClipboardItem
{
    Q_OBJECT
public:
    //! Конструктор класса
    explicit TextClipboardItem(QPlainTextEdit *editor);

    //! Возвращает возможность вызова команды "Вырезать"
    bool canCut();

    //! Возвращает возможность вызова команды "Копировать"
    bool canCopy();

    //! Возвращает возможность вызова команды "Вставить"
    bool canPaste();

    //! Возвращает возможность вызова команды "Выделить все"
    bool canSelectAll();

    //! Вызов команды "Вырезать"
    void cut();

    //! Вызов команды "Копировать"
    void copy();

    //! Вызов команды "Вставить"
    void paste();

    //! Вызов команды "Выделить все"
    void selectAll();

private slots:
    //! Обработка сигнала выбора текста
    void selectionChange();

private:
    QPlainTextEdit *m_editor;

};

}}

#endif // TEXTCLIPBOARDITEM_H
