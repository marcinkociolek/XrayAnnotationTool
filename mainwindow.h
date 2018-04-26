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
    cv::Mat ImIn;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonOpenFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
