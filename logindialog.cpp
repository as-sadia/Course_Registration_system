#include "logindialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("COURSE REGISTRATION SYSTEM - LOGIN");
    this->setFixedSize(450, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(15);

    QLabel *titleLabel = new QLabel("<h2><b>COURSE REGISTRATION SYSTEM</b></h2>", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *userLabel = new QLabel("User ID", this);
    userIdEdit = new QLineEdit(this);
    userIdEdit->setPlaceholderText("Enter User ID (e.g. 1001)");

    QLabel *passLabel = new QLabel("Password", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Enter Password");

    QLabel *roleLabel = new QLabel("Role", this);
    roleCombo = new QComboBox(this);
    roleCombo->addItem("Student");
    roleCombo->addItem("Admin / Teacher");

    loginButton = new QPushButton("LOGIN", this);
    loginButton->setFixedHeight(35);
    loginButton->setStyleSheet("background-color: #f5f5f5; border: 1px solid #ccc; font-weight: bold;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(userLabel);
    mainLayout->addWidget(userIdEdit);
    mainLayout->addWidget(passLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(roleLabel);
    mainLayout->addWidget(roleCombo);
    mainLayout->addWidget(loginButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
}

void LoginDialog::handleLogin()
{
    QString userId = userIdEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (userId.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Failed", "Please enter both User ID and Password.");
        return;
    }

    accept();
}