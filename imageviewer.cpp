/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif

#include "imageviewer.h"

//! [0]
ImageViewer::ImageViewer()
{
    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
    fitToWindowAct->setChecked(true);
}

//! [0]
//! [2]

bool ImageViewer::loadFile(const QString &fileName)
{
    QImage image(fileName);
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
//! [2] //! [3]
    imageLabel->setPixmap(QPixmap::fromImage(image));
//! [3] //! [4]
    scaleFactor = 1.0;

    printAct->setEnabled(true);
    saveAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    gistogram->setEnabled(true);
    dissectionA->setEnabled(true);
    dissectionB->setEnabled(true);
    viewImage->setEnabled(true);
    sobel->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
    setWindowFilePath(fileName);
    fitToWindow();
    return true;
}

//! [4]

//! [2]

//! [1]
void ImageViewer::open()
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
    imageOriginal = *(imageLabel->pixmap());
}
//! [1]

//! [5]
void ImageViewer::print()
//! [5] //! [6]
{
    Q_ASSERT(imageLabel->pixmap());
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
#endif
}

void ImageViewer::save()
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
    const QPixmap *a = imageLabel->pixmap();
    a->save(&file,"PNG");
}

void ImageViewer::viewImageF()
{
    imageLabel->setPixmap(imageOriginal);
}

void ImageViewer::gistogramGenerate()
{
    float gist[255];
    for(int i = 0; i < 255; i++)
    {
        gist[i] = 0;
    }
    QPixmap a = *(imageLabel->pixmap());
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
    imageLabel->setPixmap(pixImage);

    fitToWindow();
}

void ImageViewer::dissectionAGenerate()
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
            if (g > 128)
            {
                c =1;
            }
            outImage.setPixel(wi,hi,c);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(outImage));
    fitToWindow();
}

void ImageViewer::dissectionBGenerate()
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
            if ((g > 85) && (g < 170))
            {
                c =1;
            }
            outImage.setPixel(wi,hi,c);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(outImage));
    fitToWindow();
}

void ImageViewer::sobelF()
{
    QPixmap a = *(imageLabel->pixmap());
    QImage image = a.toImage();
    QImage outImage(image.width(),image.height(),QImage::Format_RGB32);
    int h = image.height();
    int w = image.width();
    for (int hi = 1; hi < h-1; ++hi)
    {
        for (int wi = 1; wi < w-1; ++wi)
        {
            int h1 = getH1(&(image),wi,hi);
            int h2 = getH2(&(image),wi,hi);
            int s = (int) sqrt(h1*h1 + h2*h2);
            QRgb rgb = qRgb(s,s,s);

            outImage.setPixel(wi,hi,rgb);
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(outImage));
}



int ImageViewer::getH1(QImage * image, int w, int h)
{
    int ** matrixGREY = getMatrixGrey(image,w,h);

    matrixGREY[0][0] = matrixGREY[0][0]*(1);
    matrixGREY[0][1] = matrixGREY[0][1]*(2);
    matrixGREY[0][2] = matrixGREY[0][2]*(1);

    matrixGREY[1][0] = matrixGREY[1][0]*(0);
    matrixGREY[1][1] = matrixGREY[1][1]*(0);
    matrixGREY[1][2] = matrixGREY[1][2]*(0);

    matrixGREY[2][0] = matrixGREY[2][0]*(-1);
    matrixGREY[2][1] = matrixGREY[2][1]*(-2);
    matrixGREY[2][2] = matrixGREY[2][2]*(-1);
    int det = getDet(matrixGREY);
     std::cout << "det " << det  << std::endl;
    return det;
}

//checkmatr
int ImageViewer::getDet(int ** a)
{
    std::cout << a[0][0] << " " << a[0][1] << " " << a[0][2] << std::endl;
    std::cout << a[1][0] << " " << a[1][1] << " " << a[1][2] << std::endl;
    std::cout << a[2][0] << " " << a[2][1] << " " << a[2][2] << std::endl;
    return a[0][0]*a[1][1]*a[2][2]+a[2][0]*a[0][1]*a[1][2]+a[1][0]*a[2][1]*a[0][2]-a[2][0]*a[1][1]*a[0][2]-a[1][0]*a[0][1]*a[2][2]-a[0][0]*a[2][1]*a[1][2];
}

//checkmatr
int ** ImageViewer::getMatrixGrey(QImage * image, int w, int h)
{
    QRgb matrixRGB[3][3] = {
        image->pixel(w-1,h-1),image->pixel(w,h-1),image->pixel(w+1,h-1),
        image->pixel(w-1,h),image->pixel(w,h),image->pixel(w+1,h),
        image->pixel(w-1,h+1),image->pixel(w,h+1),image->pixel(w+1,h+1)
    };

    int** matrixGREY = new int*[3];
    for (int i = 0; i < 3; i++) {
        matrixGREY[i] = new int[3];
        for (int j = 0; j < 3; j++) {
            int g = qGray(matrixRGB[i][j]);
            matrixGREY[i][j] = g;
        }
    };
    return matrixGREY;
}


int ImageViewer::getH2(QImage *image, int w, int h)
{
    int ** matrixGREY = getMatrixGrey(image,w,h);

    matrixGREY[0][0] = matrixGREY[0][0]*(-1);
    matrixGREY[0][1] = matrixGREY[0][1]*(0);
    matrixGREY[0][2] = matrixGREY[0][2]*(1);

    matrixGREY[1][0] = matrixGREY[1][0]*(-2);
    matrixGREY[1][1] = matrixGREY[1][1]*(0);
    matrixGREY[1][2] = matrixGREY[1][2]*(2);

    matrixGREY[2][0] = matrixGREY[2][0]*(-1);
    matrixGREY[2][1] = matrixGREY[2][1]*(0);
    matrixGREY[2][2] = matrixGREY[2][2]*(1);
    return getDet(matrixGREY);
}



void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

//! [10] //! [11]
void ImageViewer::normalSize()
//! [11] //! [12]
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}
//! [12]

