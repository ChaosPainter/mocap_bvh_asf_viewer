#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMatrix4x4>
#include <QVector3D>
#include "data.h"
#include <QMainWindow>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <QtCore>
#include <QtGui>
#include <QTreeWidget>
#include <QDebug>
#include <QOpenGLWidget>
#include <qwidgetgl.h>
#include <QTimer>
#include "qcustomplot.h"

//LIBS +=opengl32.lib
/*
using namespace std;
struct cords{
    double x;
    double y;
    double z;
};*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Data main_data;
    int frame;
    bool stop=true;
    QTimer *timer;
    float interval;
    int fps;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void AddRoot_bvh(QString name);
    void AddChild_bvh(QString name,QTreeWidgetItem *parent);

    void AddRoot_asf(QString name);
    void AddChild_asf(QString name,QTreeWidgetItem *parent);

    //void calc_positions_bvh(hierarchy_bvh& h_bvh,frames_data& f_bvh);
    //void calc_positions_asf(hierarchy_asf& h_asf,frames_data& f_amc);

//    void calc_root_0_bvh(hierarchy_bvh& h_bvh,frames_data& f_bvh);
//    void calc_root_1_bvh(int frame_num,hierarchy_bvh& h_bvh,frames_data& f_bvh);
//    void calc_joint_0_bvh(hierarchy_bvh& h_bvh,frames_data& f_bvh,string name);
//    void calc_joint_1_bvh(int frame_num,hierarchy_bvh& h_bvh,frames_data& f_bvh,string name);

//    void calc_root_0_asf(hierarchy_asf& h_asf,frames_data& f_amc);
//    void calc_root_1_asf(int frame_num,hierarchy_asf& h_asf,frames_data& f_amc);
//    void calc_joint_0_asf(hierarchy_asf& h_asf,frames_data& f_amc,string name);
//    void calc_joint_1_asf(int frame_num,hierarchy_asf& h_asf,frames_data& f_amc,string name);

//    void test_matrix(QMatrix4x4 matrix);

    //void setup_table();
    void fill_table();


    //void update_anim();- jest draw() zamiast tego
    void graph(string text);
        void draw();
//    void tree(QString name);

    //void update_pos_table();
    //void update_rotation();
    //void update_position();

public slots:
    //void test ();
    //void test_1();

    void frame_iter();
    //void t_out();

private slots:

    //void show_about(QAction *Action);

    //void showRMenu(QPoint& point);

    void on_stop_button_clicked();

    void on_play_button_clicked();

    void on_pause_button_clicked();

    void on_step_forward_clicked();

    void on_step_backward_clicked();

    //void on_pushButton_7_clicked();

    //void on_spinBox_valueChanged(int arg1);

    //void on_pushButton_8_clicked();

    void on_offsets_clicked();

    void on_bvh_clicked();

    void on_asf_clicked();

    void on_slider_sliderMoved(int position);

    //void on_comboBox_2_editTextChanged(const QString &arg1);

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void showPointToolTip1(QMouseEvent *event);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_positionX_stateChanged(int arg1);

    void on_positionY_stateChanged(int arg1);

    void on_positionZ_stateChanged(int arg1);



    //void test_QMatrix4x4();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

//    void on_button_offset_apply_clicked();

    void on_angleX_stateChanged(int arg1);

    void on_angleY_stateChanged(int arg1);

    void on_angleZ_stateChanged(int arg1);

    //void on_positionY_clicked();

    void on_actionAbout_triggered();

    //void on_horizontalSlider_2_valueChanged(int value);

    void on_cylinders_clicked();

    void on_lines_clicked();
    //void cylinder();

    //void on_line_len_textChanged(const QString &arg1);

    void on_slider_sliderPressed();

    void on_actionthrow_asf_triggered();

    void on_actionthrow_bvh_triggered();

    void on_order_clicked();



    void on_frames_second_sliderPressed();

    void on_frames_second_sliderMoved(int position);

    void on_frames_second_sliderReleased();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
