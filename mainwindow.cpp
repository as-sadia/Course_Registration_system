#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QFormLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("STUDENT PORTAL - COURSE REGISTRATION (CRUD Enabled)");
    this->resize(1000, 600);

    setupUI();
    loadCoursesFromFile(); // READ from .txt
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Header Section
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("<h2><b>STUDENT PORTAL - COURSE REGISTRATION</b></h2>", this);
    QPushButton *logoutBtn = new QPushButton("Logout", this);
    logoutBtn->setFixedWidth(80);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(logoutBtn);

    // Student Information Section
    QHBoxLayout *infoLayout = new QHBoxLayout();
    QLabel *studentInfo = new QLabel("<b>Name:</b> 1001 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>Semester:</b> Autumn-2026 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <b>Department:</b> CSE", this);
    infoLayout->addWidget(studentInfo);

    // CRUD Control Buttons (Create & Delete)
    QHBoxLayout *crudLayout = new QHBoxLayout();
    QPushButton *addCourseBtn = new QPushButton("+ Add New Course (Create)", this);
    QPushButton *deleteCourseBtn = new QPushButton("- Delete Selected Course (Delete)", this);
    addCourseBtn->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold; padding: 6px 12px;");
    deleteCourseBtn->setStyleSheet("background-color: #c0392b; color: white; font-weight: bold; padding: 6px 12px;");

    crudLayout->addWidget(addCourseBtn);
    crudLayout->addWidget(deleteCourseBtn);
    crudLayout->addStretch();

    // Course Table Setup
    courseTable = new QTableWidget(this);
    courseTable->setColumnCount(8);
    QStringList headers = {"Select", "Course Code", "Course Name", "Credit Hours", "Section", "Sub Section", "Status", "Remarks"};
    courseTable->setHorizontalHeaderLabels(headers);
    courseTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    courseTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    courseTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    courseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Footer Layout
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    totalCreditLabel = new QLabel("<b>Total Credit Selected: 0.0</b>", this);
    QPushButton *submitBtn = new QPushButton("Save & Submit (Update .txt)", this);
    submitBtn->setStyleSheet("background-color: #2b579a; color: white; font-weight: bold; padding: 8px 15px;");

    bottomLayout->addWidget(totalCreditLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(submitBtn);

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(crudLayout);
    mainLayout->addWidget(courseTable);
    mainLayout->addLayout(bottomLayout);

    this->setCentralWidget(centralWidget);

    connect(addCourseBtn, &QPushButton::clicked, this, &MainWindow::addCourse);
    connect(deleteCourseBtn, &QPushButton::clicked, this, &MainWindow::deleteCourse);
    connect(submitBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(logoutBtn, &QPushButton::clicked, this, &QMainWindow::close);
}

// 1. READ Operation: Load Data from courses.txt
void MainWindow::loadCoursesFromFile()
{
    courseList.clear();
    QFile file("courses.txt");

    if (!file.exists()) {
        // Default initial data if file doesn't exist yet
        courseList = {
            {"CSE-1221", "Computer Programming II", 3.0, "Female", "A", "Regular", "Selected"},
            {"CSE-1222", "Computer Programming II Lab", 1.5, "Female", "A", "Regular", "Selected"},
            {"CSE-1223", "Discrete Mathematics", 3.0, "Female", "A", "Regular", "Selected"},
            {"CSE-1230", "Competitive Programming 1", 1.0, "Female", "A", "Regular", "Selected"},
            {"EEE-1221", "Electronics", 3.0, "Female", "A", "Regular", "Selected"},
            {"EEE-1222", "Electronics Lab", 1.5, "Female", "A", "Regular", "Selected"},
            {"MATH-1207", "Mathematics II", 3.0, "Female", "A", "Regular", "Selected"},
            {"PHY-1201", "Physics II", 3.0, "Female", "A", "Regular", "Not Selected"},
            {"GEED-1201", "Basic Principles of Islam", 2.0, "Female", "A", "Regular", "Not Selected"}
        };
        saveCoursesToFile();
    } else {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                QStringList parts = line.split("|");
                if (parts.size() == 7) {
                    PortalCourse c;
                    c.code = parts[0];
                    c.name = parts[1];
                    c.credit = parts[2].toDouble();
                    c.section = parts[3];
                    c.subSection = parts[4];
                    c.status = parts[5];
                    c.remarks = parts[6];
                    courseList.append(c);
                }
            }
            file.close();
        }
    }

    courseTable->setRowCount(0);
    courseTable->setRowCount(courseList.size());

    for (int i = 0; i < courseList.size(); ++i) {
        QTableWidgetItem *checkBoxItem = new QTableWidgetItem();
        checkBoxItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        checkBoxItem->setCheckState(courseList[i].remarks == "Selected" ? Qt::Checked : Qt::Unchecked);
        courseTable->setItem(i, 0, checkBoxItem);

        courseTable->setItem(i, 1, new QTableWidgetItem(courseList[i].code));
        courseTable->setItem(i, 2, new QTableWidgetItem(courseList[i].name));
        courseTable->setItem(i, 3, new QTableWidgetItem(QString::number(courseList[i].credit)));
        courseTable->setItem(i, 4, new QTableWidgetItem(courseList[i].section));
        courseTable->setItem(i, 5, new QTableWidgetItem(courseList[i].subSection));
        courseTable->setItem(i, 6, new QTableWidgetItem(courseList[i].status));
        courseTable->setItem(i, 7, new QTableWidgetItem(courseList[i].remarks));
    }

    updateCredits();
    connect(courseTable, &QTableWidget::itemChanged, this, &MainWindow::updateCredits);
}

