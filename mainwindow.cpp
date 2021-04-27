 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QErrorMessage>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <data.h>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector>
#include <QtCore>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QtGui>
#include <QDebug>
#include <QOpenGLWidget>
#include <QMatrix3x3>
#include "qwidgetgl.h"
#include <iomanip>





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderHidden(true);
 // ui.treeWidget. nazwa kolumny zmienić na "Hierarchia"
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //connect(ui->treeWidget,SIGNAL(on_treeWidget_customContextMenuRequested(const QPoint&)),this,SLOT(on_treeWidget_customContextMenuRequested(const QPoint&)));
    connect(ui->openGLWidget_2,SIGNAL(frameSwapped()),ui->openGLWidget_2,SLOT(update()));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this,SLOT(showPointToolTip1(QMouseEvent*)));
//    QTimer *asfTimer = new QTimer(this);
//    QTimer *bvhTimer = new QTimer(this);
   // ui->tableWidget = new QTableWidget(this);
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer,SIGNAL(timeout()), this,SLOT(frame_iter()));
   // connect(timer,SIGNAL(timeout()), this,SLOT(t_out()));
   // test_matrix_transforms();
    frame=0;
    interval=0;


    QString text="XYZ";
    ui->comboBox_3->addItem(text);
     text="XZY";
    ui->comboBox_3->addItem(text);
     text="YZX";
    ui->comboBox_3->addItem(text);
     text="YXZ";
    ui->comboBox_3->addItem(text);
     text="ZXY";
    ui->comboBox_3->addItem(text);
     text="ZYX";
    ui->comboBox_3->addItem(text);

   //test_QMatrix4x4();

  // qDebug()<<"odwrócić kolejność rotacji w bvh i asf //wystarczy zmienić for na i = channels.size oraz i = axis order.size i dof.size";

 }

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::showRMenu(QPoint &point){
//    //qDebug()<<"showAbout\n";


//}

void MainWindow::AddRoot_bvh(QString name){
    QTreeWidgetItem *itm=new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,name);
    ui->treeWidget->addTopLevelItem(itm);
    for (int i = 0; i < main_data.h_bvh.joints[0].children.size(); ++i) {
        AddChild_bvh(QString::fromStdString(main_data.h_bvh.joints[0].children[i]),itm);
    }
}
void MainWindow::AddChild_bvh(QString name,QTreeWidgetItem *parent){
    QTreeWidgetItem *itm=new QTreeWidgetItem();
    int t=0;
    itm->setText(0,name);
    parent->addChild(itm);
    for (int i = 0; i <main_data.h_bvh.joints.size()  ; ++i) {
        if (main_data.h_bvh.joints[i].name==name.toStdString()){
            t=i;
            break;
        }
    }
    for (int i = 0; i < main_data.h_bvh.joints[t].children.size() ;++i) {
        AddChild_bvh(QString::fromStdString(main_data.h_bvh.joints[t].children[i]),itm);
    }
}

void MainWindow::AddRoot_asf(QString name){
    QTreeWidgetItem *itm=new QTreeWidgetItem(ui->treeWidget);
    itm->setText(0,name);
    ui->treeWidget->addTopLevelItem(itm);
    for (int i = 0; i < main_data.h_asf.root.children.size(); ++i) {
        AddChild_asf(QString::fromStdString(main_data.h_asf.root.children[i]),itm);
    }
}
void MainWindow::AddChild_asf(QString name,QTreeWidgetItem *parent){
    QTreeWidgetItem *itm=new QTreeWidgetItem();
    int t=0;
    itm->setText(0,name);
    parent->addChild(itm);
    for (int i = 0; i <main_data.h_asf.joints.size()  ; ++i) {
        if (main_data.h_asf.joints[i].name==name.toStdString()){
            t=i;
            break;
        }
    }
    for (int i = 0; i < main_data.h_asf.joints[t].children.size() ;++i) {
        string text =main_data.h_asf.joints[t].children[i];
        AddChild_asf(QString::fromStdString(main_data.h_asf.joints[t].children[i]),itm);
    }
}



//void MainWindow::on_pushButton_clicked()//set frame to 0;
//{
//    frame=0;
//    timer->stop();
//    ui->horizontalSlider->setValue(0);
//    draw();
//}

void MainWindow::on_step_backward_clicked()
{
    if (frame>0)
    {
        frame--;
        ui->slider->setValue(frame);
        draw();
    }

}

void MainWindow::on_pause_button_clicked()
{
    timer->stop();
}

void MainWindow::on_play_button_clicked() ///start
{

    timer->start();

}

void MainWindow::on_step_forward_clicked()
{
    if (ui->bvh->isChecked())
    {
        if (frame<main_data.h_bvh.joints[0].cords.size()-1)
        {
            frame++;
            ui->slider->setValue(frame);
            draw();
        }
    }
    else
    {
        if (ui->asf->isChecked())
        {
            if (frame<main_data.h_asf.joints[0].cords.size()-1)
            {
                frame++;
                ui->slider->setValue(frame);
                draw();
            }
        }
    }
}

//void MainWindow::on_pushButton_7_clicked() //test button++++++++++++++++++++++
//{
//    //draw();
//    /*
//    QVector<float> temp;
//    for (int i = 0; i < main_data.f_bvh.frames.size(); ++i)
//    {
//        for (int j = 0; j < main_data.f_bvh.frames[i].channel_data.size(); ++j)
//        {
//            for (int k = 0; k < main_data.f_bvh.frames[i].channel_data[j].channels.size(); ++k)
//            {
//                temp.push_back(main_data.f_bvh.frames[i].channel_data[j].channels[k]);
//            }
//        }
//        qDebug()<<temp;
//        temp.clear();
//    }
//    */
//    //cylinder();

//    //main_data.asf_reorder(main_data.h_asf,main_data.f_amc,ui->comboBox_3->currentText().toStdString());

//    return_asf r_asf;
//    r_asf=main_data.bvh_to_asf_conversion(main_data.h_bvh,main_data.f_bvh);




//    QString filename="Data.txt";
//    QFile file( filename );
//    file.remove();
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
//    QTextStream stream( &file );
//    for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
//    {
//        stream<<QString::fromStdString(main_data.h_bvh.joints[i].name)<<"\n";
//        for (int j = 0; j < main_data.h_bvh.joints[i].global_matrix.size(); ++j)
//        {
//            for (int k = 0; k < 4; ++k)
//            {
//                //QVector4D vec=main_data.h_bvh.joints[i].global_matrix[j].column(k); row >> column
//                QVector4D vec=main_data.h_bvh.joints[i].global_matrix[j].row(k);
//                stream<<vec.x()<<" "<<vec.y()<<" "<<vec.z()<<" "<<vec.w()<<"\n";
//            }
//            stream<<"\n";
//        }
//        stream<<"\n";
//    }
//    file.close();

////    QString filename="Data.txt";
////    QFile file( filename );
////    file.remove();
////    file.open(QIODevice::WriteOnly | QIODevice::Text);
////    QTextStream stream( &file );
////    QMatrix4x4 mat;
////    mat.rotate(30,1,0,0);
////    for (int k = 0; k < 4; ++k)
////    {
////        //QVector4D vec=mat.column(k); //row >> column
////        QVector4D vec=mat.row(k);
////        stream<<vec.x()<<" "<<vec.y()<<" "<<vec.z()<<" "<<vec.w()<<"\n";
////    }
////    stream<<"\n";

//}

//void MainWindow::on_spinBox_valueChanged(int arg1)
//{

//}

//void MainWindow::on_pushButton_8_clicked()
//{

//}

void MainWindow::on_order_clicked() //zmiana order
{
    if(ui->bvh->isChecked())
    {
        if(main_data.h_bvh.joints.size()<=0)
        {
            return;
        }
        int temp=0;
        for (int i = 0; i <main_data.h_bvh.joints.size()  ; ++i)
        {
            if (main_data.h_bvh.joints[i].name==ui->comboBox_2->currentText().toStdString())
            {
                temp=i;
                break;
            }
        }
//        main_data.h_bvh.joints[temp].offset[0]=ui->line_x->text().toDouble();
//        main_data.h_bvh.joints[temp].offset[1]=ui->line_y->text().toDouble();
//        main_data.h_bvh.joints[temp].offset[2]=ui->line_z->text().toDouble();
        //main_data.calc_bvh_primary_rot_order_cords_angles(main_data.h_bvh,main_data.f_bvh);
        main_data.bvh_reorder(main_data.h_bvh,main_data.f_bvh,ui->comboBox_3->currentText().toStdString());
        main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
        fill_table();
        on_comboBox_currentTextChanged(ui->comboBox->currentText());
        draw();
        //przerysuj
        }



    if(ui->asf->isChecked())
    {
        if(main_data.h_asf.joints.size()<=0)
        {
            return;
        }
        int temp;
        for (int i = 0; i <main_data.h_asf.joints.size()  ; ++i)
        {
            if (main_data.h_asf.joints[i].name==ui->comboBox_2->currentText().toStdString())
            {
                temp=i;
                break;
            }
        }
//        double x,y,z,l,dx,dy,dz;

////        l=ui->line_len->text().toDouble();
//        x=ui->line_x->text().toDouble();
//        y=ui->line_y->text().toDouble();
//        z=ui->line_z->text().toDouble();
//        if (ui->comboBox_2->currentText().toStdString()==main_data.h_asf.joints[0].name)
//        {
//            main_data.h_asf.joints[temp].direction[0]=x;

//            main_data.h_asf.joints[temp].direction[1]=y;
//            main_data.h_asf.joints[temp].direction[2]=z;
//        }
//        else
//        {
//            l=sqrt(((x*x)+(y*y)+(z*z)));
//            dx=x/l;
//            dy=y/l;
//            dz=z/l;
//            main_data.h_asf.joints[temp].length=l;
//            main_data.h_asf.joints[temp].direction[0]=dx;

//            main_data.h_asf.joints[temp].direction[1]=dy;
//            main_data.h_asf.joints[temp].direction[2]=dz;
//        }
//        if (l>=0&&x<=1&&x>=-1&&y<=1&&y>=-1&&z<=1&&z>=-1)
//            {
//                main_data.h_asf.joints[temp].length=l;
//                main_data.h_asf.joints[temp].direction[0]=x;
//                main_data.h_asf.joints[temp].direction[1]=y;
//                main_data.h_asf.joints[temp].direction[2]=z;
//            }
        //przerysuj
        main_data.asf_reorder(main_data.h_asf,main_data.f_amc,ui->comboBox_3->currentText().toStdString());
        main_data.asf_position(main_data.h_asf,main_data.f_amc);
        fill_table();
        on_comboBox_currentTextChanged(ui->comboBox->currentText());
        draw();
    }


}

