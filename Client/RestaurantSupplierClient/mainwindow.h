#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin();

    void on_loginButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;

    QTimer *connectionTimer;

    void sendLoginRequest(const QString &username, const QString &password);
    void handleResponse(const QJsonObject &response);
    void updateConnectionStatus(bool connected);
};

#endif
