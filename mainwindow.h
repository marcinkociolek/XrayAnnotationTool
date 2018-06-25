#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    std::string FileNameWithPath;
    std::string DirectoryPath;
    cv::Mat ImIn;
    cv::Mat ImSDA;
    cv::Mat ImROI;
    cv::Mat ImShowPC;
    cv::Mat ImShowGray;
    cv::Mat ImShowSDA;
    int maxX;
    int maxY;

    int linePrevX;
    int linePrevY;

    cv::Point boxCorner1;
    cv::Point boxCorner2;


    int dispalyMode;
    int editMode;

    int imageShowScale;
    int sdaRadius;

    bool showPC;
    bool showGray;
    bool showSDA;

    double dispMinPC;
    double dispMaxPC;

    double dispMinGray;
    double dispMaxGray;

    double dispMinSDA;
    double dispMaxSDA;

    int penSize;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void MainWindow::OpenImage();
    void MainWindow::SDArecalculate();
    void MainWindow::ShowImages();
    void MainWindow::ShowImageWithMask();
    void MainWindow::ScaleImages();
    void MainWindow::PlaceShapeOnImage(cv::Mat Im, int x, int y, int size,unsigned short val);


private slots:
    //void on_widgetImage_mousePressed(QPoint point, int butPressed);

    void on_widgetImage_mouseMoved(QPoint point, int butPressed);

    //void on_widgetImage_KeyPressed(int key);

    void on_pushButtonOpenFile_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBoxScale_valueChanged(int arg1);

    void on_doubleSpinBoxPCMin_valueChanged(double arg1);

    void on_doubleSpinBoxPCMax_valueChanged(double arg1);

    void on_doubleSpinBoxGrayMin_valueChanged(double arg1);

    void on_doubleSpinBoxGrayMax_valueChanged(double arg1);

    void on_doubleSpinBoxSDAMin_valueChanged(double arg1);

    void on_doubleSpinBoxSDAMax_valueChanged(double arg1);

    void on_comboBoxDispalyMode_currentIndexChanged(int index);

    void on_pushButton_clicked();

    void on_spinBoxSDARadius_valueChanged(int arg1);

    void on_spinBoxPenSize_valueChanged(int arg1);

    void on_pushButtonFillHoles_clicked();

    void on_pushButtonSaveROI_clicked();

    void on_pushButtonReload_clicked();

    void on_pushButtonClearROI_clicked();

    void on_checkBoxShowPC_toggled(bool checked);

    void on_checkBoxShowGray_toggled(bool checked);

    void on_checkBoxShowSDA_toggled(bool checked);

    void on_comboBoxEditMode_currentIndexChanged(int index);

    void on_pushButtonSaveCropped_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
