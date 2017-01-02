#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QFileDialog>
#include <fstream>
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

    Ui::MainWindow *ui;

    void InitModel();

    void resizeEvent(QResizeEvent* event) override;

private slots:
    void on_btnGenerate_clicked();

    void on_hsAnimationParameter_sliderMoved(int position);

    void on_btnStart_clicked();

    void on_btnPrevious_clicked();

    void on_btnNext_clicked();
    
    void on_btnAddPoint_clicked();

    void on_btnClear_clicked();
    void on_btnLoadFile_clicked();
};

#endif // MAINWINDOW_H
