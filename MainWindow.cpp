/*
    Copyright (c) 2018 James McLean

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <vcap/vcap.h>

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    capturing_ = false;

    int index = 0;
    int result;

    vcap_device device;

    do {
        result = vcap_enum_devices(&device, index);

        if (result == VCAP_ENUM_ERROR) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        if (result == VCAP_ENUM_OK) {
            devices_.push_back(device);
        }

    } while (result != VCAP_ENUM_INVALID && ++index);

    if (devices_.size() == 0) {
        QMessageBox::critical(this, tr("Error"), tr("No cameras found!"));
        QApplication::quit();
    }

    for (unsigned i = 0; i < devices_.size(); i++) {
        ui->cameraComboBox->addItem(devices_[i].path);
    }

    device_ = devices_[0];

    connect(ui->actionStartCapture, SIGNAL(triggered(bool)), this, SLOT(startCapture()));
    connect(ui->actionStopCapture, SIGNAL(triggered(bool)), this, SLOT(stopCapture()));
    connect(ui->actionImportSettings, SIGNAL(triggered(bool)), this, SLOT(importSettings()));
    connect(ui->actionExportSettings, SIGNAL(triggered(bool)), this, SLOT(exportSettings()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(quit()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetControls()));
    connect(ui->cameraComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchCamera(QString)));
    connect(ui->sizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchSize(QString)));
    connect(ui->frameRateComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchRate(QString)));
    connect(ui->snapshotButton, SIGNAL(clicked()), this, SLOT(snapshot()));
}

MainWindow::~MainWindow() {
    if (capturing_)
        stopCapture();

    delete ui;
}

void MainWindow::startCapture() {
    if (!capturing_) {
        fg_ = vcap_open(&device_);

        if (!fg_) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        vcap_size_itr* itr = vcap_new_size_itr(fg_, VCAP_FMT_RGB24);

        if (!vcap_size_itr_next(itr, &frameSize_)) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to get initial frame size"));
            QApplication::quit();
        }

        vcap_free(itr);

        if (vcap_set_fmt(fg_, VCAP_FMT_RGB24, frameSize_) == -1) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        frame_ = vcap_alloc_frame(fg_);
        clone_ = vcap_alloc_frame(fg_);

        if (!frame_) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        addControls();
        addFrameSizes();
        addFrameRates();

        captureTimer_ = startTimer(0);
        capturing_ = true;
    }
}

void MainWindow::stopCapture() {
    if (capturing_) {
        capturing_ = false;
        killTimer(captureTimer_);

        vcap_free_frame(clone_);
        vcap_free_frame(frame_);
        vcap_close(fg_);

        removeControls();
        removeFrameSizes();
        removeFrameRates();
    }
}

void MainWindow::importSettings() {
    if (capturing_) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Import Settings",
                                                        QDir::currentPath(),
                                                        "JSON (*.json)",
                                                        nullptr,
                                                        QFileDialog::DontUseNativeDialog);
        if (!fileName.isNull()) {
            if (vcap_import_settings(fg_, fileName.toLatin1().data()) == -1) {
                QMessageBox::warning(this, tr("Error"), vcap_get_error());
            } else {
                updateControls();
                updateFrameSize();
                updateFrameRate();
                checkControls();
            }
        }
    }
}

void MainWindow::exportSettings() {
    if (capturing_) {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        "Export Settings",
                                                        QDir::currentPath() + "/settings.json",
                                                        "JSON (*.json)",
                                                        nullptr,
                                                        QFileDialog::DontUseNativeDialog);

        if (!fileName.isNull()) {
            if (vcap_export_settings(fg_, fileName.toLatin1().data()) == -1) {
                QMessageBox::warning(this, tr("Error"), vcap_get_error());
            }
        }
    }
}

void MainWindow::quit() {
    stopCapture();
    QCoreApplication::quit();
}

void MainWindow::resetControls() {
    if (vcap_reset_all_ctrls(fg_) == -1) {
        QMessageBox::warning(this, tr("Error"), vcap_get_error());
    }

    updateControls();
    checkControls();
}

void MainWindow::snapshot() {
    if (capturing_)
        snapshotTimer_ = startTimer(ui->delaySpinBox->value() * 1000);
}

void MainWindow::timerEvent(QTimerEvent* event) {
    if (capturing_) {
        if (vcap_grab(fg_, frame_) == -1) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        if (event->timerId() == snapshotTimer_) {
            killTimer(snapshotTimer_);

            vcap_copy_frame(clone_, frame_);

            QString fileName = QFileDialog::getSaveFileName(this,
                                                            "Save Image",
                                                            QDir::currentPath(),
                                                            "Images (*.png *.jpg)",
                                                            nullptr,
                                                            QFileDialog::DontUseNativeDialog);

            if (!fileName.isNull()) {
                if (ui->formatComboBox->currentText() == "PNG") {
                    if (vcap_save_png(clone_, fileName.toLatin1().data()) == -1) {
                        QMessageBox::warning(this, tr("Error"), QString("Unable to save PNG: ") + vcap_get_error());
                    }
                } else {
                    if (vcap_save_jpeg(clone_, fileName.toLatin1().data()) == -1) {
                        QMessageBox::warning(this, tr("Error"), QString("Unable to save JPEG: ") + vcap_get_error());
                    }
                }
            }
        } else {
            displayImage(static_cast<int>(frame_->size.width), static_cast<int>(frame_->size.height), frame_->data);
        }
    }
}

void MainWindow::switchCamera(const QString &device) {
    bool wasCapturing = capturing_;

        if (capturing_)
            stopCapture();

    for (unsigned i = 0; i < devices_.size(); i++) {
        if (devices_[i].path == device) {
            device_ = devices_[i];
            break;
        }
    }

    if (wasCapturing)
        startCapture();
}

void MainWindow::switchSize(const QString &sizeStr) {
    if (capturing_) {
        capturing_ = false;
        killTimer(captureTimer_);

        QStringList parts = sizeStr.split("x");

        uint32_t width = parts[0].toUInt();
        uint32_t height = parts[1].toUInt();

        vcap_free_frame(frame_);

        vcap_size size = { width, height };

        if (vcap_set_fmt(fg_, VCAP_FMT_RGB24, size) == -1) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        frameSize_ = size;
        frame_ = vcap_alloc_frame(fg_);

        removeFrameRates();
        addFrameRates();

        captureTimer_ = startTimer(0);
        capturing_ = true;
    }
}

void MainWindow::switchRate(const QString &rateStr) {
    if (capturing_) {
        capturing_ = false;
        killTimer(captureTimer_);

        QStringList parts = rateStr.split("/");

        uint32_t numerator = parts[0].toUInt();
        uint32_t denominator = parts[1].toUInt();

        vcap_rate rate = { numerator, denominator };

        if (vcap_set_rate(fg_, rate) == -1) {
            QMessageBox::critical(this, tr("Error"), vcap_get_error());
            QApplication::quit();
        }

        frameRate_ = rate;

        captureTimer_ = startTimer(0);
        capturing_ = true;
    }
}

void MainWindow::addControls() {
    vcap_ctrl_desc desc;
    vcap_ctrl_itr* itr = vcap_new_ctrl_itr(fg_);

    while (vcap_ctrl_itr_next(itr, &desc)) {
        switch (desc.type) {
        case VCAP_CTRL_TYPE_BOOLEAN:
            controls_.emplace_back(new BooleanControl(fg_, desc));
            break;

        case VCAP_CTRL_TYPE_INTEGER:
            controls_.emplace_back(new IntegerControl(fg_, desc));
            break;

        case VCAP_CTRL_TYPE_MENU:
            controls_.emplace_back(new MenuControl(fg_, desc));
            break;

        case VCAP_CTRL_TYPE_BUTTON:
            controls_.emplace_back(new ButtonControl(fg_, desc));
            break;

        case VCAP_CTRL_TYPE_INTEGER_MENU:
            controls_.emplace_back(new IntegerMenuControl(fg_, desc));
            break;

        default:
            continue;
        }

        ui->controlsForm->addRow(reinterpret_cast<char*>(desc.name), controls_.back()->widget());
        connect(controls_.back().get(), SIGNAL(changed()), this, SLOT(controlChanged()));
    }

    if (vcap_ctrl_itr_error(itr))
        QMessageBox::warning(this, tr("Error"), vcap_get_error());

    vcap_free(itr);

    for (unsigned i = 0; i < controls_.size(); i++) {
        controls_[i]->check();
    }
}

void MainWindow::controlChanged() {
    checkControls();
}

void MainWindow::removeControls() {
    controls_.resize(0);

    QLayoutItem* item = nullptr;

    // WARNING: The following constant may change if additional widgets are added to controlsForm
    while ((item = ui->controlsForm->itemAt(14)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
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

void MainWindow::addFrameSizes() {
    vcap_size size;
    vcap_size_itr* itr = vcap_new_size_itr(fg_, VCAP_FMT_RGB24);

    while (vcap_size_itr_next(itr, &size)) {
        QString sizeStr = QString::number(size.width) + "x" + QString::number(size.height);
        ui->sizeComboBox->addItem(sizeStr);
    }

    if (vcap_size_itr_error(itr))
        QMessageBox::warning(this, tr("Error"), vcap_get_error());

    vcap_free(itr);

    updateFrameSize();
}

void MainWindow::removeFrameSizes() {
    while (ui->sizeComboBox->count() > 0) {
        ui->sizeComboBox->removeItem(0);
    }
}

void MainWindow::updateFrameSize() {
    vcap_fmt_id id;
    vcap_get_fmt(fg_, &id, &frameSize_);

    QString sizeStr = QString::number(frameSize_.width) + "x" + QString::number(frameSize_.height);

    for (int i = 0; i < ui->sizeComboBox->count(); i++) {
        if (ui->sizeComboBox->itemText(i) == sizeStr) {
            ui->sizeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::addFrameRates() {
    vcap_rate rate;
    vcap_rate_itr* itr = vcap_new_rate_itr(fg_, VCAP_FMT_RGB24, frameSize_);

    while (vcap_rate_itr_next(itr, &rate)) {
        ui->frameRateComboBox->addItem(QString::number(rate.numerator) + "/" + QString::number(rate.denominator));
    }

    if (vcap_rate_itr_error(itr))
        QMessageBox::warning(this, tr("Error"), vcap_get_error());

    vcap_free(itr);

    updateFrameRate();
}

void MainWindow::removeFrameRates() {
    while (ui->frameRateComboBox->count() > 0) {
        ui->frameRateComboBox->removeItem(0);
    }
}

void MainWindow::updateFrameRate() {
    vcap_get_rate(fg_, &frameRate_);
    QString frameRateStr = QString::number(frameRate_.numerator) + "/" + QString::number(frameRate_.denominator);

    for (int i = 0; i < ui->sizeComboBox->count(); i++) {
        if (ui->sizeComboBox->itemText(i) == frameRateStr) {
            ui->sizeComboBox->setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::displayImage(int width, int height, std::uint8_t* data) {
    QImage qImage = colorToQImage(width, height, data);
    ui->videoImage->setPixmap(QPixmap::fromImage(qImage));
}
