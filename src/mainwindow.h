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

private:
    Ui::MainWindow *ui;
    void configure();
    void createGraphics();
    void loadHexData(QByteArray array, int bytes);
    void loadFormatChunks(QByteArray array);
    void loadSignalGraph();



};

#endif // MAINWINDOW_H


