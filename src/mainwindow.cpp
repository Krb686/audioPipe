#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include "GetTimeMs64.cpp"
#include "GraphicsWorker.cpp"
#include "DataFile.cpp"
#include <QThread>
using namespace std;


QGraphicsScene *sceneRef;

int t1, t2;


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
    QObject::connect(ui->actionZoomIn, &QAction::triggered, this, &MainWindow::zoomIn);
    QObject::connect(ui->actionZoomOut, &QAction::triggered, this, &MainWindow::zoomOut);

    QThread *thread = new QThread();
    GraphicsWorker *worker = new GraphicsWorker();
    this->worker = worker;


    this->worker->moveToThread(thread);
    thread->start();

    QObject::connect(this, &MainWindow::beginGraphicsWorker, this->worker, &GraphicsWorker::run);
    QObject::connect(worker, &GraphicsWorker::sceneReady, this, &MainWindow::setScene);

    ui->label_riff_spacer_1->setText("");
    ui->label_riff_value_1->setText("");
    ui->label_riff_value_2->setText("");
    ui->label_riff_value_3->setText("");


    //ui->label_format_spacer_1->setText("");
    ui->label_format_spacer_2->setText("");
    ui->label_format_spacer_3->setText("");
    ui->label_format_spacer_4->setText("");
    ui->label_format_spacer_5->setText("");
    ui->label_format_spacer_6->setText("");
    ui->label_format_spacer_7->setText("");
    ui->label_format_spacer_8->setText("");

    ui->label_data_value_1->setText("");
    ui->label_data_value_2->setText("");

    //setStyleSheet("#label_format_spacer_1 { color: qrgba(255, 255, 255, 0); }");
}

void MainWindow::openFile()
{


    const QString RIFF = QString("52494646");
    const QString WAVE = QString("57415645");

    int size;
    char *memblock;
    QString contentType, fileType;

    QString fileName = QFileDialog::getOpenFileName(0);

    //t1 = GetTimeMs64();
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
    //qDebug() << "done reading";



    QByteArray array = QByteArray(memblock, size).toHex().toUpper();


    contentType.append(array.mid(0, 8));
    if(contentType == RIFF)
    {
        //qDebug() << "Content Type: RIFF";
    }
    else
    {
        //qDebug() << "Could not determine content type";
    }

    fileType.append(array.mid(16, 8));
    if(fileType == WAVE)
    {
        //qDebug() << "File Type: WAVE";

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
        //qDebug() << "Could not determine file type";
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

        //qDebug() << "loaded all";
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

        //qDebug() << "loaded 10k";
        s.append("->");
    }

    //qDebug() << "done making string";
    ui->textEdit->setText(s);
    loadFormatChunks(array);
}

void MainWindow::loadFormatChunks(QByteArray array)
{
    DataFile wavFile;

    wavFile.hexRiffChunkId         = array.mid(0, 8);

    wavFile.hexRiffChunkSize       = array.mid(8, 8);
    wavFile.hexRiffChunkFormat     = array.mid(16, 8);

    wavFile.hexFormatChunkId       = array.mid(24, 8);
    wavFile.hexFormatChunkSize     = array.mid(32, 8);
    wavFile.hexFormatAudioFormat   = array.mid(40, 4);
    wavFile.hexFormatNumChannels   = array.mid(44, 4);
    wavFile.hexFormatSampleRate    = array.mid(48, 8);
    wavFile.hexFormatByteRate      = array.mid(56, 8);
    wavFile.hexFormatBlockAlign    = array.mid(64, 4);
    wavFile.hexFormatBitsPerSample = array.mid(68, 4);

    wavFile.hexDataChunkId         = array.mid(72, 8);
    wavFile.hexDataChunkSize       = array.mid(80, 8);
    wavFile.hexDataChunk           = array.mid(88, array.length());

    wavFile.hexRiffChunkIdText = QByteArray::fromHex(wavFile.hexRiffChunkId);
    ui->label_riff_value_1->setText(wavFile.hexRiffChunkIdText);

    wavFile.riffChunkSize = littleEndianToNumber(wavFile.hexRiffChunkSize, wavFile.hexRiffChunkId.size());
    ui->label_riff_value_2->setText(QString::number(wavFile.riffChunkSize));

    wavFile.hexRiffChunkFormatText = QByteArray::fromHex(wavFile.hexRiffChunkFormat);
    ui->label_riff_value_3->setText(wavFile.hexRiffChunkFormatText);
    //===================//


    wavFile.hexFormatChunkIdText = QByteArray::fromHex(wavFile.hexFormatChunkId);
    ui->label_format_value_1->setText(wavFile.hexFormatChunkIdText);

    wavFile.formatChunkSize = littleEndianToNumber(wavFile.hexFormatChunkSize, wavFile.hexFormatChunkSize.size());
    ui->label_format_value_2->setText(QString::number(wavFile.formatChunkSize));

    wavFile.formatAudioFormat = littleEndianToNumber(wavFile.hexFormatAudioFormat, wavFile.hexFormatAudioFormat.size());
    ui->label_format_value_3->setText(QString::number(wavFile.formatAudioFormat));

    wavFile.formatNumChannels = littleEndianToNumber(wavFile.hexFormatNumChannels, wavFile.hexFormatNumChannels.size());
    ui->label_format_value_4->setText(QString::number(wavFile.formatNumChannels));

    wavFile.formatSampleRate = littleEndianToNumber(wavFile.hexFormatSampleRate, wavFile.hexFormatSampleRate.size());
    ui->label_format_value_5->setText(QString::number(wavFile.formatSampleRate));

    wavFile.formatByteRate = littleEndianToNumber(wavFile.hexFormatByteRate, wavFile.hexFormatByteRate.size());
    ui->label_format_value_6->setText(QString::number(wavFile.formatByteRate));

    wavFile.formatBlockAlign = littleEndianToNumber(wavFile.hexFormatBlockAlign, wavFile.hexFormatBlockAlign.size());
    ui->label_format_value_7->setText(QString::number(wavFile.formatBlockAlign));

    wavFile.formatBitsPerSample = littleEndianToNumber(wavFile.hexFormatBitsPerSample, wavFile.hexFormatBitsPerSample.size());
    ui->label_format_value_8->setText(QString::number(wavFile.formatBitsPerSample));
    //============//

    wavFile.hexDataChunkIdText = QByteArray::fromHex(wavFile.hexDataChunkId);
    ui->label_data_value_1->setText(wavFile.hexDataChunkIdText);

    wavFile.dataChunkSize = littleEndianToNumber(wavFile.hexDataChunkSize, wavFile.hexDataChunkSize.size());
    ui->label_data_value_2->setText(QString::number(wavFile.dataChunkSize));


    this->worker->setDataFile(wavFile);

    //loadSignalGraph(wavFile);
    emit beginGraphicsWorker();


}

void MainWindow::loadSignalGraph(DataFile wavFile)
{

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

void MainWindow::zoomOut()
{
    ui->graphicsView->scale(.9, .9);
}

void MainWindow::zoomIn()
{
    ui->graphicsView->scale(1.1, 1.1);
}



void MainWindow::setScene(QGraphicsScene *sceneIncoming)
{

    QGraphicsScene *scene = new QGraphicsScene(sceneIncoming);
    ui->graphicsView->setScene(sceneIncoming);
    ui->graphicsView->show();
}













