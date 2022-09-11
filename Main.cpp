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
        //code = a.exec();
    }
    catch (const std::exception& e)
    {
        QMessageBox::critical(0, "Error", e.what());
        return EXIT_FAILURE;
    }

    return code;
}
