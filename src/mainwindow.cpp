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

    ui->label_riff_spacer_1->setText("");
    ui->label_riff_value_1->setText("");
    ui->label_riff_value_2->setText("");
    ui->label_riff_value_3->setText("");


    ui->label_format_spacer_1->setText("");
    ui->label_format_spacer_2->setText("");
    ui->label_format_spacer_3->setText("");
    ui->label_format_spacer_4->setText("");
    ui->label_format_spacer_5->setText("");
    ui->label_format_spacer_6->setText("");
    ui->label_format_spacer_7->setText("");


}

void MainWindow::openFile()
{
    const QString RIFF = QString("52494646");
    const QString WAVE = QString("57415645");

    int size;
    char *memblock;
    QString contentType, fileType;

    QString fileName = QFileDialog::getOpenFileName(0);
    string filePath = fileName.toStdString();

    ifstream file (filePath, ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size+1];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
    }
    qDebug() << "done reading";



    QByteArray array = QByteArray(memblock, size).toHex().toUpper();


    contentType.append(array.mid(0, 8));
    if(contentType == RIFF)
    {
        qDebug() << "Content Type: RIFF";
    }
    else
    {
        qDebug() << "Could not determine content type";
    }

    fileType.append(array.mid(16, 8));
    if(fileType == WAVE)
    {
        qDebug() << "File Type: WAVE";

        if(size > 10000)
        {
            loadHexData(array, 10000);
        }
        else
        {
            loadHexData(array, -1);
        }

        delete[] memblock;

    }
    else
    {
        qDebug() << "Could not determine file type";
        delete[] memblock;
    }




}

void MainWindow::loadHexData(QByteArray array, int bytes)
{

    QString s;
    int i = 0;
    int count1 = 0;
    int count2 = 0;

    if(bytes == -1)
    {
        for(i=0;i<array.length();i++)
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

        qDebug() << "loaded all";
    }
    else
    {
        for(i=0;i<bytes;i++)
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

        qDebug() << "loaded 10k";
        s.append("->");
    }

    qDebug() << "done making string";
    ui->textEdit->setText(s);
    loadFormatChunks(array);
}

void MainWindow::loadFormatChunks(QByteArray array)
{

    QString riffChunkId = QString(array.mid(0,2) + " " + array.mid(2, 2) + " " + array.mid(4, 2) + " " + array.mid(6, 2));
    ui->label_riff_value_1->setText(riffChunkId);

    //QString riffChunkSize = QString(array.mid(8, 2))

    loadSignalGraph(array);

}

void MainWindow::loadSignalGraph(QByteArray array)
{
    //Create scene with new keyword from heap so it does not disappear from the stack
    QGraphicsScene *scene = new QGraphicsScene();

    QPen pen = QPen(Qt::DashLine);

    //Draw axis
    scene->addLine(-100, 0, 100, 0, pen);
    scene->addLine(0, -100, 0, 100, pen);


    int i;
    QString riffChunkId, riffChunkSize, riffChunkFormat,
            formatChunkId, formatChunkSize, formatAudioFormat, formatNumChannels, formatSampleRate, formatBitRate, formatBlockAlign, formatBitsPerSample,
            dataChunkId, dataChunkSize, dataChunk;

    riffChunkId         = array.mid(0, 8);

    riffChunkSize       = array.mid(8, 8);
    riffChunkFormat     = array.mid(16, 8);

    formatChunkId       = array.mid(24, 8);
    formatChunkSize     = array.mid(32, 8);
    formatAudioFormat   = array.mid(40, 4);
    formatNumChannels   = array.mid(44, 4);
    formatSampleRate    = array.mid(48, 8);
    formatBitRate      = array.mid(56, 8);
    formatBlockAlign    = array.mid(64, 4);
    formatBitsPerSample = array.mid(68, 4);

    dataChunkId         = array.mid(72, 8);
    dataChunkSize       = array.mid(80, 8);
    dataChunk           = array.mid(88, array.length());


    qDebug() << riffChunkId << "\n";
    qDebug() << riffChunkSize << "\n";
    qDebug() << riffChunkFormat << "\n";
    qDebug() << formatChunkId << "\n";
    qDebug() << formatChunkSize << "\n";
    qDebug() << formatAudioFormat << "\n";
    qDebug() << formatNumChannels << "\n";
    qDebug() << formatSampleRate << "\n";
    qDebug() << formatBitRate << "\n";
    qDebug() << formatBlockAlign << "\n";
    qDebug() << formatBitsPerSample << "\n";
    qDebug() << dataChunkId << "\n";
    qDebug() << dataChunkSize << "\n";



    string str = string() + char(riffChunkId.mid(0, 2).toDouble());

    QByteArray text = QByteArray::fromHex(array.mid(0, 2));



    ui->label_riff_value_2->setText(text);
}



















