#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "data/img.h"

class Net;
class QGraphicsView;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_filterBtn_clicked();
    void on_noiseSlider_valueChanged();

protected:
    virtual bool eventFilter(QObject *, QEvent *);
    
private:
    Ui::MainWindow *ui;
    Net* _pNet;
    const ImgData* _pImage;
    ImgData _image;

    void setImage(QGraphicsView* pView, const ImgData& img);
    void select(const ImgData& img);
    void showError(const ImgData& filtered, const ImgData& origin);
    void hideResults();
    ImgData makeNoise(const ImgData& img);
};

#endif // MAINWINDOW_H