void MainWindow::on_bvh_clicked()//bvh
{
    if (main_data.h_bvh.joints.size()>0)
    {
        ui->comboBox->blockSignals(true);
        ui->comboBox_2->blockSignals(true);
        ui->comboBox->clear();
        ui->comboBox_2->clear();

        for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
        {
            ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
            ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
        }
       ui->comboBox->blockSignals(false);
       ui->comboBox_2->blockSignals(false);
       ui->treeWidget->clear();
       AddRoot_bvh(QString::fromStdString(main_data.h_bvh.root.name));
       ui->treeWidget->expandAll();
       on_comboBox_currentTextChanged(ui->comboBox->currentText());
       int s_max=main_data.f_bvh.frames.size();
       ui->slider->setMaximum(s_max);
       frame=0;
       ui->slider->setValue(0);
       fill_table();
       draw();
    }
    else
    {
        ui->bvh->setChecked(false);
        ui->asf->setChecked(true);
        frame=0;
    }

}


void MainWindow::on_asf_clicked()//asf
{
    if (main_data.h_asf.joints.size()>0)
    {
       ui->comboBox->blockSignals(true);
       ui->comboBox_2->blockSignals(true);
       ui->comboBox->clear();
       ui->comboBox_2->clear();


       for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
       {

           ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
           ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
       }
       ui->comboBox->blockSignals(false);
       ui->comboBox_2->blockSignals(false);

       ui->treeWidget->clear();
       AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
       ui->treeWidget->expandAll();
       on_comboBox_currentTextChanged(ui->comboBox->currentText());
       int s_max=main_data.f_amc.frames.size();
       ui->slider->setMaximum(s_max);
       frame=0;
       ui->slider->setValue(0);
       fill_table();
       draw();
    }

    else
    {
        ui->bvh->setChecked(true);
        ui->asf->setChecked(false);
        frame=0;
    }
}

void MainWindow::on_slider_sliderMoved(int position)
{
    frame=position;
    draw();
}

//void MainWindow::on_comboBox_2_editTextChanged(const QString &arg1)
//{


//}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{

    //dla bvh
    if(ui->bvh->isChecked())
    {
        int temp=0;
        for (int i = 0; i <main_data.h_bvh.joints.size()  ; ++i) {
            if (main_data.h_bvh.joints[i].name==arg1.toStdString()){
                temp=i;
                break;
            }
        }
        QString text1;
        float num=0;
        //ui->lineEdit_5->setText(QString::number(main_data.h_bvh.joints[temp].id));
        if (temp==0)
        {
            text1=QString::fromStdString("root");
        }
        else
        {
            text1=QString::fromStdString(main_data.h_bvh.joints[temp].parent);
        }
        text1.append(" - ");
        text1.append(arg1);
        ui->label_6->setText(text1);
//        ui->label_2->setText("OFFSET X");
//        ui->label_3->setText("OFFSET Y");
//        ui->label_4->setText("OFFSET Z");
        for (int j = 0; j < main_data.h_bvh.joints[temp].offset.size(); ++j)
        {
            num=num+(main_data.h_bvh.joints[temp].offset[j]*main_data.h_bvh.joints[temp].offset[j]);
        }
        float length=sqrt(num);
        //ui->line_len->setText(QString::number(length));
        ui->line_x->setText(QString::number(main_data.h_bvh.joints[temp].offset[0]));
        ui->line_y->setText(QString::number(main_data.h_bvh.joints[temp].offset[1]));
        ui->line_z->setText(QString::number(main_data.h_bvh.joints[temp].offset[2]));
        fill_table();
    }

    //dla asf
    if(ui->asf->isChecked())
    {
        int temp=0;
        for (int i = 0; i <main_data.h_asf.joints.size()  ; ++i) {
            if (main_data.h_asf.joints[i].name==arg1.toStdString()){
                temp=i;
                break;
            }
        }
        //float num=0;
    //    ui->label_2->setText("OFFSET X");
    //    ui->label_3->setText("OFFSET Y");
    //    ui->label_4->setText("OFFSET Z");
        float length;
        if (temp==0)
        {
            length=1;
        }
        else
        {
            length=main_data.h_asf.joints[temp].length;
        }

        //ui->line_len->setText(QString::number(length));
        double x,y,z;
        x=main_data.h_asf.joints[temp].direction[0]*length;
        y=main_data.h_asf.joints[temp].direction[1]*length;
        z=main_data.h_asf.joints[temp].direction[2]*length;
        ui->line_x->setText(QString::number(x));
        ui->line_y->setText(QString::number(y));
        ui->line_z->setText(QString::number(z));
        fill_table();
    }

}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)//combobox od wykresu
{   
    string text=arg1.toStdString();
    graph(text);


}



