/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glwidget.h>
#include <qcustomplot.h>
#include <qwidgetgl.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_BVH;
    QAction *actionLoad_ASF;
    QAction *actionLoad_AMC;
    QAction *actionSave_ASF_AMC_as_BVH;
    QAction *actionSave_BVH_as_ASF_AMC;
    QAction *actionSave_BVH;
    QAction *actionSave_ASF_AMC;
    QAction *actionSave;
    QAction *actionLoad;
    QAction *actionAbout;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox_2;
    QLabel *label_6;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *line_len;
    QLineEdit *line_x;
    QLineEdit *line_y;
    QLineEdit *line_z;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_3;
    QPushButton *pushButton_5;
    QPushButton *pushButton_4;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QLabel *label_5;
    QWidget *verticalLayoutWidget_6;
    QVBoxLayout *verticalLayout_6;
    QSlider *horizontalSlider;
    QWidget *verticalLayoutWidget_7;
    QVBoxLayout *verticalLayout_7;
    QGroupBox *groupBox;
    QCheckBox *positionZ;
    QCheckBox *positionX;
    QCheckBox *positionY;
    QComboBox *comboBox;
    QCheckBox *angleX;
    QCheckBox *angleY;
    QCheckBox *angleZ;
    QWidget *verticalLayoutWidget_8;
    QVBoxLayout *verticalLayout_8;
    QCustomPlot *customPlot;
    QWidget *verticalLayoutWidget_9;
    QVBoxLayout *verticalLayout_9;
    QPushButton *pushButton_2;
    QComboBox *comboBox_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QTreeWidget *treeWidget;
    GLWidget *openGLWidget;
    Qwidgetgl *openGLWidget_2;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1246, 801);
        MainWindow->setLayoutDirection(Qt::LeftToRight);
        actionLoad_BVH = new QAction(MainWindow);
        actionLoad_BVH->setObjectName(QStringLiteral("actionLoad_BVH"));
        actionLoad_ASF = new QAction(MainWindow);
        actionLoad_ASF->setObjectName(QStringLiteral("actionLoad_ASF"));
        actionLoad_AMC = new QAction(MainWindow);
        actionLoad_AMC->setObjectName(QStringLiteral("actionLoad_AMC"));
        actionSave_ASF_AMC_as_BVH = new QAction(MainWindow);
        actionSave_ASF_AMC_as_BVH->setObjectName(QStringLiteral("actionSave_ASF_AMC_as_BVH"));
        actionSave_BVH_as_ASF_AMC = new QAction(MainWindow);
        actionSave_BVH_as_ASF_AMC->setObjectName(QStringLiteral("actionSave_BVH_as_ASF_AMC"));
        actionSave_BVH = new QAction(MainWindow);
        actionSave_BVH->setObjectName(QStringLiteral("actionSave_BVH"));
        actionSave_ASF_AMC = new QAction(MainWindow);
        actionSave_ASF_AMC->setObjectName(QStringLiteral("actionSave_ASF_AMC"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 0, 291, 43));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        comboBox_2 = new QComboBox(verticalLayoutWidget);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));

        verticalLayout->addWidget(comboBox_2);

        label_6 = new QLabel(verticalLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setLayoutDirection(Qt::LeftToRight);
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_6);

        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QStringLiteral("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 40, 61, 121));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_2->addWidget(label);

        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout_2->addWidget(label_2);

        label_3 = new QLabel(verticalLayoutWidget_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        label_4 = new QLabel(verticalLayoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_2->addWidget(label_4);

        verticalLayoutWidget_4 = new QWidget(centralWidget);
        verticalLayoutWidget_4->setObjectName(QStringLiteral("verticalLayoutWidget_4"));
        verticalLayoutWidget_4->setGeometry(QRect(70, 40, 231, 121));
        verticalLayout_4 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        line_len = new QLineEdit(verticalLayoutWidget_4);
        line_len->setObjectName(QStringLiteral("line_len"));

        verticalLayout_4->addWidget(line_len);

        line_x = new QLineEdit(verticalLayoutWidget_4);
        line_x->setObjectName(QStringLiteral("line_x"));

        verticalLayout_4->addWidget(line_x);

        line_y = new QLineEdit(verticalLayoutWidget_4);
        line_y->setObjectName(QStringLiteral("line_y"));

        verticalLayout_4->addWidget(line_y);

        line_z = new QLineEdit(verticalLayoutWidget_4);
        line_z->setObjectName(QStringLiteral("line_z"));

        verticalLayout_4->addWidget(line_z);

        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(300, 310, 601, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        pushButton = new QPushButton(horizontalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_3 = new QPushButton(horizontalLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_5 = new QPushButton(horizontalLayoutWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        horizontalLayout->addWidget(pushButton_5);

        pushButton_4 = new QPushButton(horizontalLayoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);

        pushButton_6 = new QPushButton(horizontalLayoutWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));

        horizontalLayout->addWidget(pushButton_6);

        pushButton_7 = new QPushButton(horizontalLayoutWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout->addWidget(pushButton_7);

        label_5 = new QLabel(horizontalLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout->addWidget(label_5);

        verticalLayoutWidget_6 = new QWidget(centralWidget);
        verticalLayoutWidget_6->setObjectName(QStringLiteral("verticalLayoutWidget_6"));
        verticalLayoutWidget_6->setGeometry(QRect(300, 279, 601, 31));
        verticalLayout_6 = new QVBoxLayout(verticalLayoutWidget_6);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalSlider = new QSlider(verticalLayoutWidget_6);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_6->addWidget(horizontalSlider);

        verticalLayoutWidget_7 = new QWidget(centralWidget);
        verticalLayoutWidget_7->setObjectName(QStringLiteral("verticalLayoutWidget_7"));
        verticalLayoutWidget_7->setGeometry(QRect(10, 340, 160, 191));
        verticalLayout_7 = new QVBoxLayout(verticalLayoutWidget_7);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(verticalLayoutWidget_7);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        positionZ = new QCheckBox(groupBox);
        positionZ->setObjectName(QStringLiteral("positionZ"));
        positionZ->setGeometry(QRect(0, 60, 81, 17));
        positionX = new QCheckBox(groupBox);
        positionX->setObjectName(QStringLiteral("positionX"));
        positionX->setGeometry(QRect(0, 20, 81, 17));
        positionY = new QCheckBox(groupBox);
        positionY->setObjectName(QStringLiteral("positionY"));
        positionY->setGeometry(QRect(0, 40, 81, 17));
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(0, 0, 158, 20));
        angleX = new QCheckBox(groupBox);
        angleX->setObjectName(QStringLiteral("angleX"));
        angleX->setGeometry(QRect(0, 80, 70, 17));
        angleY = new QCheckBox(groupBox);
        angleY->setObjectName(QStringLiteral("angleY"));
        angleY->setGeometry(QRect(0, 100, 70, 17));
        angleZ = new QCheckBox(groupBox);
        angleZ->setObjectName(QStringLiteral("angleZ"));
        angleZ->setGeometry(QRect(0, 120, 70, 17));

        verticalLayout_7->addWidget(groupBox);

        verticalLayoutWidget_8 = new QWidget(centralWidget);
        verticalLayoutWidget_8->setObjectName(QStringLiteral("verticalLayoutWidget_8"));
        verticalLayoutWidget_8->setGeometry(QRect(170, 340, 731, 191));
        verticalLayout_8 = new QVBoxLayout(verticalLayoutWidget_8);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(0, 0, 0, 0);
        customPlot = new QCustomPlot(verticalLayoutWidget_8);
        customPlot->setObjectName(QStringLiteral("customPlot"));

        verticalLayout_8->addWidget(customPlot);

        verticalLayoutWidget_9 = new QWidget(centralWidget);
        verticalLayoutWidget_9->setObjectName(QStringLiteral("verticalLayoutWidget_9"));
        verticalLayoutWidget_9->setGeometry(QRect(10, 160, 291, 31));
        verticalLayout_9 = new QVBoxLayout(verticalLayoutWidget_9);
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        pushButton_2 = new QPushButton(verticalLayoutWidget_9);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout_9->addWidget(pushButton_2);

        comboBox_3 = new QComboBox(centralWidget);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));
        comboBox_3->setGeometry(QRect(10, 320, 291, 20));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 290, 289, 22));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        radioButton_2 = new QRadioButton(layoutWidget);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        horizontalLayout_3->addWidget(radioButton_2);

        radioButton = new QRadioButton(layoutWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        horizontalLayout_3->addWidget(radioButton);

        treeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(900, 0, 289, 531));
        openGLWidget = new GLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(310, 10, 581, 261));
        openGLWidget_2 = new Qwidgetgl(centralWidget);
        openGLWidget_2->setObjectName(QStringLiteral("openGLWidget_2"));
        openGLWidget_2->setGeometry(QRect(310, 540, 501, 151));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1246, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionLoad_BVH->setText(QApplication::translate("MainWindow", "Load BVH", nullptr));
        actionLoad_ASF->setText(QApplication::translate("MainWindow", "Load ASF", nullptr));
        actionLoad_AMC->setText(QApplication::translate("MainWindow", "Load AMC", nullptr));
        actionSave_ASF_AMC_as_BVH->setText(QApplication::translate("MainWindow", "Save ASF/AMC as BVH", nullptr));
        actionSave_BVH_as_ASF_AMC->setText(QApplication::translate("MainWindow", "Save BVH as ASF/AMC", nullptr));
        actionSave_BVH->setText(QApplication::translate("MainWindow", "Save BVH", nullptr));
        actionSave_ASF_AMC->setText(QApplication::translate("MainWindow", "Save ASF/AMC", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save", nullptr));
        actionLoad->setText(QApplication::translate("MainWindow", "Load", nullptr));
        actionAbout->setText(QApplication::translate("MainWindow", "About", nullptr));
#ifndef QT_NO_STATUSTIP
        comboBox_2->setStatusTip(QApplication::translate("MainWindow", "Joint / Bone", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        comboBox_2->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Bone / Joint of Hierarchy</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        label_6->setText(QApplication::translate("MainWindow", "parent - child", nullptr));
        label->setText(QApplication::translate("MainWindow", "length", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "direction X", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "direction y", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "direction z", nullptr));
#ifndef QT_NO_STATUSTIP
        line_len->setStatusTip(QApplication::translate("MainWindow", "length", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        line_x->setStatusTip(QApplication::translate("MainWindow", "offset / direction X", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        line_y->setStatusTip(QApplication::translate("MainWindow", "offset / direction Y", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        line_z->setStatusTip(QApplication::translate("MainWindow", "offset / direction Z", nullptr));
#endif // QT_NO_STATUSTIP
        pushButton->setText(QApplication::translate("MainWindow", "pocz\304\205tek", nullptr));
        pushButton_3->setText(QApplication::translate("MainWindow", "klatka -1", nullptr));
        pushButton_5->setText(QApplication::translate("MainWindow", "prz\303\263d", nullptr));
        pushButton_4->setText(QApplication::translate("MainWindow", "stop", nullptr));
        pushButton_6->setText(QApplication::translate("MainWindow", "klatka +1", nullptr));
        pushButton_7->setText(QApplication::translate("MainWindow", "koniec", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "klatka nr.", nullptr));
#ifndef QT_NO_STATUSTIP
        horizontalSlider->setStatusTip(QApplication::translate("MainWindow", "Frame Slider", nullptr));
#endif // QT_NO_STATUSTIP
        groupBox->setTitle(QApplication::translate("MainWindow", "GroupBox", nullptr));
#ifndef QT_NO_STATUSTIP
        positionZ->setStatusTip(QApplication::translate("MainWindow", "Position Z in Time", nullptr));
#endif // QT_NO_STATUSTIP
        positionZ->setText(QApplication::translate("MainWindow", "position Z", nullptr));
#ifndef QT_NO_STATUSTIP
        positionX->setStatusTip(QApplication::translate("MainWindow", "Position X in Time", nullptr));
#endif // QT_NO_STATUSTIP
        positionX->setText(QApplication::translate("MainWindow", "position X", nullptr));
#ifndef QT_NO_STATUSTIP
        positionY->setStatusTip(QApplication::translate("MainWindow", "Position Y in Time", nullptr));
#endif // QT_NO_STATUSTIP
        positionY->setText(QApplication::translate("MainWindow", "position Y", nullptr));
#ifndef QT_NO_STATUSTIP
        comboBox->setStatusTip(QApplication::translate("MainWindow", "Joint / Bone", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        comboBox->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Bone / Joint of Hierarchy</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_STATUSTIP
        angleX->setStatusTip(QApplication::translate("MainWindow", "Angle X in Time", nullptr));
#endif // QT_NO_STATUSTIP
        angleX->setText(QApplication::translate("MainWindow", "angle X", nullptr));
#ifndef QT_NO_STATUSTIP
        angleY->setStatusTip(QApplication::translate("MainWindow", "Angle Y in Time", nullptr));
#endif // QT_NO_STATUSTIP
        angleY->setText(QApplication::translate("MainWindow", "angle Y", nullptr));
#ifndef QT_NO_STATUSTIP
        angleZ->setStatusTip(QApplication::translate("MainWindow", "Angle Z in Time", nullptr));
#endif // QT_NO_STATUSTIP
        angleZ->setText(QApplication::translate("MainWindow", "angle Z", nullptr));
#ifndef QT_NO_STATUSTIP
        customPlot->setStatusTip(QApplication::translate("MainWindow", "Graph", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_STATUSTIP
        pushButton_2->setStatusTip(QApplication::translate("MainWindow", "Apply changes to length and direction of bone", nullptr));
#endif // QT_NO_STATUSTIP
        pushButton_2->setText(QApplication::translate("MainWindow", "Apply", nullptr));
#ifndef QT_NO_STATUSTIP
        comboBox_3->setStatusTip(QApplication::translate("MainWindow", "Order of Rotation", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        comboBox_3->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Order of rotation.</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_STATUSTIP
        radioButton_2->setStatusTip(QApplication::translate("MainWindow", "ASF/AMC Data", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        radioButton_2->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Data from ASF/AMC files.</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        radioButton_2->setText(QApplication::translate("MainWindow", "ASF/AMC", nullptr));
#ifndef QT_NO_STATUSTIP
        radioButton->setStatusTip(QApplication::translate("MainWindow", "BVH Data", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        radioButton->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Data from BVH file</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        radioButton->setText(QApplication::translate("MainWindow", "BVH", nullptr));
#ifndef QT_NO_STATUSTIP
        treeWidget->setStatusTip(QApplication::translate("MainWindow", "Tree of hierarchy.", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        treeWidget->setWhatsThis(QString());
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_STATUSTIP
        openGLWidget->setStatusTip(QApplication::translate("MainWindow", "Animation", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        openGLWidget->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Animation</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_STATUSTIP
        openGLWidget_2->setStatusTip(QApplication::translate("MainWindow", "Animation", nullptr));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        openGLWidget_2->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>Animation</p></body></html>", nullptr));
#endif // QT_NO_WHATSTHIS
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
