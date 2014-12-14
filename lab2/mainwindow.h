#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

#include <QRgb>
#include <QImage>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <array>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    bool loadFile(const QString &);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionPrint_triggered();
    void on_actionSave_triggered();
    void on_pushButton_6_clicked();
    void on_button_gistogramm_clicked();
    void on_actionExit_triggered();
    void on_porog_a_clicked();
    void fitToWindow();

    void on_action_Fit_to_Window_triggered();

    void zoomIn();
    void zoomOut();
    void normalSize();
    void on_porog_b_clicked();

    void on_pushButton_3_clicked();
    
private:
    Ui::MainWindow *ui;


    //QLabel *imageLabel;
    //QScrollArea *scrollArea;
    double scaleFactor;

    int getH1(QImage * image,int w,int h, int readOrGreenOrBlue);
    int getH2(QImage * image, int w, int h, int readOrGreenOrBlue);
    int getDet(int ** a);
    int ** getMatrixGrey(QImage *image, int w, int h, int readOrGreenOrBlue);
    int getNewGRB(QImage  image, int wi, int hi, int readOrGreenOrBlue);

    #ifndef QT_NO_PRINTER
    QPrinter printer;
    #endif

    QAction *openAct;
    QAction *printAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *viewImage;
    QAction *gistogram;
    QAction *dissectionA;
    QAction *dissectionB;
    QAction *sobel;

    QAction *aboutAct;
    QAction *aboutQtAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *labOneMenu;
    QMenu *helpMenu;

    QPixmap imageOriginal;
    void scaleImage(double factor);
    void updateActions();

    void zeroall(int **&a, int wight, int height);
    int whichArea(int **&a, int wight, int height);
    void setArea(int **&a, int wight, int height, int area);
    int minimum(int a,int b);
    void updateMatrix(int **&a, int wight, int height, int i, int j);
};

#endif // MAINWINDOW_H





//Q_OBJECT

//public:
//ImageViewer();


//private slots:
//void open();
//void print();
//void save();
//void viewImageF();
//void gistogramGenerate();
//void dissectionAGenerate();
//void dissectionBGenerate();
//void sobelF();
//void zoomIn();
//void zoomOut();
//void normalSize();
//void fitToWindow();
//void about();

//private:
//void createActions();
//void createMenus();
//void updateActions();
//void scaleImage(double factor);
//void adjustScrollBar(QScrollBar *scrollBar, double factor);

//};
////! [0]

//#endif
