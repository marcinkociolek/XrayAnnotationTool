#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "gradient.h"
#include "DispLib.h"
#include "SDALib.h"
#include "RegionU16Lib.h"



using namespace std;
using namespace cv;

//------------------------------------------------------------------------------------------------------
void ShowImage()
{

}

//########################################################################################
//         My functions
//########################################################################################
//------------------------------------------------------------------------------------------------------
void MainWindow::OpenImage()
{
    ImIn = imread(FileNameWithPath, CV_LOAD_IMAGE_ANYDEPTH);

    if(ImIn.empty())
        return;

    maxX = ImIn.cols;
    maxY = ImIn.rows;


    ImROI.release();
    ImROI = imread(FileNameWithPath + "ROI.tiff", CV_LOAD_IMAGE_ANYDEPTH);
    if(ImROI.empty())
        ImROI = Mat::zeros(maxY,maxX,CV_16U);

    ui->pushButtonSaveROI->setEnabled(false);
    SDArecalculate();

}
//------------------------------------------------------------------------------------------------------
void MainWindow::SDArecalculate()
{
    int sdaRoiPixCount = 0;
    ImSDA.release();
    ImSDA = CalculateSDAL(ImIn, Mat::ones(maxY,maxX,CV_16U), sdaRadius, &sdaRoiPixCount);
    ui->doubleSpinBoxSDAMax->setMaximum(sdaRoiPixCount);
    ui->doubleSpinBoxSDAMax->setValue(sdaRoiPixCount);
    ShowImages();
}
//------------------------------------------------------------------------------------------------------
void MainWindow::ShowImages()
{
    if(ImIn.empty())
        return;
    ImShowPC = ShowImage16PseudoColor(ImIn,dispMinPC,dispMaxPC);//ShowImage16Gray(ImIn,0.0,3000.0);
    ImShowGray = ShowImage16Gray(ImIn,dispMinGray,dispMaxGray);//ShowImage16Gray(ImIn,0.0,3000.0);
    ImShowSDA = ShowImage16PseudoColor(ImSDA,dispMinSDA,dispMaxSDA);//ShowImage16Gray(ImIn,0.0,3000.0);
    if(showPC)
        imshow("PC",ImShowPC);
    if(showGray)
        imshow("Gray",ImShowGray);
    if(showSDA)
        imshow("SDA",ImShowSDA);
    ShowImageWithMask();

}
void MainWindow::ShowImageWithMask()
{
    if(ImIn.empty())
        return;
    if(ImROI.empty())
        return;

    switch (dispalyMode)
    {
    case 1:
        ui->widgetInputImage->paintBitmap(ShowSolidRegionOnImageInBlack(GetContour5(ImROI),ImShowPC));
        break;
    case 2:
        ui->widgetInputImage->paintBitmap(ShowSolidRegionOnImage(ImROI,ImShowGray));
        break;
    case 3:
        ui->widgetInputImage->paintBitmap(ShowSolidRegionOnImage(GetContour5(ImROI),ImShowGray));
        break;
    case 4:
        ui->widgetInputImage->paintBitmap(ShowSolidRegionOnImageInBlack(ImROI,ImShowSDA));
        break;
    case 5:
        ui->widgetInputImage->paintBitmap(ShowSolidRegionOnImageInBlack(GetContour5(ImROI),ImShowSDA));
        break;

    default:
        ui->widgetInputImage->paintBitmap(ShowSolidRegionOnImageInBlack(ImROI,ImShowPC));
        break;
    }

    //ui->widgetInputImage->setGeometry(positionX,positionY,scaledX,scaledY);
    //ui->widgetInputImage->repaint();
    ScaleImages();
}
//------------------------------------------------------------------------------------------------------
void MainWindow::ScaleImages()
{
    int scaledX = maxX*imageShowScale;
    int scaledY = maxY*imageShowScale;
    //ui->widgetInputImage->setGeometry(positionX,positionY,scaledX,scaledY);
    //ui->scrollAreaWidgetContents->setGeometry(positionX,positionY,scaledX,scaledY);
    ui->widgetInputImage->setMinimumWidth(scaledX);
    ui->widgetInputImage->setMinimumHeight(scaledY);

    //ui->widgetInputImage->setGeometry(positionX,positionY,scaledX,scaledY);
    ui->widgetInputImage->repaint();

}
//----------------------------------------------------------------------------------------------------------
void MainWindow::PlaceShapeOnImage(Mat Im, int x, int y, int size,unsigned short val)
{
    int maxXm1 = maxX-1;
    int maxYm1 = maxY-1;
    if(y >= 0 && y < maxY && x >= 0 && x < maxY)
        Im.at<unsigned short>(y,x) = val;
    if(size > 1)
    {
        if(y>0)
            Im.at<unsigned short>(y-1,x) = val;
        if(y < maxYm1)
            Im.at<unsigned short>(y+1,x) = val;
        if(x>0)
            Im.at<unsigned short>(y,x-1) = val;
        if(x < maxXm1)
            Im.at<unsigned short>(y,x+1) = val;
    }
    if(size > 2)
    {
        if(y > 0 && x > 0)
            Im.at<unsigned short>(y-1,x-1) = val;
        if(y > 0 && x < maxXm1)
            Im.at<unsigned short>(y-1,x+1) = val;
        if(y < maxYm1 && x > 0)
            Im.at<unsigned short>(y+1,x-1) = val;
        if(y < maxYm1 && x < maxXm1)
            Im.at<unsigned short>(y+1,x+1) = val;
    }
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    DirectoryPath = "E:/Biodra/SRC_anonimized Tiff/";

    ui->comboBoxDispalyMode->addItem("Display PC region");
    ui->comboBoxDispalyMode->addItem("Display PC contour");
    ui->comboBoxDispalyMode->addItem("Display Gray region");
    ui->comboBoxDispalyMode->addItem("Display Gray contour");
    ui->comboBoxDispalyMode->addItem("Display SDA region");
    ui->comboBoxDispalyMode->addItem("Display SDA contour");
    ui->comboBoxDispalyMode->setCurrentIndex(0);
    dispalyMode = ui->comboBoxDispalyMode->currentIndex();

    showPC = ui->checkBoxShowPC->checkState();
    showGray = ui->checkBoxShowGray->checkState();
    showSDA = ui->checkBoxShowSDA->checkState();

    imageShowScale = ui->spinBoxScale->value();

    dispMinPC = ui->doubleSpinBoxPCMin->value();
    dispMaxPC = ui->doubleSpinBoxPCMax->value();

    dispMinGray = ui->doubleSpinBoxGrayMin->value();
    dispMaxGray = ui->doubleSpinBoxGrayMax->value();

    dispMinSDA = ui->doubleSpinBoxSDAMin->value();
    dispMaxSDA = ui->doubleSpinBoxSDAMax->value();

    sdaRadius = ui->spinBoxSDARadius->value();
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


    ui->spinBoxX->setValue(x);
    ui->spinBoxY->setValue(y);
    //if(x < 0 || y < 0 || x >= maxX - 1 || y > = maxY - 1)
    //    return;

    if(butPressed == 1)
    {
        PlaceShapeOnImage(ImROI, x, y, penSize,1);
        ui->pushButtonSaveROI->setEnabled(true);
    }
    if(butPressed & 0x2)
    {
        PlaceShapeOnImage(ImROI, x, y, penSize,0);
        ui->pushButtonSaveROI->setEnabled(true);
    }
    ShowImageWithMask();
    //ShowImages();
}
/*
void MainWindow::on_widgetImage_mousePressed(QPoint point, int butPressed)
{
    //ui->spinBoxValGradient->setValue(point.x());
    //ui->spinBoxValIntensity->setValue(point.y());
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
*/
//########################################################################################
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

    OpenImage();
    ShowImages();
    ScaleImages();


    //int positionX = 0;
    //int positionY = 0;



}

