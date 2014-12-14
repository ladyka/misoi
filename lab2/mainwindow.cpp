#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"


const int RED = 1;
const int GREEN = 2;
const int BLUE = 3;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
    imageOriginal = *(ui->imageLabel->pixmap());
}

void MainWindow::on_actionPrint_triggered()
{
    Q_ASSERT(ui->imageLabel->pixmap());
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    //! [6] //! [7]
    QPrintDialog dialog(&printer, this);
    //! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = ui->imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(ui->imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *(ui->imageLabel)->pixmap());
    }
#endif
}

void MainWindow::on_actionSave_triggered()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Save File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    QString fileName = dialog.getSaveFileName();


    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    const QPixmap *a = ui->imageLabel->pixmap();
    a->save(&file,"PNG");
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->imageLabel->setPixmap(imageOriginal);

}


bool MainWindow::loadFile(const QString &fileName)
{
    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        ui->imageLabel->setPixmap(QPixmap());
        ui->imageLabel->adjustSize();
        return false;
    }
//! [2] //! [3]
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
//! [3] //! [4]
    scaleFactor = 1.0;

//    printAct->setEnabled(true);
//    saveAct->setEnabled(true);
//    fitToWindowAct->setEnabled(true);
//    gistogram->setEnabled(true);
//    dissectionA->setEnabled(true);
//    dissectionB->setEnabled(true);
//    viewImage->setEnabled(true);
//    sobel->setEnabled(true);
    //updateActions();

//    if (!fitToWindowAct->isChecked())
//        ui->imageLabel->adjustSize();
    setWindowFilePath(fileName);
    //fitToWindow();
    return true;
}

