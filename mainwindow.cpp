#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qrcodegen.hpp"

using namespace qrcodegen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGenerate_clicked()
{
    QString text = ui->inputText->text();

    if (text.isEmpty())
    {
        return;
    }

    QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::MEDIUM);

    int scale = 6;
    int qrSize = qr.getSize();
    int imageSize = qrSize * scale;

    QImage image(imageSize, imageSize, QImage::Format_RGB32);
    image.fill(bgColor);


    QPainter painter(&image);
    painter.setBrush(fgColor);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < qrSize; i++)
    {
        for (int j = 0; j < qrSize; j++)
        {
            if (qr.getModule(j, i))
            {
                QRectF r(j * scale, i * scale, scale, scale);

                if (rounded == 1)
                {
                    painter.drawEllipse(r);
                }
                else
                {
                    painter.drawRect(r);
                }
            }
        }
    }


    ui->qrLabel->setPixmap(
        QPixmap::fromImage(image).scaled(
            ui->qrLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            )
        );
}

void MainWindow::on_btnSave_clicked()
{
    if (!ui->qrLabel->pixmap())
    {
        return;
    }
    QString fileName = QFileDialog::getSaveFileName
    (
        this, "SaveQrCode", "", "PNG Image (*.png);;JPEG Image (*.jpg)"
    );
    if (fileName.isEmpty())
    {
        return;
    }
    ui->qrLabel->pixmap().save(fileName);
}

void MainWindow::on_btnCopy_clicked()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(ui->qrLabel->pixmap());
}


void MainWindow::on_btnFgColor_clicked()
{
    QColor color = QColorDialog::getColor(fgColor, this, "Select QR Color");
    if (color.isValid())
    {
        fgColor = color;
    }
}


void MainWindow::on_btnBgColor_clicked()
{
    QColor color = QColorDialog::getColor(bgColor, this, "Select Background color");
    if (color.isValid())
    {
        bgColor = color;
    }
}

void MainWindow::on_cbRounded_checkStateChanged(const Qt::CheckState &arg1)
{
    if (rounded == 1)
    {
        rounded = 0;
    }
    else
    {
        rounded = 1;
    }
}

