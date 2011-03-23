#include <QtGui/qpainter.h>

#include "include/mainwindow.h"
#include "include/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->qimage = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent*){
        QPainter qpainter(this);
        
        qpainter.drawImage(0, 0, *qimage);
}

// void MainWindow::getImage(QImage *qimage){
//     if (this->qimage != NULL) delete (this->image);
//     this->qimage = new QImage (qimage);
// }