void MainWindow::on_button_gistogramm_clicked()
{
    float gist[255];
    for(int i = 0; i < 255; i++)
    {
        gist[i] = 0;
    }
    QPixmap a = *(ui->imageLabel->pixmap());
    QImage image = a.toImage();
    int h = image.height();
    int w = image.width();
    for (int hi = 0; hi < h; ++hi) {
        for (int wi = 0; wi < w; ++wi) {
            QRgb qrgb = image.pixel(wi,hi);
            int g = qGray(qrgb);
            gist[g]++;
        }
    }

    float max = 0;
    for(int i = 0; i < 255; i++)
    {
        if (gist[i] > max)
        {
            max = gist[i];
        }
    }

    int imgGist[255];

    for(int i = 0; i < 255; i++)
    {
        imgGist[i] = (int) gist[i]/max*100;
    }

    QImage gistogramImage(255,101,QImage::Format_Mono);

    for(int j = 0; j < 101; j++) {
        for (int i = 0; i<255 ; i++) {
            if (imgGist[i] < j) {
                gistogramImage.setPixel(i,100-j,1);
            } else {
                gistogramImage.setPixel(i,100-j,0);
            }
        }
    }
    QPixmap pixImage = QPixmap::fromImage(gistogramImage);
    ui->imageLabel->setPixmap(pixImage);

    //fitToWindow();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_porog_a_clicked()
{
    QImage image = imageOriginal.toImage();
    QImage outImage(image.width(),image.height(),QImage::Format_Mono);
    int h = image.height();
    int w = image.width();
    for (int hi = 0; hi < h; ++hi)
    {
        for (int wi = 0; wi < w; ++wi)
        {
            QRgb qrgb = image.pixel(wi,hi);
            int g = qGray(qrgb);
            int c = 0;
            if (g > ui->horizontalSlider_a->value())
            {
                c =1;
            }
            outImage.setPixel(wi,hi,c);
        }
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(outImage));
    fitToWindow();
}

void MainWindow::fitToWindow()
{
    bool fitToWindow = ui->fitToWindowAct->isChecked();
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void MainWindow::on_action_Fit_to_Window_triggered()
{
    fitToWindow();
}


void MainWindow::zoomIn()
{
    scaleImage(1.25);
}

void MainWindow::zoomOut()
{
    scaleImage(0.8);
}

void MainWindow::normalSize()
{
    ui->imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::updateActions()
{
    ui->zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    ui->zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    ui->normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(ui->imageLabel->pixmap());
    scaleFactor *= factor;
    ui->imageLabel->resize(scaleFactor * ui->imageLabel->pixmap()->size());

//    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
//    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    ui->zoomInAct->setEnabled(scaleFactor < 3.0);
    ui->zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MainWindow::on_porog_b_clicked()
{
    QImage image = imageOriginal.toImage();
    QImage outImage(image.width(),image.height(),QImage::Format_Mono);
    int h = image.height();
    int w = image.width();
    for (int hi = 0; hi < h; ++hi)
    {
        for (int wi = 0; wi < w; ++wi)
        {
            QRgb qrgb = image.pixel(wi,hi);
            int g = qGray(qrgb);
            int c = 0;
            if ((g > ui->horizontalSlider_b_1->value()) && (g < ui->horizontalSlider_b_2->value()))
            {
                c =1;
            }
            outImage.setPixel(wi,hi,c);
        }
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(outImage));
    fitToWindow();
}

void MainWindow::on_pushButton_3_clicked()
{
    QPixmap pixmap = *(ui->imageLabel->pixmap());
    QImage image = pixmap.toImage();
    //if (image.format() == QImage::Format_Mono) {
        int wight = image.width();
        int height = image.height();
        int countOfObject = 0;
        int **matrix;

        zeroall(matrix,wight,height);

        for (int w = 1; w < wight-1; ++w) {
            for (int h = 1; h < height-1; ++h) {
                QRgb qrgb = image.pixel(w,h);
                int c =  qGray(qrgb);
                if (c == 0 ) {

                    // если пиксели (x − 1,y), (x,y − 1) не отнесены ни к одной из найденных областей:
                    int areaA = whichArea(matrix,w-1,h);
                    int areaB = whichArea(matrix,w,h-1);
                    if ((areaA == 0) && (areaB == 0) )
                    {
                        // прибавить счетчик областей n = n + 1;
                        countOfObject++;

                        //приписать пиксель (x,y) к области n;
                        setArea(matrix,w,h,countOfObject);
                    } else {
                        //если только один из пикселей (x − 1,y), (x,y − 1) отнесен к одной из областей i:
                        if ((areaA > 0) && (areaB == 0)) {
                            //приписать пиксель (x,y) к области i;
                            setArea(matrix,w,h,areaA);
                        } else if ((areaB > 0) && (areaA == 0)) {
                            //приписать пиксель (x,y) к области i;
                            setArea(matrix,w,h,areaB);
                        } else {
                            //если оба пикселя (x − 1,y), (x,y − 1) отнесены к найденной области i:
                            if (areaA == areaB) {
                                //приписать пиксель (x,y) к области i;
                                setArea(matrix,w,h,areaB);
                            } else {
                                //если оба пикселя (x − 1,y), (x,y − 1) отнесены к разным областям i, j:

                                // i = j : приписать пиксель (x,y) к области i;
                                int mainArea = minimum(areaA,areaB);
                                setArea(matrix,w,h,mainArea);
                                // зафиксировать эквивалентность областей i и j;
                                updateMatrix(matrix,w,h,areaA,areaB);
                            }
                        }
                    }
                }

//                1. если пиксель (x,y) помечен единицей на исходном бинарном изображении:
//                2. 	если пиксели (x − 1,y), (x,y − 1) не отнесены ни к одной из
//                3. 	найденных областей:
//                4. 		прибавить счетчик областей n = n + 1;
//                5. 		приписать пиксель (x,y) к области n;
//                6. 	если только один из пикселей (x − 1,y), (x,y − 1) отнесен к одной из
//                7.	 областей i:
//                8. 		приписать пиксель (x,y) к области i;
//                9. 	если оба пикселя (x − 1,y), (x,y − 1) отнесены к найденной
//                10. 	области i:
//                11. 		приписать пиксель (x,y) к области i;
//                12. 	если оба пикселя (x − 1,y), (x,y − 1) отнесены к разным
//                13. 	областям i, j: i = j:
//                14. 		приписать пиксель (x,y) к области i;
//                15. 		зафиксировать эквивалентность областей i и j;
//                16. перейти к следующему пикселю.
            }
        }
//    } else {
//        //бінарізіруй ізображеній!!!
//    }

//        for (int i = 0; i < wight; ++i) {
//            for (int j = 0; j < height; ++j) {
//                std::cout << matrix[i][j] << ' ';
//            }
//            std::cout << std::endl;
//        }
            std::cout << countOfObject << std::endl;
}


void MainWindow::zeroall(int **&a, int wight, int height)
{
    a = new int * [wight];
    for (int i = 0;i <wight; i++) {
        *(a +i) = new int [height];
        for (int j = 0; j<height; ++j) {
            a[i][j] = 0;
        }
    }
}

/* если тру то пренадлежит какой-то области
 */
int MainWindow::whichArea(int **&a, int wight, int height)
{
    return a[wight][height];
}

void MainWindow::setArea(int **&a, int wight, int height, int area)
{
    a[wight][height] = area;
}

int MainWindow::minimum(int a, int b)
{
    if (a > b) {
        return b;
    }
    return a;
}

void MainWindow::updateMatrix(int **&a, int wight, int height, int i, int j)
{
    int areaA;
    int areaB;
    if (i > j) {
        areaA = j;
        areaB = i;
    } else {
        areaA = i;
        areaB = j;
    }
    for (int i = 0;i <wight; i++) {
        for (int j = 0; j<height; ++j) {
            if (a[i][j] == areaB) {
                a[i][j] = areaA;
            }
        }
    }
}
