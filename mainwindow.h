#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "model.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Model *model;

    void InitModel();

private slots:
    void on_btnGenerate_clicked();

    void on_hsAnimationParameter_sliderMoved(int position);

private:

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
