#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qrcodegen.hpp"

using namespace qrcodegen;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fillComboStyle();
    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::saveSettings()
{
    QJsonObject json;

    json["fgColor"] = fgColor.name(QColor::HexArgb);
    json["bgColor"] = bgColor.name(QColor::HexArgb);
    json["typeIndex"] = ui->cbStyle->currentIndex();

    QJsonDocument doc(json);

    QFile file("settings.json");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::loadSettings()
{
    QFile file("settings.json");
    if (!file.exists())
    {
        return;
    }

    if (file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        QJsonObject json = doc.object();

        fgColor = QColor(json["fgColor"].toString());
        bgColor = QColor(json["bgColor"].toString());

        int idx = json["typeIndex"].toInt();
        if (idx >= 0 && idx < ui->cbStyle->count())
        {
            ui->cbStyle->setCurrentIndex(idx);
        }
    }
}


void MainWindow::fillComboStyle()
{
    ui->cbStyle->clear();
    ui->cbStyle->addItem("Squares", QVariant::fromValue((int)QrStyle::Squares));
    ui->cbStyle->addItem("Rounded", QVariant::fromValue((int)QrStyle::Rounded));
    ui->cbStyle->addItem("Soft Rounded", QVariant::fromValue((int)QrStyle::SoftRounded));
}

QrStyle MainWindow::getSelectedStyle() const
{
    QrStyle style = (QrStyle) ui->cbStyle->currentData().toInt();

    return style;
}

void MainWindow::on_btnGenerate_clicked()
{
    QString text = ui->inputText->text();
    if (text.isEmpty())
    {
        return;
    }

    QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::MEDIUM);

    int scale = 8;
    int qrSize = qr.getSize();
    int imageSize = qrSize * scale;

    QImage image(imageSize, imageSize, QImage::Format_ARGB32);
    image.fill(bgColor);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(fgColor);
    painter.setPen(Qt::NoPen);

    QrStyle style = getSelectedStyle();

    for (int y = 0; y < qrSize; y++)
    {
        for (int x = 0; x < qrSize; x++)
        {
            if (!qr.getModule(x, y))
            {
                continue;
            }

            QRect rect(x * scale, y * scale, scale, scale);
            rect.adjust(0, 0, 1, 1);


            switch (style)
            {
                case QrStyle::Squares:
                {
                    painter.drawRect(rect);
                    break;
                }

                case QrStyle::Rounded:
                {
                    painter.drawEllipse(rect);
                    break;
                }
                case QrStyle::SoftRounded:
                {
                    bool top    = (y > 0 && qr.getModule(x, y - 1));
                    bool bottom = (y < qrSize - 1 && qr.getModule(x, y + 1));
                    bool left   = (x > 0 && qr.getModule(x - 1, y));
                    bool right  = (x < qrSize - 1 && qr.getModule(x + 1, y));

                    bool tl = !top && !left;
                    bool tr = !top && !right;
                    bool bl = !bottom && !left;
                    bool br = !bottom && !right;

                    int r = scale * 0.5;

                    QPainterPath path;
                    path.moveTo(rect.left() + (tl ? r : 0), rect.top());

                    path.lineTo(rect.right() - (tr ? r : 0), rect.top());
                    if (tr) path.quadTo(rect.right(), rect.top(), rect.right(), rect.top() + r);

                    path.lineTo(rect.right(), rect.bottom() - (br ? r : 0));
                    if (br) path.quadTo(rect.right(), rect.bottom(), rect.right() - r, rect.bottom());

                    path.lineTo(rect.left() + (bl ? r : 0), rect.bottom());
                    if (bl) path.quadTo(rect.left(), rect.bottom(), rect.left(), rect.bottom() - r);

                    path.lineTo(rect.left(), rect.top() + (tl ? r : 0));
                    if (tl) path.quadTo(rect.left(), rect.top(), rect.left() + r, rect.top());

                    painter.drawPath(path);
                    break;

                }
            }
        }
    }

    ui->qrLabel->setPixmap(QPixmap::fromImage(image).scaled(
        ui->qrLabel->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        ));
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
