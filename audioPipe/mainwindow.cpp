#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QTranslator>
#include <QTextStream>
#include <QDebug>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

using namespace std;

//Constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->configure();
}

//Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configure(){

    QObject::connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    ui->tableWidget->setColumnCount(16);
    ui->tableWidget->setRowCount(1);
}

void MainWindow::openFile()
{
    size_t size;
    char *memblock;

    QString fileName = QFileDialog::getOpenFileName(0);
    string text = fileName.toStdString();

    ifstream file (text, ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size+1];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
    }

    uint i, rowCount, colCount = 0;

    for(i=0;i<size;i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(memblock[i], 16).toUpper());
        ui->tableWidget->setItem(rowCount, colCount, item);
        colCount++;

        if (colCount == 16)
        {
            rowCount++;
            colCount-=16;
            ui->tableWidget->setRowCount(rowCount+1);
        }

    }


    delete[] memblock;
}

















