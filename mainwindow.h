#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>
#include <QClipboard>
#include <QColorDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnGenerate_clicked();

    void on_btnSave_clicked();

    void on_btnCopy_clicked();

    void on_btnFgColor_clicked();

    void on_btnBgColor_clicked();

private:
    Ui::MainWindow *ui;

    QColor fgColor = Qt::black;
    QColor bgColor = Qt::white;
};
#endif // MAINWINDOW_H
