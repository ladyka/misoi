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
#include <vector>

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

    void zoomIn();
    void zoomOut();
    void normalSize();
    void on_porog_b_clicked();

    void on_pushButton_3_clicked();

    void on_k_alg_but_clicked();

    void on_fitToWindowAct_triggered();

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
    void updateMatrixPrin(int **&a, int wight, int height, int areaA, int areaB);
    int square(int **&a, int wight, int height,int elementId);
    int perimetr(int **&a, int wight, int height,int elementId);
    bool isPerimeterPoint(int **&a, int wight, int height, int elementId);
    int density(int square, int perimeter);
    int countOfObject_parce;
    int **matrix;

    int wight;
    int height;

    int getMinIndex(int * a,int size);
    void draw();
    //int cost(int ***vectors, int * &sizes, int countClusters, int p, int **&random_K_means_m_j, int var);
};

#endif // MAINWINDOW_H
