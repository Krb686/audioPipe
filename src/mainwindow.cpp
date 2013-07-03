#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include "getTimeMs64.cpp"
#include "MyThread.cpp"
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
    if(sceneRef)
    {
        delete sceneRef;
        //qDebug() << "deleted!";
    }
    else
    {
        //qDebug() << "no scene!";
    }



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


    /*
    qDebug() << "hexRiffChunkId " << wavFile.hexRiffChunkId << "\n";
    qDebug() << "hexRiffChunkSize " <<wavFile.hexRiffChunkSize << "\n";
    qDebug() << "hexRiffChunkFormat " <<wavFile.hexRiffChunkFormat << "\n";
    qDebug() << "hexFormatChunkId " <<wavFile.hexFormatChunkId << "\n";
    qDebug() << "hexFormatChunkSize " <<wavFile.hexFormatChunkSize << "\n";
    qDebug() << "hexFormatAudioFormat " <<wavFile.hexFormatAudioFormat << "\n";
    qDebug() << "hexFormatNumChannels " <<wavFile.hexFormatNumChannels << "\n";
    qDebug() << "hexFormatSampleRate " <<wavFile.hexFormatSampleRate << "\n";
    qDebug() << "formatBitRate " <<wavFile.hexFormatByteRate << "\n";
    qDebug() << "hexFormatBlockAlign " <<wavFile.hexFormatBlockAlign << "\n";
    qDebug() << "hexFormatBitsPerSample " <<wavFile.hexFormatBitsPerSample << "\n";
    qDebug() << "hexDataChunkId " <<wavFile.hexDataChunkId << "\n";
    qDebug() << "hexDataChunkSize " <<wavFile.hexDataChunkSize << "\n";
    */

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


    //loadSignalGraph(wavFile);

    MyThread *thread1 = new MyThread();

    thread1->

}

void MainWindow::loadSignalGraph(DataFile wavFile)
{
    //Create scene with new keyword from heap so it does not disappear from the stack
    QGraphicsScene *scene = new QGraphicsScene();
    sceneRef = scene;

    QPainterPath *path = new QPainterPath(QPointF(0, 0));
    QPainter *painter = new QPainter();

    //ui->graphicsView->scale(.5, .35);

    QPen pen = QPen(Qt::DashLine);

    //Draw axis
    scene->addLine(-100, 0, 100, 0, pen);
    scene->addLine(0, -100, 0, 100, pen);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

    //Access wavFile data
    if(wavFile.hexFormatChunkIdText != "fmt ")
    {
        //qDebug() << "error";
        //qDebug() << wavFile.hexFormatChunkIdText;
        exit(-1);
    }

    if (wavFile.formatAudioFormat != 1)
    {
        //qDebug() << "error, unsupported audio format.";
        exit(-1);
    }

    //continue
    //qDebug() << "good";
    int i;


    for(i=0;i<wavFile.hexDataChunk.size();i+= wavFile.formatBlockAlign*2)
    {
        // read signal


        if(wavFile.formatNumChannels == 1 && wavFile.formatBitsPerSample == 8)
        {
            bool ok;
            QByteArray sample = wavFile.hexDataChunk.mid(i, 2);
            int sampleValue = sample.toInt(&ok, 16);

            path->lineTo(i, sampleValue);
        }
        else if(wavFile.formatNumChannels == 1 && wavFile.formatBitsPerSample == 16)
        {
            QByteArray sample = wavFile.hexDataChunk.mid(i, 4);
            bool ok;
            int sampleValue = sample.toInt(&ok, 16);

            //2's complement
            if(sampleValue > 32767)
            {
                sampleValue = (sampleValue - 65535);
            }

            path->lineTo(i, sampleValue);

        }
        else if(wavFile.formatNumChannels == 2 && wavFile.formatBitsPerSample == 8)
        {
            QByteArray leftSample = wavFile.hexDataChunk.mid(i, 2);
            QByteArray rightSample = wavFile.hexDataChunk.mid(i+2, 2);
        }
        else if(wavFile.formatNumChannels == 2 && wavFile.formatBitsPerSample == 16)
        {
            QByteArray leftSample = wavFile.hexDataChunk.mid(i, 4);
            QByteArray rightSample = wavFile.hexDataChunk.mid(i+4, 4);
        }
    }

    painter->fillPath(*path, Qt::blue);

    scene->addPath(*path);

    delete path;
    delete painter;

    //t2 = GetTimeMs64();
    //qDebug() << (t2 - t1);


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

    int i;

}

void MainWindow::zoomIn()
{
    ui->graphicsView->scale(1.1, 1.1);
}

/*
void QGraphicsView::paintEvent(QPaintEvent *e)
{
    static int count = 0;
    qDebug("paintEvent, %d", count++);
}
*/













