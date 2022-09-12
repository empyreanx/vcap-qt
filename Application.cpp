#include "Application.hpp"

#include <QMessageBox>

#include <iostream>

Application::Application(int& argc, char* argv[]) :  QApplication(argc, argv)
{
}

bool Application::notify(QObject* receiver, QEvent* event)
{
    try
    {
        return QApplication::notify(receiver, event);
    }
    catch(...)
    {
        exceptionPtr_ = std::current_exception();
        exit();
    }

    return false;
}

int Application::run()
{
    //try
    //{
        int code = exec();

        if (exceptionPtr_)
        {
            std::rethrow_exception(exceptionPtr_);
        }

        return code;
    /*}
    catch (std::exception& e)
    {
        displayError(e.what());
        return 1;
    }
    catch (...)
    {
        displayError("Unknown Exception");
        return 1;
    }*/
}

void Application::displayError(std::string msg)
{
    QMessageBox::critical(0, "Error", QString::fromStdString(msg));
}
