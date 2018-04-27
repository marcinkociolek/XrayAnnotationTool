#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "gradient.h"
#include "DispLib.h"



using namespace std;
using namespace cv;

//------------------------------------------------------------------------------------------------------
void ShowImage()
{

}



//------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DirectoryPath = "E:/Biodra/SRC_anonimized Tiff/";
}

MainWindow::~MainWindow()
{
    delete ui;
}

//########################################################################################
//         My slots
//########################################################################################
void MainWindow::on_widgetImage_mouseMoved(QPoint point, int butPressed)

{

    int x = point.x()/imageShowScale;
    int y = point.y()/imageShowScale;

    //if(x < 0 || y < 0 || x >= maxX - 1 || y > = maxY - 1)
    //    return;

    if(butPressed == 1)
    {
    }
    if(butPressed & 0x2)
    {
    }

    //ShowImages();
}


void MainWindow::on_widgetImage_KeyPressed(int key)
{
    switch(key)
    {
    case Qt::Key_Space:
        break;
    case Qt::Key_S:
    case Qt::Key_Insert:
        break;
    case Qt::Key_A:
    case Qt::Key_PageUp:
        break;
    case Qt::Key_Z:
    case Qt::Key_PageDown:
        break;
    case Qt::Key_Up:
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;

    case Qt::Key_Home:
        break;
    case Qt::Key_End:
        break;
    case Qt::Key_R:
        break;
    case Qt::Key_P:
        break;
    case Qt::Key_L:
        break;


    default:
        break;
    }
}
//########################################################################################
void MainWindow::on_widgetImage_mousePressed(QPoint point)
{
    //ui->spinBoxValGradient->setValue(point.x());
    //ui->spinBoxValIntensity->setValue(point.y());
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    QFileDialog dialog(this, "Open File");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.jpg *.tif)"));
    dialog.setDirectory(DirectoryPath.c_str());


    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();
    else
        return;
    if(!fileNames.size())
        return;

    FileNameWithPath = fileNames.at(0).toStdString();//dialog.getOpenFileName(this).toStdString();
    DirectoryPath  = dialog.directory().path().toStdString();

    ImIn = imread(FileNameWithPath, CV_LOAD_IMAGE_ANYDEPTH);

    if(ImIn.empty())
        return;

    Mat ImShowGray = ShowImage16Gray(ImIn,0.0,3000.0);
    imshow("test",ImShowGray);
    ui->widgetInputImage->paintBitmap(ImShowGray);
    ui->widgetInputImage->repaint();

}
