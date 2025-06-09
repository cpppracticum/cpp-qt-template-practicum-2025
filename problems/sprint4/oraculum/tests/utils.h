#ifndef UTILS_H
#define UTILS_H

#include <QtTest>


template<typename T>
void getChild(T*& dst, QObject* parent, const QString& object_name, const QString& type_name)
{

    dst = parent->findChild<T *>(object_name);
    QString message;
    if (type_name != "")
    {
        message = QString("В %3 не найден %1 типа %2").arg(object_name).arg(type_name).arg(parent->objectName());
    }
    else
    {
        message = QString("В %2 не найден %1").arg(object_name).arg(parent->objectName());;
    }

    QVERIFY2(dst, qPrintable(message));
}

template<typename T>
T* getChild(QObject* parent, const QString& object_name, const QString& type_name = {})
{
    T* dst = nullptr;
    getChild<T>(dst, parent, object_name, type_name);

    return dst;

}

template <typename T>
void getChildByText(T*& dst, QObject* parent, const QString& object_text, const QString& type_name = {})
{
    const auto children = parent->findChildren<T *>();
    for (T* child : children) {
        if (child->text() == object_text) {
            dst = child;
            return;
        }
    }

    QString error_message = "В " + parent->objectName() + " не найдено "
        + (type_name == QString{} ? " объекта " : type_name)
        + " c текстом " + object_text;

    QVERIFY2(false, qPrintable(error_message));
}

template <typename T>
T* getChildByText(QObject* parent, const QString& object_text, const QString& type_name = {}) {
    T* dst = {};
    getChildByText<T>(dst, parent, object_text, type_name);
    return dst;
}

inline void clickWidget(QWidget* widget)
{
    QTest::mouseClick(widget, Qt::LeftButton);
}

#endif  // UTILS_H
