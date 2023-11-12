#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Player = new QMediaPlayer();
    audioOutput = new QAudioOutput;

    ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->pushButton_SeekBackword->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_SeekForward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Vol_btn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(1);
    ui->horizontalSlider_Volume->setValue(30);
    audioOutput->setVolume(float(0.3));
    Player->setAudioOutput(audioOutput);

    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    ui->horizontalSlider_Duration->setRange(0, Player->duration() / 1000);

    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::uploadFile); // for the texxt btn

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}

void MainWindow::positionChanged(qint64 duration)
{
    if (!ui->horizontalSlider_Duration->isSliderDown()) {
        ui->horizontalSlider_Duration->setValue(duration / 1000);
    }
    updateDuration(duration / 1000);
}

void MainWindow::updateDuration(qint64 Duration)
{
    if (Duration || mDuration){
    QTime CurrentTime((Duration / 3600) % 60, (Duration / 60) % 60, Duration % 60, (Duration * 1000) % 1000);
    QTime TotalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);

    QString Format = "";
    if (mDuration > 3600) {
        Format = "hh:mm:ss";
    }else
    {
        Format = "mm:ss";
    }
    ui->label_Current_Time->setText(CurrentTime.toString(Format));
    ui->label_Total_Time->setText(TotalTime.toString(Format));
    }
}



void MainWindow::on_actionOpen_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Select video file"), "", tr("MP4 Files (*.mp4)"));
    Video = new QVideoWidget();

    Video->setGeometry(5, 5, ui->groupBox->width() -10, ui->groupBox->height()-10);

    Video->setParent(ui->groupBox);

    Player->setVideoOutput(Video);

    Player->setSource(QUrl(FileName));

    Video->setVisible(true);

    Video->show();

}


void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{
    Player->setPosition(value * 1000);
}

void MainWindow::on_pushButton_Play_clicked()
{
    if (IS_Paused == true) {
        IS_Paused = false;
        Player->play();
        ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else {
        IS_Paused = true;
        Player->pause();
        ui->pushButton_Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void MainWindow::on_pushButton_SeekBackword_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);

}

void MainWindow::on_pushButton_SeekForward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}


void MainWindow::on_pushButton_Stop_clicked()
{
    Player->stop();
}


void MainWindow::on_pushButton_Vol_btn_clicked()
{
    if (IS_Muted == false) {
        IS_Muted = true;
        ui->pushButton_Vol_btn->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));

        audioOutput->setMuted(true);

    }else {
        IS_Muted = false;
        ui->pushButton_Vol_btn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        audioOutput->setMuted(false);
    }
}


void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    audioOutput->setVolume(float(value));
}

void MainWindow::uploadFile() // the other half (txt)
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", QString(), "Text Files (*.txt);;All Files (*)");

    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        ui->textEdit->setPlainText(content);
        file.close();
        } else {
        // Handle error opening the file
        }
    }
}
