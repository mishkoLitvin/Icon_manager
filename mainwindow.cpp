#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCoreApplication::setOrganizationName("My uniwers");
    QCoreApplication::setApplicationName("Icon creator");
    setting = new QSettings("settings.icm", QSettings::IniFormat);

    modelIconsFile = new QFileSystemModel;
    modelIconsFile->setRootPath(setting->value("file/dir").toString());
    ui->treeView->setModel(modelIconsFile);
    ui->treeView->setRootIndex(modelIconsFile->index(setting->value("file/dir").toString()));
    ui->treeView->setColumnWidth(0, 200);

    saveDialog = new SaveDialog();

    grScene = new QGraphicsScene();

    grScene->setBackgroundBrush(QBrush(Qt::transparent));
    QBrush grBrash;

    grSize.setCoords(0,0,setting->value("size/imageW").toInt(),setting->value("size/imageH").toInt());
    grRectBkgrItem = grScene->addRect(grSize, QPen(QColor(Qt::transparent),0),grBrash);

    grGradient = new QLinearGradient(grRectBkgrItem->boundingRect().topLeft(),
                               grRectBkgrItem->boundingRect().bottomRight());
    grGradient->setColorAt(1, QColor(15,100,120));
    grGradient->setColorAt(0, QColor(15,10,10));

    grEffect = new QGraphicsBlurEffect();
    grEffect->setBlurRadius(0.1);
    grRectBkgrItem->setGraphicsEffect(grEffect);
    grRectBkgrItem->setBrush(*grGradient);

    grPixLogo = new QPixmap(grSize.size());
    grPixLogo->fill(Qt::transparent);
    grPixLogoItem = grScene->addPixmap(*grPixLogo);

    grImg = new QImage(grSize.size(), QImage::Format_ARGB32);
    grSaveImg = new QImage(grSize.width(),grSize.height()*2, QImage::Format_ARGB32);

    grPixSymb = new QPixmap(grSize.size());
    grPixSymb->fill(Qt::transparent);
    grPixSymb->load("symbols/tt2.png");
    *grPixSymb = grPixSymb->scaled(grSize.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);


    grEffectSymb = new QGraphicsBlurEffect();
//    grEffectSymb->setBlurRadius(0);

    grPixItem = grScene->addPixmap(*grPixSymb);
//    grPixItem->setGraphicsEffect(grEffectSymb);

    grColorizeSymbol = new QGraphicsColorizeEffect;

    grPixItem->setGraphicsEffect(grColorizeSymbol);

    grPixLight = new QPixmap();
    grColorizeLight = new QGraphicsColorizeEffect;
    grColorizeLight->setColor(Qt::white);
    grColorizeLight->setStrength(0.2);


    grPixLightItem = grScene->addPixmap(*grPixLight);
    grPixLightItem->setGraphicsEffect(grColorizeLight);
    grPixLightItem->setPos(0,grSize.height());

    this->reloadPixLight();


    ui->graphicsView->setScene(grScene);

    connect(ui->checkBoxSymbAuto, SIGNAL(toggled(bool)), ui->spinBoxSymbXPos, SLOT(setDisabled(bool)));
    connect(ui->checkBoxSymbAuto, SIGNAL(toggled(bool)), ui->spinBoxSymbYPos, SLOT(setDisabled(bool)));
    connect(ui->checkBoxSymbAuto, SIGNAL(toggled(bool)), ui->labelSymbX, SLOT(setDisabled(bool)));
    connect(ui->checkBoxSymbAuto, SIGNAL(toggled(bool)), ui->labelSymbY, SLOT(setDisabled(bool)));


    connect(ui->pButtonColor1, SIGNAL(clicked(bool)), this, SLOT(color1Sel()));
    connect(ui->pButtonColor2, SIGNAL(clicked(bool)), this, SLOT(color2Sel()));
    connect(ui->pButtonColor3, SIGNAL(clicked(bool)), this, SLOT(color3Sel()));
    connect(ui->dSpinBoxBkgrBlur, SIGNAL(valueChanged(double)), this, SLOT(blurBkgrSel()));
    connect(ui->spinBoxH, SIGNAL(valueChanged(int)), this, SLOT(imageResize()));
    connect(ui->spinBoxW, SIGNAL(valueChanged(int)), this, SLOT(imageResize()));
    connect(ui->pButtonLoadLogo, SIGNAL(clicked(bool)), this, SLOT(loadLogo()));
    connect(ui->dSpinBoxBkgrnLogoSize, SIGNAL(valueChanged(double)), this, SLOT(sizeLogoSel()));
    connect(ui->spinBoxLogoXPos, SIGNAL(valueChanged(int)), this, SLOT(sizeLogoSel()));
    connect(ui->spinBoxLogoYPos, SIGNAL(valueChanged(int)), this, SLOT(sizeLogoSel()));

    connect(ui->pButtonColorSymbol, SIGNAL(clicked(bool)), this, SLOT(colorSymbolSel()));
    connect(ui->dSpinBoxSymbolSize, SIGNAL(valueChanged(double)), this, SLOT(sizeSymbSel()));
    connect(ui->spinBoxSymbXPos, SIGNAL(valueChanged(int)), this, SLOT(sizeSymbSel()));
    connect(ui->spinBoxSymbYPos, SIGNAL(valueChanged(int)), this, SLOT(sizeSymbSel()));
    connect(ui->checkBoxSymbAuto, SIGNAL(clicked(bool)), this, SLOT(sizeSymbSel()));
    connect(ui->dSpinBoxSymbColor, SIGNAL(valueChanged(double)), this, SLOT(transpSymbolSel()));

    connect(ui->pButtonColorMask, SIGNAL(clicked(bool)), this, SLOT(colorMaskSel()));
    connect(ui->dSpinBoxMask, SIGNAL(valueChanged(double)), this, SLOT(transpMaskSel()));

    connect(ui->pButtonLoad, SIGNAL(clicked(bool)), this, SLOT(loadSymbol()));
    connect(ui->pButtonSave, SIGNAL(clicked(bool)), this, SLOT(saveIcon()));

    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(loadSymbol(QModelIndex)));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(loadSymbolPrev(QModelIndex)));

    connect(ui->pButtonLoadLib, SIGNAL(clicked(bool)), this, SLOT(loadLibrary()));
    this->restoreSettings();

}

