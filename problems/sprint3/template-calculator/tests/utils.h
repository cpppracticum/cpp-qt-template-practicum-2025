#ifndef UTILS_H
#define UTILS_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QLabel>
#include <QtTest>

template<class T>
void findElementByText(T*& child, QObject* parent, const QString& label, const QString& type_name) {
    auto children = parent->findChildren<T*>();

    for (const auto& child_ : children)
    {
        if (child_->text() == label)
        {
            child = child_;
            return;
        }
    }

    QString message = QString("В %3 не найден %2 c текстом %1").arg(label).arg(type_name).arg(parent->objectName());
    QVERIFY2(child, qPrintable(message));
}


template<class T>
T* findElementByText(QObject* parent, const QString& label, const QString& type_name) {

    T* child = nullptr;
    findElementByText<T>(child, parent, label, type_name);
    return child;
}


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

template<typename T>
bool compare(T a, T b, double epsilon = 1e-6) {
    return std::abs(a - b) < epsilon;
}

#endif  // UTILS_H
