#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::InitModel()
{
    QRectF size = ui->graphicsView->size();

    model = new Model(size.width(), size.height(), ui->sbNumOfPoints->value());

    model->Init();

    model->Display();

    ui->graphicsView->setScene(model->Scene());
}

void MainWindow::on_btnGenerate_clicked()
{
    model->Clear();

    model->SetNumOfPoints(ui->sbNumOfPoints->value());
    model->Init();

    model->Display();
}

void MainWindow::on_hsAnimationParameter_sliderMoved(int position)
{
    // Animation parameter from 0 to 1
    double ap = (double)position / ui->hsAnimationParameter->maximum();

    model->SetAnimationParameter(ap * model->Height());

    model->Display();
}
