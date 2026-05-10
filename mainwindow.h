#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatclient.h" // Jana's Network Core

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // This must be in 'private slots' to fix your Error
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    ChatClient *chatClient; // Jana's Networking Object [cite: 186, 205]
};
#endif // MAINWINDOW_H