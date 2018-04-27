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

    int imageShowScale;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_widgetImage_mousePressed(QPoint point);

    void on_widgetImage_mouseMoved(QPoint point, int butPressed);

    void on_widgetImage_KeyPressed(int key);

    void on_pushButtonOpenFile_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
