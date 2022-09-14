// SPDX-License-Identifier: MIT

#include <exception>

#include "Application.hpp"
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    //QApplication a(argc, argv);

    int code = 0;

    try
    {
        MainWindow w;
        w.show();

        code = a.run();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(0, "Error", e.what());
        return EXIT_FAILURE;
    }
    catch (...)
    {
        QMessageBox::critical(0, "Error", "Unknown exception");
        return EXIT_FAILURE;
    }

    return code;
}
