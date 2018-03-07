#include "savedialog.h"
#include "ui_savedialog.h"

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);

    grScene = new QGraphicsScene();

    ui->graphicsView->setScene(grScene);
    grPixmap = new QPixmap();
    pixItem = grScene->addPixmap(*grPixmap);
}

SaveDialog::~SaveDialog()
{
    delete ui;
}

void SaveDialog::veiwDialog(QImage pix)
{
    *grPixmap = QPixmap::fromImage(pix);
    pixItem->setPixmap(*grPixmap);
    pixItem->update();

    grScene->update();

    this->show();

}

void SaveDialog::on_buttonBox_accepted()
{
    QString fName;
    fName = QFileDialog::getSaveFileName(this, "Save icon.","", tr("Images (*.bmp *png)"));

    QImage img;
    if(ui->radioButton1616->isChecked())
        img = grPixmap->scaled(16,32).toImage();
    if(ui->radioButton3232->isChecked())
        img = grPixmap->scaled(32,64).toImage();
    if(ui->radioButton4242->isChecked())
        img = grPixmap->scaled(42,84).toImage();
    if(ui->radioButton6464->isChecked())
        img = grPixmap->scaled(64,128).toImage();
    if(ui->radioButtonCustom->isChecked())
        img = grPixmap->scaled(ui->spinBoxX->value(),ui->spinBoxY->value()*2).toImage();
    if(ui->checkBoxHalf->isChecked())
    {
        img = img.copy(0,0,img.width(),img.height()/2);
        ui->checkBoxHalf->setChecked(false);
    }
    if(ui->checkBoxBW->isChecked())
    {
        img = img.convertToFormat(QImage::Format_Grayscale8);
        ui->checkBoxBW->setChecked(false);
    }
    if(ui->checkBoxBlack->isChecked())
    {
        img.fill(Qt::black);
        ui->checkBoxBlack->setChecked(false);
    }

    if(ui->radioButtonBMP->isChecked())
        img.save(fName, "BMP");
    if(ui->radioButtonPNG->isChecked())
        img.save(fName, "PNG");
}

void SaveDialog::on_pButtonSave_clicked()
{
    QString fName;
    fName = QFileDialog::getSaveFileName(this, "Save icon.","", tr("Images (*.bmp *.png)"));

    QImage img;
    if(ui->radioButton1616->isChecked())
        img = grPixmap->scaled(16,32).toImage();
    if(ui->radioButton3232->isChecked())
        img = grPixmap->scaled(32,64).toImage();
    if(ui->radioButton4242->isChecked())
        img = grPixmap->scaled(42,84).toImage();
    if(ui->radioButton6464->isChecked())
        img = grPixmap->scaled(64,128).toImage();
    if(ui->radioButtonCustom->isChecked())
        img = grPixmap->scaled(ui->spinBoxX->value(),ui->spinBoxY->value()*2).toImage();
    if(ui->checkBoxHalf->isChecked())
    {
        img = img.copy(0,0,img.width(),img.height()/2);
        ui->checkBoxHalf->setChecked(false);
    }
    if(ui->checkBoxBW->isChecked())
    {
        img = img.convertToFormat(QImage::Format_Grayscale8);
        ui->checkBoxBW->setChecked(false);
    }
    if(ui->checkBoxBlack->isChecked())
    {
        img.fill(Qt::black);
        ui->checkBoxBlack->setChecked(false);
    }


    if(ui->radioButtonBMP->isChecked())
        img.save(fName, "BMP");
    if(ui->radioButtonPNG->isChecked())
        img.save(fName, "PNG");
}
