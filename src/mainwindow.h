#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>



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

private:
    Ui::MainWindow *ui;

    struct DataFile{
        QByteArray hexRiffChunkId, hexRiffChunkSize, hexRiffChunkFormat;
        QByteArray hexFormatChunkId, hexFormatChunkSize, hexFormatAudioFormat, hexFormatNumChannels, hexFormatSampleRate, hexFormatByteRate, hexFormatBlockAlign, hexFormatBitsPerSample;
        QByteArray hexDataChunkId, hexDataChunkSize, hexDataChunk;

        QByteArray hexRiffChunkIdText, hexRiffChunkFormatText, hexFormatChunkIdText, hexDataChunkIdText;

        int riffChunkSize;
        int formatChunkSize, formatAudioFormat, formatNumChannels, formatSampleRate, formatByteRate, formatBlockAlign, formatBitsPerSample;
        int dataChunkSize;
    };

    void configure();
    void loadHexData(QByteArray array, int bytes);
    void loadFormatChunks(QByteArray array);
    void loadSignalGraph(DataFile wavFile);
    int littleEndianToNumber(QByteArray array, int size);



};

#endif // MAINWINDOW_H


