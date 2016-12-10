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
    model = new Model(ui->graphicsView->width(),
            ui->graphicsView->height(),
            ui->sbNumOfPoints->value());

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
