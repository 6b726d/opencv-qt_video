#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

bool open_window = false;
bool play_video = true;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_find_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Open a file", "", "Video File (*.*)*");
    ui->le_video->setText(file_name);
}


void MainWindow::on_pb_open_clicked()
{
    open_window = true;
    play_video = true;

    bool video_is_open = false;

    if (ui->rb_video->isChecked()) {
        cv::VideoCapture video;
        cv::Mat video_frame;
        cv::String video_name = ui->le_video->text().toStdString();
        video.open(video_name);
        try {
            if (video.isOpened() == true) {
                video_is_open = true;
            }
            else {
                throw(505);
            }
        }  catch (...) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Error opening video stream or file.");
            messageBox.setFixedSize(500,200);
        }
        if (video_is_open) {
            cv::String window_name = "Video";
            cv::namedWindow(window_name, cv::WINDOW_NORMAL);

            while (true) {
                if (!open_window) {
                    cv::destroyWindow(window_name);
                    cv::waitKey(1);
                    break;
                }
                if (play_video) {
                    video >> video_frame;
                }
                if (video_frame.empty()) {
                    cv::destroyWindow(window_name);
                    cv::waitKey(1);
                    break;
                }
                else {
                    cv::imshow(window_name, video_frame);
                }
                if (cv::waitKey(10) == 27) {
                    cv::destroyWindow(window_name);
                    cv::waitKey(1);
                    break;
                }
            }
        }
    }
    if (ui->rb_camera->isChecked()) {
        cv::VideoCapture stream;
        int cam_idx = ui->sp_cam_idx->value();
        cv::String video_name = ui->le_stream->text().toStdString();
        stream.open(cam_idx);
        int frame_width = stream.get(cv::CAP_PROP_FRAME_WIDTH);
        int frame_height = stream.get(cv::CAP_PROP_FRAME_HEIGHT);
        int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
        double fps = 10.0;
        cv::VideoWriter writer(video_name, codec, fps, cv::Size(frame_width, frame_height), true);

        try {
            if (stream.isOpened() == true) {
                video_is_open = true;
            }
            else {
                throw(505);
            }
        }  catch (...) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Error opening video stream or file.");
            messageBox.setFixedSize(500,200);
        }
        if (video_is_open) {
            cv::String window_name = "Stream";
            cv::namedWindow(window_name, cv::WINDOW_NORMAL);

            while (true) {
                if (!open_window) {
                    cv::destroyWindow(window_name);
                    cv::waitKey(1);
                    break;
                }
                cv::Mat stream_frame;
                stream >> stream_frame;
                writer.write(stream_frame);
                cv::imshow(window_name, stream_frame);
                if (cv::waitKey(10) == 27) {
                    cv::destroyWindow(window_name);
                    cv::waitKey(1);
                    break;
                }
            }
        }
    }
}

void MainWindow::on_pb_play_clicked()
{
    play_video = !play_video;
}


void MainWindow::on_pb_close_clicked()
{
    open_window = false;
}


void MainWindow::on_pb_quit_clicked()
{
    close();
}

