#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ChatClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSendClicked();
    void onConnected();
    void onDisconnected();
    void onErrorOccurred(const QString &message);
    void onChatMessageReceived(const QString &sender, const QString &text);
    void onRawJsonReceived(const QString &jsonText);

private:
    void appendMessage(const QString &message);
    void updateConnectionStatus(const QString &statusText);

    Ui::MainWindow *ui;
    ChatClient *m_client;
};
#endif // MAINWINDOW_H