#include "GraphicsWorker.h"
#include <QDebug>


GraphicsWorker::GraphicsWorker()
{
    //This reference is kept across run() calls to prevent memory leak on scene pointer.
    this->sceneRef = 0;
}


GraphicsWorker::~GraphicsWorker()
{
    //destructor
}




void GraphicsWorker::run()
{
    qDebug() << "Starting graphics worker...";



    //Create scene with new keyword from heap so it does not disappear from the stack
    QGraphicsScene *scene = new QGraphicsScene();


    //If scene ref exists, delete old, else set a scene ref.
    if(!this->sceneRef)
    {
        this->sceneRef = scene;
    }
    else
    {
        delete this->sceneRef;
        this->sceneRef = scene;
    }


    QPainterPath *path = new QPainterPath(QPointF(0, 0));
    QPainter *painter = new QPainter();

    QPen pen = QPen(Qt::DashLine);


    //Draw axis
    scene->addLine(-100, 0, 100, 0, pen);
    scene->addLine(0, -100, 0, 100, pen);

    //


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



    //Change scene owner to mainThread
    scene->moveToThread(QApplication::instance()->thread());


    emit sceneReady(scene);

    delete path;
    delete painter;
}


void GraphicsWorker::setDataFile(DataFile wavFile)
{
    this->wavFile = wavFile;
}

