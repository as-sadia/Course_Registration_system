#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QVector>

struct PortalCourse {
    QString code;
    QString name;
    double credit;
    QString section;
    QString subSection;
    QString status;
    QString remarks;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onRegisterClicked();
    void updateCredits();
    void addCourse();      // Create
    void deleteCourse();   // Delete

private:
    QTableWidget *courseTable;
    QLabel *totalCreditLabel;
    QVector<PortalCourse> courseList;

    void setupUI();
    void loadCoursesFromFile(); // Read
    void saveCoursesToFile();   // Update / Save
};

#endif // MAINWINDOW_H