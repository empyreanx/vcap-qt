// SPDX-License-Identifier: MIT

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <vcap.h>

#include "Iterator.hpp"

#include <QFileDialog>
#include <QTimerEvent>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define VCAP_SETTINGS_IMPLEMENTATION
#include "vcap_settings.h"

#include <fstream>
#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    vd_(nullptr), capturing_(false), captureTimer_(0), snapshotTimer_(0), avgDelta_(0.0)
{
    ui->setupUi(this);

    addDevices();

    uint32_t buffer_count = devices_[0].streaming ? 3 : 0;
    vd_ = vcap_create_device(devices_[0].path, true, buffer_count);

    if (!vd_)
        throw std::runtime_error( "Unable to create video device");

    connect(ui->actionStartCapture, SIGNAL(triggered()), this, SLOT(startCapture()));
    connect(ui->actionStopCapture, SIGNAL(triggered()), this, SLOT(stopCapture()));
    connect(ui->actionImportSettings, SIGNAL(triggered()), this, SLOT(importSettings()));
    connect(ui->actionExportSettings, SIGNAL(triggered()), this, SLOT(exportSettings()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetControls()));
    connect(ui->cameraComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchCamera(QString)));
    connect(ui->sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchSize(QString)));
    connect(ui->frameRateComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchRate(QString)));
    connect(ui->snapshotButton, SIGNAL(clicked()), this, SLOT(snapshot()));

    setStatusBar(&statusBar_);
}

MainWindow::~MainWindow()
{
    if (capturing_)
        stopCapture();

    if (vd_)
        vcap_destroy_device(vd_);

    delete ui;
}

void MainWindow::startCapture() {
    if (!capturing_)
    {
        avgDelta_ = 0.0;
        stopwatch_.reset();

        if (vcap_open(vd_) == VCAP_ERROR)
            throw std::runtime_error(vcap_get_error(vd_));

        IteratorPtr itr(vcap_size_iterator(vd_, VCAP_FMT_RGB24));

        if (!vcap_next_size(itr.get(), &frameSize_))
            throw std::runtime_error(std::string("Unable to get initial size: ") + vcap_get_error(vd_));

        if (vcap_set_format(vd_, VCAP_FMT_RGB24, frameSize_) != VCAP_OK)
            throw std::runtime_error(vcap_get_error(vd_));

        imageSize_ = vcap_get_image_size(vd_);
        image_ = new uint8_t[imageSize_];

        if (!image_)
            throw std::bad_alloc();

        addControls();
        addFrameSizes();
        addFrameRates();

        if (vcap_start_stream(vd_)  == VCAP_ERROR)
            throw std::runtime_error(vcap_get_error(vd_));

        captureTimer_ = startTimer(0);
        capturing_ = true;
    }
}

void MainWindow::stopCapture()
{
    if (capturing_) {
        capturing_ = false;

        if (captureTimer_ != 0)
            killTimer(captureTimer_);

        delete [] image_;
        vcap_close(vd_);

        removeControls();
        removeFrameSizes();
        removeFrameRates();
    }
}

void MainWindow::importSettings()
{
    if (capturing_)
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Import Settings",
                                                        QDir::currentPath(),
                                                        "JSON (*.json)",
                                                        nullptr,
                                                        QFileDialog::DontUseNativeDialog);
        if (!fileName.isNull())
        {
            std::ifstream fileIn(fileName.toStdString());
            std::string json_str;

            if (fileIn)
            {
                std::ostringstream sstream;
                sstream << fileIn.rdbuf();
                json_str = sstream.str();
                fileIn.close();

                if (vcap_import_settings(vd_, json_str.c_str()) == VCAP_ERROR)
                    throw std::runtime_error(vcap_get_error(vd_));

                updateControls();
                updateFrameSize();
                updateFrameRate();
                checkControls();
            }
        }
    }
}

void MainWindow::exportSettings()
{
    if (capturing_)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        "Export Settings",
                                                        QDir::currentPath() + "/settings.json",
                                                        "JSON (*.json)",
                                                        nullptr,
                                                        QFileDialog::DontUseNativeDialog);

        if (!fileName.isNull())
        {
            char* json_str = nullptr;

            if (vcap_export_settings(vd_, &json_str) == VCAP_ERROR)
                throw std::runtime_error(vcap_get_error(vd_));

            std::ofstream fileOut(fileName.toStdString());
            fileOut << json_str;
            fileOut.close();

            free(json_str);
        }
    }
}

void MainWindow::quit() {
    stopCapture();
    QCoreApplication::quit();
}

void MainWindow::resetControls()
{
    if (vcap_reset_all_controls(vd_) == VCAP_ERROR)
        throw std::runtime_error(vcap_get_error(vd_));

    //checkControls();
    updateControls();
    checkControls();
}

