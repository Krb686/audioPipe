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
    ui->label_format_spacer_8->setText("");

    ui->label_data_value_1->setText("");
    ui->label_data_value_2->setText("");


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
    int i;

    QString riffChunkId, riffChunkSize, riffChunkFormat,
            formatChunkId, formatChunkSize, formatAudioFormat, formatNumChannels, formatSampleRate, formatByteRate, formatBlockAlign, formatBitsPerSample,
            dataChunkId, dataChunkSize, dataChunk;

    riffChunkId         = array.mid(0, 8);

    riffChunkSize       = array.mid(8, 8);
    riffChunkFormat     = array.mid(16, 8);

    formatChunkId       = array.mid(24, 8);
    formatChunkSize     = array.mid(32, 8);
    formatAudioFormat   = array.mid(40, 4);
    formatNumChannels   = array.mid(44, 4);
    formatSampleRate    = array.mid(48, 8);
    formatByteRate      = array.mid(56, 8);
    formatBlockAlign    = array.mid(64, 4);
    formatBitsPerSample = array.mid(68, 4);

    dataChunkId         = array.mid(72, 8);
    dataChunkSize       = array.mid(80, 8);
    dataChunk           = array.mid(88, array.length());


    qDebug() << "RiffChunkId " << riffChunkId << "\n";
    qDebug() << "riffChunkSize " <<riffChunkSize << "\n";
    qDebug() << "riffChunkFormat " <<riffChunkFormat << "\n";
    qDebug() << "formatChunkId " <<formatChunkId << "\n";
    qDebug() << "formatChunkSize " <<formatChunkSize << "\n";
    qDebug() << "formatAudioFormat " <<formatAudioFormat << "\n";
    qDebug() << "formatNumChannels " <<formatNumChannels << "\n";
    qDebug() << "formatSampleRate " <<formatSampleRate << "\n";
    qDebug() << "formatBitRate " <<formatByteRate << "\n";
    qDebug() << "formatBlockAlign " <<formatBlockAlign << "\n";
    qDebug() << "formatBitsPerSample " <<formatBitsPerSample << "\n";
    qDebug() << "dataChunkId " <<dataChunkId << "\n";
    qDebug() << "dataChunkSize " <<dataChunkSize << "\n";



    //string str = string() + char(riffChunkId.mid(0, 2).toDouble());

    //QByteArray riffChunkIdText = QByteArray::fromHex(array.mid(0, 2)) + QByteArray::fromHex(array.mid(0, 2)) + QByteArray::fromHex(array.mid(0, 2)) + QByteArray::fromHex(array.mid(0, 2));
    QByteArray q;

    q.append(riffChunkId);
    QByteArray riffChunkIdText = QByteArray::fromHex(q);
    ui->label_riff_value_1->setText(riffChunkIdText);
    q.clear();

    q.append(riffChunkSize);
    i = littleEndianToNumber(q, q.size());
    ui->label_riff_value_2->setText(QString::number(i));
    q.clear();

    q.append(riffChunkFormat);
    QByteArray riffChunkFormatText = QByteArray::fromHex(q);
    ui->label_riff_value_3->setText(riffChunkFormatText);
    q.clear();
    //===================//


    q.append(formatChunkId);
    QByteArray formatChunkIdText = QByteArray::fromHex(q);
    ui->label_format_value_1->setText(formatChunkIdText);
    q.clear();

    q.append(formatChunkSize);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_2->setText(QString::number(i));
    q.clear();

    q.append(formatAudioFormat);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_3->setText(QString::number(i));
    q.clear();

    q.append(formatNumChannels);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_4->setText(QString::number(i));
    q.clear();

    q.append(formatSampleRate);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_5->setText(QString::number(i));
    q.clear();

    q.append(formatByteRate);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_6->setText(QString::number(i));
    q.clear();

    q.append(formatBlockAlign);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_7->setText(QString::number(i));
    q.clear();

    q.append(formatBitsPerSample);
    i = littleEndianToNumber(q, q.size());
    ui->label_format_value_8->setText(QString::number(i));
    q.clear();
    //============//

    q.append(dataChunkId);
    QByteArray dataChunkIdText = QByteArray::fromHex(q);
    ui->label_data_value_1->setText(dataChunkIdText);
    q.clear();

    q.append(dataChunkSize);
    i = littleEndianToNumber(q, q.size());
    ui->label_data_value_2->setText(QString::number(i));
    q.clear();





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

    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();



}

int MainWindow::littleEndianToNumber(QByteArray array, int length)
{
    bool ok;
    QByteArray q;

    //Reverse from little endian to big endian
    for(int i=0;i<length;i+=2)
    {
        q.prepend(array.mid(i, 2));
    }


    //Convert from base 16
    return q.toInt(&ok, 16);
}



















