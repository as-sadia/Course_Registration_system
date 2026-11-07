#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

private slots:
    void handleLogin();

private:
    QLineEdit *userIdEdit;
    QLineEdit *passwordEdit;
    QComboBox *roleCombo;
    QPushButton *loginButton;
};

#endif // LOGINDIALOG_H