#ifndef DATA_H
#define DATA_H
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector>
#include <QQuaternion>
using std::string;
using std::vector;
using std::ofstream;
using std::fstream;
using std::istringstream;
using std::reverse;





struct translations
{
    float x=0;
    float y=0;
    float z=0;
};

struct cords{
    double x=0;
    double y=0;
    double z=0;
};
struct angles{
    double x=0;
    double y=0;
    double z=0;
};
struct jfc
{
    vector<vector<cords>> data;
};



struct joint_asf {
    int id;
    string name;
    vector<double> direction;
    double length =0;
    vector<double> axis;
    vector<char> axis_order;
    vector<string> dof;
    int dof_num;
    vector<double> limits;
    vector<string> children;
    string parent;
    vector<cords> cords;
    vector<angles> angles;
    QMatrix4x4 transformation_matrix;
    int self_index_frame;
    int self_index_joint;
    int parent_index_joint;
    vector<int> child_index_joint;
    QVector<QMatrix4x4> point_to_world_matrices;
    QVector<QMatrix4x4> local_matrix;
    //bool dummy_node= false;
    QVector<QMatrix4x4> vector_to_position;
    vector <translations> trans;
    QMatrix4x4 C;
    QMatrix4x4 Cinv;
    QVector<QMatrix4x4> self_matrix;



};
struct root_asf {
    vector<string> order;
    vector<double> position;
    vector<double> orientation;

    vector<char> axis;
    int id;
    vector<string> children;
    string name;
    string parent;
    vector<angles> angles;
    vector<cords> cords;
    int self_index_frame;
    int parent_index_joint;
    vector<int> child_index_joint;
    int self_index_joint;
    QVector<QMatrix4x4> point_to_world_matrices;
    vector <translations> trans;
    QVector<QMatrix4x4> vector_to_position;




};


struct info_asf {
    double mass =1;
    double length =1;
     bool angle ; //if true then deg, if false then rad
    string name = "asf";
    string version ="0";

};
struct hierarchy_asf
{
    vector<joint_asf> joints;
    root_asf root;
    info_asf info;

};
struct joint_data
{
    string name;
    int id;
    vector <double> channels;

};
struct frame
{
    int num;
    vector <joint_data> channel_data;

};
struct frames_data {
    vector <frame> frames;
    int frames_count;
    float frame_time;
};





struct joint_bvh {
    int id;
    int depth;
    string name;
    vector<double> offset;
    vector<string> channels;
    int channel_count;
    string parent;
    vector<string> children;
    //vector<double> end_site_offset;
    vector<cords> cords;
    vector<angles> angles;
    QMatrix4x4 transformation_matrix;
    vector<int> child_index_joint;
    int parent_index_joint;
    int self_index_frame;
    int self_index_joint;
    QVector<QMatrix4x4> global_matrix;
    QVector<QMatrix4x4> local_transformation_matrix;
    vector <translations> trans;
    QVector<QMatrix4x4> vector_to_position;
    QVector<QMatrix4x4> self_matrix;


};


struct root_bvh {
    int id;
    string name;
    int depth;
    vector<double> start_cords;
    vector<string> channel_names;
    int channel_count;
    string parent;
    vector<string> children;
    vector<angles> angles;
    vector<cords> cords;
    vector<int> child_index_joint;
    int child_index_parent;
    int self_index_frame;
    int self_index_joint;
    QMatrix4x4 transformation_matrix;
    QVector<QMatrix4x4> point_to_world_matrices;
    QVector<QMatrix4x4> vector_to_position;



};
struct hierarchy_bvh {
    vector<joint_bvh> joints;
    root_bvh root;
};

struct ret_r_j
{
    hierarchy_bvh hierarchy;
    string child;
};

struct return_bvh {
    hierarchy_bvh h_bvh;
    frames_data f_bvh;
};
struct return_asf {
    hierarchy_asf h_asf;
    frames_data f_amc;
};

class Data
{
public:
    Data();

public:
    hierarchy_asf h_asf;
    frames_data f_amc;
    hierarchy_bvh h_bvh;
    frames_data f_bvh;
    //jfc bvh_joint_frame_cordinant;
   // jfc asf_joint_frame_cordinant;
  //  cords rotate(double c_x,double c_y,double c_z,string name,int t1,int t2,int i,hierarchy_bvh h_bvh,frames_data f_bvh);
    //cords translate(double c_x,double c_y,double c_z,int i,hierarchy_bvh h_bvh,frames_data f_bvh);
    return_bvh asf_to_bvh(hierarchy_asf &h_asf, frames_data &f_asf);//i zapisy
    return_asf bvh_to_asf(hierarchy_bvh &h_bvh, frames_data &f_bvh);//

    return_asf bvh_to_asf_conversion(hierarchy_bvh &h_bvh, frames_data &f_bvh);
    return_bvh asf_to_bvh_conversion(hierarchy_asf &h_asf, frames_data &f_amc);


    frames_data create_frames_asf(fstream &file,hierarchy_asf &h_asf);
    frames_data create_frames_bvh(hierarchy_bvh &h_bvh, fstream &file, string &text);

    hierarchy_asf create_asf(fstream& file);
    return_bvh create_bvh(fstream &file);
    hierarchy_bvh create_root_bvh(fstream& file,int &id, string &text);
    root_asf create_root_asf(fstream& file);

