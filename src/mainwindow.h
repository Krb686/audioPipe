#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "GraphicsWorker.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void openFile();
    void zoomOut();
    void zoomIn();
    void setScene(QGraphicsScene *scene);

signals:
    void beginGraphicsWorker();

private:
    Ui::MainWindow *ui;



    GraphicsWorker *worker;

    void configure();
    void loadHexData(QByteArray array, int bytes);
    void loadFormatChunks(QByteArray array);
    void loadSignalGraph(DataFile wavFile);
    int littleEndianToNumber(QByteArray array, int size);



};

#endif // MAINWINDOW_H


