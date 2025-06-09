#ifndef UTILS_H
#define UTILS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
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

inline void clickWidget(QWidget* widget)
{
    QTest::mouseClick(widget, Qt::LeftButton);
}

#endif  // UTILS_H