void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{



    if(ui->bvh->isChecked())
    {
        //qDebug()<<"udududu\n";
        //QPoint global_pos=ui->treeWidget->mapToGlobal(pos);
        //QModelIndex index = ui->treeWidget->indexAt(pos);
        if(main_data.h_bvh.joints.size()<=0)
        {
            return;
        }
        //qDebug()<<"stan 1\n";
        QTreeWidgetItem *itm=ui->treeWidget->itemAt(pos);
        //qDebug()<<"stan 2\n";
        if (itm==NULL)
        {
            return;
        }


        string text= itm->text(0).toStdString();//nazwa jointa
        //qDebug()<<"stan 3\n";
        if (text!=main_data.h_bvh.joints[0].name)
        { //dla roota nic nie dodawać


            QMenu *menu=new QMenu(ui->treeWidget);
            QAction *myaction0  = new QAction("dodaj joint powyżej",this);
            QAction *myaction1  = new QAction("dodaj joint poniżej",this);
            QAction *myaction2  = new QAction("zmień nazwę",this);
            QAction *myaction3  = new QAction("usuń joint",this);
            QAction *myaction4  = new QAction("zmień rodzica",this);
            menu->addAction(myaction0);
            menu->addAction(myaction1);
            menu->addAction(myaction2);
            menu->addAction(myaction3);
            menu->addAction(myaction4);//comment tymczsowy
            //  connect(myaction1, SIGNAL(triggered()), this, SLOT(newDev()));//chyba wywalić
            //  connect(myaction2, SIGNAL(triggered()), this, SLOT(newDev()));
            //  connect(myaction3, SIGNAL(triggered()), this, SLOT(newDev()));
            QAction *action =menu->exec((QCursor::pos()));
            if(action==0)
            {
                return;
            }
           if(action->text().toStdString()==myaction0->text().toStdString())
           {
               //qDebug()<<"dodaj joint powyżej bvh\n";
                bool ok;
                string new_name;
                QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Joint"), QLineEdit::Normal, "", &ok);
                if (ok && !name.isEmpty())
                {
                    new_name=name.toStdString();
                }
                else
                {
                    return;
                }
                main_data.bvh_add_joint_above(main_data.h_bvh,main_data.f_bvh,new_name,text);

                ui->treeWidget->clear();
                AddRoot_bvh(QString::fromStdString(main_data.h_bvh.joints[0].name));

                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();
                for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
                {
                    ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                    ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();

                main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
                fill_table();
                //main_data.bvh_pc_index(main_data.h_bvh,main_data.f_bvh);
                draw();
            }



            if(action->text().toStdString()==myaction1->text().toStdString())
            {
                //qDebug()<<"dodaj joint poniżej bvh\n";

                bool ok;
                string new_name;
                QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Joint"), QLineEdit::Normal, "", &ok);
                if (ok && !name.isEmpty())
                {
                    new_name=name.toStdString();
                }
                else
                {
                    return;
                }

                main_data.bvh_add_joint_below(main_data.h_bvh,main_data.f_bvh,new_name,text);

                /////zmainy w boxach i drzewku;
                ui->treeWidget->clear();
                AddRoot_bvh(QString::fromStdString(main_data.h_bvh.joints[0].name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();
                for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
                {
                    ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                    ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();

                main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
                fill_table();
                //main_data.bvh_pc_index(main_data.h_bvh,main_data.f_bvh);
                draw();
            }


            if(action->text().toStdString()==myaction2->text().toStdString())
            {
                //qDebug()<<"zmień nazwę bvh\n";

                bool ok;
                string new_name;
                QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Name"), QLineEdit::Normal, "", &ok);
                if (ok && !name.isEmpty())
                {
                    new_name=name.toStdString();
                }
                else
                {
                    return;
                }

                main_data.bvh_change_name(main_data.h_bvh,main_data.f_bvh,text,new_name);


                ui->treeWidget->clear();
                AddRoot_bvh(QString::fromStdString(main_data.h_bvh.joints[0].name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();


                for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
                {

                ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();
                main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
               // main_data.bvh_pc_index(main_data.h_bvh,main_data.f_bvh);
                draw();
            }


            if(action->text().toStdString()==myaction3->text().toStdString())
            {
                //qDebug()<<"usuń bvh\n";

                main_data.bvh_remove_joint(main_data.h_bvh,main_data.f_bvh,text);

                ui->treeWidget->clear();
                AddRoot_bvh(QString::fromStdString(main_data.h_bvh.root.name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();


                for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
                {
                ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();
                main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
               // main_data.bvh_pc_index(main_data.h_bvh,main_data.f_bvh);
                draw();
            }

            if(action->text().toStdString()==myaction4->text().toStdString())
            {
                //

                bool ok;
                string new_name;
                QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Parent"), QLineEdit::Normal, "", &ok);
                if (ok && !name.isEmpty())
                {
                    new_name=name.toStdString();
                }
                else
                {
                    return;
                }

                main_data.bvh_change_parent(main_data.h_bvh,main_data.f_bvh,text,new_name);


                ui->treeWidget->clear();
                AddRoot_bvh(QString::fromStdString(main_data.h_bvh.joints[0].name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();


                for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
                {

                ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();
                main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
               // main_data.bvh_pc_index(main_data.h_bvh,main_data.f_bvh);
                draw();
            }
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////            asf
    if(ui->asf->isChecked())
    {
        if(main_data.h_asf.joints.size()<=0)
        {
            return;
        }
        QTreeWidgetItem *itm=ui->treeWidget->itemAt(pos);
        if (itm==NULL)
        {
            return;
        }

        string text= itm->text(0).toStdString();//nazwa jointa

        if (text!=main_data.h_asf.root.name)
        { //dla roota nic nie dodawać        
        QMenu *menu=new QMenu(ui->treeWidget);
        QAction *myaction0  = new QAction("dodaj joint powyżej",this);
        QAction *myaction1  = new QAction("dodaj joint poniżej",this);
        QAction *myaction2  = new QAction("zmień nazwę",this);
        QAction *myaction3  = new QAction("usuń joint",this);
        QAction *myaction4  = new QAction("zmień rodzica",this);
        menu->addAction(myaction0);
        menu->addAction(myaction1);
        menu->addAction(myaction2);
        menu->addAction(myaction3);
        menu->addAction(myaction4);//comment tymczsowy
        //  connect(myaction1, SIGNAL(triggered()), this, SLOT(newDev()));//chyba wywalić
        //  connect(myaction2, SIGNAL(triggered()), this, SLOT(newDev()));
        //  connect(myaction3, SIGNAL(triggered()), this, SLOT(newDev()));
        QAction *action =menu->exec((QCursor::pos()));
        if(action==0)
        {
            return;
        }

        if(action->text().toStdString()==myaction0->text().toStdString())
        {//dodanie zmienić z bvh na asf


            bool ok;
            string new_name;
            QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Bone"), QLineEdit::Normal, "", &ok);
            if (ok && !name.isEmpty())
            {
                new_name=name.toStdString();
            }
            else
            {
                return;
            }

            //qDebug()<<"dodaj joint asf\n";


            main_data.asf_add_joint_above(main_data.h_asf,main_data.f_amc,new_name,text);



            /////zmainy w boxach i drzewku;
            ui->treeWidget->clear();
            AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
            ui->comboBox->blockSignals(true);
            ui->comboBox_2->blockSignals(true);
            ui->comboBox->clear();
            ui->comboBox_2->clear();


            for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
            {
                ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
            }
            ui->comboBox->blockSignals(false);
            ui->comboBox_2->blockSignals(false);
            ui->treeWidget->expandAll();
            main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
            main_data.asf_position(main_data.h_asf,main_data.f_amc);
            on_comboBox_currentTextChanged(ui->comboBox->currentText());
            fill_table();
            draw();

        }


        if(action->text().toStdString()==myaction1->text().toStdString()) // asf add below
        {
            bool ok;
            string new_name;
            QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New bone"), QLineEdit::Normal, "", &ok);
            if (ok && !name.isEmpty())
            {
                new_name=name.toStdString();
            }
            else
            {
                return;
            }

            //qDebug()<<"dodaj joint asf\n";

            main_data.asf_add_joint_below(main_data.h_asf,main_data.f_amc,new_name,text);

            ui->treeWidget->clear();
            AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
            ui->comboBox->blockSignals(true);
            ui->comboBox_2->blockSignals(true);
            ui->comboBox->clear();
            ui->comboBox_2->clear();


            for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
            {

                ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
            }
            ui->comboBox->blockSignals(false);
            ui->comboBox_2->blockSignals(false);
            ui->treeWidget->expandAll();
            main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
            main_data.asf_position(main_data.h_asf,main_data.f_amc);
            on_comboBox_currentTextChanged(ui->comboBox->currentText());
            fill_table();
            draw();
        }


            if(action->text().toStdString()==myaction2->text().toStdString())
            {
                //qDebug()<<"zmień nazwę asf\n";
                bool ok;
                string new_name;
                QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Name"), QLineEdit::Normal, QDir::home().dirName(), &ok);
                if (ok && !name.isEmpty())
                {
                    new_name=name.toStdString();

                }

                main_data.asf_change_name(main_data.h_asf,main_data.f_amc,text,new_name);

                ui->treeWidget->clear();
                AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();


                for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
                {

                ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();
                main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
                main_data.asf_position(main_data.h_asf,main_data.f_amc);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
                fill_table();
                draw();

            }
            if(action->text().toStdString()==myaction3->text().toStdString())
            {


                main_data.asf_remove_joint(main_data.h_asf,main_data.f_amc,text);

                ui->treeWidget->clear();
                AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();


                for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
                {
                    ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                    ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();
                main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
                main_data.asf_position(main_data.h_asf,main_data.f_amc);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
                fill_table();
                draw();

            }
            if(action->text().toStdString()==myaction4->text().toStdString())
            {
                bool ok;
                string new_name;
                QString name=QInputDialog::getText(this, tr("QInputDialog::getText()"),tr("New Parent"), QLineEdit::Normal, "", &ok);
                if (ok && !name.isEmpty())
                {
                    new_name=name.toStdString();
                }
                else
                {
                    return;
                }

                main_data.asf_change_parent(main_data.h_asf,main_data.f_amc,text,new_name);

                ui->treeWidget->clear();
                AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
                ui->comboBox->blockSignals(true);
                ui->comboBox_2->blockSignals(true);
                ui->comboBox->clear();
                ui->comboBox_2->clear();


                for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
                {
                    ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                    ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
                }
                ui->comboBox->blockSignals(false);
                ui->comboBox_2->blockSignals(false);
                ui->treeWidget->expandAll();
                main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
                main_data.asf_position(main_data.h_asf,main_data.f_amc);
                on_comboBox_currentTextChanged(ui->comboBox->currentText());
                fill_table();
                draw();

            }




        }
    }
}


void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

}


//void MainWindow::test()
//{
////    QVector<Vertex> array;                        kod zmienia między trujkątem a kwadratem
////    if(ui->openGLWidget_2->flaga=="cube")
////    {
////        array=triangle;
////        ui->openGLWidget_2->flaga="triangle";
////    }
////    else
////    {
////        array=cube;
////        ui->openGLWidget_2->flaga="cube";
////    }

////    ui->openGLWidget_2->refresh(array);





////    QVector<Vertex> full_vertex_vector;


////   Vertex y_rot = Vertex( QVector3D(  0.0f,  0.0f,  0.3f), QVector3D( 1.0f, 0.0f, 0.0f ) );
////   Vertex x_rot = Vertex( QVector3D(  0.0f,  0.3f,  0.0f), QVector3D( 0.0f, 1.0f, 0.0f ) );
////   //Vertex z_rot = Vertex( QVector3D(  0.3f,  0.0f,  0.0f), QVector3D( 0.0f, 0.0f, 1.0f ) );

////   QVector<Vertex> x_vert;
////   QVector<Vertex> y_vert;
////  // QVector<Vertex> z_vert;

////   QMatrix4x4 tm;
////   //tm.setColumn(3,);
////   for (int i = 0; i < 36; ++i)
////   {
////       Vertex temp_x;
////       Vertex temp_y;
////       //Vertex temp_z;
////       QMatrix4x4 mat;

////       temp_x=x_rot;
////       mat.setToIdentity();
////       mat.rotate(i*10,1,0,0);
////       temp_x.setPosition(mat*temp_x.position());
////       x_vert.push_back(temp_x);

////       temp_y=y_rot;
////       mat.setToIdentity();
////       mat.rotate(i*10,0,1,0);
////       temp_y.setPosition(mat*temp_y.position());
////       y_vert.push_back(temp_y);

//////       temp_z=z_rot;
//////       mat.setToIdentity();
//////       mat.rotate(i*10,0,0,1);
//////       temp_z.setPosition(mat*temp_z.position());
//////       z_vert.push_back(temp_z);
////   }
////   QVector3D test_vec(4.0f,1.0f,2.0f);
////   QVector3D test_start(0.0f,0.0f,0.0f);
////   QVector3D test_up(0.0f,1.0f,0.0f);
////   QMatrix4x4 test_mat;
////   test_mat.setToIdentity();
////   test_mat.lookAt(test_start,test_vec,test_up);


////   QVector<Vertex> x_vert_prim;
////   QVector<Vertex> y_vert_prim;
////   QVector<Vertex> z_vert_prim;

////   for (int i = 0; i < 36; ++i)
////   {
////       Vertex temp_x=x_vert[i];
////       Vertex temp_y=y_vert[i];
//////      Vertex temp_z=z_vert[i];
////       QMatrix4x4 mat;
////       mat.setToIdentity();
////       mat.translate(0,1,0);


////       temp_x.setPosition(mat*temp_x.position());
////      x_vert_prim.push_back(temp_x);


////       temp_y.setPosition(mat*temp_y.position());
////       y_vert_prim.push_back(temp_y);


////      // temp_z.setPosition(mat*temp_z.position());
////      // z_vert_prim.push_back(temp_z);

////   }

////   for (int i = 0; i < 36; ++i)
////   {
////       int n=i;
////       int m=i+1;
////       if (m>=36)
////       {
////           m=0;
////       }
////       //x_cc
////       full_vertex_vector.push_back(x_vert_prim[n]);
////       full_vertex_vector.push_back(x_vert[n]);
////       full_vertex_vector.push_back(x_vert[m]);

////       full_vertex_vector.push_back(x_vert_prim[m]);
////       full_vertex_vector.push_back(x_vert_prim[n]);
////       full_vertex_vector.push_back(x_vert[m]);

////       //x_c
////       full_vertex_vector.push_back(x_vert_prim[n]);
////       full_vertex_vector.push_back(x_vert[m]);
////       full_vertex_vector.push_back(x_vert[n]);

////       full_vertex_vector.push_back(x_vert_prim[m]);
////       full_vertex_vector.push_back(x_vert[m]);
////       full_vertex_vector.push_back(x_vert_prim[n]);

////       //y_cc
////       full_vertex_vector.push_back(y_vert_prim[n]);
////       full_vertex_vector.push_back(y_vert[n]);
////       full_vertex_vector.push_back(y_vert[m]);

////       full_vertex_vector.push_back(y_vert_prim[m]);
////       full_vertex_vector.push_back(y_vert_prim[n]);
////       full_vertex_vector.push_back(y_vert[m]);

////       //y_c
////       full_vertex_vector.push_back(y_vert_prim[n]);
////       full_vertex_vector.push_back(y_vert[m]);
////       full_vertex_vector.push_back(y_vert[n]);

////       full_vertex_vector.push_back(y_vert_prim[m]);
////       full_vertex_vector.push_back(y_vert[m]);
////       full_vertex_vector.push_back(y_vert_prim[n]);


////   }

////    ui->openGLWidget_2->refresh(full_vertex_vector,1);


//}

//void MainWindow::test_1()
//{

//        QVector<Vertex> full_vertex_vector;
//        QVector<Vertex> bottom;
//        QVector<Vertex> top;
//        Vertex point = Vertex( QVector3D(  0.3f,  0.0f,  0.0f), QVector3D( 0.0f, 1.0f, 0.0f ) );
//        Vertex point_inside = Vertex( QVector3D(  0.3f,  0.0f,  0.0f), QVector3D( 0.0f, 1.0f, 0.0f ) );
//        Vertex middle_bottom = Vertex( QVector3D(  0.0f,  0.0f,  0.0f), QVector3D( 1.0f, 0.0f, 0.0f ) );
//        Vertex middle_top = Vertex( QVector3D(  0.0f,  0.6f,  0.0f), QVector3D( 0.0f, 0.0f, 1.0f ) );
//        QMatrix4x4 y_trans;
//        y_trans.translate(0,0.6f,0);
           
//           for (int i = 0; i < 72; ++i)
//           {
//               Vertex temp_x;

//               QMatrix4x4 mat;

//               temp_x=point;
//               mat.setToIdentity();
//               mat.rotate(i*5,0,1,0);
//               temp_x.setPosition(mat*temp_x.position());
//               bottom.push_back(temp_x);

//               temp_x.setPosition(y_trans*temp_x.position());
//               top.push_back(temp_x);

//           }
//        for (int i = 0; i < 72; ++i)
//           {
//                int n=i;
//                int m=i+1;
//                if (m>=72)
//                {
//                     m=0;
//                }

//                //_cc

//                full_vertex_vector.push_back(top[n]);
//                full_vertex_vector.push_back(bottom[n]);
//                full_vertex_vector.push_back(bottom[m]);

//                full_vertex_vector.push_back(top[m]);
//                full_vertex_vector.push_back(top[n]);
//                full_vertex_vector.push_back(bottom[m]);
//                //_cc_top
//                full_vertex_vector.push_back(middle_top);
//                full_vertex_vector.push_back(top[n]);
//                full_vertex_vector.push_back(top[m]);
//                //_cc_bottom
//                full_vertex_vector.push_back(middle_bottom);
//                full_vertex_vector.push_back(bottom[m]);
//                full_vertex_vector.push_back(bottom[n]);



//           }
        
//            QMatrix4x4 look_at;
//            look_at.setToIdentity();
//            QVector3D eye(0.0f, 0.0f, 0.0f);
//            QVector3D center(1.0f, 0.0f, 0.0f);
//            QVector3D up(0.0f, 1.0f, 0.0f);
//            look_at.lookAt(eye,center,up);
//            test_matrix(look_at);

//            QVector3D test(0.0f, 0.5f, 1.0f);
//            test=look_at*test;

//            qDebug()<<test<<"\n";

//            look_at.transposed();

//             test=look_at*test;
        

//             ui->openGLWidget_2->refresh(full_vertex_vector,0);
//}

void MainWindow::draw()
{
    QVector<Vertex> full_vertex_vector;
    Vertex temp_parent = Vertex( QVector3D(  0.0f,  0.0f,  0.0f), QVector3D( 0.0f, 1.0f, 0.0f ) );
    Vertex temp_child = Vertex( QVector3D(  0.0f,  0.0f,  0.0f), QVector3D( 0.0f, 1.0f, 0.0f ) );

    Vertex vert1 = Vertex( QVector3D(  1000.0f,  0.0f,  0.0f), QVector3D( 1.0f, 0.0f, 0.0f ));
    Vertex vert2 = Vertex( QVector3D(  -1000.0f,  0.0f,  0.0f), QVector3D( 1.0f, 0.0f, 0.0f ));

    full_vertex_vector.push_back(vert1);
    full_vertex_vector.push_back(vert2);

    Vertex vert3 = Vertex( QVector3D(  0.0f,  0.0f,  1000.0f), QVector3D( 0.0f, 0.0f, 1.0f ));
    Vertex vert4 = Vertex( QVector3D(  0.0f,  0.0f,  -1000.0f), QVector3D( 0.0f, 0.0f, 1.0f ));

    full_vertex_vector.push_back(vert3);
    full_vertex_vector.push_back(vert4);

    if (ui->openGLWidget_2->cylinders)
    {
        QVector<QVector3D> vect_v3d;
        QVector3D v(1,0,0);
        QMatrix4x4 mat;
        QVector3D temp;

        for (int i = 0; i < 36; ++i)
        {
            mat.setToIdentity();
            mat.rotate(i*10,0,1,0);
            temp=mat*v;
            vect_v3d.push_back(temp);

        }

        //bvh
        int color = 0.5;
        if (ui->bvh->isChecked())
        {
            for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
            {

               QColor color = QColor::fromHsl(360.0/main_data.h_bvh.joints.size() * i, 255, 128);
                for (int j = 0; j < main_data.h_bvh.joints[i].children.size(); ++j)
                {
                    QVector3D x0(main_data.h_bvh.joints[i].cords[frame].x,main_data.h_bvh.joints[i].cords[frame].y,main_data.h_bvh.joints[i].cords[frame].z);
                    int c=main_data.h_bvh.joints[i].child_index_joint[j];
                    QVector3D x1(main_data.h_bvh.joints[c].cords[frame].x,main_data.h_bvh.joints[c].cords[frame].y,main_data.h_bvh.joints[c].cords[frame].z);
                    QVector3D child_vec=x1-x0;
                    float len=child_vec.length();
                    QVector3D temp(0,len,0);
                    QQuaternion quat=QQuaternion::rotationTo(temp,child_vec);
                    quat.normalize();
                    QMatrix4x4 mat4x4;
                    mat4x4.setToIdentity();
                    mat4x4.rotate(quat);

                    QVector<QVector3D> parent;
                    QVector<QVector3D> child;
                    QVector3D temp1;
                    QVector3D temp2;
                    for (int k = 0; k < vect_v3d.size(); ++k)
                    {
                        temp1=mat4x4*vect_v3d[k];
                        temp1=temp1+x0;

                        temp2=mat4x4*vect_v3d[k];
                        temp2=temp2+x1;

                        parent.push_back(temp1);
                        child.push_back(temp2);
                    }
                    //dodawanie do full_vertex_vector


                    for (int i = 0; i < 36; ++i)
                    {
                        int n=i;
                        int m=i+1;
                        if (m>=36)
                        {
                            m=0;
                        }
                        Vertex p_n(parent[n],QVector3D( color.redF(), color.greenF(), color.blueF() ));
                        Vertex p_m(parent[m],QVector3D(color.redF(), color.greenF(), color.blueF() ));
                        Vertex c_n(child[n],QVector3D(color.redF(), color.greenF(), color.blueF()));
                        Vertex c_m(child[m],QVector3D( color.redF(), color.greenF(), color.blueF()));


                        //x_cc
                        full_vertex_vector.push_back(c_n);
                        full_vertex_vector.push_back(p_n);
                        full_vertex_vector.push_back(p_m);

                        full_vertex_vector.push_back(c_m);
                        full_vertex_vector.push_back(c_n);
                        full_vertex_vector.push_back(p_m);

//                        //x_c
                        full_vertex_vector.push_back(c_n);
                        full_vertex_vector.push_back(p_m);
                        full_vertex_vector.push_back(p_n);

                        full_vertex_vector.push_back(c_m);
                        full_vertex_vector.push_back(p_m);
                        full_vertex_vector.push_back(c_n);

                    }
                }
            }
         ui->openGLWidget_2->refresh(full_vertex_vector,0);
        }
        //asf
        if (ui->asf->isChecked())
        {
            for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
            {
                QColor color = QColor::fromHsl(360.0/main_data.h_asf.joints.size() * i, 255, 128);
                for (int j = 0; j < main_data.h_asf.joints[i].children.size(); ++j)
                {
                    QVector3D x0(main_data.h_asf.joints[i].cords[frame].x,main_data.h_asf.joints[i].cords[frame].y,main_data.h_asf.joints[i].cords[frame].z);
                    int c=main_data.h_asf.joints[i].child_index_joint[j];
                    QVector3D x1(main_data.h_asf.joints[c].cords[frame].x,main_data.h_asf.joints[c].cords[frame].y,main_data.h_asf.joints[c].cords[frame].z);
                    QVector3D child_vec=x1-x0;
                    float len=child_vec.length();
                    QVector3D temp(0,len,0);
                    QQuaternion quat=QQuaternion::rotationTo(temp,child_vec);
                    quat.normalize();
                    QMatrix4x4 mat4x4;
                    mat4x4.setToIdentity();
                    mat4x4.rotate(quat);

                    QVector<QVector3D> parent;
                    QVector<QVector3D> child;
                    QVector3D temp1;
                    QVector3D temp2;
                    for (int k = 0; k < vect_v3d.size(); ++k)
                    {
                        temp1=mat4x4*vect_v3d[k];
                        temp1=temp1+x0;

                        temp2=mat4x4*vect_v3d[k];
                        temp2=temp2+x1;

                        parent.push_back(temp1);
                        child.push_back(temp2);
                    }
                    //dodawanie do full_vertex_vector

                    for (int i = 0; i < 36; ++i)
                    {
                        int n=i;
                        int m=i+1;
                        if (m>=36)
                        {
                            m=0;
                        }
                        Vertex p_n(parent[n],QVector3D( color.redF(), color.greenF(), color.blueF() ));
                        Vertex p_m(parent[m],QVector3D( color.redF(), color.greenF(), color.blueF() ));
                        Vertex c_n(child[n],QVector3D( color.redF(), color.greenF(), color.blueF() ));
                        Vertex c_m(child[m],QVector3D( color.redF(), color.greenF(), color.blueF() ));
                        //x_cc
                        full_vertex_vector.push_back(c_n);
                        full_vertex_vector.push_back(p_n);
                        full_vertex_vector.push_back(p_m);

                        full_vertex_vector.push_back(c_m);
                        full_vertex_vector.push_back(c_n);
                        full_vertex_vector.push_back(p_m);

                        //x_c
                        full_vertex_vector.push_back(c_n);
                        full_vertex_vector.push_back(p_m);
                        full_vertex_vector.push_back(p_n);

                        full_vertex_vector.push_back(c_m);
                        full_vertex_vector.push_back(p_m);
                        full_vertex_vector.push_back(c_n);

                    }
                }
            }
         }
            ui->openGLWidget_2->refresh(full_vertex_vector,0);

    }


    if (ui->openGLWidget_2->lines)
    {

        if(ui->bvh->isChecked())
        {
            for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
            {
                for (int j = 0; j < main_data.h_bvh.joints[i].children.size(); ++j)
                {
                    temp_parent.m_position=QVector3D(main_data.h_bvh.joints[i].cords[frame].x , main_data.h_bvh.joints[i].cords[frame].y , main_data.h_bvh.joints[i].cords[frame].z);
                    int child_index=main_data.h_bvh.joints[i].child_index_joint[j];
                    temp_child.m_position=QVector3D(main_data.h_bvh.joints[child_index].cords[frame].x,main_data.h_bvh.joints[child_index].cords[frame].y,main_data.h_bvh.joints[child_index].cords[frame].z);

                    full_vertex_vector.push_back(temp_parent);
                    full_vertex_vector.push_back(temp_child);
                }
            }
            ui->openGLWidget_2->refresh(full_vertex_vector,0);
        }
        if(ui->asf->isChecked())
        {
            for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
            {
                for (int j = 0; j < main_data.h_asf.joints[i].children.size(); ++j)
                {
                      temp_parent.m_position=QVector3D(main_data.h_asf.joints[i].cords[frame].x,main_data.h_asf.joints[i].cords[frame].y,main_data.h_asf.joints[i].cords[frame].z);
                      int child_index=main_data.h_asf.joints[i].child_index_joint[j];
                      joint_asf join=main_data.h_asf.joints[child_index];
                      float x=main_data.h_asf.joints[child_index].cords[frame].x;//tu?
                      float y=main_data.h_asf.joints[child_index].cords[frame].y;
                      float z=main_data.h_asf.joints[child_index].cords[frame].z;
                      temp_child.m_position=QVector3D(x,y,z);
                      full_vertex_vector.push_back(temp_parent);
                      full_vertex_vector.push_back(temp_child);
                }
            }
            ui->openGLWidget_2->refresh(full_vertex_vector,0);
        }
    }




}

void MainWindow::frame_iter()
{
    if (ui->bvh->isChecked())
    {
        if (main_data.h_bvh.joints.size()>0)
        {
            frame++;
            if (frame>=main_data.h_bvh.joints[0].cords.size())
            {
                frame=0;
                timer->stop();
            }
            draw();
        }
    }
    if(ui->asf->isChecked())
    {
        if (main_data.h_asf.joints.size()>0)
        {
            frame++;
            if (frame>=main_data.h_asf.joints[0].cords.size())
            {
                frame=0;
                timer->stop();
            }
            draw();
        }
    }
    ui->slider->setValue(frame);

}

//void MainWindow::t_out()
//{
//    //qDebug()<<"tout"<<"\n";
//}

//void MainWindow::test_matrix(QMatrix4x4 matrix)
//{
//    //qDebug()<<matrix<<"\n";
//}

//void MainWindow::setup_table()
//{
//    ui->tableWidget = new QTableWidget(this);

//}

void MainWindow::fill_table()
{
    ui->tableWidget->clear();

    if (ui->bvh->isChecked())
    {
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setRowCount(main_data.h_bvh.joints[0].cords.size());
        int index;
        for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
        {
            if (main_data.h_bvh.joints[i].name==ui->comboBox_2->currentText().toStdString())
            {
                index=i;
                break;
            }
        }

        QTableWidgetItem *item = new QTableWidgetItem(QString(""));
        //ui->tableWidget->setItem(0,0,item);
        item = new QTableWidgetItem(QString("Position X"));
        ui->tableWidget->setHorizontalHeaderItem(0,item);
        item = new QTableWidgetItem(QString("Position Y"));
        ui->tableWidget->setHorizontalHeaderItem(1,item);
        item = new QTableWidgetItem(QString("Position Z"));
        ui->tableWidget->setHorizontalHeaderItem(2,item);
        item = new QTableWidgetItem(QString("Angle X"));
        ui->tableWidget->setHorizontalHeaderItem(3,item);
        item = new QTableWidgetItem(QString("Angle Y"));
        ui->tableWidget->setHorizontalHeaderItem(4,item);
        item = new QTableWidgetItem(QString("Angle Z"));
        ui->tableWidget->setHorizontalHeaderItem(5,item);

        for (int i = 0; i < main_data.h_bvh.joints[index].cords.size(); ++i) //dane z konkretnego wybranego joint
        {
           //item = new QTableWidgetItem(QString::number(i));
            //ui->tableWidget->setItem(i+1,0,item);
            float tx,ty,tz,ax,ay,az;

//            tx=qRound(main_data.h_bvh.joints[index].cords[i].x*10000)/10000;
//            ty=qRound(main_data.h_bvh.joints[index].cords[i].y*10000)/10000;
//            tz=qRound(main_data.h_bvh.joints[index].cords[i].z*10000)/10000;
//            ax=qRound(main_data.h_bvh.joints[index].angles[i].x*10000)/10000;
//            ay=qRound(main_data.h_bvh.joints[index].angles[i].y*10000)/10000;
//            az=qRound(main_data.h_bvh.joints[index].angles[i].z*10000)/10000;
            tx=main_data.h_bvh.joints[index].cords[i].x;
            ty=main_data.h_bvh.joints[index].cords[i].y;
            tz=main_data.h_bvh.joints[index].cords[i].z;
            ax=main_data.h_bvh.joints[index].angles[i].x;
            ay=main_data.h_bvh.joints[index].angles[i].y;
            az=main_data.h_bvh.joints[index].angles[i].z;



            item = new QTableWidgetItem(QString::number(tx));
            ui->tableWidget->setItem(i,0,item);
            item = new QTableWidgetItem(QString::number(ty));
            ui->tableWidget->setItem(i,1,item);
            item = new QTableWidgetItem(QString::number(tz));
            ui->tableWidget->setItem(i,2,item);
            item = new QTableWidgetItem(QString::number(ax));
            ui->tableWidget->setItem(i,3,item);
            item = new QTableWidgetItem(QString::number(ay));
            ui->tableWidget->setItem(i,4,item);
            item = new QTableWidgetItem(QString::number(az));
            ui->tableWidget->setItem(i,5,item);
        }


    }

    if (ui->asf->isChecked())
    {
        ui->tableWidget->setColumnCount(6);
        ui->tableWidget->setRowCount(main_data.h_asf.joints[0].cords.size());
        int index;
        for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
        {
            if (main_data.h_asf.joints[i].name==ui->comboBox_2->currentText().toStdString())
            {
                index=i;
                break;
            }
        }

        QTableWidgetItem *item = new QTableWidgetItem(QString(""));
        //ui->tableWidget->setItem(0,0,item);
        item = new QTableWidgetItem(QString("Position X"));
        ui->tableWidget->setHorizontalHeaderItem(0,item);
        item = new QTableWidgetItem(QString("Position Y"));
        ui->tableWidget->setHorizontalHeaderItem(1,item);
        item = new QTableWidgetItem(QString("Position Z"));
        ui->tableWidget->setHorizontalHeaderItem(2,item);
        item = new QTableWidgetItem(QString("Angle X"));
        ui->tableWidget->setHorizontalHeaderItem(3,item);
        item = new QTableWidgetItem(QString("Angle Y"));
        ui->tableWidget->setHorizontalHeaderItem(4,item);
        item = new QTableWidgetItem(QString("Angle Z"));
        ui->tableWidget->setHorizontalHeaderItem(5,item);

        for (int i = 0; i < main_data.h_asf.joints[index].cords.size(); ++i) //dane z konkretnego wybranego joint
        {
            //item = new QTableWidgetItem(QString::number(i));
            //ui->tableWidget->setItem(i+1,0,item);
            item = new QTableWidgetItem(QString::number(main_data.h_asf.joints[index].cords[i].x));
            ui->tableWidget->setItem(i,0,item);
            item = new QTableWidgetItem(QString::number(main_data.h_asf.joints[index].cords[i].y));
            ui->tableWidget->setItem(i,1,item);
            item = new QTableWidgetItem(QString::number(main_data.h_asf.joints[index].cords[i].z));
            ui->tableWidget->setItem(i,2,item);
            item = new QTableWidgetItem(QString::number(main_data.h_asf.joints[index].angles[i].x));
            ui->tableWidget->setItem(i,3,item);
            item = new QTableWidgetItem(QString::number(main_data.h_asf.joints[index].angles[i].y));
            ui->tableWidget->setItem(i,4,item);
            item = new QTableWidgetItem(QString::number(main_data.h_asf.joints[index].angles[i].z));
            ui->tableWidget->setItem(i,5,item);
        }


    }

    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::graph(string text)
{
    if(ui->bvh->isChecked())
    {

        vector<double> x;
        vector<double> y;
        vector<double> z;
        vector<double> ax;
        vector<double> ay;
        vector<double> az;
        vector<double> time;

        int j=0; //index wzywanego
        while(main_data.h_bvh.joints[j].name!=text)
        {
            j++;
        }
        for (int i = 0; i < main_data.h_bvh.joints[j].cords.size(); ++i)
        {
            x.push_back(main_data.h_bvh.joints[j].cords[i].x);
            y.push_back(main_data.h_bvh.joints[j].cords[i].y);
            z.push_back(main_data.h_bvh.joints[j].cords[i].z);
            //time.push_back(i*main_data.f_bvh.frame_time);
            time.push_back(i);
        }

        for (int a = 0; a < main_data.h_bvh.joints[j].angles.size(); ++a)
        {
            double x,y,z;
            x=main_data.h_bvh.joints[j].angles[a].x;
            y=main_data.h_bvh.joints[j].angles[a].y;
            z=main_data.h_bvh.joints[j].angles[a].z;
            ax.push_back(x);
            ay.push_back(y);
            az.push_back(z);
        }

        ui->customPlot->clearGraphs();
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(x));
        if (ui->positionX->isChecked())
        {
            ui->customPlot->graph(0)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(0)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(y));
        if (ui->positionY->isChecked())
        {
            ui->customPlot->graph(1)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(1)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::green));
        ui->customPlot->graph(2)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(z));
        if (ui->positionZ->isChecked())
        {
            ui->customPlot->graph(2)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(2)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::darkRed));
        ui->customPlot->graph(3)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(ax));
        if (ui->angleX->isChecked())
        {
            ui->customPlot->graph(3)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(3)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::magenta));
        ui->customPlot->graph(4)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(ay));
        if (ui->angleY->isChecked())
        {
            ui->customPlot->graph(4)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(4)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(5)->setPen(QPen(Qt::cyan));
        ui->customPlot->graph(5)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(az));
        if (ui->angleZ->isChecked())
        {
            ui->customPlot->graph(5)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(5)->setVisible(false);
        }

        QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
        ui->customPlot->xAxis->setTicker(fixedTicker);

        fixedTicker->setTickStep(1.0);
        ui->customPlot->xAxis->setLabel("Frame Number");
        ui->customPlot->yAxis->setLabel("x,y,z");
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
        ui->customPlot->replot();


}

    if(ui->asf->isChecked()){

        vector<double> x;
        vector<double> y;
        vector<double> z;
        vector<double> ax;
        vector<double> ay;
        vector<double> az;
        vector<double> time;

        int j=0; //index wzywanego
        while(main_data.h_asf.joints[j].name!=text)
        {
            j++;
        }


        for (int i = 0; i < main_data.h_asf.joints[j].cords.size(); ++i)
        {
            x.push_back(main_data.h_asf.joints[j].cords[i].x);
            y.push_back(main_data.h_asf.joints[j].cords[i].y);
            z.push_back(main_data.h_asf.joints[j].cords[i].z);
            //time.push_back(i*0.03333);
            time.push_back(i);
        }

        for (int a = 0; a <main_data.h_asf.joints[j].angles.size(); ++a)
        {
            ax.push_back(main_data.h_asf.joints[j].angles[a].x);
            ay.push_back(main_data.h_asf.joints[j].angles[a].y);
            az.push_back(main_data.h_asf.joints[j].angles[a].z);

        }


        ui->customPlot->clearGraphs();
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(x));

        if (ui->positionX->isChecked())
        {
            ui->customPlot->graph(0)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(0)->setVisible(false);
        }
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(y));

        if (ui->positionY->isChecked())
        {
            ui->customPlot->graph(1)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(1)->setVisible(false);
        }
        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::green));
        ui->customPlot->graph(2)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(z));
        if (ui->positionZ->isChecked())
        {
            ui->customPlot->graph(2)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(2)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::darkRed));
        ui->customPlot->graph(3)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(ax));
        if (ui->angleX->isChecked())
        {
            ui->customPlot->graph(3)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(3)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::magenta));
        ui->customPlot->graph(4)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(ay));
        if (ui->angleY->isChecked())
        {
            ui->customPlot->graph(4)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(4)->setVisible(false);
        }

        ui->customPlot->addGraph();
        ui->customPlot->graph(5)->setPen(QPen(Qt::cyan));
        ui->customPlot->graph(5)->setData(QVector<double>::fromStdVector(time), QVector<double>::fromStdVector(az));
        if (ui->angleZ->isChecked())
        {
            ui->customPlot->graph(5)->setVisible(true);
        }
        else
        {
            ui->customPlot->graph(5)->setVisible(false);
        }



        ui->customPlot->xAxis->setLabel("Frame Number");
        ui->customPlot->yAxis->setLabel("x,y,z");
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom );
        ui->customPlot->replot();
    }
}

