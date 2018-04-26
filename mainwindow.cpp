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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    QFileDialog dialog(this, "Open File");
    dialog.setFileMode(QFileDialog::ExistingFile);
    //dialog.setDirectory(InputDirectory.string().c_str());

    //QStringList FileList= dialog.e
    if(dialog.exec())
    {
        FileNameWithPath = dialog.getOpenFileName().toStdString();
    }
    else
         return;

    ImIn = imread(FileNameWithPath, CV_LOAD_IMAGE_ANYDEPTH);

}