MainWindow::~MainWindow()
{
    saveDialog->close();
    setting->sync();
    delete ui;
}

void MainWindow::restoreSettings()
{
    ui->spinBoxH->setValue(setting->value("size/imageH").toInt());
    ui->spinBoxW->setValue(setting->value("size/imageW").toInt());
    ui->dSpinBoxSymbColor->setValue(setting->value("size/symbol").toFloat());
    ui->dSpinBoxSymbolSize->setValue(setting->value("size/symbScale").toFloat());
    ui->dSpinBoxBkgrBlur->setValue(setting->value("size/bkgrBlur").toFloat());
    ui->dSpinBoxMask->setValue(setting->value("size/maskInt").toFloat());


    this->colorSymbolSel(QColor(setting->value("colors/symbCol").value<QColor>()));
    this->color1Sel(QColor(setting->value("colors/bkgrdCol1").value<QColor>()));
    this->color2Sel(QColor(setting->value("colors/bkgrdCol2").value<QColor>()));
    this->color3Sel(QColor(setting->value("colors/bkgrdCol3").value<QColor>()));

    this->colorMaskSel(QColor(setting->value("colors/maskCol").value<QColor>()));


    this->imageResize();
}

void MainWindow::reloadPixLight()
{
    *grImg = QImage(grSize.size(), QImage::Format_ARGB32);

    grImg->fill(Qt::transparent);
    QPainter grPainter(grImg);
    grScene->render(&grPainter,grImg->rect(),grSize);

    grPixLight->fill(Qt::transparent);
    *grPixLight = QPixmap::fromImage(*grImg);
    grPixLightItem->setPixmap(*grPixLight);
//    grPixLightItem->setGraphicsEffect(grColorizeLight);

    grScene->update();
}

void MainWindow::colorSymbolSel()
{
    QColor col = QColorDialog::getColor();
    QPalette pal = ui->pButtonColorSymbol->palette();
    pal.setColor(QPalette::Button, col);
    setting->setValue("colors/symbCol", col);

    ui->pButtonColorSymbol->setAutoFillBackground(true);
    ui->pButtonColorSymbol->setPalette(pal);
    ui->pButtonColorSymbol->update();

    grColorizeSymbol->setColor(col);
    grScene->update();
    this->reloadPixLight();
}

void MainWindow::colorSymbolSel(QColor col)
{
    QPalette pal = ui->pButtonColorSymbol->palette();
    pal.setColor(QPalette::Button, col);

    ui->pButtonColorSymbol->setAutoFillBackground(true);
    ui->pButtonColorSymbol->setPalette(pal);
    ui->pButtonColorSymbol->update();

    grColorizeSymbol->setColor(col);
    grScene->update();
    this->reloadPixLight();
}

void MainWindow::transpSymbolSel()
{
    grColorizeSymbol->setStrength(ui->dSpinBoxSymbColor->value());
    setting->setValue("size/symbol", ui->dSpinBoxSymbColor->value());
    this->reloadPixLight();
}

void MainWindow::blurSymbolSel()
{
    grPixItem->setGraphicsEffect(grEffectSymb);
    this->reloadPixLight();
}

