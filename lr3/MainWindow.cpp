#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsPixmapItem>

#include "model/Net.h"
#include "data/img.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _pNet = new Net{S, N, P};
    setImage(ui->sImage, S);
    setImage(ui->nImage, N);
    setImage(ui->pImage, P);
    select(S);
    ui->sImage->installEventFilter(this);
    ui->nImage->installEventFilter(this);
    ui->pImage->installEventFilter(this);
    hideResults();
    ui->noiseLabel->setText(QString::number(ui->noiseSlider->value()) + "%");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _pNet;
}

void MainWindow::on_filterBtn_clicked()
{
    auto imageF = _pNet->filter(_image);
    setImage(ui->filteredImage, imageF);
    showError(imageF, *_pImage);
}

void MainWindow::on_noiseSlider_valueChanged()
{
    _image = makeNoise(*_pImage);
    setImage(ui->selectedImage, _image);
    ui->noiseLabel->setText(QString::number(ui->noiseSlider->value()) + "%");
}

bool MainWindow::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        hideResults();
        if (w == ui->sImage) {
            select(S);
        } else if (w == ui->nImage) {
            select(N);
        } else if (w == ui->pImage) {
            select(P);
        }
    }
    return false;
}

void MainWindow::setImage(QGraphicsView *pView, const ImgData &img)
{
    if (pView->scene() == nullptr) {
        pView->setScene(new QGraphicsScene);
    }
    pView->scene()->clear();
    auto data = std::array<QRgb, 16*16> {{0}};
    std::transform(begin(img), end(img), begin(data),
                   [](int8_t t)->QRgb{return t == 1 ? qRgb(0,0,0) : qRgb(0xff, 0xff, 0xff);});
    auto image = QImage{(uchar*)data.data(), 16, 16, QImage::Format_RGB32};
    auto pImgItem = pView->scene()->addPixmap(QPixmap::fromImage(image.copy()));
    pImgItem->setScale(6);
}

void MainWindow::select(const ImgData& img)
{
    _pImage = &img;
    _image = makeNoise(img);
    setImage(ui->selectedImage, _image);
}

void MainWindow::showError(const ImgData &filtered, const ImgData &origin)
{
    auto errors = 0;
    for (auto i = 0u; i < origin.size(); ++i) {
        if (origin[i] != filtered[i]) {
            ++errors;
        }
    }
    ui->errCountLable->setVisible(true);
    ui->errLabel->setVisible(true);
    ui->errorPTextLabel->setVisible(true);
    ui->errPLabel->setVisible(true);

    ui->errCountLable->setText(QString::number(errors));
    ui->errorPTextLabel->setText(QString::number(double(errors) / origin.size() * 100) + QString("%"));
}

void MainWindow::hideResults()
{
    ui->errCountLable->setVisible(false);
    ui->errLabel->setVisible(false);
    ui->errorPTextLabel->setVisible(false);
    ui->errPLabel->setVisible(false);
    if (ui->filteredImage->scene() != nullptr)
        ui->filteredImage->scene()->clear();
}

ImgData MainWindow::makeNoise(const ImgData &img)
{
    const auto percent = ui->noiseSlider->value();
    auto points = std::vector<int>(img.size());
    std::iota(begin(points), end(points), 0);
    auto res = img;
    const auto pointsCount = img.size() * percent / 100;
    for (auto i = 0u; i < pointsCount; ++i) {
        auto const index = qrand() % points.size();
        auto const p = points[index];
        res[p] = res[p] == 1 ? 0 : 1;
        points.erase(begin(points) + index);
    }
    return res;
}


