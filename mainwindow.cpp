#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(nullptr)
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
    if (nullptr != model)
    {
        delete model;
    }

    QRect rect = ui->graphicsView->rect();

    ui->dsbX->setRange(0, rect.width());
    ui->dsbY->setRange(0, rect.height());

    model = new Model(this, rect.width(), rect.height());
    model->Scene()->setSceneRect(rect);
    ui->graphicsView->setScene(model->Scene());

    model->Init();
    model->Display(true);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QRect rect = ui->graphicsView->rect();

    ui->dsbX->setRange(0, rect.width());
    ui->dsbY->setRange(0, rect.height());

    if (model)
    {
        model->Scene()->setSceneRect(rect);
        model->SetWidth(rect.width());
        model->SetHeight(rect.height());

        model->Display(true);
    }
}

void MainWindow::on_btnGenerate_clicked()
{
    model->SetAnimationParameter(model->ModelToDisplayY(0));
    model->SetAnimationOngoing(false);
    model->SetAnimationToOngoing(false);

    model->Clear(true);

    for (int i = 0; i < ui->sbNumOfPoints->value(); i++)
    {
        model->AddPoint(model->Width() * (double)rand() / (double)RAND_MAX,
            model->Height() * (double)rand() / (double)RAND_MAX);
    }

    model->Init();
    model->Display(true);
}

void MainWindow::on_hsAnimationParameter_sliderMoved(int position)
{
    // Animation parameter from 0 to 1
    double ap = (double)position / ui->hsAnimationParameter->maximum();

    model->SetAnimationParameter(model->GetYFromAP(ap),
        false /* updateSlider */);

    model->Display();
}

void MainWindow::on_btnStart_clicked()
{
    if (model->AnimationOngoing())
    {
        model->SetAnimationOngoing(false);
    }
    else
    {
        model->SetAnimationOngoing(true);
    }
}

void MainWindow::on_btnPrevious_clicked()
{
    model->AnimateToPrevious();
}

void MainWindow::on_btnNext_clicked()
{
    model->AnimateToNext();
}

void MainWindow::on_btnAddPoint_clicked()
{
    model->AddPoint(ui->dsbX->value(), ui->dsbY->value());
    model->Init();
    model->Display(true);
}

void MainWindow::on_btnClear_clicked()
{
    model->Clear(true);
}

void MainWindow::on_btnLoadFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open Image"), ".", tr("Image Files (*.txt)"));

    std::ifstream in(fileName.toStdString(), std::ifstream::in);

    double x, y;

    while (in >> x && in >> y)
    {
        if (x < 0 || y < 0 || x > model->Width() || y > model->Height())
        {
            continue;
        }
        model->AddPoint(x, y);
    }

    model->Init();
    model->Display(true);
}
