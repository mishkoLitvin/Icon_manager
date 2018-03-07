#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLinearGradient>
#include <QColorDialog>
#include <QBrush>
#include <QRgb>
#include <QGraphicsBlurEffect>
#include <QPixmap>
#include <QPainter>
#include <QPicture>
#include <QFileDialog>
#include <QSettings>
#include <QFileSystemModel>


#include <savedialog.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SaveDialog *saveDialog;

    QLinearGradient *grGradient;
    QGraphicsRectItem *grRectBkgrItem, *grRectLightItem;
    QGraphicsPixmapItem *grPixLogoItem, *grPixItem, *grPixLightItem;
    QImage *grImg, *grSaveImg;
    QPixmap *grPixLight, *grPixSymb, *grPixLogo;
    QGraphicsColorizeEffect *grColorizeLight, *grColorizeSymbol;
    QGraphicsBlurEffect *grEffect, *grEffectSymb;
    QGraphicsScene *grScene;
    QRect grSize;
    QSettings *setting;
    QFileSystemModel *modelIconsFile;

    void restoreSettings();

private slots:
    void color1Sel();
    void color2Sel();
    void color3Sel();
    void color1Sel(QColor col);
    void color2Sel(QColor col);
    void color3Sel(QColor col);
    void blurBkgrSel();
    void imageResize();
    void loadLogo();
    void sizeLogoSel();


    void reloadPixLight();
    void colorSymbolSel();
    void colorSymbolSel(QColor col);
    void transpSymbolSel();
    void blurSymbolSel();
    void sizeSymbSel();

    void colorMaskSel();
    void colorMaskSel(QColor col);
    void transpMaskSel();

    void loadSymbol();
    void loadSymbol(QModelIndex ind);
    void loadSymbolPrev(QModelIndex ind);
    void loadLibrary();

    void saveIcon();



};

#endif // MAINWINDOW_H
