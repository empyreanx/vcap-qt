// SPDX-License-Identifier: MIT

#ifndef VCAP_HPP
#define VCAP_HPP

#include <cstdint>
#include <vector>

#include <QCheckBox>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QThread>
#include <QWidget>

#include <vcap.h>

#include "BooleanControl.hpp"
#include "ButtonControl.hpp"
#include "ControlWrapper.hpp"
#include "IntegerControl.hpp"
#include "MenuControl.hpp"
#include "IntegerMenuControl.hpp"
#include "Stopwatch.hpp"

#include "Utility.hpp"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
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

public:
    void checkControls();

private:
    void addDevices();

    void addControls();
    void removeControls();
    void updateControls();

    void addFrameSizes();
    void removeFrameSizes();
    void updateFrameSize();

    void addFrameRates();
    void removeFrameRates();
    void updateFrameRate();

    double calcAvgDelta(double delta);
    void displayAvgDelta(double avgDelta);

    void snapshot(uint8_t imageData[], size_t imageSize);

protected:
    void timerEvent(QTimerEvent* event);

private:
    Ui::MainWindow* ui;

    QStatusBar statusBar_;

    vcap_device* vd_;

    bool capturing_;
    int captureTimer_;
    int snapshotTimer_;

    Stopwatch stopwatch_;
    double avgDelta_;

    size_t imageSize_;
    uint8_t* image_;
    vcap_rate frameRate_;
    vcap_size frameSize_;

    std::vector<vcap_device_info> devices_;
    std::vector<std::unique_ptr<ControlWrapper>> controls_;

    void displayImage(int width, int height, unsigned char *data);
};

#endif // VCAP_HPP