//void MainWindow::tree(QString name)
//{

//}



void MainWindow::showPointToolTip1(QMouseEvent *event){
        double x = ui->customPlot->xAxis->pixelToCoord(event->pos().x());
        double y = ui->customPlot->yAxis->pixelToCoord(event->pos().y());

        QToolTip::showText(ui->customPlot->mapToGlobal(event->pos()),QString("%1 , %2").arg(x).arg(y));
}




//void MainWindow::show_about(QAction *Action)
//{
//    //qDebug()<<"showAbout\n";
//}



void MainWindow::on_positionX_stateChanged(int arg1)
{
    if( ((main_data.h_asf.joints.size()>0 && main_data.f_amc.frames.size()>0)) || main_data.h_bvh.joints.size()>0)
    {
    if (ui->positionX->isChecked()) {
        ui->customPlot->graph(0)->setVisible(true);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else{
        ui->customPlot->graph(0)->setVisible(false);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    }
}

void MainWindow::on_positionY_stateChanged(int arg1)
{
    if( ((main_data.h_asf.joints.size()>0 && main_data.f_amc.frames.size()>0)) || main_data.h_bvh.joints.size()>0)
    {
    if (ui->positionY->isChecked()) {
        ui->customPlot->graph(1)->setVisible(true);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else{
        ui->customPlot->graph(1)->setVisible(false);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    }
}

void MainWindow::on_positionZ_stateChanged(int arg1)
{
    if( ((main_data.h_asf.joints.size()>0 && main_data.f_amc.frames.size()>0)) || main_data.h_bvh.joints.size()>0)
    {
    if (ui->positionZ->isChecked()) {
        ui->customPlot->graph(2)->setVisible(true);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else{
        ui->customPlot->graph(2)->setVisible(false);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    }
}



//void MainWindow::test_QMatrix4x4(){
////QMatrix4x4 matrix(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);

//float tab[4][4];
//QMatrix4x4 matrix;
//QMatrix4x4 invert;
//matrix.setToIdentity();

////matrix.translate(1,2,3);
//matrix.rotate(30,1,0,0);
//matrix.rotate(60,0,1,0);
//matrix.rotate(90,0,0,1);

//    for (int i = 0; i < 4; ++i) {
//        QVector4D vect=matrix.row(i);
//        //qDebug()<<vect.x()<<vect.y()<<vect.z()<<vect.w();
//        tab[i][0]=vect.x();
//        tab[i][1]=vect.y();
//        tab[i][2]=vect.z();
//        tab[i][3]=vect.w();
//    }
////qDebug()<<QString::fromStdString(" ");
//invert=matrix.inverted();
//        for (int i = 0; i < 4; ++i) {
//            QVector4D vect=invert.row(i);
//            //qDebug()<<vect.x()<<vect.y()<<vect.z()<<vect.w();
//            tab[i][0]=vect.x();
//            tab[i][1]=vect.y();
//            tab[i][2]=vect.z();
//            tab[i][3]=vect.w();
//        }
//        //qDebug()<<QString::fromStdString(" ");
//  matrix=invert*matrix;
//  for (int i = 0; i < 4; ++i) {
//      QVector4D vect=matrix.row(i);
//      //qDebug()<<vect.x()<<vect.y()<<vect.z()<<vect.w();
//      tab[i][0]=vect.x();
//      tab[i][1]=vect.y();
//      tab[i][2]=vect.z();
//      tab[i][3]=vect.w();
//  }
//  //test_matrix(matrix);


//}

void MainWindow::on_actionLoad_triggered()
{
    QString desktop_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString file_name = QFileDialog::getOpenFileName(this,"Open file",desktop_path,"*.bvh ;; *.asf"); //okno wczytania
    //QMessageBox::information(this,"..",file_name);

    if(file_name=="")//sprawdzenie czy nie jest pusty
    {
        return;
    }

    QFileInfo file_info(file_name);
    QString extension=file_info.suffix();
    fstream file;
    file.open((file_name.toStdString()).c_str());

    if(extension=="bvh") // pliki z rozszerzeniem bvh
    {
        qDebug()<<"0\n";
        return_bvh_hierarchy_motion r_bvh;
        qDebug()<<"1\n";

        r_bvh=main_data.create_bvh(file);
        qDebug()<<"2\n";

        if(r_bvh.exception.code!=0)
        {
            QString str=QString::fromStdString(r_bvh.exception.msg);
            str.append(" ");
            str.append(QString::number(r_bvh.exception.code));
            QMessageBox::information(this,"..",str);
            return;
        }
        //QMessageBox::information(this,"..","bvh");


        main_data.h_bvh=r_bvh.h_bvh;
        main_data.f_bvh=r_bvh.f_bvh;;
        file.close();

        //main_data.calc_bvh_pos_angle(main_data.h_bvh,main_data.f_bvh,ui->comboBox_3->currentText().toStdString()); //policzenie wszystkich położeń (stare)
        //main_data.calc_bvh_primary_rot_order_cords_angles(main_data.h_bvh,main_data.f_bvh);  //versja 2.0
        main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);

        ui->bvh->setChecked(true);
        ui->asf->setChecked(false);
        ui->comboBox->blockSignals(true);
         ui->comboBox_2->blockSignals(true);
            ui->comboBox->clear();
            ui->comboBox_2->clear();


             for (int i = 0; i < main_data.h_bvh.joints.size(); ++i)
             {
                ui->comboBox->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
                ui->comboBox_2->addItem(QString::fromStdString(main_data.h_bvh.joints[i].name));
             }

             ui->comboBox->blockSignals(false);
            ui->comboBox_2->blockSignals(false);

            ui->treeWidget->clear();
            QString root_name = QString::fromStdString(main_data.h_bvh.root.name);

            AddRoot_bvh(QString::fromStdString(main_data.h_bvh.root.name));
            ui->treeWidget->expandAll();
            on_comboBox_currentTextChanged(ui->comboBox->currentText());
            int s_max=main_data.f_bvh.frames.size();
            ui->slider->setMaximum(s_max);
            //setup_table();
            fill_table();
            ui->cylinders->setChecked(true);
            on_cylinders_clicked();

            double f=main_data.f_bvh.frame_time;
            interval=f*1000;
            timer->setInterval(interval);
            fps=1/f;
            ui->frames_second->setValue(fps);

//            draw();
           // stop=true;

    }
    if(extension=="asf"){
        //QMessageBox::information(this,"..","asf");
                //        fstream file;
                //        file.open((file_name.toStdString()).c_str());
        //qDebug()<<"start\n";
       return_asf_hierarchy_and_error r_asf;
       r_asf=main_data.create_asf(file);

       if(r_asf.exception.code!=0)
       {
           QString str=QString::fromStdString(r_asf.exception.msg);
           str.append(" ");
           str.append(QString::number(r_asf.exception.code));
           QMessageBox::information(this,"..",str);
           return;
       }
        main_data.h_asf=r_asf.h_asf;

        file.close();
        QMessageBox::information(this,"..","asf");
        //qDebug()<<"wczyt asf udany\n";
    //    ui->comboBox->clear();
    //    ui->comboBox_2->clear();
    //    for (int i = 0 ; i < main_data.h_asf.joints.size(); ++i) {
    //        qDebug()<<"loadASF\n";
    //        ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
    //        ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
    //    }
    //    ui->bvh->setChecked(false);
    //    ui->asf->setChecked(true);
    //    ui->treeWidget->clear();
    //    AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
    //    ui->treeWidget->expandAll();

        ui->bvh->setChecked(false);
        ui->asf->setChecked(true);
        ui->comboBox->blockSignals(true);
        ui->comboBox_2->blockSignals(true);
        ui->comboBox->clear();
        ui->comboBox_2->clear();

        //qDebug()<<"widgety1";

         for (int i = 0; i < main_data.h_asf.joints.size(); ++i)
        {
             ui->comboBox->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
             ui->comboBox_2->addItem(QString::fromStdString(main_data.h_asf.joints[i].name));
        }
            ui->comboBox->blockSignals(false);
            ui->comboBox_2->blockSignals(false);   //poprawić wczytywani bo inaczej crush

            ui->treeWidget->clear();
            //qDebug()<<QString::fromStdString(main_data.h_asf.root.name);
            AddRoot_asf(QString::fromStdString(main_data.h_asf.root.name));
            ui->treeWidget->expandAll();

            //qDebug()<<"widgety2";

            QString file_name = QFileDialog::getOpenFileName(this,"Open file",desktop_path,"*.amc"); //pobranie nazwy pliku (ścieżka)
            //QMessageBox::information(this,"..",file_name);
            if(file_name==""){
                return;
            }
            fstream file1;
            file1.open((file_name.toStdString()).c_str());
            main_data.f_amc=main_data.create_frames_asf(file1,main_data.h_asf);
            file1.close();
            QMessageBox::information(this,"..","amc");
            //calc_positions_asf(main_data.h_asf,main_data.f_amc);
            //main_data.calc_asf_pos_angle(main_data.h_asf,main_data.f_amc,ui->comboBox_3->currentText().toStdString());
            main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
            QMessageBox::information(this,"..","pc_index");
            main_data.asf_position(main_data.h_asf,main_data.f_amc);
            QMessageBox::information(this,"..","position");
            on_comboBox_currentTextChanged(ui->comboBox->currentText());
            //setup_table();
            fill_table();
            ui->cylinders->setChecked(true);
            on_cylinders_clicked();
            int s_max=main_data.f_amc.frames.size();
            ui->slider->setMaximum(s_max);
            //main_data.asf_pc_index(main_data.h_asf,main_data.f_amc);
            //qDebug()<<"start\n";
            interval=0.03*1000;
            timer->setInterval(interval);
            fps=1/0.03;
            ui->frames_second->setValue(fps);

//            draw();
         //   stop=true;

    }
}

void MainWindow::on_actionSave_triggered()
{
    QString desktop_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString file_name = QFileDialog::getSaveFileName(this,"Save file",desktop_path,"*.bvh ;; *.asf"); //pobranie nazwy pliku (ścieżka)
    //QMessageBox::information(this,"..",file_name);
    if(file_name==""){
        return;
    }
    QFileInfo file_info(file_name);
    QString extension=file_info.suffix();
    if(extension=="bvh"){
        //QMessageBox::information(this,"..","bvh");



        if (ui->bvh->isChecked())
        {//bvh
            if (main_data.h_bvh.joints.size()>0)
            {
                ofstream file;
                file.open((file_name.toStdString()).c_str());
                return_bvh_hierarchy_motion r_bvh;
                //main_data.bvh_reorder(main_data.h_bvh,main_data.f_bvh,ui->comboBox_3->currentText().toStdString()); reorder przed zapisem
                main_data.write_bvh(file,main_data.h_bvh,main_data.f_bvh);
                file.close();
            }
        }
        if (ui->asf->isChecked())
        {//asf
            if (main_data.h_asf.joints.size()>0)
            {
                return_bvh_hierarchy_motion r_bvh;
                ofstream file;
                file.open((file_name.toStdString()).c_str());
                //r_asf=main_data.asf_reorder(main_data.h_asf,main_data.f_amc,ui->comboBox_3->currentText().toStdString()); //reorder przed konwersją i zapisem
                //double test =r_asf.f_amc.frames[0].channel_data[2].channels[2];
                r_bvh= main_data.asf_to_bvh_conversion(main_data.h_asf,main_data.f_amc);
                //test=r_bvh.f_bvh.frames[0].channel_data[2].channels[2];
                main_data.write_bvh(file,r_bvh.h_bvh,r_bvh.f_bvh);
                file.close();
            }
        }
    }
    if(extension=="asf"){
        //QMessageBox::information(this,"..","asf");


        if (ui->bvh->isChecked()) {//bvh
            if (main_data.h_bvh.joints.size()>0) {
            return_asf r_asf;
            return_bvh_hierarchy_motion r_bvh;
            ofstream file;
            file.open((file_name.toStdString()).c_str());
            //qDebug()<<"start\n";
            //main_data.bvh_reorder(main_data.h_bvh,main_data.f_bvh,ui->comboBox_3->currentText().toStdString()); //reorder przed zapisem i konwersją

            r_asf= main_data.bvh_to_asf_conversion(main_data.h_bvh,main_data.f_bvh);
           // string t=r_asf.h_asf.joints[2].name;
             //qDebug()<<"conv\n";
            main_data.write_asf(file,r_asf.h_asf,r_asf.f_amc);
             //qDebug()<<"zapis asf\n";
            file.close();
            file_name.replace(".asf",".amc");
            //QMessageBox::information(this,"..",file_name);
            file.open((file_name.toStdString()).c_str());

            main_data.write_frames_asf(file,r_asf.h_asf,r_asf.f_amc);
             //qDebug()<<"zapis amc\n";
            file.close();
        }
        }
        if (ui->asf->isChecked()) {//asf
            if (main_data.h_asf.joints.size()>0) {

            ofstream file;
            return_asf r_asf;
            file.open((file_name.toStdString()).c_str());
            //r_asf=main_data.asf_reorder(main_data.h_asf,main_data.f_amc,ui->comboBox_3->currentText().toStdString()); //reorder przed zapisem
            main_data.write_asf(file,main_data.h_asf,main_data.f_amc);
            file.close();
            file_name.replace(".asf",".amc");
           // QMessageBox::information(this,"..",file_name);
            file.open((file_name.toStdString()).c_str());
            main_data.write_frames_asf(file,main_data.h_asf,main_data.f_amc);
            file.close();
        }
        }
    }

}

//void MainWindow::on_button_offset_apply_clicked()
//{

//}

void MainWindow::on_angleX_stateChanged(int arg1)
{
    if( (main_data.h_asf.joints.size()>0 && main_data.f_amc.frames.size()>0) || (main_data.h_bvh.joints.size()>0)){
    if (ui->angleX->isChecked()) {
        ui->customPlot->graph(3)->setVisible(true);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else{
        ui->customPlot->graph(3)->setVisible(false);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    }
}

void MainWindow::on_angleY_stateChanged(int arg1)
{
    if( (main_data.h_asf.joints.size()>0 && main_data.f_amc.frames.size()>0) || (main_data.h_bvh.joints.size()>0)){
    if (ui->angleY->isChecked()) {
        ui->customPlot->graph(4)->setVisible(true);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else{
        ui->customPlot->graph(4)->setVisible(false);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    }
}

void MainWindow::on_angleZ_stateChanged(int arg1)
{
    if( (main_data.h_asf.joints.size()>0 && main_data.f_amc.frames.size()>0) || (main_data.h_bvh.joints.size()>0)){
    if (ui->angleZ->isChecked()) {
        ui->customPlot->graph(5)->setVisible(true);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else{
        ui->customPlot->graph(5)->setVisible(false);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    }
}

//void MainWindow::on_positionY_clicked()
//{

//}

void MainWindow::on_actionAbout_triggered()
{
    QString str("Twórca: Wojciech Liszka\n"
                "Email: liszkawojciech92@gmail.com\n"
                "\n"
                "Promotor: dr.Tomasz Krzeszowski\n"
                "\n"
                "Program ten został stworzony w celu wizualizacji, analizy i konwersji danych\n"
                "z systemu Motion Capture i jest częścią pracy inżynierskiej.\n"
                "Program umożliwia analizę, położenia i kątów obrotu, kości oraz stawów\n"
                "należących do szkieletu.\n"
                "Formaty wspierane przez program to '.bvh' oraz '.asf/.amc'.\n"
                "Aplikacja umożliwia edycję kątów skierowania i długośći poszczególnych\n"
                "elementów hierarchii.\n"
                "Zmiany można wprowadzić po przez formularz po lewej stronie okna głównego\n."
                "Dodawanie, usuwanie i zmiana nazwy elementów hierarhii zrealizowane są\n"
                "po przez drzewo hierarhii umieszczone po prawej stronie okna głównego.\n"
                "Pole grafu umożliwia wyświtlenie współrzędnych x,y,z oraz kątów obrotu\n"
                "wokół osi x,y,z dla wybranej części szkieletu oraz przy określonej kolejności\n"
                "rotacji.\n"
                "Animacja umożliwia wizualizację poruszania się danego szkieletu zgodnie z danymi\n"
                "pochodzącymi z plików systemu motion capture."
                );
    QMessageBox::about(this,"About",str);
}

//void MainWindow::on_horizontalSlider_2_valueChanged(int value) {
//}

void MainWindow::on_cylinders_clicked()
{
    //ui->lines->setChecked(false);
    ui->openGLWidget_2->lines=false;
    ui->openGLWidget_2->cylinders=true;
    draw();


}

void MainWindow::on_lines_clicked()
{
    //ui->cylinders->setChecked(false);
    ui->openGLWidget_2->cylinders=false;
    ui->openGLWidget_2->lines=true;
    draw();
}

QVector<Vertex>  cyl(QVector3D vec, QVector3D up, QVector3D off, QVector3D color)
{

    QVector<Vertex> full_vertex_vector;
    QVector<QVector3D> v1;
    QVector<QVector3D> v0;

    for (int i = 0; i < 36; ++i)
    {
        QMatrix4x4 mat;
        mat.rotate(i*10,0,1,0);
        QVector3D vec1=mat*vec;
        v0.push_back(vec1 + off);
        v1.push_back(vec1+up + off);
    }
    for (int i = 0; i < 36; ++i)
    {
        int n=i;
        int m=i+1;
        if (m>=36)
        {
            m=0;
        }
        Vertex p_n(v0[n], color); //QVector3D( 0.0f,1.0f,0.0f));
        Vertex p_m(v0[m], color); //QVector3D( 0.0f,1.0f,0.0f));
        Vertex c_n(v1[n], color); //QVector3D( 0.0f,1.0f,0.0f));
        Vertex c_m(v1[m], color); //QVector3D( 0.0f,1.0f,0.0f));

        Vertex p_n1(v0[n], color); //QVector3D( 1.0f,0.0f,0.0f));
        Vertex p_m1(v0[m], color); //QVector3D( 1.0f,0.0f,0.0f));
        Vertex c_n1(v1[n], color); //QVector3D( 1.0f,0.0f,0.0f));
        Vertex c_m1(v1[m], color); //QVector3D( 1.0f,0.0f,0.0f));

        //_cc
#if 0
        full_vertex_vector.push_back(c_n);
        full_vertex_vector.push_back(p_n);
        full_vertex_vector.push_back(p_m);

        full_vertex_vector.push_back(c_m);
        full_vertex_vector.push_back(c_n);
        full_vertex_vector.push_back(p_m);
#endif
        //_c
#if 1
        full_vertex_vector.push_back(c_n1);
        full_vertex_vector.push_back(p_m1);
        full_vertex_vector.push_back(p_n1);

        full_vertex_vector.push_back(c_m1);
        full_vertex_vector.push_back(p_m1);
        full_vertex_vector.push_back(c_n1);
    #endif
    }
    return     full_vertex_vector;
}
//void MainWindow::cylinder()
//{
//    auto vec1 = cyl(QVector3D(0.1,0,0),
//                    QVector3D(0,0.75,0),
//                    QVector3D(0,0,0),
//                    QVector3D(1,0,0)
//                    );
//    auto vec2 = cyl(QVector3D(0.1,0,0),
//                    QVector3D(0,0.5,0),
//                    QVector3D(0.5,0.5,0),
//                    QVector3D(0,1,0));
////vec1.append(vec2);
//    Vertex vert1 = Vertex( QVector3D(  1000.0f,  0.0f,  0.0f), QVector3D( 1.0f, 0.0f, 0.0f ));
//    Vertex vert2 = Vertex( QVector3D(  -1000.0f,  0.0f,  0.0f), QVector3D( 1.0f, 0.0f, 0.0f ));

//    Vertex vert3 = Vertex( QVector3D(  0.0f,  0.0f,  1000.0f), QVector3D( 0.0f, 0.0f, 1.0f ));
//    Vertex vert4 = Vertex( QVector3D(  0.0f,  0.0f,  -1000.0f), QVector3D( 0.0f, 0.0f, 1.0f ));

//    QVector <Vertex> full_vertex_vector;
//    full_vertex_vector.push_back(vert1);
//    full_vertex_vector.push_back(vert2);

//    full_vertex_vector.push_back(vert3);
//    full_vertex_vector.push_back(vert4);

//    full_vertex_vector.append(vec1);
//    full_vertex_vector.append(vec2);
//    ui->openGLWidget_2->refresh(full_vertex_vector,0);

//}

//void MainWindow::on_line_len_textChanged(const QString &arg1)
//{

//}

void MainWindow::on_slider_sliderPressed()
{
    int position=ui->slider->value();
    frame=position;
    draw();
}



void MainWindow::on_actionthrow_asf_triggered()
{
    QString desktop_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString file_name = QFileDialog::getSaveFileName(this,"Save file",desktop_path,"*.txt"); //pobranie nazwy pliku (ścieżka)
    QMessageBox::information(this,"..",file_name);
    if(file_name==""){
        return;
    }
    ofstream file;
    file.open((file_name.toStdString()).c_str());

    for (int f = 0; f < main_data.h_asf.joints[0].cords.size(); ++f)
    {
        file<<"frame "<<f<<"\n";
        for (int j = 0; j < main_data.h_asf.joints.size(); ++j)
        {
            file<<"name "<<main_data.h_asf.joints[j].name<<"\n";
            file<<"position x "<<main_data.h_asf.joints[j].cords[f].x<<", y "<<main_data.h_asf.joints[j].cords[f].y<<", z "<<main_data.h_asf.joints[j].cords[f].z<<"\n";
            file<<"angles x "<<main_data.h_asf.joints[j].angles[f].x<<", y "<<main_data.h_asf.joints[j].angles[f].y<<" , z "<<main_data.h_asf.joints[j].angles[f].z<<"\n";

            file<<"self-matrix\n";
            QMatrix4x4 matrix0=main_data.h_asf.joints[j].self_matrix[f];
            for (int x = 0; x <= 3; ++x)
            {
                QVector4D vec=matrix0.row(x);
                file<<vec.x()<<"\t"<<vec.y()<<"\t"<<vec.z()<<"\t"<<vec.w()<<"\n";
            }


            file<<"to-point matrix\n";
            QMatrix4x4 matrix1=main_data.h_asf.joints[j].vector_to_position[f];
            for (int x = 0; x <= 3; ++x)
            {
                QVector4D vec=matrix1.row(x);
                file<<vec.x()<<"\t"<<vec.y()<<"\t"<<vec.z()<<"\t"<<vec.w()<<"\n";
            }

            file<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<"\n";
        }
        file<<"-----------------------------------------------------------------------------------------------------------------------------"<<"\n";
    }
    file.close();
}

void MainWindow::on_actionthrow_bvh_triggered()
{
    QString desktop_path=QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString file_name = QFileDialog::getSaveFileName(this,"Save file",desktop_path,"*.txt"); //pobranie nazwy pliku (ścieżka)
    QMessageBox::information(this,"..",file_name);
    if(file_name==""){
        return;
    }
    ofstream file;
    file.open((file_name.toStdString()).c_str());

    for (int f = 0; f < main_data.h_bvh.joints[0].cords.size(); ++f)
    {
        file<<"frame "<<f<<"\n";
        for (int j = 0; j < main_data.h_bvh.joints.size(); ++j)
        {
            file<<"name "<<main_data.h_bvh.joints[j].name<<"\n";
            file<<"position x "<<main_data.h_bvh.joints[j].cords[f].x<<", y "<<main_data.h_bvh.joints[j].cords[f].y<<", z "<<main_data.h_bvh.joints[j].cords[f].z<<"\n";
            file<<"angles x "<<main_data.h_bvh.joints[j].angles[f].x<<", y "<<main_data.h_bvh.joints[j].angles[f].y<<" , z "<<main_data.h_bvh.joints[j].angles[f].z<<"\n";

            file<<"self-matrix\n";
            QMatrix4x4 matrix0=main_data.h_bvh.joints[j].self_matrix[f];
            for (int x = 0; x <= 3; ++x)
            {
                QVector4D vec=matrix0.row(x);
                file<<vec.x()<<"\t"<<vec.y()<<"\t"<<vec.z()<<"\t"<<vec.w()<<"\n";
            }


            file<<"to-point matrix\n";
            QMatrix4x4 matrix1=main_data.h_bvh.joints[j].vector_to_position[f];
            for (int x = 0; x <= 3; ++x)
            {
                QVector4D vec=matrix1.row(x);
                file<<vec.x()<<"\t"<<vec.y()<<"\t"<<vec.z()<<"\t"<<vec.w()<<"\n";
            }

            file<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<"\n";
        }
        file<<"-----------------------------------------------------------------------------------------------------------------------------"<<"\n";
    }
    file.close();
}

void MainWindow::on_offsets_clicked()
{

    if(ui->bvh->isChecked())
    {
        if(main_data.h_bvh.joints.size()<=0)
        {
            return;
        }
//        int temp;
//        for (int i = 0; i <main_data.h_bvh.joints.size()  ; ++i)
//        {
//            if (main_data.h_bvh.joints[i].name==ui->comboBox_2->currentText().toStdString())
//            {
//                temp=i;
//                break;
//            }
//        }
        main_data.change_offset_bvh(main_data.h_bvh,ui->comboBox_3->currentText().toStdString(),ui->line_x->text().toDouble(),ui->line_y->text().toDouble(),ui->line_z->text().toDouble());
//        main_data.h_bvh.joints[temp].offset[0]=ui->line_x->text().toDouble();
//        main_data.h_bvh.joints[temp].offset[1]=ui->line_y->text().toDouble();
//        main_data.h_bvh.joints[temp].offset[2]=ui->line_z->text().toDouble();
        //main_data.calc_bvh_primary_rot_order_cords_angles(main_data.h_bvh,main_data.f_bvh);
//        main_data.bvh_reorder(main_data.h_bvh,main_data.f_bvh,ui->comboBox_3->currentText().toStdString());
        main_data.bvh_positions(main_data.h_bvh,main_data.f_bvh);
        fill_table();
        on_comboBox_currentTextChanged(ui->comboBox->currentText());
        draw();
        //przerysuj
        }



    if(ui->asf->isChecked())
    {
        if(main_data.h_asf.joints.size()<=0)
        {
            return;
        }
        int temp;
        for (int i = 0; i <main_data.h_asf.joints.size()  ; ++i)
        {
            if (main_data.h_asf.joints[i].name==ui->comboBox_2->currentText().toStdString())
            {
                temp=i;
                break;
            }
        }
        double x,y,z;

//        l=ui->line_len->text().toDouble();
        x=ui->line_x->text().toDouble();
        y=ui->line_y->text().toDouble();
        z=ui->line_z->text().toDouble();


//        if (l>=0&&x<=1&&x>=-1&&y<=1&&y>=-1&&z<=1&&z>=-1)
//            {
//                main_data.h_asf.joints[temp].length=l;
//                main_data.h_asf.joints[temp].direction[0]=x;
//                main_data.h_asf.joints[temp].direction[1]=y;
//                main_data.h_asf.joints[temp].direction[2]=z;
//            }
        //przerysuj
        main_data.change_offset_asf(main_data.h_asf,ui->comboBox_3->currentText().toStdString(),x,y,z);
        main_data.asf_position(main_data.h_asf,main_data.f_amc);
        fill_table();
        on_comboBox_currentTextChanged(ui->comboBox->currentText());
        draw();
    }
}

void MainWindow::on_stop_button_clicked()
{
    frame=0;
    timer->stop();
    ui->slider->setValue(0);
    draw();
}



void MainWindow::on_frames_second_sliderPressed()
{
    interval=1/ui->frames_second->value()*1000;
    timer->setInterval(interval);
    fps=ui->frames_second->value();
    QString text="frames per second ";
    text.append(QString::number(fps));
    ui->fps->setText(text);
}

void MainWindow::on_frames_second_sliderMoved(int position)
{
    interval=1/ui->frames_second->value()*1000;
    timer->setInterval(interval);
    fps=ui->frames_second->value();
    QString text="frames per second ";
    text.append(QString::number(fps));
    ui->fps->setText(text);
}

void MainWindow::on_frames_second_sliderReleased()
{
    interval=1/ui->frames_second->value()*1000;
    timer->setInterval(interval);
    fps=ui->frames_second->value();
    QString text="frames per second ";
    text.append(QString::number(fps));
    ui->fps->setText(text);
}
