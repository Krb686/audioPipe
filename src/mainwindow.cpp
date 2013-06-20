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
#include <sstream>
#include <string>

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
}

void MainWindow::openFile()
{
    size_t size;
    char *memblock;
    char *hexBuffer;

    QString fileName = QFileDialog::getOpenFileName(0);
    string filePath = fileName.toStdString();

    ifstream file (filePath, ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size+1];
        hexBuffer = new char [size+1];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
    }
    qDebug() << "done reading";

    uint i = 0;

    QByteArray array = QByteArray(memblock, size).toHex().toUpper();
    QString s;

    int count1 = 0;
    int count2 = 0;
    for(i=0;i<size;i++)
    {
        count1++;
        count2++;
        s.append(array.at(i));
        if (count1 == 2)
        {
            s.append('\t');
            count1-= 2;
        }
        if (count2 == 32)
        {
            s.append("\n\n");
            count2-= 32;
        }
    }

    delete[] memblock;
    qDebug() << "done making string";
    ui->textEdit->setText(s);
}

