//! [13]
void ImageViewer::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}
//! [14]



void ImageViewer::about()
{
    QMessageBox::about(this, tr("About Lab"),
            tr("Ladyka Andrei"));
}

//! [17]
void ImageViewer::createActions()
//! [17] //! [18]
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    printAct = new QAction(tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setEnabled(false);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    saveAct = new QAction(tr("&Save..."), this);
    saveAct->setEnabled(false);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    viewImage = new QAction(tr("Показать изображение оригинальное"), this);
    viewImage->setEnabled(false);
    connect(viewImage,SIGNAL(triggered()), this, SLOT(viewImageF()));

    gistogram = new QAction(tr("Generate gistoramm"), this);
    gistogram->setEnabled(false);
    connect(gistogram,SIGNAL(triggered()), this, SLOT(gistogramGenerate()));

    dissectionA = new QAction(tr("Препарирование изображения A"), this);
    dissectionA->setEnabled(false);
    connect(dissectionA,SIGNAL(triggered()), this, SLOT(dissectionAGenerate()));

    dissectionB = new QAction(tr("Препарирование изображения B"), this);
    dissectionB->setEnabled(false);
    connect(dissectionB,SIGNAL(triggered()), this, SLOT(dissectionBGenerate()));


    sobel = new QAction(tr("Оператор собеля"), this);
    sobel->setEnabled(false);
    connect(sobel,SIGNAL(triggered()), this, SLOT(sobelF()));


    zoomInAct = new QAction(tr("Zoom &In (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    zoomInAct->setEnabled(false);
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    zoomOutAct->setEnabled(false);
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Normal Size"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Fit to Window"), this);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));




}
//! [18]

//! [19]
void ImageViewer::createMenus()
//! [19] //! [20]
{
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(printAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu = new QMenu(tr("&View"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    labOneMenu = new QMenu(tr("&Lab One"), this);
    labOneMenu->addAction(viewImage);
    labOneMenu->addAction(gistogram);
    labOneMenu->addAction(dissectionA);
    labOneMenu->addAction(dissectionB);
    labOneMenu->addAction(sobel);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(labOneMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(helpMenu);
}
//! [20]

//! [21]
void ImageViewer::updateActions()
//! [21] //! [22]
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