void MainWindow::on_spinBox_valueChanged(int arg1)
{

}


void MainWindow::on_spinBoxScale_valueChanged(int arg1)
{
    imageShowScale = arg1;
    ScaleImages();
}

void MainWindow::on_doubleSpinBoxPCMin_valueChanged(double arg1)
{
    dispMinPC = arg1;
    ShowImages();
}

void MainWindow::on_doubleSpinBoxPCMax_valueChanged(double arg1)
{
    dispMaxPC = arg1;
    ShowImages();
}

void MainWindow::on_doubleSpinBoxGrayMin_valueChanged(double arg1)
{
    dispMinGray = arg1;
    ShowImages();
}

void MainWindow::on_doubleSpinBoxGrayMax_valueChanged(double arg1)
{
    dispMaxGray = arg1;
    ShowImages();
}

void MainWindow::on_doubleSpinBoxSDAMin_valueChanged(double arg1)
{
    dispMinSDA = arg1;
    ShowImages();
}

void MainWindow::on_doubleSpinBoxSDAMax_valueChanged(double arg1)
{
    dispMaxSDA = arg1;
    ShowImages();
}

void MainWindow::on_comboBoxDispalyMode_currentIndexChanged(int index)
{
    dispalyMode = index;
    ShowImageWithMask();

}

void MainWindow::on_pushButton_clicked()
{
   SDArecalculate();
}

void MainWindow::on_spinBoxSDARadius_valueChanged(int arg1)
{
    sdaRadius = arg1;
}

void MainWindow::on_spinBoxPenSize_valueChanged(int arg1)
{
    penSize = arg1;
}

void MainWindow::on_pushButtonFillHoles_clicked()
{
    FillBorderWithValue(ImROI, 0xFFFF);
    OneRegionFill5Fast1(ImROI,  0xFFFF);
    FillHoles(ImROI);
    DeleteRegionFromImage(ImROI, 0xFFFF);
    ShowImageWithMask();
}

void MainWindow::on_pushButtonSaveROI_clicked()
{
    if(ImIn.empty())
        return;
    if(ImROI.empty())
        return;
    imwrite(FileNameWithPath + "ROI.tiff",ImROI);
    ui->pushButtonSaveROI->setEnabled(false);
}

void MainWindow::on_pushButtonReload_clicked()
{
    if(ImIn.empty())
        return;
    ImROI.release();
    ImROI = imread(FileNameWithPath + "ROI.tiff", CV_LOAD_IMAGE_ANYDEPTH);
    if(ImROI.empty())
        ImROI = Mat::zeros(maxY,maxX,CV_16U);

    ui->pushButtonSaveROI->setEnabled(false);
    ShowImageWithMask();
}

void MainWindow::on_pushButtonClearROI_clicked()
{
    if(ImIn.empty())
        return;
    ImROI.release();
    ImROI = Mat::zeros(maxY,maxX,CV_16U);
    ui->pushButtonSaveROI->setEnabled(true);
    ShowImageWithMask();
}

void MainWindow::on_checkBoxShowPC_toggled(bool checked)
{
    showPC = checked;
    if(!showPC)
        destroyWindow("PC");
    else
        ShowImages();
}

void MainWindow::on_checkBoxShowGray_toggled(bool checked)
{
    showGray = checked;
    if(!showGray)
        destroyWindow("Gray");
    else
        ShowImages();
}

void MainWindow::on_checkBoxShowSDA_toggled(bool checked)
{
    showSDA = checked;
    if(!showSDA)
        destroyWindow("SDA");
    else
        ShowImages();
}
