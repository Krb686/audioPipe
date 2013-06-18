#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QFileDialog>

void fileOpen()
{

    QString fileName = QFileDialog::getOpenFileName(0);

    std::string text = fileName.toLocal8Bit().constData();
    std::cout << text;
    /*
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        ui->textEdit->setText(in.readAll());
        file.close();
    }


    //cout << "Hello!";
    */
}

//Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->actionOpen, &QAction::triggered, fileOpen);
}

//Destructor
MainWindow::~MainWindow()
{
    delete ui;
}














