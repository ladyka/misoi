#include <QApplication>
#include <QCommandLineParser>

#include "imageviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("LAB 2 MISOI"));
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption();
    commandLineParser.addPositionalArgument(ImageViewer::tr("[file]"), ImageViewer::tr("Image file to open."));
    commandLineParser.process(QCoreApplication::arguments());
    ImageViewer imageViewer;
    if (!commandLineParser.positionalArguments().isEmpty()
        && !imageViewer.loadFile(commandLineParser.positionalArguments().front())) {
        return -1;
    }
    imageViewer.show();
    return app.exec();
}


/**
 * 1. При необходимости выполнить коррекцию яркости изображения (см. лаб. работу №1).
 *
 * 2. Перевести цветное изображение в бинарное изображение (см. лаб. работу №1).
 *
 * 3. Реализовать пороговую бинаризацию изображения (см. лаб. работу №1).
 *
 * 4. Выделить связные области на изображении (рекурсивный алгоритм или алгоритм
 * последовательного сканирования).
 *
 * 5. Определить свойства объектов, вычислить систему признаков для объектов,
 * представленных на изображении (площадь, периметр, компактность, вытянутость,
 * статистические моменты). Вычисление всех признаков обязательно. Проанализировать,
 * какие из признаков являются наиболее информативными.
 *
 * 6. Используя алгоритм k-means, k-medians либо k-medoids, определить
 * принадлежность объекта к одному из кластеров (классов).
 * Реализация данного алгоритма обязательна.
 */

/*************************************************************
 * Алгоритм выделения связных областей : Последовательного сканирования
 * Алгоритм кластеризации : k-medoids
 */

/*************************************************************
 * Контрольные вопросы
 * 1. Геометрические признаки при описании объектов.
 * 2. Фотометрические признаки при описании объектов.
 * 3. Кластерный анализ.
 * 4. Алгоритм k-means.
 * 5. Алгоритм k-medians.
 * 6. Алгоритм k-medoids.
 */
