#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::InitModel()
{
    if (model == nullptr)
    {
        QSize size = ui->graphicsView->size();
        model = new Model(size.width(), size.height());
        ui->graphicsView->setScene(model->Scene());
    }

    model->SetNumOfPoints(ui->sbNumOfPoints->value());

    model->Init();
    model->Display();
}

void MainWindow::on_btnGenerate_clicked()
{
    InitModel();
}

void MainWindow::on_hsAnimationParameter_sliderMoved(int position)
{
    // Animation parameter from 0 to 1
    double ap = (double)position / ui->hsAnimationParameter->maximum();

    model->SetAnimationParameter(model->GetYFromAP(ap));

    model->Display();
}
