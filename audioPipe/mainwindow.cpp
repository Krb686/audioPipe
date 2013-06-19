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
    ui->tableWidget->setRowCount(16);
    /*
    for(int i=0;i<16;i++)
    {
        ui->tableWidget->setColumnWidth(i, 50);
    }
    */
}

void MainWindow::openFile()
{
    size_t size;

    char *memblock;

    QString fileName = QFileDialog::getOpenFileName(0);
    string text = fileName.toStdString();
    //cout << text;


    ifstream file (text, ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size+1];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
    }
    else
    {
        cout << "unopen";
    }

    int i, result;
    result = _setmode( _fileno( stdin ), _O_BINARY );
    if( result == -1 )
        perror( "Cannot set mode" );
    else
        for ( size_t i = 0; i < strlen(memblock); i++ )
        {
            cout << "memblock["<<i<<"] " << memblock[i];
            cout << "\n";
            cout << memblock[i] << " + 'a' = " << ( memblock[i] + 'a' );
            cout << "\n\n";

        }




    int rowCount = 0;
    int colCount = 0;

    //QTableWidgetItem *item = new QTableWidgetItem("hello");

    //ui->tableWidget->setItem(1, 1, item);


    for(i=0;i<16;i++)
    {
        //cout << typeid(memblock[i]);

        //cout << &memblock[i];
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(memblock[i], 16).toUpper());
        ui->tableWidget->setItem(rowCount, colCount, item);
        colCount++;
        if (colCount == 15)
        {
            rowCount++;
            colCount-=15;
        }

    }


    delete[] memblock;


    //cout << "Hello!";

}

















