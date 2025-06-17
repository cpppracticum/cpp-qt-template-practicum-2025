#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void ShowQuestion();

    void Reset();

private slots:
    void on_btn_yes_clicked();

    void on_btn_no_clicked();

private:
    // Разместите тут переменные, использующиеся в бинарном поиске:
    // a_, b_, m_.
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
