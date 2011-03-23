#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/qpainter.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage *qimage;
    
protected:
        void paintEvent(QPaintEvent*);
        

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