    ret_r_j create_joint_bvh(string parent, hierarchy_bvh &hierarchy, fstream& file,int &id,int fl, string &text);
    joint_asf create_joints_asf(fstream& file);

    void write_bvh(ofstream& bvh, hierarchy_bvh &h_bvh, frames_data &f_bvh);
    void write_joint_bvh(ofstream& file, hierarchy_bvh &h_bvh, int id);
    void add_joint_to_write_list(vector<int> &index_list, vector<int> &frame_index, hierarchy_bvh &h_bvh, frames_data &f_bvh); //wypisanie w ospowiedniej kolejności danych z frames,
    void add_joint_to_write_list_child(vector<int> &index_list, vector<int> &frame_index, hierarchy_bvh &h_bvh, frames_data &f_bvh, int joint_index);// ważne bo musi sie zgadzać  kolejność z hierarchią

    void write_asf(ofstream& asf, hierarchy_asf &h_asf, frames_data &f_asf);
    void write_joint_asf(ofstream& file, hierarchy_asf &h_asf);
    void write_frames_asf(ofstream& file, hierarchy_asf &h_asf, frames_data &f_amc);

//    void calc_bvh_pos_angle(hierarchy_bvh &h_bvh,frames_data &f_bvh, string rot_order);//remove
//    void calc_asf_pos_angle(hierarchy_asf &h_asf,frames_data &f_amc, string rot_order);//remove

//    void calc_root_bvh(hierarchy_bvh &h_bvh,frames_data &f_bvh, string rot_order,int frame_index );//remove
//    void calc_root_asf(hierarchy_asf &h_asf,frames_data &f_amc, string rot_order,int frame_index );//remove
//    void calc_joint_bvh(hierarchy_bvh &h_bvh,frames_data &f_bvh, string rot_order,int frame_index,string joint_name );//remove
//    void calc_joint_asf(hierarchy_asf &h_asf,frames_data &f_amc, string rot_order,int frame_index ,string joint_name );//remove

    angles calc_matrix_3_rot(QMatrix4x4 matrix, string order);

    void bvh_reorder(hierarchy_bvh &h_bvh_org, frames_data &f_bvh_org, string rot_order);
    void asf_reorder(hierarchy_asf &h_asf, frames_data &f_amc, string rot_order );

    void bvh_depth(hierarchy_bvh &h_bvh);
    void bvh_depth_child(hierarchy_bvh &h_bvh, int index, int parent);
    void bvh_pc_index(hierarchy_bvh &h_bvh, frames_data &f_bvh);
    void asf_pc_index(hierarchy_asf &h_asf, frames_data &f_amc);


    void bvh_positions(hierarchy_bvh &h_bvh, frames_data &f_bvh);
    void bvh_root(hierarchy_bvh &h_bvh);
    void bvh_joint(hierarchy_bvh &h_bvh, int index, int parent, double x, double y,double z);
    void bvh_root_frame(hierarchy_bvh &h_bvh, frames_data &f_bvh, int frame);
    void bvh_joint_frame(hierarchy_bvh &h_bvh, frames_data &f_bvh, int frame, int index, int parent, double x, double y,double z);

    void asf_position(hierarchy_asf &h_asf, frames_data &f_amc);
    void asf_root(hierarchy_asf &h_asf);
    void asf_joint(hierarchy_asf &h_asf, int index, int parent, double x, double y, double z);
    void asf_root_frame(hierarchy_asf &h_asf, frames_data &f_amc, int f);
    void asf_joint_frame(hierarchy_asf &h_asf, frames_data &f_amc, int f, int index, int parent, double x, double y, double z);

    void bvh_add_joint_above(hierarchy_bvh &h_bvh, frames_data &f_bvh, string new_joint_name, string child_name);
    void bvh_add_joint_below(hierarchy_bvh &h_bvh, frames_data &f_bvh, string new_joint_name, string parent_name);
    void bvh_change_name(hierarchy_bvh &h_bvh, frames_data &f_bvh,string old_name, string new_name);
    void bvh_remove_joint(hierarchy_bvh &h_bvh, frames_data &f_bvh,string name);
    void bvh_change_parent(hierarchy_bvh &h_bvh, frames_data &f_bvh, string joint_name, string new_parent_name);

    void asf_add_joint_above(hierarchy_asf &h_asf, frames_data &f_amc, string new_joint_name, string child_name);
    void asf_add_joint_below(hierarchy_asf &h_asf, frames_data &f_amc, string new_joint_name, string parent_name);
    void asf_change_name(hierarchy_asf &h_asf, frames_data &f_amc, string old_name, string new_name);
    void asf_remove_joint(hierarchy_asf &h_asf, frames_data &f_amc, string name);
    void asf_change_parent(hierarchy_asf &h_asf, frames_data &f_amc, string joint_name, string new_parent_name);

    int find_joint_index_h_bvh(hierarchy_bvh &h_bvh, string name);
    int find_joint_index_f_bvh(frames_data &f_bvh, string name);

    int find_joint_index_h_asf(hierarchy_asf &h_asf, string name);
    int find_joint_index_f_amc(frames_data &f_amc, string name);

    void change_offset_bvh(hierarchy_bvh &h_bvh, string name,float x,float y,float z);
    void change_offset_asf(hierarchy_asf &h_asf, string name,float x,float y,float z);

    void amc_rad_to_deg(hierarchy_asf &h_asf,frames_data &f_amc);

};

#endif // DATA_H
