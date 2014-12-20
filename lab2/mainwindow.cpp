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
    countOfObject_parce  = -1;
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

    // /home/user/projects/misoi/lab2
    // QDir::currentPath()
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath(): "/home/user/projects/misoi/lab2" );
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");

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
//    ui->zoomInAct->setEnabled(!fitToWindowAct->isChecked());
//    ui->zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
//    ui->normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
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
        wight = image.width();
        height = image.height();
        countOfObject_parce = 0;

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
                        countOfObject_parce++;

                        //приписать пиксель (x,y) к области n;
                        setArea(matrix,w,h,countOfObject_parce);
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
                                updateMatrix(matrix,wight,height,areaA,areaB);
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
    //draw();












        int elements_parce[countOfObject_parce];
        for (int var = 0; var < countOfObject_parce; ++var) {
            elements_parce[var] = 0;
        }
        for (int i = 0; i < wight; ++i) {
            for (int j = 0; j < height; ++j) {
                elements_parce[matrix[i][j]] = 1;
            }
        }

        //Дано:
        int p = 4; //countOfVectors
        // Набор векторов x_i, i = 1,…, p;
        int **klasters = new int * [p];
        int klasterIncrement = -1;
        std::vector<int> elements;
        for (int elementId = 0; elementId < countOfObject_parce; ++elementId) {
            if (elements_parce[elementId] ==1) {
                elements.push_back(elementId);
                klasterIncrement++;
                int mySquare = square(matrix,wight,height,elementId);
                int myPerimeter = perimetr(matrix,wight,height,elementId);
                int myDensity = density(mySquare,myPerimeter);
                *(klasters + klasterIncrement) = new int [p];
                klasters[klasterIncrement][0] = elementId;
                klasters[klasterIncrement][1] = mySquare;
                klasters[klasterIncrement][2] = myPerimeter;
                klasters[klasterIncrement][3] = myDensity;
            }
        }

        for (int i = 0; i < klasterIncrement+1; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << klasters[i][j] << " ";
            }
            std::cout << std::endl;
        }


        int elementsSize = (int) elements.size();



        // k – число кластеров, на которые нужно разбить набор .x_i
        int countClusters = 4;

        //Найти:
        //k средних векторов mj, j = 1,…, k (центров кластеров);
        //отнести каждый из векторов x_i к одному из k кластеров;
        //int **m = new  int* [countClusters];

        //Алгоритм:
        //1. Случайным образом выбрать k средних m_j j = 1,…, k;
        int random_K_means_m_j[countClusters][p];
        for (int i = 0; i < countClusters; ++i) {
            int randOmV = (rand() % (elementsSize-1)) + 1;
            for (int j = 0; j < p; ++j) {
                random_K_means_m_j[i][j] = klasters[randOmV][j];
            }
        }

        bool changed = false;


        do {
            //2. Для каждого xi i = 1,…,p подсчитать расстояние до каждого из mj j=1,…, k,
            //Отнести (приписать) xi к кластеру j’, расстояние до центра которого mj’ минимально;

            for (int i = 0; i < elementsSize; ++i) {
                int diffArray[countClusters];
                for (int j = 0; j < countClusters; ++j) {
                    int diff = 0;
                    for (int j_p= 0; j_p < p; ++j_p) {

                        //std::cout << random_K_means_m_j[j][j_p] << std::endl;
                        //std::cout << klasters[i][j_p] << std::endl;
                        int d = (random_K_means_m_j[j][j_p]-klasters[i][j_p]);
                        diff += d*d;
                    }
                    diffArray[j] = diff;
                    //в массів діфф
                }
                int minIndex = getMinIndex(diffArray,countClusters);
                //тут нужно покрасить область в в цвет кластера, которому она подходит
                std::cout << "Объект " << elements[i] << "принадлежит " << random_K_means_m_j[minIndex][0] << std::endl;
                updateMatrix(matrix,wight,height,elements[i],random_K_means_m_j[minIndex][0]);
            }

            //3. Пересчитать средние (центр масс) mj j=1,…, k по всем кластерам;
            changed = false;
        } while (changed);//4. Повторять шаги 2, 3, пока кластеры не перестанут изменяться.



        std::cout <<"END FUNCTION" << std::endl;
        draw();
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

int MainWindow::square(int **&a, int wight, int height, int elementId)
{
    int countOfPixels = 0;
    for (int var = 0; var < wight; ++var) {
        for (int j = 0; j < height; ++j) {
            if (a[var][j] == elementId) {
                countOfPixels++;
            }
        }
    }
    std::cout << "Площадь объекта номер " << elementId << " равна " << countOfPixels << std::endl;
    return countOfPixels;
}

int MainWindow::perimetr(int **&a, int wight, int height, int elementId)
{
    int countOfPixels = 0;
    for (int i = 1; i < wight-1; ++i) {
        for (int j = 1; j < height-1; ++j) {
            if (a[i][j] == elementId) {
                if (isPerimeterPoint(a,i,j,elementId)) {
                    countOfPixels++;
                }
            }
        }
    }
    std::cout << "Периметр объекта номер " << elementId << " равна " << countOfPixels << std::endl;
    return countOfPixels;
}

bool MainWindow::isPerimeterPoint(int **&a, int wight, int height, int elementId)
{
    if (
            (a[wight-1][height] != elementId)
            || (a[wight][height-1] != elementId)
            || (a[wight][height+1] != elementId)
            || (a[wight-1][height] != elementId) ) {
        return true;
    } else {
        return false;
    }
}

int MainWindow::density(int square, int perimeter)
{
    return perimeter*perimeter/square;
}

int MainWindow::getMinIndex(int * a, int size)
{
    int minIndex = 0;
    int minValue = a[minIndex];
    for (int var = 1; var < size; ++var) {
        if (a[var] < minValue) {
            minIndex = var;
            minValue = a[var];
        }
    }
    return minIndex;
}

void MainWindow::draw()
{
    if (countOfObject_parce !=-1 ) {
        int step = 16581375/countOfObject_parce;
        QImage image(imageOriginal.width(),imageOriginal.height(),QImage::Format_RGB32);
        for (int i = 0; i < imageOriginal.width(); ++i) {
            for (int j = 0; j < imageOriginal.height(); ++j) {
                image.setPixel(i,j,step*matrix[i][j]);
            }
        }
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));
       // delete image;
    }
}





void MainWindow::on_k_alg_but_clicked()
{

}

void MainWindow::on_fitToWindowAct_triggered()
{
    fitToWindow();
}
