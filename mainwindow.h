#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_pushButton_clicked();

    void on_scale_slider_valueChanged(int value);

    void on_off_slider_valueChanged(int value);

    void on_updateButton_clicked();
    void gotClick (int *) ;

private:
    Ui::MainWindow *ui;
    float scaleval, offval ;
};

#endif // MAINWINDOW_H