void MainWindow::snapshot() {
    if (capturing_)
        snapshotTimer_ = startTimer(ui->delaySpinBox->value() * 1000);
}

void MainWindow::timerEvent(QTimerEvent* event)
{
    if (capturing_)
    {
        double delta = stopwatch_.stop();
        double avgDelta = calcAvgDelta(delta);
        stopwatch_.start();

        displayAvgDelta(avgDelta);

        if (vcap_capture(vd_, imageSize_, image_) == VCAP_ERROR)
        {
            killTimer(captureTimer_);
            captureTimer_ = 0;
            throw std::runtime_error(vcap_get_error(vd_));
        }

        displayImage(static_cast<int>(frameSize_.width), static_cast<int>(frameSize_.height), image_);

        if (event->timerId() == snapshotTimer_)
        {
            killTimer(snapshotTimer_);
            snapshotTimer_ = 0;
            snapshot(image_, imageSize_);
        }
    }
}

void MainWindow::switchCamera(const QString &device_path)
{
    bool wasCapturing = capturing_;

    if (capturing_)
        stopCapture();

    for (unsigned i = 0; i < devices_.size(); i++)
    {
        if (devices_[i].path == device_path)
        {
            if (vd_)
                vcap_destroy_device(vd_);

            uint32_t buffer_count = devices_[i].streaming ? 3 : 0;

            vd_ = vcap_create_device(devices_[i].path, true, buffer_count);

            if (!vd_)
                throw std::runtime_error( "Unable to create video device");

            break;
        }
    }

    if (wasCapturing)
        startCapture();
}

void MainWindow::switchSize(const QString &sizeStr)
{
    if (capturing_)
    {
        capturing_ = false;
        killTimer(captureTimer_);

        QStringList parts = sizeStr.split("x");

        uint32_t width = parts[0].toUInt();
        uint32_t height = parts[1].toUInt();
        vcap_size size = { width, height };

        if (vcap_set_format(vd_, VCAP_FMT_RGB24, size) != VCAP_OK)
            throw std::runtime_error(vcap_get_error(vd_));

        frameSize_ = size;

        delete [] image_;
        imageSize_ = vcap_get_image_size(vd_);
        image_ = new uint8_t[imageSize_];

        removeFrameRates();
        addFrameRates();

        captureTimer_ = startTimer(0);
        capturing_ = true;
    }
}

void MainWindow::switchRate(const QString &rateStr)
{
    if (capturing_)
    {
        capturing_ = false;
        killTimer(captureTimer_);

        QStringList parts = rateStr.split("/");

        uint32_t numerator = parts[0].toUInt();
        uint32_t denominator = parts[1].toUInt();

        vcap_rate rate = { numerator, denominator };

        if (vcap_set_rate(vd_, rate) != VCAP_OK)
            throw std::runtime_error(vcap_get_error(vd_));

        frameRate_ = rate;

        captureTimer_ = startTimer(0);
        capturing_ = true;
    }
}

void MainWindow::addDevices()
{
    int result, index = 0;

    do
    {
        vcap_device_info info = {};

        result = vcap_enumerate_devices(index, &info);

        if (result == VCAP_OK)
            devices_.push_back(info);
    }
    while (result != VCAP_INVALID && ++index);

    if (devices_.size() == 0)
        throw std::runtime_error("No cameras found");

    for (unsigned i = 0; i < devices_.size(); i++)
        ui->cameraComboBox->addItem(devices_[i].path);
}

void MainWindow::addControls()
{
    vcap_control_info info;
    IteratorPtr itr(vcap_control_iterator(vd_));

    while (vcap_next_control(itr.get(), &info))
    {
        switch (info.type)
        {
            case VCAP_CTRL_TYPE_BOOLEAN:
                controls_.emplace_back(new BooleanControl(vd_, info));
                break;

            case VCAP_CTRL_TYPE_INTEGER:
                controls_.emplace_back(new IntegerControl(vd_, info));
                break;

            case VCAP_CTRL_TYPE_MENU:
                controls_.emplace_back(new MenuControl(vd_, info));
                break;

            case VCAP_CTRL_TYPE_BUTTON:
                controls_.emplace_back(new ButtonControl(vd_, info));
                break;

            case VCAP_CTRL_TYPE_INTEGER_MENU:
                controls_.emplace_back(new IntegerMenuControl(vd_, info));
                break;

            default:
                continue;
        }

        ui->controlsForm->addRow(reinterpret_cast<char*>(info.name), controls_.back()->widget());
        connect(controls_.back().get(), SIGNAL(changed()), this, SLOT(controlChanged()));
    }

    if (vcap_iterator_error(itr.get()))
        throw std::runtime_error(vcap_get_error(vd_));

    checkControls();
}

