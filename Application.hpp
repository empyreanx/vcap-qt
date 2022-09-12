#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QApplication>
#include <exception>

class Application : public QApplication
{
public:
    Application(int& argc, char* argv[]);

    bool notify(QObject* receiver, QEvent* event);
    int run();

    void displayError(std::string msg);

private:
    std::exception_ptr exceptionPtr_ = nullptr;
};

#endif // APPLICATION_HPP
