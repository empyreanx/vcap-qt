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

#ifndef VCAP_HPP
#define VCAP_HPP

#include <cstdint>
#include <vector>

#include <QCheckBox>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QThread>
#include <QWidget>

#include <vcap/vcap.h>

#include "BooleanControl.hpp"
#include "ButtonControl.hpp"
#include "ControlWrapper.hpp"
#include "IntegerControl.hpp"
#include "IntegerMenuControl.hpp"
#include "MenuControl.hpp"

#include "Utils.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void controlChanged();
    void startCapture();
    void stopCapture();
    void importSettings();
    void exportSettings();
    void quit();

    void resetControls();
    void switchCamera(const QString &device);
    void switchSize(const QString &sizeStr);
    void switchRate(const QString &rateStr);

    void snapshot();

private:
    void addControls();
    void removeControls();
    void checkControls();
    void updateControls();

    void addFrameSizes();
    void removeFrameSizes();
    void updateFrameSize();

    void addFrameRates();
    void removeFrameRates();
    void updateFrameRate();

protected:
    void timerEvent(QTimerEvent* event);

private:
    Ui::MainWindow* ui;

    int captureTimer_;
    int snapshotTimer_;

    bool capturing_;

    vcap_fg* fg_;
    vcap_frame* frame_;
    vcap_frame* clone_;
    vcap_rate frameRate_;
    vcap_size frameSize_;

    std::vector<vcap_fg> devices_;
    std::vector<std::unique_ptr<ControlWrapper>> controls_;

    void displayImage(int width, int height, unsigned char *data);
};

#endif // VCAP_HPP