void MainWindow::controlChanged()
{
    //updateControls();
    checkControls();
}

void MainWindow::removeControls()
{
    // WARNING: The following constant may change if additional widgets are added to controlsForm
    const static int lastIndex = 14;

    controls_.resize(0);

    QLayoutItem* item = nullptr;

    while ((item = ui->controlsForm->itemAt(lastIndex)) != nullptr)
    {
        if (item->widget())
            delete item->widget();
    }
}

void MainWindow::checkControls() {
    for (unsigned i = 0; i < controls_.size(); i++) {
        controls_[i]->check();
    }
}

void MainWindow::updateControls() {
    for (unsigned i = 0; i < controls_.size(); i++) {
        controls_[i]->update();
    }
}

void MainWindow::addFrameSizes()
{
    vcap_size size;
    IteratorPtr itr(vcap_size_iterator(vd_, VCAP_FMT_RGB24));

    while (vcap_next_size(itr.get(), &size))
    {
        QString sizeStr = QString::number(size.width) + "x" + QString::number(size.height);
        ui->sizeComboBox->addItem(sizeStr);
    }

    if (vcap_iterator_error(itr.get()))
        throw std::runtime_error(vcap_get_error(vd_));

    updateFrameSize();
}

void MainWindow::removeFrameSizes()
{
    while (ui->sizeComboBox->count() > 0)
    {
        ui->sizeComboBox->removeItem(0);
    }
}

void MainWindow::updateFrameSize()
{
    vcap_format_id id;

    if (vcap_get_format(vd_, &id, &frameSize_) != VCAP_OK)
        throw std::runtime_error(vcap_get_error(vd_));

    QString sizeStr = QString::number(frameSize_.width) + "x" + QString::number(frameSize_.height);

    for (int i = 0; i < ui->sizeComboBox->count(); i++)
    {
        if (ui->sizeComboBox->itemText(i) == sizeStr)
        {
            ui->sizeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::addFrameRates()
{
    vcap_rate rate;
    IteratorPtr itr(vcap_rate_iterator(vd_, VCAP_FMT_RGB24, frameSize_));

    while (vcap_next_rate(itr.get(), &rate))
        ui->frameRateComboBox->addItem(QString::number(rate.numerator) + "/" + QString::number(rate.denominator));

    if (vcap_iterator_error(itr.get()))
        throw std::runtime_error(vcap_get_error(vd_));

    updateFrameRate();
}

void MainWindow::removeFrameRates()
{
    while (ui->frameRateComboBox->count() > 0)
        ui->frameRateComboBox->removeItem(0);
}

void MainWindow::updateFrameRate()
{
    if (vcap_get_rate(vd_, &frameRate_))
        throw std::runtime_error(vcap_get_error(vd_));

    QString frameRateStr = QString::number(frameRate_.numerator) + "/" + QString::number(frameRate_.denominator);

    for (int i = 0; i < ui->sizeComboBox->count(); i++)
    {
        if (ui->sizeComboBox->itemText(i) == frameRateStr)
        {
            ui->sizeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

double MainWindow::calcAvgDelta(double delta)
{
    avgDelta_ = 0.25 * delta + 0.75 * avgDelta_;
    return avgDelta_;
}

void MainWindow::displayAvgDelta(double avgDelta)
{
    if (avgDelta > 0.0)
        statusBar_.showMessage(QString("FPS: ") + QString::number(1.0 / avgDelta));
    else
        statusBar_.showMessage("FPS: 0.0");
}

void MainWindow::snapshot(uint8_t imageData[], size_t imageSize)
{

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save Image",
                                                    QDir::currentPath(),
                                                    "Images (*.png *.jpg)",
                                                    nullptr,
                                                    QFileDialog::DontUseNativeDialog);

    if (!fileName.isNull())
    {
        if (ui->formatComboBox->currentText() == "PNG")
        {

            if (0 == stbi_write_png(fileName.toStdString().c_str(), frameSize_.width, frameSize_.height, 3, imageData, 3 * frameSize_.width))
                throw std::runtime_error("Unable to save PNG");
        }
        else
        {
            if (0 == stbi_write_jpg(fileName.toStdString().c_str(), frameSize_.width, frameSize_.height, 3, imageData, 50))
                throw std::runtime_error("Unable to save JPG");
        }
    }
}


void MainWindow::displayImage(int width, int height, std::uint8_t* data)
{
    QImage qImage = colorToQImage(width, height, data);
    ui->videoImage->setPixmap(QPixmap::fromImage(qImage));
}
