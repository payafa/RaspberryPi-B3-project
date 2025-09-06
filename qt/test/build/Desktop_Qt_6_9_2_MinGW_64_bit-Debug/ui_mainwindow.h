/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_2;
    QLabel *camera;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QPushButton *forwardButton;
    QPushButton *leftButton;
    QPushButton *pushButton_5;
    QPushButton *rightButton;
    QPushButton *backButton;
    QSpinBox *speed;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *handDriving;
    QRadioButton *selfDriving;
    QCheckBox *autoAvoidance;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout;
    QDoubleSpinBox *temperature;
    QDoubleSpinBox *humidity;
    QDoubleSpinBox *distance;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setObjectName("verticalLayout_4");
        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName("groupBox_3");
        verticalLayout_2 = new QVBoxLayout(groupBox_3);
        verticalLayout_2->setObjectName("verticalLayout_2");
        camera = new QLabel(groupBox_3);
        camera->setObjectName("camera");
        camera->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));

        verticalLayout_2->addWidget(camera);


        verticalLayout_4->addWidget(groupBox_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        forwardButton = new QPushButton(groupBox);
        forwardButton->setObjectName("forwardButton");

        gridLayout->addWidget(forwardButton, 0, 1, 1, 1);

        leftButton = new QPushButton(groupBox);
        leftButton->setObjectName("leftButton");

        gridLayout->addWidget(leftButton, 1, 0, 1, 1);

        pushButton_5 = new QPushButton(groupBox);
        pushButton_5->setObjectName("pushButton_5");

        gridLayout->addWidget(pushButton_5, 1, 1, 1, 1);

        rightButton = new QPushButton(groupBox);
        rightButton->setObjectName("rightButton");

        gridLayout->addWidget(rightButton, 1, 2, 1, 1);

        backButton = new QPushButton(groupBox);
        backButton->setObjectName("backButton");

        gridLayout->addWidget(backButton, 2, 1, 1, 1);

        speed = new QSpinBox(groupBox);
        speed->setObjectName("speed");

        gridLayout->addWidget(speed, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName("groupBox_4");
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setObjectName("verticalLayout_3");
        handDriving = new QRadioButton(groupBox_4);
        handDriving->setObjectName("handDriving");
        handDriving->setChecked(true);

        verticalLayout_3->addWidget(handDriving);

        selfDriving = new QRadioButton(groupBox_4);
        selfDriving->setObjectName("selfDriving");

        verticalLayout_3->addWidget(selfDriving);

        autoAvoidance = new QCheckBox(groupBox_4);
        autoAvoidance->setObjectName("autoAvoidance");

        verticalLayout_3->addWidget(autoAvoidance);


        horizontalLayout->addWidget(groupBox_4);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        verticalLayout = new QVBoxLayout(groupBox_2);
        verticalLayout->setObjectName("verticalLayout");
        temperature = new QDoubleSpinBox(groupBox_2);
        temperature->setObjectName("temperature");

        verticalLayout->addWidget(temperature);

        humidity = new QDoubleSpinBox(groupBox_2);
        humidity->setObjectName("humidity");

        verticalLayout->addWidget(humidity);

        distance = new QDoubleSpinBox(groupBox_2);
        distance->setObjectName("distance");

        verticalLayout->addWidget(distance);


        horizontalLayout->addWidget(groupBox_2);


        verticalLayout_4->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\346\221\204\345\203\217\345\244\264\345\233\236\346\230\276", nullptr));
        camera->setText(QString());
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\350\241\214\350\277\233\346\216\247\345\210\266", nullptr));
        forwardButton->setText(QCoreApplication::translate("MainWindow", "\345\211\215\350\277\233", nullptr));
        leftButton->setText(QCoreApplication::translate("MainWindow", "\345\267\246\350\275\254", nullptr));
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242", nullptr));
        rightButton->setText(QCoreApplication::translate("MainWindow", "\345\217\263\350\275\254", nullptr));
        backButton->setText(QCoreApplication::translate("MainWindow", "\345\220\216\351\200\200", nullptr));
        speed->setPrefix(QCoreApplication::translate("MainWindow", "\351\200\237\345\272\246\357\274\232", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "\345\212\237\350\203\275\346\250\241\345\274\217\345\274\200\345\205\263", nullptr));
        handDriving->setText(QCoreApplication::translate("MainWindow", "\346\211\213\345\212\250\351\251\276\351\251\266", nullptr));
        selfDriving->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\345\276\252\350\277\271", nullptr));
        autoAvoidance->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\212\250\351\201\277\351\232\234", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\346\230\276\347\244\272", nullptr));
        temperature->setPrefix(QCoreApplication::translate("MainWindow", "\346\270\251\345\272\246\357\274\232", nullptr));
        humidity->setPrefix(QCoreApplication::translate("MainWindow", "\346\271\277\345\272\246\357\274\232", nullptr));
        distance->setPrefix(QCoreApplication::translate("MainWindow", "\345\211\215\346\226\271\351\232\234\347\242\215\347\211\251\350\267\235\347\246\273\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