void MainWindow::sizeSymbSel()
{
    QPixmap grPixSymbTemp = grPixSymb->scaled(grSize.size().width()*(ui->dSpinBoxSymbolSize->value()/100.),
                                   grSize.size().height()*(ui->dSpinBoxSymbolSize->value()/100.),
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setting->setValue("size/symbScale", ui->dSpinBoxSymbolSize->value());
    grPixItem->setPixmap(grPixSymbTemp);
    if(ui->checkBoxSymbAuto->isChecked())
    {
        grPixItem->setPos(grSize.width()/2.-grPixSymbTemp.size().width()/2, 15*(1-(ui->dSpinBoxSymbolSize->value()/100.)));
        ui->spinBoxSymbXPos->setValue(grSize.width()/2.-grPixSymbTemp.size().width()/2);
        ui->spinBoxSymbYPos->setValue(15*(1-(ui->dSpinBoxSymbolSize->value()/100.)));
    }
    else
    {
        grPixItem->setPos(ui->spinBoxSymbXPos->value(),ui->spinBoxSymbYPos->value());
    }
    this->reloadPixLight();
}

void MainWindow::colorMaskSel()
{
    QColor col = QColorDialog::getColor();
    QPalette pal = ui->pButtonColorMask->palette();
    pal.setColor(QPalette::Button, col);
    setting->setValue("colors/maskCol", col);

    ui->pButtonColorMask->setAutoFillBackground(true);
    ui->pButtonColorMask->setPalette(pal);
    ui->pButtonColorMask->update();
    grColorizeLight->setColor(col);
    this->reloadPixLight();

}

void MainWindow::colorMaskSel(QColor col)
{
    QPalette pal = ui->pButtonColorMask->palette();
    pal.setColor(QPalette::Button, col);

    ui->pButtonColorMask->setAutoFillBackground(true);
    ui->pButtonColorMask->setPalette(pal);
    ui->pButtonColorMask->update();
    grColorizeLight->setColor(col);
    this->reloadPixLight();

}

void MainWindow::transpMaskSel()
{
    grColorizeLight->setStrength(ui->dSpinBoxMask->value());
    setting->setValue("size/maskInt", ui->dSpinBoxMask->value());
    this->reloadPixLight();
}

void MainWindow::loadSymbol()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open symbol...");

    grPixSymb->fill(Qt::transparent);
    grPixSymb->load(fileName);
    *grPixSymb = grPixSymb->scaled(grSize.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    grPixItem->setPixmap(*grPixSymb);
    this->sizeSymbSel();
    this->reloadPixLight();
}

void MainWindow::loadSymbol(QModelIndex ind)
{
    if(!modelIconsFile->isDir(ind))
    {
        grPixSymb->fill(Qt::transparent);
        grPixSymb->load(modelIconsFile->filePath(ind));
        *grPixSymb = grPixSymb->scaled(grSize.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
        grPixItem->setPixmap(*grPixSymb);
        this->sizeSymbSel();
        this->reloadPixLight();
    }
}

void MainWindow::loadSymbolPrev(QModelIndex ind)
{
    if(!modelIconsFile->isDir(ind))
    {
        QPixmap pix;
        pix.load(modelIconsFile->filePath(ind));
        pix = pix.scaled(200,200,Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labelSymbolPrev->setPixmap(pix);
    }
}

void MainWindow::loadLibrary()
{
    QString dName = QFileDialog::getExistingDirectory(this, "Select symbol folder library");
    setting->setValue("file/dir", dName);
    modelIconsFile->setRootPath(dName);
    ui->treeView->setRootIndex(modelIconsFile->index(dName));

}

void MainWindow::saveIcon()
{
    *grSaveImg = QImage(grSize.size().width(), grSize.size().height()*2, QImage::Format_ARGB32);
    grSaveImg->fill(Qt::white);

    QPainter grPainter(grSaveImg);
    grScene->render(&grPainter,grSaveImg->rect(), QRect(0,0,grSize.width(),grSize.height()*2));

    saveDialog->veiwDialog(*grSaveImg);
//    grSaveImg->save(fName, "BMP");
}

void MainWindow::color1Sel()
{
    QColor col = QColorDialog::getColor();
    QPalette pal = ui->pButtonColor1->palette();
    pal.setColor(QPalette::Button, col);
    setting->setValue("colors/bkgrdCol1", col);

    ui->pButtonColor1->setAutoFillBackground(true);
    ui->pButtonColor1->setPalette(pal);
    ui->pButtonColor1->update();
    grGradient->setColorAt(0, col);
    grRectBkgrItem->setBrush(*grGradient);
    grRectBkgrItem->update(grRectBkgrItem->rect());
    grScene->update();
    this->reloadPixLight();

}

void MainWindow::color2Sel()
{
    QColor col = QColorDialog::getColor();
    QPalette pal = ui->pButtonColor2->palette();
    pal.setColor(QPalette::Button, col);
    setting->setValue("colors/bkgrdCol2", col);

    ui->pButtonColor2->setAutoFillBackground(true);
    ui->pButtonColor2->setPalette(pal);
    ui->pButtonColor2->update();
    grGradient->setColorAt(0.5, col);
    grRectBkgrItem->setBrush(*grGradient);
    grRectBkgrItem->update(grRectBkgrItem->rect());
    grScene->update();
    this->reloadPixLight();


}

void MainWindow::color3Sel()
{
    QColor col = QColorDialog::getColor();
    QPalette pal = ui->pButtonColor3->palette();
    pal.setColor(QPalette::Button, col);
    setting->setValue("colors/bkgrdCol3", col);

    ui->pButtonColor3->setAutoFillBackground(true);
    ui->pButtonColor3->setPalette(pal);
    ui->pButtonColor3->update();
    grGradient->setColorAt(1.0, col);
    grRectBkgrItem->setBrush(*grGradient);
    grRectBkgrItem->update(grRectBkgrItem->rect());
    grScene->update();
    this->reloadPixLight();


}

void MainWindow::color1Sel(QColor col)
{
    QPalette pal = ui->pButtonColor1->palette();
    pal.setColor(QPalette::Button, col);
    setting->setValue("colors/bkgrdCol1", col);

    ui->pButtonColor1->setAutoFillBackground(true);
    ui->pButtonColor1->setPalette(pal);
    ui->pButtonColor1->update();
    grGradient->setColorAt(0, col);
    grRectBkgrItem->setBrush(*grGradient);
    grRectBkgrItem->update(grRectBkgrItem->rect());
    grScene->update();
    this->reloadPixLight();

}

void MainWindow::color2Sel(QColor col)
{
    QPalette pal = ui->pButtonColor2->palette();
    pal.setColor(QPalette::Button, col);
    ui->pButtonColor2->setAutoFillBackground(true);
    ui->pButtonColor2->setPalette(pal);
    ui->pButtonColor2->update();
    grGradient->setColorAt(0.5, col);
    grRectBkgrItem->setBrush(*grGradient);
    grRectBkgrItem->update(grRectBkgrItem->rect());
    grScene->update();
    this->reloadPixLight();


}

void MainWindow::color3Sel(QColor col)
{
    QPalette pal = ui->pButtonColor3->palette();
    pal.setColor(QPalette::Button, col);
    ui->pButtonColor3->setAutoFillBackground(true);
    ui->pButtonColor3->setPalette(pal);
    ui->pButtonColor3->update();
    grGradient->setColorAt(1.0, col);
    grRectBkgrItem->setBrush(*grGradient);
    grRectBkgrItem->update(grRectBkgrItem->rect());
    grScene->update();
    this->reloadPixLight();


}

void MainWindow::blurBkgrSel()
{
    grEffect->setBlurRadius(ui->dSpinBoxBkgrBlur->value());
    grRectBkgrItem->setGraphicsEffect(grEffect);
    setting->setValue("size/bkgrBlur",ui->dSpinBoxBkgrBlur->value());
    this->reloadPixLight();
}

void MainWindow::imageResize()
{
    setting->setValue("size/imageH", ui->spinBoxH->value());
    setting->setValue("size/imageW", ui->spinBoxW->value());

    grSize.setWidth(ui->spinBoxW->value());
    grSize.setHeight(ui->spinBoxH->value());
    grRectBkgrItem->setRect(grSize);
    grRectBkgrItem->update(grRectBkgrItem->rect());

    this->sizeSymbSel();
    grScene->update();

    grPixLightItem->setPos(0,grSize.height());

    this->reloadPixLight();
}

void MainWindow::loadLogo()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open logo...");

    grPixLogo->fill(Qt::transparent);
    grPixLogo->load(fileName);
    *grPixLogo = grPixLogo->scaled(grSize.size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    grPixLogoItem->setPixmap(*grPixLogo);
    this->sizeLogoSel();
}

void MainWindow::sizeLogoSel()
{
    QPixmap grPixSymbTemp = grPixLogo->scaled(grSize.size().width()*(ui->dSpinBoxBkgrnLogoSize->value()/100.),
                                   grSize.size().height()*(ui->dSpinBoxBkgrnLogoSize->value()/100.),
                                   Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //setting->setValue("size/symbScale", ui->dSpinBoxSymbolSize->value());
    grPixLogoItem->setPixmap(grPixSymbTemp);

    grPixLogoItem->setPos(ui->spinBoxLogoXPos->value(),ui->spinBoxLogoYPos->value());
    this->reloadPixLight();
}
