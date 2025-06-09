#pragma once

#include <QMainWindow>

enum class Genre : int {
    kComedy = 0,
    kAction = 1,
    kDrama = 2,
    kFairyTale = 3,
    kAnimation = 4,
    kHorror= 5
};

struct Movie {
    QString name;
    Genre genre;
    double rating;
    bool is_watched;

    bool operator==(const Movie& movie) const{
        return name == movie.name &&
               genre == movie.genre;
    }
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_add_clicked();

    void on_cb_list_wait_currentIndexChanged(int index);

    void on_cb_list_done_currentIndexChanged(int index);

private:
    // Заполняет два списка. При этом выбирает
    // нужный фильм в одном из них.
    void ApplyModel(const Movie &selection);

    // Заполняет интерфейс на основе данных фильма.
    void ApplyMovie(const Movie &movie);

    // Получает фильм на основе данных интерфейса.
    Movie GetMovie() const;

    // Возвращает описание по фильму.
    static QString ToString(const Movie &movie);

    // Делает из списка фильмов список строк.
    static QStringList ToString(const QList<Movie>& movies);


private:
    Ui::MainWindow *ui;
    QList<Movie> movies_;
};
