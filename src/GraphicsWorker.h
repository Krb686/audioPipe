#ifndef GRAPHICSWORKER_H
#define GRAPHICSWORKER_H

#include <QGraphicsView>
#include "DataFile.h"

class GraphicsWorker : public QObject
 {
     Q_OBJECT

public:
    GraphicsWorker();
    ~GraphicsWorker();


public slots:
    void run();
    void setDataFile(DataFile wavFile);

signals:
    void sceneReady(QGraphicsScene *scene);
    void error(QString err);

private:
    DataFile wavFile;
    QThread *mainThread;
    QGraphicsScene *sceneRef;
};

#endif
