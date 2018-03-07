#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>
#include <QRect>
#include <QFileDialog>

#include <QDebug>

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = 0);
    ~SaveDialog();

    void veiwDialog(QImage pix);

private slots:
    void on_buttonBox_accepted();

    void on_pButtonSave_clicked();

private:
    Ui::SaveDialog *ui;

    QGraphicsScene *grScene;
    QGraphicsPixmapItem *pixItem;
    QPixmap *grPixmap;
};

#endif // SAVEDIALOG_H