// 2. UPDATE / SAVE Operation: Save Data to courses.txt
void MainWindow::saveCoursesToFile()
{
    QFile file("courses.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < courseTable->rowCount(); ++i) {
            QString code = courseTable->item(i, 1)->text();
            QString name = courseTable->item(i, 2)->text();
            QString credit = courseTable->item(i, 3)->text();
            QString section = courseTable->item(i, 4)->text();
            QString subSection = courseTable->item(i, 5)->text();
            QString status = courseTable->item(i, 6)->text();
            QString remarks = courseTable->item(i, 7)->text();

            out << code << "|" << name << "|" << credit << "|" << section << "|"
                << subSection << "|" << status << "|" << remarks << "\n";
        }
        file.close();
    }
}

// 3. CREATE Operation: Add new course dynamically
void MainWindow::addCourse()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Course");

    QFormLayout form(&dialog);
    QLineEdit codeEdit(&dialog);
    QLineEdit nameEdit(&dialog);
    QDoubleSpinBox creditEdit(&dialog);
    creditEdit.setRange(0.5, 6.0);
    creditEdit.setSingleStep(0.5);
    creditEdit.setValue(3.0);

    form.addRow("Course Code:", &codeEdit);
    form.addRow("Course Name:", &nameEdit);
    form.addRow("Credit Hours:", &creditEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (codeEdit.text().isEmpty() || nameEdit.text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Course Code and Name cannot be empty!");
            return;
        }

        int row = courseTable->rowCount();
        courseTable->insertRow(row);

        QTableWidgetItem *checkBoxItem = new QTableWidgetItem();
        checkBoxItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        checkBoxItem->setCheckState(Qt::Checked);
        courseTable->setItem(row, 0, checkBoxItem);

        courseTable->setItem(row, 1, new QTableWidgetItem(codeEdit.text()));
        courseTable->setItem(row, 2, new QTableWidgetItem(nameEdit.text()));
        courseTable->setItem(row, 3, new QTableWidgetItem(QString::number(creditEdit.value())));
        courseTable->setItem(row, 4, new QTableWidgetItem("Female"));
        courseTable->setItem(row, 5, new QTableWidgetItem("A"));
        courseTable->setItem(row, 6, new QTableWidgetItem("Regular"));
        courseTable->setItem(row, 7, new QTableWidgetItem("Selected"));

        saveCoursesToFile();
        updateCredits();
        QMessageBox::information(this, "Success", "New course added and saved to file!");
    }
}

// 4. DELETE Operation: Remove course dynamically
void MainWindow::deleteCourse()
{
    int currentRow = courseTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Delete Error", "Please select a row from the table first!");
        return;
    }

    int ret = QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this course?",
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        courseTable->removeRow(currentRow);
        saveCoursesToFile();
        updateCredits();
        QMessageBox::information(this, "Deleted", "Course removed and file updated!");
    }
}

void MainWindow::updateCredits()
{
    courseTable->blockSignals(true);

    double total = 0.0;
    for (int i = 0; i < courseTable->rowCount(); ++i) {
        if (courseTable->item(i, 0)->checkState() == Qt::Checked) {
            total += courseTable->item(i, 3)->text().toDouble();
            courseTable->setItem(i, 7, new QTableWidgetItem("Selected"));
        } else {
            courseTable->setItem(i, 7, new QTableWidgetItem("Not Selected"));
        }
    }
    totalCreditLabel->setText("<b>Total Credit Selected: " + QString::number(total) + "</b>");

    saveCoursesToFile(); // Auto update file on check change
    courseTable->blockSignals(false);
}

void MainWindow::onRegisterClicked()
{
    saveCoursesToFile();
    QMessageBox::information(this, "Registration Saved", "All choices and updates saved permanently to 'courses.txt'!");
}