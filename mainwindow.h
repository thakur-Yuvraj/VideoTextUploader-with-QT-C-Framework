#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void durationChanged(qint64 duration);

    void positionChanged(qint64 duration);

    void on_actionOpen_triggered();

    void on_horizontalSlider_Duration_valueChanged(int value);

    void on_pushButton_Play_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_Vol_btn_clicked();

    void on_horizontalSlider_Volume_valueChanged(int value);

    void on_pushButton_SeekBackword_clicked();

    void on_pushButton_SeekForward_clicked();

    void uploadFile(); // text uploader

private:
    Ui::MainWindow *ui;
    QMediaPlayer* Player;
    QVideoWidget* Video;
    QAudioOutput* audioOutput;
    qint64 mDuration;
    bool IS_Paused = true;
    bool IS_Muted = false;

    void updateDuration(qint64 Duration);
};
#endif // MAINWINDOW_H
