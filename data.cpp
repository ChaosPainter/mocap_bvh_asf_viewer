#include "data.h"
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <QMatrix4x4>
#include <QVector4D>
#include <QVector3D>
#include <math.h>

 Data::Data()
{

}


return_bvh Data::create_bvh(fstream &file) {
    int id = 0;
    frames_data f_bvh;
    frames_data f_data;
    string path;
    hierarchy_bvh h_bvh;
    root_bvh root_bvh;
    joint_bvh j_bvh;
    vector<joint_bvh> v_j_bvh;
    return_bvh r_bvh;
    string text = "";

    while (text != "HIERARCHY" && file.eof() != true)
    {						///tu jesteś w sprawdzanu

        file >> text;

    }

    if (text == "HIERARCHY" && file.eof() != true)
    {
        file >> text;
        if (text == "ROOT")
        {
            h_bvh = create_root_bvh(file, id, text);

            j_bvh.id = 0; //dumy joint
            j_bvh.name = h_bvh.root.name;
            j_bvh.channels = h_bvh.root.channel_names;
            j_bvh.offset=h_bvh.root.start_cords;
            j_bvh.channel_count=h_bvh.root.channel_count;
            j_bvh.channels=h_bvh.root.channel_names;
            j_bvh.children=h_bvh.root.children;
            //j_bvh.parent="root";
            h_bvh.joints.push_back(j_bvh);

            vector <joint_bvh> joints(h_bvh.joints.size());
            for (int i = 0; i < h_bvh.joints.size(); ++i)
            {
                joints[h_bvh.joints[i].id]=h_bvh.joints[i];
            }

            h_bvh.joints.clear();
            h_bvh.joints=joints;



        }
        else{}}
          //  cout << "wtf";// błąd formatowania pliku brak otwarcia ROOT



    while (text != "MOTION" && file.eof() != true) {						///tu jesteś w sprawdzanu

        file >> text;

    }
    if (text == "MOTION" && file.eof() != true)
    {
        f_data = create_frames_bvh(h_bvh, file, text);

    }
    bvh_pc_index(h_bvh,f_data);
    bvh_depth(h_bvh);
    r_bvh.f_bvh=f_data;
    r_bvh.h_bvh=h_bvh;
    return r_bvh;

}
hierarchy_bvh Data::create_root_bvh(fstream& file,int &id, string &text) { ///hierarchia bvh

    ret_r_j temp;
    double num;
    hierarchy_bvh hierarchy;
    file >> text;
    hierarchy.root.name = text;
    hierarchy.root.id = id;
    while (text != "OFFSET" && file.eof() != true)
    {
        file >> text;

    }
    for (int i = 0; i < 3; i++)
    {
        file >> num;
        hierarchy.root.start_cords.push_back(num);

    }
    while (text != "CHANNELS" && file.eof() != true)
    {
        file >> text;

    }
    file >> num;
    hierarchy.root.channel_count = num;

    for (int i = 0; i < hierarchy.root.channel_count; i++)
    {
        file >> text;
        hierarchy.root.channel_names.push_back(text);

    }
    while (text != "MOTION" && file.eof() != true)
    {
        file >> text;
        if (text=="JOINT")
        {
            temp=create_joint_bvh(hierarchy.root.name, hierarchy, file,id,0,text);
            hierarchy = temp.hierarchy;
            hierarchy.root.children.push_back(temp.child);
        }

    }

    return hierarchy;

}
ret_r_j Data::create_joint_bvh(string parent, hierarchy_bvh &hierarchy, fstream& file, int &id, int fl, string &text) { ///joint bvh
    ret_r_j temp;
    ret_r_j temp2;
    joint_bvh t_j;
    double num;


    if (fl == 0) { // normal joint
        file >> text;
        id++;
        t_j.id = id;

        t_j.name = text;
        t_j.parent = parent;
        temp.hierarchy = hierarchy;


        while (text != "OFFSET" && file.eof() != true)
        {
            file >> text;

        }
        for (int i = 0; i < 3; i++)
        {
            file >> num;
            t_j.offset.push_back(num);

        }
        while (text != "CHANNELS" && file.eof() != true)
        {
            file >> text;

        }
        file >> num;
        t_j.channel_count = num;

        for (int i = 0; i < t_j.channel_count; i++)
        {
            file >> text;
            t_j.channels.push_back(text);

        }
        while (text != "MOTION" && file.eof() != true)
        {
            file >> text;
            if (text == "JOINT")
            {
                temp2 = create_joint_bvh(t_j.name, temp.hierarchy, file, id,0,text);
                temp.hierarchy = temp2.hierarchy;
                t_j.children.push_back(temp2.child);

            }
            else if (text == "End")
            {
                temp2 = create_joint_bvh(t_j.name, temp.hierarchy, file, id,1,text);
                temp.hierarchy = temp2.hierarchy;
                t_j.children.push_back(temp2.child);
            }
            else if (text == "}")
            {

                break;
            }

        }
    }
        else//endsite  traktowanie end_site jako joint.
        {
            id++;
            t_j.id = id;

            t_j.name = "End_Site_";
            t_j.name.append(parent);
            t_j.name.append(std::to_string(id));
            t_j.parent = parent;
            t_j.channel_count=0;
            temp.hierarchy = hierarchy;
            while (text != "OFFSET" && file.eof() != true)
            {
                file >> text;

            }
            for (int i = 0; i < 3; i++)
            {
                file >> num;
                t_j.offset.push_back(num);

            }
            file>>text;

        }

    temp.child = t_j.name;
    temp.hierarchy.joints.push_back(t_j);

    return temp;
}
frames_data Data::create_frames_bvh(hierarchy_bvh &h_bvh, fstream& file, string &text){ ///frames bvh

    frames_data f_data;
    //int stuff;
    double num;
    joint_data j_d;



    while (text != "Frames:" && file.eof() != true)
    {
        file >> text;

    }
    file >> f_data.frames_count;

    while (text != "Frame" && file.eof() != true)
    {
        file >> text;

    }
    file >> text;
    file >> f_data.frame_time;

    getline(file,text);

    int x = 0;
    while (file.eof()!=true )
    {
        frame f;

        getline(file, text);
        if(text=="")
            return f_data;
        istringstream buf(text);


        for (int i = 0; i < h_bvh.joints.size(); i++)
        {
            if (h_bvh.joints[i].channel_count>0)
            {   joint_data j_d;
                vector<double> temp;

                j_d.id = h_bvh.joints[i].id;
                j_d.name = h_bvh.joints[i].name;

                for (int j = 0; j < h_bvh.joints[i].channel_count; j++)
                {
                    buf >> num;
                    temp.push_back(num);
                }

              j_d.channels = temp;
              f.channel_data.push_back(j_d);
            }

        }
        f.num = x;
        x++;
        f_data.frames.push_back(f);

//        bool test2=file.eof();
//        int test=f_data.frames.size();

    }

    return f_data;
}
void Data::write_bvh(ofstream& file, hierarchy_bvh &h_bvh, frames_data &f_bvh)  ///napisać write tak żeby informacje z frame również były dodawane zgodnie hierarchią nie z czytaniem tzn.
                                                                              ///root ->dziecko1 ->dziecko1_dziecka1 ->endsite(brak danych f) ->dziecko2_dziecka1 ->dziecko2-> dziecko1_dziecka2->dziecko1_dziecka1_dziecka2->endsite(brak danych f)
{

    file << "HIERARCHY" << "\n";
    file << "ROOT " << h_bvh.joints[0].name << "\n";
    file << "{" << "\n";
    file << "\t";
    file << "OFFSET ";
    for (int i = 0; i < h_bvh.joints[0].offset.size(); i++)
    {
        file << h_bvh.joints[0].offset[i];
        file << " ";
    }
    file << "\n";
    file << "\t";
    file << "CHANNELS "<<h_bvh.joints[0].channels.size()<<" ";
    for (int i = 0; i < h_bvh.joints[0].channels.size(); i++)
    {
        file << h_bvh.joints[0].channels[i] << " ";
    }
    file << "\n";
    for (int i = 0; i < h_bvh.joints[0].children.size(); i++)
    {
        int j = find_joint_index_h_bvh(h_bvh,h_bvh.joints[0].children[i]);

        write_joint_bvh(file,h_bvh,j);
    }
    file << "}" << "\n";
    file << "MOTION" << "\n"; //tutaj musi być poprawka jak opisane powyżej^^
    file << "Frames: " << f_bvh.frames.size() << "\n";
    file << "Frame Time: " << f_bvh.frame_time << "\n";

    vector<int> index_list;
    vector<int> frame_index_list;

    add_joint_to_write_list(index_list,frame_index_list,h_bvh,f_bvh);
    for (int i = 0; i < f_bvh.frames.size(); ++i)
    {
        for (int j = 0; j < frame_index_list.size(); ++j)
        {
            for (int k = 0; k < f_bvh.frames[i].channel_data[frame_index_list[j]].channels.size(); ++k)
            {
                file << f_bvh.frames[i].channel_data[frame_index_list[j]].channels[k] << " ";
            }
        }
        file << "\n";
    }

}
void Data::write_joint_bvh(ofstream& file, hierarchy_bvh &h_bvh, int id) {
    if(h_bvh.joints[id].children.size()>0)
    {
        for (int i = 0; i < h_bvh.joints[id].depth; ++i)
        {
            file << "\t";
        }
        file << "JOINT " << h_bvh.joints[id].name << "\n";
        for (int i = 0; i < h_bvh.joints[id].depth; ++i)
        {
            file << "\t";
        }
        file << "{" << "\n";
        for (int i = 0; i < h_bvh.joints[id].depth+1; ++i)
        {
            file << "\t";
        }
        file << "OFFSET ";
        for (int i = 0; i < h_bvh.joints[id].offset.size(); i++)
            {
                file << h_bvh.joints[id].offset[i] << " ";
            }
        file << "\n";
        if (h_bvh.joints[id].channels.size()>0)
        {
            for (int i = 0; i < h_bvh.joints[id].depth+1; ++i)
            {
                file << "\t";
            }
            file << "CHANNELS " << h_bvh.joints[id].channels.size() << " ";
            for (int i = 0; i < h_bvh.joints[id].channels.size(); i++)
            {
                file << h_bvh.joints[id].channels[i] << " ";
            }
            file << "\n";
        }

        for (int i = 0; i < h_bvh.joints[id].children.size(); i++)
        {
            int j = find_joint_index_h_bvh(h_bvh,h_bvh.joints[id].children[i]);

            write_joint_bvh(file, h_bvh, j);
        }
    }

    else
    {
        for (int i = 0; i < h_bvh.joints[id].depth; ++i)
        {
            file << "\t";
        }
        file << "End Site\n";
        for (int i = 0; i < h_bvh.joints[id].depth; ++i)
        {
            file << "\t";
        }
        file << "{" << "\n";
        for (int i = 0; i < h_bvh.joints[id].depth+1; ++i)
        {
            file << "\t";
        }
        file<<"OFFSET ";
        for (int i = 0; i < h_bvh.joints[id].offset.size(); ++i) {
            file<<h_bvh.joints[id].offset[i]<<" ";
        }
        file<<"\n";

        if (h_bvh.joints[id].channels.size()>0)
        {
            for (int i = 0; i < h_bvh.joints[id].depth; ++i)
            {
                file << "\t";
            }
            file << "CHANNELS " << h_bvh.joints[id].channels.size() << " ";
            for (int i = 0; i < h_bvh.joints[id].channels.size(); i++)
            {
                file << h_bvh.joints[id].channels[i] << " ";
            }
            file << "\n";
        }
    }
    for (int i = 0; i < h_bvh.joints[id].depth; ++i)
    {
        file << "\t";
    }
    file << "}" << "\n";

}


void Data:: write_frames_asf(ofstream& file, hierarchy_asf &h_asf, frames_data &f_asf) {


    file << ":FULLY - SPECIFIED" << "\n";
    file << ":DEGREES" ;
    for (int i = 0; i < f_asf.frames.size(); i++)
    {
        file<< "\n" << i+1 ;
        for (int j = 0; j < f_asf.frames[i].channel_data.size(); j++)
        {   file << "\n";
            file << f_asf.frames[i].channel_data[j].name << " ";
            for (int k = 0; k <f_asf.frames[i].channel_data[j].channels.size(); k++)
            {
                file << f_asf.frames[i].channel_data[j].channels[k] << " ";
            }

        }
    }
}

void Data::add_joint_to_write_list(vector<int> &index_list, vector<int> &frame_index, hierarchy_bvh &h_bvh, frames_data &f_bvh)
{
    if (h_bvh.joints[0].channels.size()>0)
    {
        index_list.push_back(0);
        frame_index.push_back(find_joint_index_f_bvh(f_bvh,h_bvh.joints[0].name));
    }
    for (int i = 0; i < h_bvh.joints[0].children.size(); ++i)
    {
        int index=find_joint_index_h_bvh(h_bvh,h_bvh.joints[0].children[i]);
        add_joint_to_write_list_child(index_list,frame_index,h_bvh,f_bvh,index);
    }
}

void Data::add_joint_to_write_list_child(vector<int> &index_list, vector<int> &frame_index, hierarchy_bvh &h_bvh, frames_data &f_bvh, int joint_index)
{
    if(h_bvh.joints[joint_index].channels.size()>0)
    {
        index_list.push_back(joint_index);
        frame_index.push_back(find_joint_index_f_bvh(f_bvh,h_bvh.joints[joint_index].name));
    }
    for (int i = 0; i < h_bvh.joints[joint_index].children.size(); ++i)
    {
        int index=find_joint_index_h_bvh(h_bvh,h_bvh.joints[joint_index].children[i]);
        add_joint_to_write_list_child(index_list,frame_index,h_bvh,f_bvh,index);
    }
}



void Data:: write_asf(ofstream& file, hierarchy_asf &h_asf, frames_data &f_asf) {

    file << ":name " << h_asf.info.name << "\n";
    file << ":version " << h_asf.info.version << "\n";
    file << ":units " << "\n";
    file << "mass " << h_asf.info.mass << "\n";
    file << "length " << h_asf.info.length << "\n";
    file << "angle " << h_asf.info.angle << "\n";
    file << ":root" << "\n";
    file << "\t";
    file << "order ";
    for (int i = 0; i < h_asf.joints[0].dof.size(); i++)
    {
        file << h_asf.joints[0].dof[i]<<" ";
    }
    file << "\n";
    file << "\t";
    file << "axis ";
    for (int i = 0; i < h_asf.joints[0].axis_order.size(); i++)
    {
        file << h_asf.joints[0].axis_order[i];
    }
    file << "\n";
    file << "\t";
    file << "position ";
    for (int i = 0; i < h_asf.joints[0].direction.size(); i++)
    {
        file << h_asf.joints[0].direction[i] << " ";

    }
    file << "\n";
    file << "\t";
    file << "orientation ";
    for (int i = 0; i < h_asf.joints[0].axis.size(); i++)
    {
        file << h_asf.joints[0].axis[i] << " ";

    }
    file << "\n";
    file << ":bonedata" << "\n";
    for (int i = 1; i < h_asf.joints.size(); i++)
    {
        file << "begin\n";
        file << "\t";
        file << "id " << h_asf.joints[i].id << "\n";
        file << "\t";
        file << "name " << h_asf.joints[i].name << "\n";
        file << "\t";
        file << "direction ";
        for (int j = 0; j < h_asf.joints[i].direction.size(); j++)
        {
            file << h_asf.joints[i].direction[j] << " ";
        }
        file << "\n";
        file << "\t";
        file << "length " << h_asf.joints[i].length << "\n";
        file << "\t";
        file << "axis ";
        for (int j = 0; j < h_asf.joints[i].axis.size(); j++)
        {
            file << h_asf.joints[i].axis[j] << " ";
        }
        for (int j = 0; j < h_asf.joints[i].axis_order.size(); j++)
        {
            file << h_asf.joints[i].axis_order[j];
        }
        file << "\n";
        if (h_asf.joints[i].dof.size()>0) {
            file << "\t";
            file << "dof ";
            for (int j = 0; j < h_asf.joints[i].dof.size(); j++)
            {
                file << h_asf.joints[i].dof[j] << " ";
            }
            file<<"\n";
            file << "\t";
            file << "limits ";
            //int tt=h_asf.joints[i].dof.size();
            for (int j = 0; j < h_asf.joints[i].dof.size(); j++)
            {

                file << "\t";
                file << "\t";
                file << "(";
                file << h_asf.joints[i].limits[j * 2];
                file << " ";
                file << h_asf.joints[i].limits[(j * 2) + 1];
                file << ")";
                file << "\n";

            }

        }
        file << "end\n";

    }
    file << ":hierarchy" << "\n";
    file << "begin" << "\n";
    for (int i = 0; i < h_asf.joints.size(); i++)
    {

            if (h_asf.joints[i].children.size()!=0)
            {

                file << "\t";
                file << h_asf.joints[i].name << " ";
                for (int j = 0; j < h_asf.joints[i].children.size(); j++)
                {
                    file << h_asf.joints[i].children[j] << " ";
                }
                file << "\n";
            }

    }
    file << "end";
}


return_bvh Data::asf_to_bvh(hierarchy_asf &h_asf, frames_data &f_asf) { ///funkcja od konwersji asf do bvh
    hierarchy_bvh h_bvh;
    frames_data f_bvh;
    return_bvh r_bvh;


    h_bvh.root.children = h_asf.root.children;
    h_bvh.root.id = h_asf.root.id;
    h_bvh.root.name = h_asf.root.name;
   // h_bvh.root.channel_names = h_asf.root.order;
    h_bvh.root.start_cords = h_asf.root.position;
    h_bvh.root.channel_count = h_asf.root.order.size();
    for (int i = 0; i < h_asf.root.order.size(); ++i)
    {
        if (h_asf.root.order[i]=="TX")
        {
           h_bvh.root.channel_names.push_back("Xposition");
        }
        else
        {
            if (h_asf.root.order[i]=="TY")
            {
                h_bvh.root.channel_names.push_back("Yposition");
            }
            else
            {
                if (h_asf.root.order[i]=="TZ")
                {
                    h_bvh.root.channel_names.push_back("Zposition");
                }
                else
                {
                    if (h_asf.root.order[i]=="RX")
                    {
                        h_bvh.root.channel_names.push_back("Xrotation");
                    }
                    else
                    {
                        if (h_asf.root.order[i]=="RY")
                        {
                            h_bvh.root.channel_names.push_back("Yrotation");
                        }
                        else
                        {
                            if (h_asf.root.order[i]=="RZ")
                            {
                                h_bvh.root.channel_names.push_back("Zrotation");
                            }
                        }
                    }
                }
            }
        }
}
    for (int i = 1; i < h_asf.joints.size(); i++)
    {
        joint_bvh j_bvh;
        j_bvh.children = h_asf.joints[i].children;
        j_bvh.channel_count = h_asf.joints[i].dof.size();
        j_bvh.name = h_asf.joints[i].name;
        j_bvh.id = h_asf.joints[i].id;
        j_bvh.parent = h_asf.joints[i].parent;
        j_bvh.channel_count=h_asf.joints[i].dof.size();



        for (int j = 0; j < h_asf.joints[i].direction.size(); j++)
        {
            j_bvh.offset.push_back(h_asf.joints[i].direction[j] * h_asf.joints[i].length);
        }

        //j_bvh.channels = h_asf.joints[i].dof;

        for (int k = 0; k < h_asf.joints[i].dof.size(); ++k)
        {
            if (h_asf.joints[i].dof[k]=="rx")
            {
                j_bvh.channels.push_back("Xrotation");
            }
            else
            {

                if (h_asf.joints[i].dof[k]=="ry")
                {
                    j_bvh.channels.push_back("Yrotation");
                }
                else
                {

                    if (h_asf.joints[i].dof[k]=="rz")
                    {
                        j_bvh.channels.push_back("Zrotation");
                    }
                }
            }
        }











        h_bvh.joints.push_back(j_bvh);

    }
    f_bvh=f_asf;

f_bvh.frame_time=0.04;
r_bvh.f_bvh=f_bvh;
r_bvh.h_bvh=h_bvh;
return r_bvh;
}



return_asf Data::bvh_to_asf(hierarchy_bvh &h_bvh, frames_data &f_bvh) { ///konwersja bvh do asf

    hierarchy_asf h_asf;
    frames_data f_asf;
    return_asf r_asf;
    h_asf.info.angle="deg";
    h_asf.info.length=1;
    h_asf.info.mass=1.0;
    h_asf.info.name="name";
    h_asf.info.version="0.0";
    h_asf.root.children = h_bvh.root.children;
    h_asf.root.id = h_bvh.root.id;
    h_asf.root.name = h_bvh.root.name;
    h_asf.root.parent = h_bvh.root.parent;

    for (int i = 0; i < h_bvh.root.channel_names.size(); i++)
    { qDebug()<<"root channel\n";
        string text;
        //double a = h_bvh.root.start_cords[i];
        text = h_bvh.root.channel_names[i];
        if (text=="Xposition")
        {
            h_asf.root.order.push_back("TX");
            h_asf.root.position.push_back(h_bvh.root.start_cords[i]);
        }
        else
        {
            if (text == "Zposition")
            {
                h_asf.root.order.push_back("TZ");
                h_asf.root.position.push_back(h_bvh.root.start_cords[i]);
            }
            else
            {
                if (text == "Yposition")
                {
                    h_asf.root.order.push_back("TY");
                    h_asf.root.position.push_back(h_bvh.root.start_cords[i]);
                }
                else
                {
                    if (text == "Zrotation")
                    {
                        h_asf.root.order.push_back("RZ");
                        h_asf.root.orientation.push_back(0);
                        h_asf.root.axis.push_back('Z');
                    }
                    else
                    {
                        if (text == "Xrotation")
                        {
                            h_asf.root.order.push_back("RX");
                            h_asf.root.orientation.push_back(0);
                            h_asf.root.axis.push_back('X');
                        }
                        else
                        {
                            if (text == "Yrotation")
                            {
                                h_asf.root.order.push_back("RY");
                                h_asf.root.orientation.push_back(0);
                                h_asf.root.axis.push_back('Y');
                            }
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < h_bvh.joints.size(); i++)
    { qDebug()<<"joint start\n";
        joint_asf j_asf;
        j_asf.name = h_bvh.joints[i].name;
        j_asf.id = h_bvh.joints[i].id;
        j_asf.children = h_bvh.joints[i].children;
        j_asf.parent = h_bvh.joints[i].parent;
        j_asf.axis={0,0,0};
        j_asf.axis_order=h_asf.root.axis;


        j_asf.length = sqrt(h_bvh.joints[i].offset[0]* h_bvh.joints[i].offset[0] + h_bvh.joints[i].offset[1]* h_bvh.joints[i].offset[1]+ h_bvh.joints[i].offset[2]* h_bvh.joints[i].offset[2]);
        for (int j = 0; j < h_bvh.joints[i].offset.size(); j++)
        { qDebug()<<"offset\n";
            j_asf.direction.push_back(h_bvh.joints[i].offset[j] / j_asf.length);

        }
        for (int j = 0; j < h_bvh.joints[i].channels.size(); j++)
        { qDebug()<<"start\n";
            string text;

            text = h_bvh.joints[i].channels[j];
            if (text == "Zrotation")
            {

                j_asf.dof.push_back("rz");
                j_asf.limits.push_back(-180);
                j_asf.limits.push_back(180);
            }
            else
            {
                if (text == "Xrotation")
                {
                    j_asf.dof.push_back("rx");
                    j_asf.limits.push_back(-180);
                    j_asf.limits.push_back(180);
                }
                else
                {
                    if (text == "Yrotation")
                    {
                    j_asf.dof.push_back("ry");
                    j_asf.limits.push_back(-180);
                    j_asf.limits.push_back(180);
                    }
                }
            }
        }

    h_asf.joints.push_back(j_asf);
    //int size=h_asf.joints.size();
    }

    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        string name=h_asf.joints[i].name;
        i=i;
    }
    f_asf = f_bvh;
    for (int i = 0; i < f_asf.frames.size(); ++i)
    {
        f_asf.frames[i].channel_data[0].name=h_asf.joints[0].name;
    }
  r_asf.f_amc=f_asf;
  r_asf.h_asf=h_asf;
  return r_asf;
}

return_asf Data::bvh_to_asf_conversion(hierarchy_bvh &h_bvh, frames_data &f_bvh)
{
    hierarchy_asf h_asf;
    frames_data f_amc;
    return_asf r_asf;

    string rotation_order;
    vector <string> channels;
    vector <string> root_channels;
    vector <char> axis;
    string order;
    for (int i = 0; i < h_bvh.joints[0].channels.size(); ++i) ///dopisac przypisania dla root;
    {
        if (h_bvh.joints[0].channels[i]=="Xrotation")
        {
            rotation_order.append("X");
            channels.push_back("rx");
            root_channels.push_back("RX");
            order.append("X");
            axis.push_back('X');
        }
        else
            if (h_bvh.joints[0].channels[i]=="Yrotation")
            {
                rotation_order.append("Y");
                channels.push_back("ry");
                root_channels.push_back("RY");
                order.append("Y");
                axis.push_back('Y');
            }
            else
                if (h_bvh.joints[0].channels[i]=="Zrotation")
                {
                    rotation_order.append("Z");
                    channels.push_back("rz");
                    root_channels.push_back("RZ");
                    order.append("Z");
                    axis.push_back('Z');
                }
                else
                    if (h_bvh.joints[0].channels[i]=="Xposition")
                    {
                        root_channels.push_back("TX");
                    }
                    else
                        if (h_bvh.joints[0].channels[i]=="Yposition")
                        {
                            root_channels.push_back("TY");
                        }
                        else
                            if (h_bvh.joints[0].channels[i]=="Zposition")
                            {
                                root_channels.push_back("TZ");
                            }
    }

       reverse(order.begin(),order.end());


    for (int i = 0; i < h_bvh.joints.size(); ++i)
    {
        joint_asf new_joint;


        new_joint.axis_order=axis;
        new_joint.axis.push_back(0);
        new_joint.axis.push_back(0);
        new_joint.axis.push_back(0);
        new_joint.id=h_bvh.joints[i].id;
        if (i==0)
        {
            new_joint.name="root";
            new_joint.direction=h_bvh.joints[i].offset;
            for (int i = 0; i < root_channels.size(); ++i)
            {
                new_joint.dof.push_back(root_channels[i]);
                new_joint.limits.push_back(-180);
                new_joint.limits.push_back(180);
            }

        }
        else
            {
                new_joint.name=h_bvh.joints[i].name;
                QVector3D vec(h_bvh.joints[i].offset[0],h_bvh.joints[i].offset[1],h_bvh.joints[i].offset[2]);
                new_joint.length=vec.length();
                if (vec.length()>0)
                {
                    new_joint.direction.push_back(h_bvh.joints[i].offset[0]/vec.length());
                    new_joint.direction.push_back(h_bvh.joints[i].offset[1]/vec.length());
                    new_joint.direction.push_back(h_bvh.joints[i].offset[2]/vec.length());

                }
                else
                    {
                        new_joint.direction.push_back(0);
                        new_joint.direction.push_back(0);
                        new_joint.direction.push_back(0);
                    }
                for (int i = 0; i < channels.size(); ++i)
                {
                    new_joint.dof.push_back(channels[i]);
                    new_joint.limits.push_back(-180);
                    new_joint.limits.push_back(180);
                }

            }

        new_joint.parent=h_bvh.joints[i].parent;
        new_joint.children=h_bvh.joints[i].children;
        if (new_joint.parent==h_bvh.joints[0].name)
        {
            new_joint.parent="root";
        }
        h_asf.joints.push_back(new_joint);
    }




        for (int i = 0; i < f_bvh.frames.size(); ++i)
        {
            int iter=i+1;
            frame new_frame;

            for (int j = 0; j < h_bvh.joints.size(); ++j)
            {
                if (j==0)
                {
                    angles ang;
                    translations tr=h_bvh.joints[0].trans[iter];
                    joint_data channel_data_root;


                    ang=calc_matrix_3_rot(h_bvh.joints[0].vector_to_position[iter].transposed(),order);


                    for (int l = 0; l < root_channels.size(); ++l)
                    {
                        if (root_channels[l]=="RX")
                        {
                            channel_data_root.channels.push_back(ang.x);
                            //channel_data_root.channels.push_back(0);
                        }
                        else
                            if (root_channels[l]=="RY")
                            {
                                channel_data_root.channels.push_back(ang.y);
                                //channel_data_root.channels.push_back(0);
                            }
                            else
                                if (root_channels[l]=="RZ")
                                {
                                    channel_data_root.channels.push_back(ang.z);
                                    //channel_data_root.channels.push_back(0);
                                }
                                else
                                    if (root_channels[l]=="TX")
                                    {
                                        channel_data_root.channels.push_back(tr.x);
                                    }
                                    else
                                        if (root_channels[l]=="TY")
                                        {
                                            channel_data_root.channels.push_back(tr.y);
                                        }
                                        else
                                            if (root_channels[l]=="TZ")
                                            {
                                                channel_data_root.channels.push_back(tr.z);
                                            }

                        }
                        channel_data_root.name="root";
                        channel_data_root.id=h_bvh.joints[0].id;
                        new_frame.channel_data.push_back(channel_data_root);
                    }
                else
                    {   //////////////////////////////////////////////////////////////////////////////////// liczenie nowych kątów dla asf
                        angles ang;
                        QMatrix4x4 inv_parent_matrix;
                        QMatrix4x4 joint_matrix=h_bvh.joints[j].vector_to_position[iter];
                        QMatrix4x4 mat;
                        int index_parent=find_joint_index_h_bvh(h_bvh,h_bvh.joints[j].parent);

                        inv_parent_matrix=h_bvh.joints[index_parent].vector_to_position[iter].inverted();

                        mat= joint_matrix*inv_parent_matrix;

                        mat=mat.transposed();

                        ang=calc_matrix_3_rot(mat,order);

                        joint_data channel_data;
                        //int index_parent=find_joint_index_h_bvh(h_bvh,h_bvh.joints[j].parent);

                        for (int x = 0; x < channels.size() ; ++x)
                        {
                            if (channels[x]=="rx")
                            {
                                channel_data.channels.push_back(ang.x);
                            }
                            else
                                if (channels[x]=="ry")
                                {
                                    channel_data.channels.push_back(ang.y);
                                }
                                else
                                    if (channels[x]=="rz")
                                    {
                                        channel_data.channels.push_back(ang.z);
                                    }

                        }
                        channel_data.name=h_bvh.joints[j].name;
                        channel_data.id=h_bvh.joints[j].id;
                        new_frame.channel_data.push_back(channel_data);

                    }


            }
            f_amc.frames.push_back(new_frame);
        }


    r_asf.h_asf=h_asf;
    r_asf.f_amc=f_amc;
    return r_asf;
}

return_bvh Data::asf_to_bvh_conversion(hierarchy_asf &h_asf, frames_data &f_amc)
{
    return_bvh ret_bvh;
    hierarchy_bvh h_bvh;
    frames_data f_bvh;

    //order
    vector<string> root_order;
    vector<string> joint_order;
    string order;
    for (int i = 0; i < h_asf.joints[0].dof.size(); ++i)
    {
        if (h_asf.joints[0].dof[i]=="RX")
        {
            root_order.push_back("Xrotation");
            joint_order.push_back("Xrotation");
            order.append("X");
        }
        else
            if (h_asf.joints[0].dof[i]=="RY")
            {
                root_order.push_back("Yrotation");
                joint_order.push_back("Yrotation");
                order.append("Y");
            }
            else
                if (h_asf.joints[0].dof[i]=="RZ")
                {
                    root_order.push_back("Zrotation");
                    joint_order.push_back("Zrotation");
                    order.append("Z");
                }
                else
                    if (h_asf.joints[0].dof[i]=="TX")
                    {
                        root_order.push_back("Xposition");
                    }
                    else
                        if (h_asf.joints[0].dof[i]=="TY")
                        {
                            root_order.push_back("Yposition");
                        }
                        else
                            if (h_asf.joints[0].dof[i]=="TZ")
                            {
                                root_order.push_back("Zposition");
                            }
    }


    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        if (h_asf.joints[i].children.size()>1)
        {
            for (int j = 0; j < h_asf.joints[i].children.size(); ++j)
            {
                string ij;
                ij.append(std::to_string(i));
                ij.append(std::to_string(j));

                asf_add_joint_above(h_asf,f_amc,ij,h_asf.joints[i].children[j]);
            }


        }
    }

    for (int i =0; i < h_asf.joints.size(); ++i)
    {
         joint_bvh new_joint;
        if (i>0)
        {
            new_joint.name=h_asf.joints[i].name;
            new_joint.offset.push_back(h_asf.joints[i].direction[0]*h_asf.joints[i].length);
            new_joint.offset.push_back(h_asf.joints[i].direction[1]*h_asf.joints[i].length);
            new_joint.offset.push_back(h_asf.joints[i].direction[2]*h_asf.joints[i].length);
            new_joint.parent=h_asf.joints[i].parent;
            new_joint.children=h_asf.joints[i].children;
            new_joint.id=h_asf.joints[i].id;
            if (new_joint.children.size()>0)
            {
                new_joint.channels=joint_order;

            }


            //channels
        }
        else
        {
            new_joint.name=h_asf.joints[i].name;
            new_joint.offset=h_asf.joints[i].direction;
            new_joint.children=h_asf.joints[i].children;
            new_joint.id=h_asf.joints[i].id;
            new_joint.channels=root_order;

            //channels
        }
        h_bvh.joints.push_back(new_joint);
    }

    //reverse(order.begin(),order.end()); //może potzrebne


    for (int f = 0; f < f_amc.frames.size(); ++f)
    {
        frame new_frame;
        int iter=f+1;

        for (int i = 0; i < h_asf.joints.size(); ++i)

        {
            if (h_asf.joints[i].children.size()>0)
            {
                QMatrix4x4 inv_joint_matrix;
                int child_index;
                QMatrix4x4 child_matrix;
                QMatrix4x4 mat;
                angles ang;
                if (i==0)
                {
                    child_index=find_joint_index_h_asf(h_asf,h_asf.joints[i].children[0]);
                    mat=h_asf.joints[child_index].vector_to_position[iter];
                }
                else
                {
                    inv_joint_matrix = h_asf.joints[i].vector_to_position[iter].inverted();
                    child_index=find_joint_index_h_asf(h_asf,h_asf.joints[i].children[0]);
                    child_matrix=h_asf.joints[child_index].vector_to_position[iter];
                    mat=child_matrix*inv_joint_matrix;
                    mat=mat.transposed();
                }


                ang=calc_matrix_3_rot(mat,order);//nie jestem pewny te kolejności i transpozycji

                joint_data channel_data;

                if (i==0)
                {
                    translations tr=h_asf.joints[i].trans[iter];
                    for (int l = 0; l < root_order.size(); ++l)
                    {
                        if (root_order[l]=="Xrotation")
                        {
                        //channel_data_root.channels.push_back(ang.x);
                        channel_data.channels.push_back(ang.x);
                        }
                            else
                            if (root_order[l]=="Yrotation")
                            {
                            //channel_data_root.channels.push_back(ang.y);
                            channel_data.channels.push_back(ang.y);
                            }
                                else
                                if (root_order[l]=="Zrotation")
                                {
                                //channel_data_root.channels.push_back(ang.z);
                                channel_data.channels.push_back(ang.z);
                                }
                                    else
                                    if (root_order[l]=="Xposition")
                                    {
                                    channel_data.channels.push_back(tr.x);
                                    }
                                        else
                                        if (root_order[l]=="Yposition")
                                        {
                                        channel_data.channels.push_back(tr.y);
                                        }
                                            else
                                            if (root_order[l]=="Zposition")
                                            {
                                            channel_data.channels.push_back(tr.z);
                                            }
                    }
                    channel_data.name=h_asf.joints[i].name;
                    channel_data.id=h_asf.joints[i].id;
                    new_frame.channel_data.push_back(channel_data);
                }
                else
                {
                    for (int l = 0; l < joint_order.size(); ++l)
                    {
                        if (joint_order[l]=="Xrotation")
                        {
                        //channel_data_root.channels.push_back(ang.x);
                        channel_data.channels.push_back(ang.x);
                        }
                            else
                            if (joint_order[l]=="Yrotation")
                            {
                            //channel_data_root.channels.push_back(ang.y);
                            channel_data.channels.push_back(ang.y);
                            }
                                else
                                if (joint_order[l]=="Zrotation")
                                {
                                //channel_data_root.channels.push_back(ang.z);
                                channel_data.channels.push_back(ang.z);
                                }

                    }
                    channel_data.name=h_asf.joints[i].name;
                    channel_data.id=h_asf.joints[i].id;
                    new_frame.channel_data.push_back(channel_data);
                }
            }
        }
        f_bvh.frames.push_back(new_frame);
    }

    bvh_depth(h_bvh);

    f_bvh.frame_time=0.01666666666;
    f_bvh.frames_count=f_bvh.frames.size();
    ret_bvh.h_bvh=h_bvh;
    ret_bvh.f_bvh=f_bvh;
    return ret_bvh;
}




frames_data Data::create_frames_asf(fstream& file,hierarchy_asf &h) { ///wczytanie ramek asf/amc
    int t =0; //ilość joint z dof; 1 za root;
    string text;
    string temp;
    int i;
    joint_data f_j;
    frame f;
    frames_data f_asf;


    for (int x = 0; x < h.joints.size();x++)
    {
        qDebug()<<"create_frames_asf joints\n";
        if (h.joints[x].dof.size() != 0)
        {
            t++;
        }
    }

    while (text!=":FULLY-SPECIFIED"&&text!=":fully-specified ")
    {
        qDebug()<<"deg\n";
        file >> text;

    }

    while (!file.eof())
    {
        qDebug()<<"eof\n";
        file >> i;
        getline(file, text);
        if (file.eof())
        {
            return f_asf;
        }

        qDebug()<<QString::number(i);
        for (int y = 0; y < t; y++) //ilość joint z istnejącymi dof
        {
            //qDebug()<<"joint z dof\n";
            getline(file, text);
            istringstream buf(text);
            buf >> temp;
            f_j.name = temp;

            for (double d; buf >> d;)
            {
                //qDebug()<<"channels\n";
                f_j.channels.push_back(d);
            }
            f.channel_data.push_back(f_j);

            f_j.channels.clear();
        }
        f.num = i;
        f_asf.frames.push_back(f);
        f.channel_data.clear();

        if(text=="")
            return f_asf;

    }
    if (h_asf.info.angle==false)
    {
        amc_rad_to_deg(h,f_asf);
        h_asf.info.angle=true;
    }

    return f_asf;
}
hierarchy_asf Data::create_asf(fstream& file) {	/// hierarchia asf
    string text;
    istringstream string_buf;
    hierarchy_asf h_asf;
    double temp_d;
    int r=0;
    info_asf inf;
    root_asf root;
    joint_asf joint;
    joint_asf t_joint;
    string temp_string;
    bool root_fl=false;
    bool bone_fl=false;
    bool hierarchy_fl=false;
    while (!file.eof())
    {
        qDebug()<<"create_asf\n";
        if ((!bone_fl)&&(!root_fl)&&(!hierarchy_fl))
        {
            file >> text;
        }

        if (root_fl)
        {
            text=":root";
        }
        if (bone_fl)
        {
            text=":bonedata";
        }
        if (hierarchy_fl)
        {
            text=":hierarchy";
        }

        if (text==":version")
        {
            r=1;
        }

        if (text==":name")
        {
            r=2;
        }

        if (text==":units")
        {
            r=3;
        }

        if (text==":root")
        {
            r=4;
        }

        if (text==":bonedata")
        {
            r=5;
        }

        if (text==":hierarchy")
        {
            r=6;
        }
        switch (r)
        {
        case 1:
            file >> text;
            h_asf.info.version= text;
            break;
        case 2:
            file >> text;
            h_asf.info.name = text;
            break;
        case 3:
            while (text!=":root")
            {
                file >> text;

                if (text == "mass")
                {
                    file >> temp_d;
                    h_asf.info.mass = temp_d;
                }
                if (text == "length")
                {
                    file >> temp_d;
                    h_asf.info.length = temp_d;
                }
                if (text == "angle")
                {
                    file >> text;
                    if (text=="deg")
                    {
                        h_asf.info.angle=true;
                    }
                    else
                        h_asf.info.angle=false;
                }
            }
            root_fl=true;
            break;
        case 4:
            while (text != ":bonedata")
            {
                file >> text;

                if (text == "order")
                {
                    getline(file, text);
                    istringstream string_buf(text);
                    for (string word; string_buf >> word;)
                    {
                        //qDebug()<<"create_asf_order\n";
                        root.order.push_back(word);
                    }
                }

                if (text == "position")
                {
                    getline(file, text);
                    istringstream string_buf(text);
                    for (double temp; string_buf >> temp;)
                    {
                        //qDebug()<<"create_asf_position\n";
                        root.position.push_back(temp);

                    }
                 }

                if (text == "orientation")
                {
                    getline(file, text);
                    istringstream string_buf(text);
                    for (double temp; string_buf >> temp;)
                    {
                        //qDebug()<<"create_asf_orientation\n";
                        root.orientation.push_back(temp);
                    }
                 }

                if (text == "axis")
                {
                    getline(file, text);
                    istringstream string_buf(text);
                    for (char temp; string_buf >> temp;)
                    {
                        //qDebug()<<"create_asf_axis\n";
                        root.axis.push_back(temp);
                    }
                }
            }


            root.name = "root";
            h_asf.root = root;

            t_joint.id = 0;
            t_joint.name = "root";
            t_joint.dof = h_asf.root.order;
//            for (int o=0;h_asf.root.order;o++) { //ujednolicenie nazewnictwa stopni sfobody

//            }
            t_joint.length=0;
            t_joint.direction=root.position;
            t_joint.axis_order=root.axis;
            t_joint.axis=root.orientation;
            t_joint.parent_index_joint=0;

            h_asf.joints.push_back(t_joint);

            root_fl=false;
            bone_fl=true;
            break;

        case 5:
            while (text!=":hierarchy")
            {
                file>>text;
                if (text == "begin")
                {
                    //qDebug()<<"create_asf_begin_joint\n";
                    joint = create_joints_asf(file);
                    h_asf.joints.push_back(joint);
                }
            }
            bone_fl=false;
            hierarchy_fl=true;
            break;
        case 6:
            while (text != "begin")
            {
                file >> text;
                //qDebug()<<"create_asf_h_begin\n";
            }
            getline(file, text);
            while (text!="end")
            {
                //qDebug()<<"create_asf_end\n";
                getline(file, text);
                istringstream string_buf(text);
                string parent_name;
                string_buf >> parent_name;
                if (parent_name == "end")
                {
                    while (!file.eof())
                    {
                        getline(file, text);
                    }
                    break;
                }
                else
                {
                    if (parent_name == "root")
                    {
                        for (string temp; string_buf >> temp;)
                        {   //qDebug()<<"create_asf_root\n";
                            h_asf.root.children.push_back(temp);
                            for (int i = 1; i < h_asf.joints.size(); i++) //nie sprawdzać zerowego bo tam jest dummy root // to jest zapis nazwy parent w joint child.
                            {
                                //qDebug()<<"create_asf_roo_joint\n";
                                if (h_asf.joints[i].name==temp)
                                {
                                    h_asf.joints[i].parent = parent_name;
                                    break;
                                }

                            }
                        }
                        h_asf.joints[0].children=h_asf.root.children;
                    }
                    else
                    {
                        int i = 0;
                        while (h_asf.joints[i].name!=parent_name)
                        {
                            //qDebug()<<"create_asf_parent\n";
                            i++;
                        }
                        for (string temp; string_buf >> temp;)
                        {
                            //qDebug()<<"create_asf_child\n";
                            h_asf.joints[i].children.push_back(temp);
                            for (int j = 1; j < h_asf.joints.size(); j++) //nie sprawdzać zerowego bo tam jest dummy root // to jest zapis nazwy parent w joint child.
                            {
                                //qDebug()<<"create_asf_child_joint\n";
                                //string lol =h_asf.joints[j].name;
                                if (h_asf.joints[j].name == temp)
                                {
                                    h_asf.joints[j].parent = parent_name;
                                    break;



                                }
                            }
                        }
                    }
                }
            }
            hierarchy_fl=false;
            break;

        default:
            break;
        }
    }


    qDebug()<<QString::fromStdString(h_asf.root.name);
    return h_asf;
    }
joint_asf Data::create_joints_asf(fstream& file) { ///joint dla asf
    
    string text;
    istringstream string_buf;
    joint_asf joint;
    double temp_d;

        while (text != "end")
        {
            file >> text;
            //qDebug() << QString::fromStdString(text + "\n");
            if (text == "id")
            {
                getline(file, text);
                istringstream string_buf(text);
                for (int temp; string_buf >> temp;)
                {
                    joint.id = temp;
                }
            }
            else
            {
                if (text == "name")
                {
                    getline(file, text);
                    istringstream string_buf(text);
                    for (string temp; string_buf >> temp;)
                    {
                        joint.name = temp;
                    }
                }
                else
                {
                    if (text == "direction")
                    {
                        getline(file, text);
                        istringstream string_buf(text);
                        for (double temp; string_buf >> temp;)
                        {
                            joint.direction.push_back(temp);
                        }
                    }
                    else
                    {
                        if (text == "length")
                        {
                            getline(file, text);
                            istringstream string_buf(text);
                            for (double temp; string_buf >> temp;)
                            {
                                joint.length=temp;
                            }
                        }
                        else
                        {
                            if (text == "axis")
                            {
                                for (int x = 0; x < 3; x++)
                                {
                                    file >> temp_d;
                                    joint.axis.push_back(temp_d);

                                }
                                file >> text;
                                istringstream string_buf(text);
                                for (char temp; string_buf >> temp;)
                                {
                                    joint.axis_order.push_back(temp);
                                }


                            }
                            else
                            {
                                if (text == "dof")
                                {
                                    getline(file, text);
                                    istringstream string_buf(text);
                                    for (string temp; string_buf >> temp;)
                                    {
                                        joint.dof.push_back(temp);

                                    }
                                    while (text != "limits")
                                    {
                                        file >> text;
                                    }
                                    for (int i = 0; i < joint.dof.size()*2; i++)
                                    {
                                        file >> text;
                                        QString t = QString::fromStdString(text);
                                        t.remove(QChar('('),Qt::CaseInsensitive);
                                        t.remove(QChar(')'),Qt::CaseInsensitive);
                                        joint.limits.push_back((t.toDouble()));
                                        //qDebug() << t;

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    return joint;

}


angles Data::calc_matrix_3_rot(QMatrix4x4 matrix, string order)
{
    float tab[4][4];
    angles ang;
    double thetaX=0;
    double thetaY=0;
    double thetaZ=0;
    double PI=acos(-1);
        for (int i = 0; i < 4; ++i)
        {
            QVector4D vect=matrix.row(i);
            tab[i][0]=vect.x();
            tab[i][1]=vect.y();
            tab[i][2]=vect.z();
            tab[i][3]=vect.w();
        }

        if(order=="XYZ")
        {
            if (tab[0][2] < 1)
            {
                if (tab[0][2] > -1)
                {
                thetaY = asin(tab[0][2]);
                thetaX = atan2(-tab[1][2],tab[2][2]);
                thetaZ = atan2(-tab[0][1],tab[0][0]);
                }
                else

                {

                thetaY = -PI/2;
                thetaX = -atan2(tab[1][0],tab[1][1]);
                thetaZ = 0;
                }
            }
            else
            {

            thetaY = +PI/2;
            thetaX = atan2(tab[1][0],tab[1][1]);
            thetaZ = 0;
            }
        }
        else
        {
            if (order=="XZY")
            {
                if (tab[0][1] < +1)
                {
                if (tab[0][1] > -1)
                {
                thetaZ = asin(-tab[0][1]);
                thetaX = atan2(tab[2][1],tab[1][1]);
                thetaY = atan2(tab[0][2],tab[0][0]);
                }
                else
                {
                thetaZ = +PI/2;
                thetaX = atan2(-tab[2][0],tab[2][2]);
                thetaY = 0;
                }
                }
                else
                {
                thetaZ = -PI/2;
                thetaX = atan2(-tab[2][0],tab[2][2]);
                thetaY = 0;
                }
            }
            else
            {
                if (order=="YXZ")
                {
                    if (tab[1][2] < +1)
                    {
                        if (tab[1][2] > -1)
                        {
                        thetaX = asin(-tab[1][2]);
                        thetaY = atan2(tab[0][2],tab[2][2]);
                        thetaZ = atan2(tab[1][0],tab[1][1]);
                        }
                        else
                        {
                        thetaX = +PI/2;
                        thetaY = -atan2(-tab[0][1],tab[0][0]);
                        thetaZ = 0;
                        }
                    }
                    else
                    {
                    thetaX = -PI/2;
                    thetaY = atan2(-tab[0][1],tab[0][0]);
                    thetaZ = 0;
                    }
                }
                else
                {
                    if(order=="YZX")
                    {
                        if (tab[1][0] < +1)
                        {
                            if (tab[1][0] > -1)
                            {
                            thetaZ = asin(tab[1][0]);
                            thetaY = atan2(-tab[2][0],tab[0][0]);
                            thetaX = atan2(-tab[1][2],tab[1][1]);
                            }
                            else
                            {
                            thetaZ = -PI/2;
                            thetaY = -atan2(tab[2][1],tab[2][2]);
                            thetaX = 0;
                            }
                        }
                        else
                        {

                        thetaZ = +PI/2;
                        thetaY = atan2(tab[2][1],tab[2][2]);
                        thetaX = 0;
                        }

                    }
                    else
                    {
                        if (order=="ZXY")
                        {
                            if (tab[2][1] < +1)
                            {
                                if (tab[2][1] > -1)
                                {

                                thetaX = asin(tab[2][1]);
                                thetaZ = atan2(-tab[0][1],tab[1][1]);
                                thetaY = atan2(-tab[2][0],tab[2][2]);
                                }
                                else
                                {
                                thetaX = -PI/2;
                                thetaZ = -atan2(tab[2][0],tab[0][0]);
                                thetaY = 0;
                                }
                            }
                            else
                            {
                            thetaX = +PI/2;
                            thetaZ = atan2(tab[2][0],tab[0][0]);
                            thetaY = 0;
                            }

                        }
                        else
                        {
                            if(order=="ZYX")
                            {
                                if (tab[2][0] < +1)
                                {
                                    if (tab[2][0] > -1)
                                    {
                                    thetaY = asin(-tab[2][0]);
                                    thetaZ = atan2(tab[1][0],tab[0][0]);
                                    thetaX = atan2(tab[2][1],tab[2][2]);
                                    }
                                    else
                                    {
                                    thetaY = +PI/2;
                                    thetaZ = atan2(-tab[1][2],tab[1][1]);
                                    thetaX = 0;
                                    }
                                }
                                else
                                {
                                thetaY = -PI/2;
                                thetaZ = atan2(-tab[1][2],tab[1][1]);
                                thetaX = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        thetaX=thetaX*57.2957795;
        thetaY=thetaY*57.2957795;
        thetaZ=thetaZ*57.2957795;

    ang.x=thetaX;
    ang.y=thetaY;
    ang.z=thetaZ;
    return ang;
}



void Data::bvh_reorder(hierarchy_bvh &h_bvh_org, frames_data &f_bvh_org, string rot_order)
{
    for (int j = 0; j < h_bvh_org.joints.size(); ++j)
    {
        if (h_bvh_org.joints[j].channels.size()>0) {

        double v_rx=0;
        double v_ry=0;
        double v_rz=0;
        double v_tx=0;
        double v_ty=0;
        double v_tz=0;
        int p_rx;
        int p_ry;
        int p_rz;
        int p_tx;
        int p_ty;
        int p_tz;

        int i1=0;

         bool tx=false;
         bool ty=false;
        bool tz=false;
        bool rx=false;
        bool ry=false;
        bool rz=false;
        vector<string> old_order=h_bvh_org.joints[j].channels;
        vector<string> new_order;

        vector<string> translations;
        vector<string> rotations;
        QMatrix4x4 mat;
        mat.setToIdentity();
        angles xyz;

        int sf=h_bvh_org.joints[j].self_index_frame;

                for (int ch = 0; ch < h_bvh_org.joints[j].channels.size(); ++ch)
                {
                    if (h_bvh_org.joints[j].channels[ch]=="Xposition")
                    {
                        p_tx=ch;
                        tx=true;
                    }
                    else if (h_bvh_org.joints[j].channels[ch]=="Yposition")
                         {
                           p_ty=ch;
                           ty=true;
                         }
                        else if (h_bvh_org.joints[j].channels[ch]=="Zposition")
                            {
                               p_tz=ch;
                               tz=true;
                            }
                            else if (h_bvh_org.joints[j].channels[ch]=="Xrotation")
                                {
                                    p_rx=ch;

                                }
                                else if (h_bvh_org.joints[j].channels[ch]=="Yrotation")
                                    {
                                        p_ry=ch;

                                    }
                                    else if (h_bvh_org.joints[j].channels[ch]=="Zrotation")
                                        {
                                            p_rz=ch;

                                        }

                }

        string str=rot_order;

        istringstream buf(str);
        for (char c; buf>>c;)
        {
            if (c=='X')
            {
                if (tx==true)
                {
                    translations.push_back("Xposition");
                }

                    rotations.push_back("Xrotation");

            }
            else if (c=='Y')
                {
                    if (ty==true)
                    {
                        translations.push_back("Yposition");
                    }

                        rotations.push_back("Yrotation");

                }
                else if (c=='Z')
                    {
                        if (tz==true)
                        {
                            translations.push_back("Zposition");
                        }

                            rotations.push_back("Zrotation");

                    }
        }

        new_order.reserve(translations.size()+rotations.size());
        new_order.insert(new_order.end(),translations.begin(),translations.end());
        new_order.insert(new_order.end(),rotations.begin(),rotations.end());

        //dla każdego frame
        for (int fr = 0; fr < f_bvh_org.frames.size(); ++fr)
        {
            mat.setToIdentity();

            //macierz rotacji i wartości translacji
            for (int cc = 0; cc < h_bvh_org.joints[j].channels.size(); ++cc)
            {
                if (h_bvh_org.joints[j].channels[cc]=="Xposition")
                {
                    v_tx=f_bvh_org.frames[fr].channel_data[sf].channels[cc];
                }
                else if (h_bvh_org.joints[j].channels[cc]=="Yposition")
                     {
                       v_ty=f_bvh_org.frames[fr].channel_data[sf].channels[cc];
                     }
                    else if (h_bvh_org.joints[j].channels[cc]=="Zposition")
                        {
                          v_tz=f_bvh_org.frames[fr].channel_data[sf].channels[cc];
                        }
                        else if (h_bvh_org.joints[j].channels[cc]=="Xrotation")
                            {
                                v_rx=f_bvh_org.frames[fr].channel_data[sf].channels[cc];
                                QMatrix4x4 Rx;
                                Rx.setToIdentity();
                                Rx.rotate(v_rx,1,0,0);
                                mat=mat*Rx;

                            }
                            else if (h_bvh_org.joints[j].channels[cc]=="Yrotation")
                                {
                                    v_ry=f_bvh_org.frames[fr].channel_data[sf].channels[cc];
                                    QMatrix4x4 Ry;
                                    Ry.setToIdentity();
                                    Ry.rotate(v_ry,0,1,0);
                                    mat=mat*Ry;

                                }
                                else if (h_bvh_org.joints[j].channels[cc]=="Zrotation")
                                    {
                                        v_rz=f_bvh_org.frames[fr].channel_data[sf].channels[cc];
                                        QMatrix4x4 Rz;
                                        Rz.setToIdentity();
                                        Rz.rotate(v_rz,0,0,1);
                                        mat=mat*Rz;

                                    }

            }
            //wylicznie nowych kątów
            xyz=calc_matrix_3_rot(mat,rot_order);
            v_rx=xyz.x;
            v_ry=xyz.y;
            v_rz=xyz.z;

            //utworzenie nowych danych według nowej rot_order
            vector<double> db;

            for (int q = 0; q < new_order.size(); ++q)
            {
                if (new_order[q]=="Xposition")
                {
                    db.push_back(v_tx);
                }
                else if (new_order[q]=="Yposition")
                     {
                       db.push_back(v_ty);
                     }
                    else if (new_order[q]=="Zposition")
                        {
                          db.push_back(v_tz);
                        }
                        else if (new_order[q]=="Xrotation")
                            {
                                db.push_back(v_rx);

                            }
                            else if (new_order[q]=="Yrotation")
                                {
                                    db.push_back(v_ry);

                                }
                                else if (new_order[q]=="Zrotation")
                                    {
                                        db.push_back(v_rz);
                                    }
            }
         f_bvh_org.frames[fr].channel_data[sf].channels=db;
        }

        h_bvh_org.joints[j].channels=new_order;
        h_bvh_org.joints[j].channel_count=new_order.size();
        }
        //wpisać nowy order


    }

    h_bvh_org.root.channel_count=h_bvh_org.joints[0].channel_count;
    h_bvh_org.root.channel_names=h_bvh_org.joints[0].channels;


}

void Data::asf_reorder(hierarchy_asf &h_asf, frames_data &f_amc, string rot_order)
{


    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        if (i==0)
        {
            vector<char> old_axis_order=h_asf.joints[0].axis_order;
            vector<string> old_dof_order=h_asf.joints[0].dof;
            vector<string> a;
            //vector<char> new_axis_order;
            vector<string> new_dof_order;
            vector<string> translation_order;
            vector<string> rotation_order;
            float ax,ay,az;
            QMatrix4x4 C;
            angles ang;

            ax=h_asf.joints[0].axis[0];
            ay=h_asf.joints[0].axis[1];
            az=h_asf.joints[0].axis[2];

            for (int iter = 0; iter < old_axis_order.size(); ++iter)
            {
                if (h_asf.joints[0].axis_order[iter]=='X')
                {
                    C.rotate(ax,1,0,0);
                }

                if (h_asf.joints[0].axis_order[iter]=='Y')
                {
                    C.rotate(ay,0,1,0);
                }

                if (h_asf.joints[0].axis_order[iter]=='Z')
                {
                    C.rotate(az,0,0,1);
                }
            }
            ang=calc_matrix_3_rot(C,rot_order);

            h_asf.joints[0].axis[0]=ang.x;
            h_asf.root.orientation[0]=ang.x;
            h_asf.joints[0].axis[1]=ang.y;
            h_asf.root.orientation[1]=ang.y;
            h_asf.joints[0].axis[2]=ang.z;
            h_asf.root.orientation[2]=ang.z;

            h_asf.joints[0].axis_order.clear();
            h_asf.root.axis.clear();

            istringstream buf(rot_order);
            for (char c; buf>>c;)
            {
                h_asf.joints[0].axis_order.push_back(c);
                h_asf.root.axis.push_back(c);
            }

            for (int i = 0; i < h_asf.joints[0].dof.size(); ++i)
            {
                if (h_asf.joints[0].dof[i]=="RX")
                {
                    old_dof_order.push_back("RX");
                }
                else
                    if (h_asf.joints[0].dof[i]=="RY")
                    {
                        old_dof_order.push_back("RY");
                    }
                    else
                        if (h_asf.joints[0].dof[i]=="RZ")
                        {
                            old_dof_order.push_back("RZ");
                        }
                        else
                            if (h_asf.joints[0].dof[i]=="TX")
                            {
                                old_dof_order.push_back("TX");
                            }
                            else
                                if (h_asf.joints[0].dof[i]=="TY")
                                {
                                    old_dof_order.push_back("TY");
                                }
                                else
                                    if (h_asf.joints[0].dof[i]=="TZ")
                                    {
                                        old_dof_order.push_back("TZ");
                                    }
            }

            istringstream buf2(rot_order);
            for (char c; buf2>>c;)
            {
                if (c=='X')
                {
                    translation_order.push_back("TX");
                    rotation_order.push_back("RX");
                }
                else
                    if (c=='Y')
                    {
                        translation_order.push_back("TY");
                        rotation_order.push_back("RY");
                    }
                    else
                        if (c=='Z')
                        {
                            translation_order.push_back("TZ");
                            rotation_order.push_back("RZ");
                        }
            }


            new_dof_order=translation_order;
            new_dof_order.insert(new_dof_order.end(),rotation_order.begin(),rotation_order.end());
            h_asf.joints[0].dof=new_dof_order;
            h_asf.root.order=new_dof_order;

            int f_p=find_joint_index_f_amc(f_amc,h_asf.joints[0].name);



            for (int i1 = 0; i1 < f_amc.frames.size(); ++i1)
            {
                float rx=0,ry=0,rz=0,tx=0,ty=0,tz=0;
                QMatrix4x4 M;
                QMatrix4x4 X;
                QMatrix4x4 Y;
                QMatrix4x4 Z;
                vector <double> channel_values;

                for (int itf = 0; itf < old_dof_order.size(); ++itf)
                {
                    if (old_dof_order[itf]=="RX")
                    {
                       rx=f_amc.frames[i1].channel_data[f_p].channels[itf];
                       X.rotate(rx,1,0,0);
                    }
                    else
                        if (old_dof_order[itf]=="RY")
                        {
                            ry=f_amc.frames[i1].channel_data[f_p].channels[itf];
                            Y.rotate(ry,0,1,0);
                        }
                        else
                            if (old_dof_order[itf]=="RZ")
                            {
                                rz=f_amc.frames[i1].channel_data[f_p].channels[itf];
                                Z.rotate(rz,0,0,1);
                            }
                            else
                                if (old_dof_order[itf]=="TX")
                                {
                                    tx=f_amc.frames[i1].channel_data[f_p].channels[itf];
                                }
                                else
                                    if (old_dof_order[itf]=="TY")
                                    {
                                        ty=f_amc.frames[i1].channel_data[f_p].channels[itf];
                                    }
                                    else
                                        if (old_dof_order[itf]=="TZ")
                                        {
                                            tz=f_amc.frames[i1].channel_data[f_p].channels[itf];
                                        }
                }
                for (int a=0;old_axis_order.size();++a)
                {
                    if (old_axis_order[a]=='X'||old_axis_order[a]=='x')
                    {
                        M=M*X;
                    }
                    else
                        if (old_axis_order[a]=='Y'||old_axis_order[a]=='y')
                        {
                            M=M*Y;
                        }
                        else
                            if (old_axis_order[a]=='Z'||old_axis_order[a]=='z')
                            {
                                M=M*Z;
                            }
                }

                ang=calc_matrix_3_rot(M,rot_order);

                for (int i2 = 0; i2 < new_dof_order.size(); ++i)
                {
                    if (new_dof_order[i2]=="RX")
                    {
                       channel_values.push_back(ang.x);
                    }
                    else
                        if (new_dof_order[i2]=="RY")
                        {
                            channel_values.push_back(ang.y);
                        }
                        else
                            if (new_dof_order[i2]=="RZ")
                            {
                                channel_values.push_back(ang.z);
                            }
                            else
                                if (new_dof_order[i2]=="TX")
                                {
                                    channel_values.push_back(tx);
                                }
                                else
                                    if (new_dof_order[i2]=="TY")
                                    {
                                        channel_values.push_back(ty);
                                    }
                                    else
                                        if (new_dof_order[i2]=="TZ")
                                        {
                                            channel_values.push_back(tz);
                                        }
                }

                f_amc.frames[i1].channel_data[f_p].channels=channel_values;
            }
        }
        else
        {
            vector<char> old_axis_order=h_asf.joints[i].axis_order;
            vector<string> old_dof_order=h_asf.joints[i].dof;
            vector<string> a;
            //vector<char> new_axis_order;
            vector<string> new_dof_order;
            //vector<string> translation_order;
            //vector<string> rotation_order;
            float ax,ay,az;
            QMatrix4x4 C;
            angles ang;

            ax=h_asf.joints[i].axis[0];
            ay=h_asf.joints[i].axis[1];
            az=h_asf.joints[i].axis[2];

            for (int iter = 0; iter < old_axis_order.size(); ++iter)
            {
                if (old_axis_order[iter]=='X')
                {
                    C.rotate(ax,1,0,0);
                }

                if (old_axis_order[iter]=='Y')
                {
                    C.rotate(ay,0,1,0);
                }

                if (old_axis_order[iter]=='Z')
                {
                    C.rotate(az,0,0,1);
                }
            }
            ang=calc_matrix_3_rot(C,rot_order);
            h_asf.joints[i].axis[0]=ang.x;
            h_asf.joints[i].axis[1]=ang.y;
            h_asf.joints[i].axis[2]=ang.z;

            h_asf.joints[i].axis_order.clear();


            istringstream buf3(rot_order);
            for (char c; buf3>>c;)
            {
                h_asf.joints[i].axis_order.push_back(c);
            }

            if (h_asf.joints[i].dof.size()>0)
            {
                istringstream buf2(rot_order);
                for (char c; buf2>>c;)
                {
                    if (c=='X')
                    {
                        new_dof_order.push_back("rx");
                    }
                    else
                        if (c=='Y')
                        {
                            new_dof_order.push_back("ry");
                        }
                        else
                            if (c=='Z')
                            {
                                new_dof_order.push_back("rz");
                            }
                }

                int in_frame_index=find_joint_index_f_amc(f_amc,h_asf.joints[i].name);

                for (int i1 = 0; i1 < f_amc.frames.size(); ++i1)
                {
                    float rx=0,ry=0,rz=0;
                    QMatrix4x4 M;
                    QMatrix4x4 X;
                    QMatrix4x4 Y;
                    QMatrix4x4 Z;
                    vector <double> channel_values;

                    for (int itf = 0; itf < old_dof_order.size(); ++itf)
                    {
                        if (old_dof_order[itf]=="rz")
                        {
                           rx=f_amc.frames[i1].channel_data[in_frame_index].channels[itf];
                           X.rotate(rx,1,0,0);
                        }
                        else
                            if (old_dof_order[itf]=="ry")
                            {
                                ry=f_amc.frames[i1].channel_data[in_frame_index].channels[itf];
                                Y.rotate(ry,0,1,0);
                            }
                            else
                                if (old_dof_order[itf]=="rz")
                                {
                                    rz=f_amc.frames[i1].channel_data[in_frame_index].channels[itf];
                                    Z.rotate(rz,0,0,1);
                                }
                    }
                    for (int a=0;old_axis_order.size();++a)
                    {
                        if (old_axis_order[a]=='X'||old_axis_order[a]=='x')
                        {
                            M=M*X;
                        }
                        else
                            if (old_axis_order[a]=='Y'||old_axis_order[a]=='y')
                            {
                                M=M*Y;
                            }
                            else
                                if (old_axis_order[a]=='Z'||old_axis_order[a]=='z')
                                {
                                    M=M*Z;
                                }
                    }

                    ang=calc_matrix_3_rot(M,rot_order);

                    for (int i = 0; i < new_dof_order.size(); ++i)
                    {
                        if (new_dof_order[i]=="rx")
                        {
                          channel_values.push_back(ang.x);
                        }
                        else
                            if (new_dof_order[i]=="ry")
                            {
                                channel_values.push_back(ang.y);
                            }
                            else
                                if (new_dof_order[i]=="rz")
                                {
                                    channel_values.push_back(ang.z);
                                }

                    }

                    f_amc.frames[i1].channel_data[in_frame_index].channels=channel_values;
                }
            }

        }
    }

}

void Data::bvh_depth(hierarchy_bvh &h_bvh)
{
    h_bvh.root.depth=0;
    h_bvh.joints[0].depth=0;
    for (int i = 0; i < h_bvh.joints[0].children.size() ; ++i)
    {
        int index_c =find_joint_index_h_bvh(h_bvh,h_bvh.joints[0].children[i]);
        bvh_depth_child(h_bvh,index_c,0);
    }
}

void Data::bvh_depth_child(hierarchy_bvh &h_bvh , int index , int parent)
{
     h_bvh.joints[index].depth=1+h_bvh.joints[parent].depth;

     for (int i = 0; i < h_bvh.joints[index].children.size() ; ++i)
     {
         int index_c =find_joint_index_h_bvh(h_bvh,h_bvh.joints[index].children[i]);
         bvh_depth_child(h_bvh,index_c,index);
     }


}

void Data::bvh_pc_index(hierarchy_bvh &h_bvh, frames_data &f_bvh)
{
    for (int i = 0; i < h_bvh.joints.size(); ++i)
    {
        h_bvh.joints[i].self_index_frame=0;
        h_bvh.joints[i].self_index_joint=0;
        h_bvh.joints[i].parent_index_joint=0;
        h_bvh.joints[i].child_index_joint.clear();

        h_bvh.joints[i].self_index_joint=i;


        if(h_bvh.joints[i].channels.size()>0)
        {
            int f=0;
            while (h_bvh.joints[i].name!=f_bvh.frames[0].channel_data[f].name)
            {
                f++;
            }
            h_bvh.joints[i].self_index_frame=f;
        }
        if (i!=0)
        {
            int p=0;
            while (h_bvh.joints[i].parent!=h_bvh.joints[p].name)
            {
                p++;
            }
            h_bvh.joints[i].parent_index_joint=p;
        }
        for (int c = 0; c < h_bvh.joints[i].children.size(); ++c)
        {
            int m=0;
            while (h_bvh.joints[i].children[c]!=h_bvh.joints[m].name)
            {
                m++;
            }

            h_bvh.joints[i].child_index_joint.push_back(m);
        }
    }


    }

//h_asf.joints[i].child_index_joint;
//h_asf.joints[i].self_index_frame;
//h_asf.joints[i].self_index_joint;
//h_asf.joints[i].parent_index_joint;


void Data::asf_pc_index(hierarchy_asf &h_asf, frames_data &f_amc)
{

    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        //self index joint
        h_asf.joints[i].self_index_joint=i;

        for (int j = 0; j < h_asf.joints[i].children.size(); ++j)
        {
            int k=0;
            for (  k; k < h_asf.joints.size(); ++k)
            {
                if (h_asf.joints[k].name==h_asf.joints[i].children[j])
                {
                    break;
                }

            }
            //child index
            h_asf.joints[i].child_index_joint.push_back(k);
        }
        if (h_asf.joints[i].dof.size()>0)
        {
            int k=0;
            for ( k = 0; k < f_amc.frames[0].channel_data.size(); k++)
            {
                if (f_amc.frames[0].channel_data[k].name==h_asf.joints[i].name) {
                    break;
                }
            }
            //self index frame
            h_asf.joints[i].self_index_frame=k;
        }
        if(i!=0)
        {
            int k=0;
            for (k = 0;  k< h_asf.joints.size(); k++)
            {
                if (h_asf.joints[k].name==h_asf.joints[i].parent)
                    {
                    break;
                    }
            }
            h_asf.joints[i].parent_index_joint=k;
        }
    }
}


void Data::bvh_positions(hierarchy_bvh &h_bvh, frames_data &f_bvh)
{
    bvh_root(h_bvh);

    for (int f = 0; f < f_bvh.frames.size(); ++f)
    {
        bvh_root_frame(h_bvh, f_bvh, f);
    }
}

void Data::bvh_root(hierarchy_bvh &h_bvh)
{
    cords cords_root;
    angles angle;
    translations tr;
    int p=0;
    h_bvh.joints[0].cords.clear();
    h_bvh.joints[0].angles.clear();
    h_bvh.root.point_to_world_matrices.clear();
    h_bvh.root.angles.clear();
    h_bvh.root.cords.clear();
    h_bvh.joints[0].trans.clear();


    cords_root.x=h_bvh.joints[0].offset[0];
    cords_root.y=h_bvh.joints[0].offset[1];
    cords_root.z=h_bvh.joints[0].offset[2];


    QMatrix4x4 mat;
    mat.setToIdentity();
    h_bvh.joints[0].vector_to_position.push_back(mat);
    h_bvh.joints[0].self_matrix.push_back(mat);
    //mat.translate(cords_root.x,cords_root.y,cords_root.z);

    string name=h_bvh.joints[0].name;
    qDebug()<<QString::fromStdString(name)<<" frame 0";
    qDebug()<<mat<<"\n";

    angle.x=0;
    angle.y=0;
    angle.z=0;

    h_bvh.joints[0].trans.push_back(tr);
    h_bvh.joints[0].transformation_matrix=mat;
    h_bvh.joints[0].cords.push_back(cords_root);
    h_bvh.joints[0].angles.push_back(angle);
    h_bvh.joints[0].global_matrix.push_back(mat);
    h_bvh.root.angles.push_back(angle);
    h_bvh.root.cords.push_back(cords_root);
    h_bvh.root.point_to_world_matrices.push_back(mat);

    int d=h_bvh.joints[0].child_index_joint.size();

    for (int i = 0; i < h_bvh.joints[0].children.size(); ++i)
    {
        bvh_joint(h_bvh, h_bvh.joints[0].child_index_joint[i],0,cords_root.x,cords_root.y,cords_root.z);
    }
}

void Data::bvh_joint(hierarchy_bvh &h_bvh, int index, int parent, double x, double y,double z)
{
    QVector3D cord;
    QMatrix4x4 transform;


    cords xyz;
    angles angle;
    QVector3D vec;
    QMatrix4x4 mat;
    int p=0;
    h_bvh.joints[index].cords.clear();
    h_bvh.joints[index].angles.clear();
    angle.x=0;
    angle.y=0;
    angle.z=0;

    mat.setToIdentity();
    h_bvh.joints[index].vector_to_position.push_back(mat);
    h_bvh.joints[index].self_matrix.push_back(mat);

    x=h_bvh.joints[index].offset[0]+x;
    y=h_bvh.joints[index].offset[1]+y;
    z=h_bvh.joints[index].offset[2]+z;
    xyz.x=x;
    xyz.y=y;
    xyz.z=z;


    angle.x=0;
    angle.y=0;
    angle.z=0;

    string name=h_bvh.joints[index].name;
    qDebug()<<QString::fromStdString(name)<<" frame 0";
    qDebug()<<mat<<"\n";


    h_bvh.joints[index].cords.push_back(xyz);
    h_bvh.joints[index].angles.push_back(angle);
    h_bvh.joints[index].global_matrix.push_back(mat);
    h_bvh.joints[index].transformation_matrix=mat;


    for (int i = 0; i < h_bvh.joints[index].children.size(); ++i)
    {
      bvh_joint(h_bvh, h_bvh.joints[index].child_index_joint[i],index, x, y, z);
    }
}

void Data::bvh_root_frame(hierarchy_bvh &h_bvh, frames_data &f_bvh, int frame)// tu jesteś
{
    QVector3D offset;
    QVector3D frame_offset;
    QVector3D sum;
    QMatrix4x4 transform;
    QMatrix4x4 local_rotation;
    translations tr;
    int f=frame;
    cords par;
    angles ang;
    double x,y,z;
    //int t=0;
    int r=0;
    transform.setToIdentity();
    local_rotation.setToIdentity();
    ang.x=0;
    ang.y=0;
    ang.z=0;


     offset.setX(h_bvh.joints[0].offset[0]);
     offset.setY(h_bvh.joints[0].offset[1]);
     offset.setZ(h_bvh.joints[0].offset[2]);
    for (int i = 0; i < h_bvh.joints[0].channels.size(); ++i)
    {
        if(h_bvh.joints[0].channels[i]=="Xposition")
        {
           
            frame_offset.setX(f_bvh.frames[f].channel_data[0].channels[i]);
            tr.x=f_bvh.frames[f].channel_data[0].channels[i];
        }
        else
            {
            if(h_bvh.joints[0].channels[i]=="Yposition")
            {
               
               frame_offset.setY(f_bvh.frames[f].channel_data[0].channels[i]);
               tr.y=f_bvh.frames[f].channel_data[0].channels[i];
            }
            else
                {
                if(h_bvh.joints[0].channels[i]=="Zposition")
                {
                    
                    frame_offset.setZ(f_bvh.frames[f].channel_data[0].channels[i]);
                    tr.z=f_bvh.frames[f].channel_data[0].channels[i];
                }
                else
                {
                    if(h_bvh.joints[0].channels[i]=="Xrotation")
                    {
                        transform.rotate(f_bvh.frames[f].channel_data[0].channels[i],QVector3D(1,0,0));\
                        ang.x=f_bvh.frames[f].channel_data[0].channels[i];
                    }
                    else
                    {
                        if(h_bvh.joints[0].channels[i]=="Yrotation")
                        {
                            transform.rotate(f_bvh.frames[f].channel_data[0].channels[i],QVector3D(0,1,0));
                            ang.y=f_bvh.frames[f].channel_data[0].channels[i];
                        }
                        else
                        {
                            if(h_bvh.joints[0].channels[i]=="Zrotation")
                            {
                                transform.rotate(f_bvh.frames[f].channel_data[0].channels[i],QVector3D(0,0,1));
                                ang.z=f_bvh.frames[f].channel_data[0].channels[i];
                            }
                        }
                    }
                }
            }
        }
    }


    x=offset.x()+frame_offset.x();
    y=offset.y()+frame_offset.y();
    z=offset.z()+frame_offset.z();


    par.x=x;
    par.y=y;
    par.z=z;
    transform=transform.transposed();
    h_bvh.joints[0].vector_to_position.push_back(transform);
    h_bvh.joints[0].self_matrix.push_back(transform);

    string name=h_bvh.joints[0].name;
    qDebug()<<QString::fromStdString(name)<<" frame "<<QString::number(frame+1);
    qDebug()<<transform<<"\n";



    h_bvh.joints[0].transformation_matrix=transform.transposed();
    h_bvh.joints[0].cords.push_back(par);
    h_bvh.joints[0].angles.push_back(ang);
    h_bvh.joints[0].global_matrix.push_back(transform);
    h_bvh.joints[0].trans.push_back(tr);
    h_bvh.root.angles.push_back(ang);
    h_bvh.root.cords.push_back(par);
    h_bvh.root.point_to_world_matrices.push_back(transform);
    h_bvh.root.transformation_matrix=transform;

    for (int i = 0; i < h_bvh.joints[0].children.size(); ++i)
    {
        bvh_joint_frame(h_bvh,f_bvh,f,h_bvh.joints[0].child_index_joint[i],0,x,y,z);
    }
}

void Data::bvh_joint_frame(hierarchy_bvh &h_bvh, frames_data &f_bvh, int frame, int index, int parent, double x, double y,double z)
{
    QVector3D offset;
    QVector3D frame_offset;
    QVector3D sum;
    QMatrix4x4 transform;
    QMatrix4x4 local_rotation;
    int f=frame;
    cords par;
    angles ang;
    int t=0;
    int r=0;
    int p=0;
    int fji;
    transform.setToIdentity();
    ang.x=0;
    ang.y=0;
    ang.z=0;

    offset.setX(h_bvh.joints[index].offset[0]);
    offset.setY(h_bvh.joints[index].offset[1]);
    offset.setZ(h_bvh.joints[index].offset[2]);


    for (int jt = 0; jt < f_bvh.frames[0].channel_data.size(); ++jt)
    {
        if (f_bvh.frames[0].channel_data[jt].id==h_bvh.joints[index].id)
        {
          fji=jt;
          break;
        }
    }

    for (int i = 0; i < h_bvh.joints[index].channels.size(); ++i)
    {

        if(h_bvh.joints[index].channels[i]=="Xrotation")
        {
            transform.rotate(f_bvh.frames[f].channel_data[fji].channels[i],QVector3D(1,0,0));\
            ang.x=f_bvh.frames[f].channel_data[fji].channels[i];
        }
        else
        {
            if(h_bvh.joints[index].channels[i]=="Yrotation")
            {
                transform.rotate(f_bvh.frames[f].channel_data[fji].channels[i],QVector3D(0,1,0));
                ang.y=f_bvh.frames[f].channel_data[fji].channels[i];
            }
            else
            {
                if(h_bvh.joints[index].channels[i]=="Zrotation")
                {
                    transform.rotate(f_bvh.frames[f].channel_data[fji].channels[i],QVector3D(0,0,1));
                    ang.z=f_bvh.frames[f].channel_data[fji].channels[i];
                }
            }
        }
    }

    QVector3D position = offset*h_bvh.joints[parent].transformation_matrix;

    h_bvh.joints[index].vector_to_position.push_back(h_bvh.joints[parent].transformation_matrix);
    h_bvh.joints[index].self_matrix.push_back(transform.transposed());

    string name=h_bvh.joints[index].name;
    qDebug()<<QString::fromStdString(name)<<" frame "<<QString::number(frame+1);
    qDebug()<<h_bvh.joints[parent].transformation_matrix<<"\n";

    QVector3D position_parent(x,y,z);
    position+=position_parent;

    transform=transform.transposed()*h_bvh.joints[parent].transformation_matrix;

    par.x=position.x();
    par.y=position.y();
    par.z=position.z();




    h_bvh.joints[index].transformation_matrix=transform;
    h_bvh.joints[index].cords.push_back(par);
    h_bvh.joints[index].angles.push_back(ang);
    h_bvh.joints[index].global_matrix.push_back(transform);


    for (int i = 0; i < h_bvh.joints[index].children.size(); ++i)
    {
        bvh_joint_frame(h_bvh,f_bvh,f,h_bvh.joints[index].child_index_joint[i],index,par.x,par.y,par.z);
    }
}

void Data::asf_position(hierarchy_asf &h_asf, frames_data &f_amc)
{
    asf_root(h_asf);
    for (int f = 0; f < f_amc.frames.size(); ++f)
    {
        asf_root_frame(h_asf,f_amc,f);
    }
}

void Data::asf_root(hierarchy_asf &h_asf)
{
    translations tr;
    cords xyz;
    angles ang;
    QMatrix4x4 M;
    QMatrix4x4 C;
    QMatrix4x4 Cinv;
    QMatrix4x4 L;
    QVector3D offset;
    float cx,cy,cz;
    offset.setX(h_asf.joints[0].direction[0]);
    offset.setY(h_asf.joints[0].direction[1]);
    offset.setZ(h_asf.joints[0].direction[2]);

    cx=h_asf.joints[0].axis[0];
    cy=h_asf.joints[0].axis[1];
    cz=h_asf.joints[0].axis[2];

    double ang_x=h_asf.joints[0].axis[0];
    double ang_y=h_asf.joints[0].axis[1];
    double ang_z=h_asf.joints[0].axis[2];
    h_asf.root.angles.clear();
    h_asf.joints[0].angles.clear();
    h_asf.root.cords.clear();
    h_asf.joints[0].cords.clear();
    h_asf.joints[0].vector_to_position.clear();

    ang.x=0;
    ang.y=0;
    ang.z=0;

    for (int i = h_asf.joints[0].axis_order.size()-1; i >= 0 ; --i)
    {
        if (h_asf.joints[0].axis_order[i]=='X')
        {
            C.rotate(ang_x,1,0,0);
            ang.x=ang_x;

        }

        if (h_asf.joints[0].axis_order[i]=='Y')
        {
            C.rotate(ang_y,0,1,0);

            ang.y=ang_y;

        }

        if (h_asf.joints[0].axis_order[i]=='Z')
        {
            C.rotate(ang_z,0,0,1);

            ang.z=ang_z;

        }
    }


    h_asf.joints[0].Cinv=C;
    h_asf.joints[0].C=Cinv;
    L=h_asf.joints[0].Cinv * M * h_asf.joints[0].C;
    h_asf.joints[0].vector_to_position.push_back(L);
    h_asf.joints[0].self_matrix.push_back(L);

    QVector3D vec=offset*L;

    string name=h_asf.joints[0].name;
    qDebug()<<QString::fromStdString(name)<<" frame "<<QString::number(0);
    qDebug()<<L<<"\n";

    xyz.x=vec.x();
    xyz.y=vec.y();
    xyz.z=vec.z();

    h_asf.joints[0].trans.push_back(tr);
    h_asf.joints[0].cords.push_back(xyz);
    h_asf.joints[0].angles.push_back(ang);
    h_asf.root.angles.push_back(ang);
    h_asf.root.cords.push_back(xyz);
    h_asf.joints[0].transformation_matrix=L;
    h_asf.joints[0].point_to_world_matrices.push_back(L);

    for (int i = 0; i < h_asf.joints[0].children.size(); ++i)
    {
        asf_joint(h_asf,h_asf.joints[0].child_index_joint[i],0,vec.x(),vec.y(),vec.z());
    }
}

void Data::asf_joint(hierarchy_asf &h_asf, int index, int parent, double x, double y, double z)
{
    cords xyz;
    angles ang;
    QMatrix4x4 M;
    QMatrix4x4 C;
    QMatrix4x4 Cinv;
    QMatrix4x4 B;
    QMatrix4x4 L;
    QVector3D offset;
    QVector3D parent_position(x,y,z);
    translations tr;


    M.setToIdentity();
    C.setToIdentity();
    Cinv.setToIdentity();
    B.setToIdentity();
    ang.x=0;
    ang.y=0;
    ang.z=0;
    h_asf.joints[index].angles.clear();
    h_asf.joints[index].cords.clear();



    int pos=0;

    for (int i = h_asf.joints[index].axis_order.size()-1; i >=0 ; --i)
    {
        if (h_asf.joints[index].axis_order[i]=='X')
        {
            C.rotate(h_asf.joints[index].axis[0],1,0,0);
            ang.x=0;
        }

        if (h_asf.joints[index].axis_order[i]=='Y')
        {
            C.rotate(h_asf.joints[index].axis[1],0,1,0);
            ang.y=0;
        }

        if (h_asf.joints[index].axis_order[i]=='Z')
        {
            C.rotate(h_asf.joints[index].axis[2],0,0,1);
            ang.z=0;
        }
    }
    h_asf.joints[index].Cinv=C;
    h_asf.joints[index].C=C.inverted();

    offset.setX(h_asf.joints[index].direction[0]*h_asf.joints[index].length);
    offset.setY(h_asf.joints[index].direction[1]*h_asf.joints[index].length);
    offset.setZ(h_asf.joints[index].direction[2]*h_asf.joints[index].length);

    L=h_asf.joints[index].Cinv * M * h_asf.joints[index].C;
    h_asf.joints[index].self_matrix.push_back(L);
    L=L*h_asf.joints[parent].transformation_matrix;
    h_asf.joints[index].vector_to_position.push_back(L);

    string name=h_asf.joints[index].name;
    qDebug()<<QString::fromStdString(name)<<" frame "<<QString::number(0);
    qDebug()<<L<<"\n";

    QVector3D vec=offset*L;
    vec=vec+parent_position;

    xyz.x=vec.x();
    xyz.y=vec.y();
    xyz.z=vec.z();

    h_asf.joints[index].trans.push_back(tr);
    h_asf.joints[index].cords.push_back(xyz);
    h_asf.joints[index].angles.push_back(ang);
    h_asf.root.angles.push_back(ang);
    h_asf.root.cords.push_back(xyz);
    h_asf.joints[index].transformation_matrix=L;
    h_asf.joints[index].point_to_world_matrices.push_back(L);

    for (int i = 0; i < h_asf.joints[index].children.size(); ++i)
    {
        asf_joint(h_asf,h_asf.joints[index].child_index_joint[i],index,vec.x(),vec.y(),vec.z());
    }
}

void Data::asf_root_frame(hierarchy_asf &h_asf, frames_data &f_amc, int f)
{
    cords xyz;
    angles ang;
    QMatrix4x4 M;
    QMatrix4x4 C;
    QMatrix4x4 Cinv;
    QMatrix4x4 B;
    QMatrix4x4 L;
    QMatrix4x4 T;
    QMatrix4x4 X;
    QMatrix4x4 Y;
    QMatrix4x4 Z;
    QVector3D offset;
    QVector3D trans;
    translations tr;

    offset.setX(h_asf.joints[0].direction[0]);
    offset.setY(h_asf.joints[0].direction[1]);
    offset.setZ(h_asf.joints[0].direction[2]);

    M.setToIdentity();
    C.setToIdentity();
    Cinv.setToIdentity();
    B.setToIdentity();
    T.setToIdentity();
    ang.x=0;
    ang.y=0;
    ang.z=0;

    int f_index;

    for (int i = 0; i < f_amc.frames[0].channel_data.size(); ++i)
    {
        if (f_amc.frames[0].channel_data[i].name==h_asf.joints[0].name)
        {
            f_index=i;
            break;
        }
    }


    for (int i = h_asf.joints[0].dof.size()-1; i >= 0; --i)
    {
        if (h_asf.joints[0].dof[i]=="TX")
        {
            trans.setX(f_amc.frames[f].channel_data[f_index].channels[i]);
            tr.x=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[0].dof[i]=="TY")
        {
            trans.setY(f_amc.frames[f].channel_data[f_index].channels[i]);
            tr.y=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[0].dof[i]=="TZ")
        {
            trans.setZ(f_amc.frames[f].channel_data[f_index].channels[i]);
            tr.z=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[0].dof[i]=="RX")
        {
            X.rotate(f_amc.frames[f].channel_data[f_index].channels[i]+h_asf.joints[0].axis[0],1,0,0);
            ang.x=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[0].dof[i]=="RY")
        {
            Y.rotate(f_amc.frames[f].channel_data[f_index].channels[i]+h_asf.joints[0].axis[1],0,1,0);
            ang.y=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[0].dof[i]=="RZ")
        {
            Z.rotate(f_amc.frames[f].channel_data[f_index].channels[i]+h_asf.joints[0].axis[2],0,0,1);
            ang.z=f_amc.frames[f].channel_data[f_index].channels[i];
        }
    }

    offset=offset+trans;
    for (int a=0;h_asf.joints[0].axis_order.size();++a)
    {
        if (h_asf.joints[0].axis_order[a]=='X'||h_asf.joints[0].axis_order[a]=='x')
        {
            M=M*X;
        }
        else
            if (h_asf.joints[0].axis_order[a]=='Y'||h_asf.joints[0].axis_order[a]=='y')
            {
                M=M*Y;
            }
            else
                if (h_asf.joints[0].axis_order[a]=='Z'||h_asf.joints[0].axis_order[a]=='z')
                {
                    M=M*Z;
                }
    }
    M=M.inverted();
    L=M;
    h_asf.joints[0].vector_to_position.push_back(L);
    h_asf.joints[0].self_matrix.push_back(L);

    string name=h_asf.joints[0].name;
    qDebug()<<QString::fromStdString(name)<<" frame "<<QString::number(f+1);
    qDebug()<<L<<"\n";

    xyz.x=offset.x();
    xyz.y=offset.y();
    xyz.z=offset.z();

    h_asf.joints[0].trans.push_back(tr);
    h_asf.joints[0].cords.push_back(xyz);
    h_asf.joints[0].angles.push_back(ang);
    h_asf.root.angles.push_back(ang);
    h_asf.root.cords.push_back(xyz);
    h_asf.joints[0].transformation_matrix=L;
    h_asf.joints[0].point_to_world_matrices.push_back(L);

    for (int i = 0; i < h_asf.joints[0].children.size(); ++i)
    {
        asf_joint_frame(h_asf,f_amc,f,h_asf.joints[0].child_index_joint[i],0,xyz.x,xyz.y,xyz.z);
    }
}

void Data::asf_joint_frame(hierarchy_asf &h_asf, frames_data &f_amc, int f, int index, int parent,double x, double y, double z)
{
    cords xyz;
    angles ang;
    QMatrix4x4 M;
    QMatrix4x4 C;
    QMatrix4x4 Cinv;
    QMatrix4x4 B;
    QMatrix4x4 L;
    QMatrix4x4 X;
    QMatrix4x4 Y;
    QMatrix4x4 Z;
    QMatrix4x4 T;
    QVector3D offset;
    QVector3D translation;
    QVector3D parent_pos(x,y,z);
    translations tr;


    offset.setX(h_asf.joints[index].direction[0]*h_asf.joints[index].length);
    offset.setY(h_asf.joints[index].direction[1]*h_asf.joints[index].length);
    offset.setZ(h_asf.joints[index].direction[2]*h_asf.joints[index].length);

    translation.setX(0);
    translation.setY(0);
    translation.setZ(0);
    M.setToIdentity();
    C.setToIdentity();
    Cinv.setToIdentity();
    B.setToIdentity();
    T.setToIdentity();
    ang.x=0;
    ang.y=0;
    ang.z=0;

    int f_index=0;

    for (int i = 0; i < f_amc.frames[0].channel_data.size(); ++i)
    {
        if (f_amc.frames[0].channel_data[i].name==h_asf.joints[index].name)
        {
            f_index=i;
            break;
        }
    }


    for (int i = h_asf.joints[index].dof.size()-1; i >= 0; --i)
    {
        if (h_asf.joints[index].dof[i]=="tx")
        {
            translation.setX(f_amc.frames[f].channel_data[f_index].channels[i]);
        }

        if (h_asf.joints[index].dof[i]=="ty")
        {
            translation.setY(f_amc.frames[f].channel_data[f_index].channels[i]);
        }

        if (h_asf.joints[index].dof[i]=="tz")
        {
            translation.setZ(f_amc.frames[f].channel_data[f_index].channels[i]);
        }

        if (h_asf.joints[index].dof[i]=="rx")
        {
            X.rotate(f_amc.frames[f].channel_data[f_index].channels[i],1,0,0);
            ang.x=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[index].dof[i]=="ry")
        {
            Y.rotate(f_amc.frames[f].channel_data[f_index].channels[i],0,1,0);
            ang.y=f_amc.frames[f].channel_data[f_index].channels[i];
        }

        if (h_asf.joints[index].dof[i]=="rz")
        {
            Z.rotate(f_amc.frames[f].channel_data[f_index].channels[i],0,0,1);
            ang.z=f_amc.frames[f].channel_data[f_index].channels[i];
        }
    }
    for (int a=0;h_asf.joints[index].axis_order.size();++a)
    {
        if (h_asf.joints[index].axis_order[a]=='X'||h_asf.joints[index].axis_order[a]=='x')
        {
            M=M*X;
        }
        else
            if (h_asf.joints[index].axis_order[a]=='Y'||h_asf.joints[index].axis_order[a]=='y')
            {
                M=M*Y;
            }
            else
                if (h_asf.joints[index].axis_order[a]=='Z'||h_asf.joints[index].axis_order[a]=='z')
                {
                    M=M*Z;
                }
    }
    M=M.inverted();
    L=h_asf.joints[index].Cinv * M * h_asf.joints[index].C;
    h_asf.joints[index].self_matrix.push_back(L);
    L=L*h_asf.joints[parent].transformation_matrix;
    offset=offset+translation;

    string name=h_asf.joints[index].name;
    qDebug()<<QString::fromStdString(name)<<" frame "<<QString::number(f+1);
    qDebug()<<L<<"\n";

    QVector3D position=offset*L;
    position+=parent_pos;

    h_asf.joints[index].vector_to_position.push_back(L);

    xyz.x=position.x();
    xyz.y=position.y();
    xyz.z=position.z();

    h_asf.joints[index].trans.push_back(tr);
    h_asf.joints[index].cords.push_back(xyz);
    h_asf.joints[index].angles.push_back(ang);
    h_asf.root.angles.push_back(ang);
    h_asf.root.cords.push_back(xyz);
    h_asf.joints[index].transformation_matrix=L;
    h_asf.joints[index].point_to_world_matrices.push_back(L);

    for (int i = 0; i < h_asf.joints[index].children.size(); ++i)
    {
        asf_joint_frame(h_asf,f_amc,f,h_asf.joints[index].child_index_joint[i],index,xyz.x,xyz.y,xyz.z);
    }
}


void Data::bvh_add_joint_above(hierarchy_bvh &h_bvh, frames_data &f_bvh, string new_joint_name, string child_name)
{

    joint_bvh joint;
    int child_index=0;
    int parent_index=0;
    int id=0;

    joint.name=new_joint_name;
    joint.offset.push_back(0);
    joint.offset.push_back(0);
    joint.offset.push_back(0);

    child_index=find_joint_index_h_bvh(h_bvh, child_name);
    string parent_name = h_bvh.joints[child_index].parent;
    parent_index=find_joint_index_h_bvh(h_bvh, parent_name);


    for (int i = 0; i < h_bvh.joints[0].channels.size(); ++i)
    {
        if (h_bvh.joints[0].channels[i]=="Zrotation" || h_bvh.joints[0].channels[i]=="Xrotation" || h_bvh.joints[0].channels[i]=="Yrotation") //sprawdzenie kanałów i dodanie ich do kanałów parent
        {
            joint.channels.push_back(h_bvh.joints[0].channels[i]);
        }
    }

    joint.channel_count=joint.channels.size();

    for (int i = 0; i < h_bvh.joints.size(); ++i)
    {
        if (h_bvh.joints[i].id>id)
        {
            id=h_bvh.joints[i].id;
        }
    }
    joint.id=id+1;

    joint.parent=parent_name;
    joint.children.push_back(child_name);
    h_bvh.joints[child_index].parent=joint.name;

    for (int j = 0; j < h_bvh.joints[parent_index].children.size(); ++j)
    {
        if (h_bvh.joints[parent_index].children[j]==child_name)
        {
            h_bvh.joints[parent_index].children.erase(h_bvh.joints[parent_index].children.begin()+j);
            h_bvh.joints[parent_index].children.push_back(joint.name);
            break;
        }
    }

    h_bvh.joints.push_back(joint);

    joint_data joint_frame_data;
    joint_frame_data.name=joint.name;
    joint_frame_data.id=joint.id;
    for (int i = 0; i < joint.channels.size(); ++i)
    {
        joint_frame_data.channels.push_back(0);
    }

    for (int i = 0; i < f_bvh.frames.size(); ++i)
    {
        f_bvh.frames[i].channel_data.push_back(joint_frame_data);
    }
    bvh_pc_index(h_bvh,f_bvh);
    bvh_depth(h_bvh);

}

void Data::bvh_add_joint_below(hierarchy_bvh &h_bvh, frames_data &f_bvh, string new_joint_name, string parent_name)
{
    joint_bvh new_joint;
    int parent_index;
    int id=0;

    new_joint.offset.push_back(0);
    new_joint.offset.push_back(0);
    new_joint.offset.push_back(0);

    new_joint.name=new_joint_name;
    new_joint.parent=parent_name;


    for (int i = 0; i < h_bvh.joints.size(); ++i)
    {
        if (h_bvh.joints[i].id>id)
        {
            id=h_bvh.joints[i].id;
        }
    }
    new_joint.id=id+1;
    new_joint.channel_count=0;

    parent_index=find_joint_index_h_bvh(h_bvh,parent_name);
    h_bvh.joints.push_back(new_joint);





    if (h_bvh.joints[parent_index].children.size()==0)
    {

        if (parent_index==0)
        {
            h_bvh.joints[parent_index].channels.clear();
            h_bvh.joints[parent_index].channels.push_back("Xposition");
            h_bvh.joints[parent_index].channels.push_back("Yposition");
            h_bvh.joints[parent_index].channels.push_back("Zposition");
            h_bvh.joints[parent_index].channels.push_back("Zrotation");
            h_bvh.joints[parent_index].channels.push_back("Xrotation");
            h_bvh.joints[parent_index].channels.push_back("Yrotation");
            h_bvh.joints[parent_index].channel_count=h_bvh.joints[parent_index].channels.size();

            int frames_exist=find_joint_index_f_bvh(f_bvh,h_bvh.joints[parent_index].name);
            if (frames_exist!=-1)
            {
                for (int i = 0; i < f_bvh.frames.size(); ++i)
                {
                    f_bvh.frames[i].channel_data.erase(f_bvh.frames[i].channel_data.begin()+frames_exist);
                }
            }

            joint_data parent_frame_data;
            parent_frame_data.name=parent_name;
            parent_frame_data.id=h_bvh.joints[parent_index].id;
            for (int i = 0; i < h_bvh.joints[parent_index].channels.size(); ++i)
            {
                parent_frame_data.channels.push_back(0);
            }

            for (int i = 0; i < f_bvh.frames.size(); ++i)
            {
                f_bvh.frames[i].channel_data.push_back(parent_frame_data);
            }
        }
        else
        {

                for (int i = 0; i < h_bvh.joints[0].channels.size(); ++i)
                {
                    if (h_bvh.joints[0].channels[i]=="Zrotation" || h_bvh.joints[0].channels[i]=="Xrotation" || h_bvh.joints[0].channels[i]=="Yrotation") //sprawdzenie kanałów i dodanie ich do kanałów parent
                    {
                        h_bvh.joints[parent_index].channels.push_back(h_bvh.joints[0].channels[i]);
                    }
                }
                h_bvh.joints[parent_index].channel_count=h_bvh.joints[parent_index].channels.size();

                joint_data parent_frame_data;
                parent_frame_data.name=parent_name;
                parent_frame_data.id=h_bvh.joints[parent_index].id;
                for (int i = 0; i < h_bvh.joints[parent_index].channels.size(); ++i)
                {
                    parent_frame_data.channels.push_back(0);
                }

                for (int i = 0; i < f_bvh.frames.size(); ++i)
                {
                    f_bvh.frames[i].channel_data.push_back(parent_frame_data);
                }


        }
    }

    h_bvh.joints[parent_index].children.push_back(new_joint_name);
    bvh_pc_index(h_bvh, f_bvh);
    bvh_depth(h_bvh);
}

void Data::bvh_change_name(hierarchy_bvh &h_bvh, frames_data &f_bvh, string old_name, string new_name)
{
    int hierarchy_joint_index=0;
    int frames_joint_index=0;
    int parent_index;

    hierarchy_joint_index= find_joint_index_h_bvh(h_bvh,old_name);
    
    frames_joint_index=find_joint_index_f_bvh(f_bvh,old_name);
    

    
    parent_index=find_joint_index_h_bvh(h_bvh,h_bvh.joints[hierarchy_joint_index].parent);
    h_bvh.joints[hierarchy_joint_index].name=new_name;//ustawineie nowej nazwy w parent
    
    for (int i = 0; i < h_bvh.joints[parent_index].children.size(); ++i) 
    {
        if (h_bvh.joints[parent_index].children[i]==old_name) 
        {
            h_bvh.joints[parent_index].children[i]=new_name;
            break;            
        }
    }
    
    for (int i = 0; i < h_bvh.joints[hierarchy_joint_index].children.size(); ++i) //ustawienie nowej nazwy w dzieciach
    {
        h_bvh.joints[h_bvh.joints[hierarchy_joint_index].child_index_joint[i]].parent=new_name;
    }
    
    for (int i = 0; i < f_bvh.frames.size(); ++i) //ustawienie nowej nazwy w frame
    {
        f_bvh.frames[i].channel_data[frames_joint_index].name=new_name;
    }
    
 
    
}

void Data::bvh_remove_joint(hierarchy_bvh &h_bvh, frames_data &f_bvh, string name)
{
    int joint_index=0;
    int parent_index=0;
    int f;
    string parent_name;


    joint_index=find_joint_index_h_bvh(h_bvh,name);
    if (joint_index==-1)
    {
        return;
    }




    parent_name=h_bvh.joints[joint_index].parent;


    parent_index=find_joint_index_h_bvh(h_bvh,parent_name);

    for (int i = 0; i < h_bvh.joints[parent_index].children.size(); ++i) // usunięcie podanego joint z dzieci parenta.
    {
        if (h_bvh.joints[parent_index].children[i]==name)
        {
            h_bvh.joints[parent_index].children.erase(h_bvh.joints[parent_index].children.begin()+i);
            break;
        }
    }


    for (int i = 0; i < h_bvh.joints[joint_index].children.size(); ++i)
    {
        h_bvh.joints[parent_index].children.push_back(h_bvh.joints[joint_index].children[i]);//przypisanie dziecka do parenta
        int child_index=find_joint_index_h_bvh(h_bvh,h_bvh.joints[joint_index].children[i]);
        h_bvh.joints[child_index].parent=parent_name;//przypisanie parent do dzieci
    }

    if (h_bvh.joints[parent_index].children.size()==0 && h_bvh.joints[parent_index].channel_count>0) //jeżeli parent nie posiada dzieci to staje sie end site
    {

        h_bvh.joints[parent_index].channels.clear();
        h_bvh.joints[parent_index].channel_count=0;
        int ind_f_p=find_joint_index_f_bvh(f_bvh,h_bvh.joints[parent_index].name);

        for (int i = 0; i < f_bvh.frames.size(); ++i)
        {
            f_bvh.frames[i].channel_data.erase(f_bvh.frames[i].channel_data.begin()+ind_f_p);
        }


    }

    if (h_bvh.joints[joint_index].channels.size()>0)
    {
       f=find_joint_index_f_bvh(f_bvh,name);

        for (int i = 0; i < f_bvh.frames.size(); ++i)//usunięcie danych joint z frame
        {
            f_bvh.frames[i].channel_data.erase(f_bvh.frames[i].channel_data.begin()+f);
        }
    }

    h_bvh.joints.erase(h_bvh.joints.begin()+joint_index);

    bvh_pc_index(h_bvh, f_bvh);
    bvh_depth(h_bvh);


}

void Data::bvh_change_parent(hierarchy_bvh &h_bvh, frames_data &f_bvh, string joint_name, string new_parent_name)
{
    int joint_index;
    int new_parent_index;
    int old_parent_index;
    string old_parent_name;
    int old_parent_frame_index;


    new_parent_index=find_joint_index_h_bvh(h_bvh,new_parent_name);
    joint_index=find_joint_index_h_bvh(h_bvh,joint_name);
    old_parent_name=h_bvh.joints[joint_index].parent;
    old_parent_index=find_joint_index_h_bvh(h_bvh,old_parent_name);
    old_parent_frame_index=find_joint_index_f_bvh(f_bvh,old_parent_name);

    h_bvh.joints[joint_index].parent=new_parent_name;

    for (int i = 0; i < h_bvh.joints[old_parent_index].children.size(); ++i) // usunięcie przenoszonego jointa z dzieci starego parenta
    {
        if (h_bvh.joints[old_parent_index].children[i]==joint_name)
        {
            h_bvh.joints[old_parent_index].children.erase(h_bvh.joints[old_parent_index].children.begin()+i);
            break;
        }
    }
    if (h_bvh.joints[old_parent_index].children.size()==0 ) // jeżeli stary parent staje się end site
    {
        old_parent_frame_index=find_joint_index_f_bvh(f_bvh,old_parent_name);

        for (int i = 0; i < f_bvh.frames.size(); ++i)
        {
            f_bvh.frames[i].channel_data.erase(f_bvh.frames[i].channel_data.begin()+old_parent_frame_index);
        }

        h_bvh.joints[old_parent_index].channels.clear();
        h_bvh.joints[old_parent_index].channel_count=0;
    }

    if (h_bvh.joints[new_parent_index].children.size()==0) //jeżeli nowy parent był endsite to dodaj mu channele
    {
        for (int i = 0; i < h_bvh.joints[0].channels.size(); ++i)
        {
            if (h_bvh.joints[0].channels[i]=="Zrotation" || h_bvh.joints[0].channels[i]=="Xrotation" || h_bvh.joints[0].channels[i]=="Yrotation")
            {
                h_bvh.joints[new_parent_index].channels.push_back(h_bvh.joints[0].channels[i]);
            }
        }
        h_bvh.joints[new_parent_index].channel_count=h_bvh.joints[new_parent_index].channels.size();

        joint_data parent_frame_data;
        parent_frame_data.name=new_parent_name;
        parent_frame_data.id=h_bvh.joints[new_parent_index].id;
        for (int i = 0; i < h_bvh.joints[new_parent_index].channels.size(); ++i)
        {
            parent_frame_data.channels.push_back(0);
        }

        for (int i = 0; i < f_bvh.frames.size(); ++i)
        {
            f_bvh.frames[i].channel_data.push_back(parent_frame_data);
        }

    }
    h_bvh.joints[new_parent_index].children.push_back(joint_name);

    bvh_pc_index(h_bvh, f_bvh);
    bvh_depth(h_bvh);


}

void Data::asf_add_joint_above(hierarchy_asf &h_asf, frames_data &f_amc, string new_joint_name, string child_name)
{
    joint_asf new_joint;
    int child_index=find_joint_index_h_asf(h_asf,child_name);
    int parent_index=find_joint_index_h_asf(h_asf,h_asf.joints[child_index].parent);
    int id=0;

    new_joint.name=new_joint_name;
    new_joint.direction.push_back(0);
    new_joint.direction.push_back(0);
    new_joint.direction.push_back(0);
    new_joint.length=0;
    new_joint.axis.push_back(0);
    new_joint.axis.push_back(0);
    new_joint.axis.push_back(0);
    new_joint.axis_order=h_asf.joints[0].axis_order;
    new_joint.parent=h_asf.joints[child_index].parent; //parent
    new_joint.children.push_back(child_name); //child
    new_joint.vector_to_position=h_asf.joints[parent_index].vector_to_position;

    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        if (h_asf.joints[i].id>id)
        {
            id=h_asf.joints[i].id;
        }
    }
    new_joint.id=id+1;

    for (int i = 0; i < h_asf.joints[0].dof.size(); ++i) //dodanie kanałów do nowego jointa
    {
        if (h_asf.joints[0].dof[i]=="RX")
        {
            new_joint.dof.push_back("rx");
        }
            else
            {
                if (h_asf.joints[0].dof[i]=="RY")
                {
                    new_joint.dof.push_back("ry");
                }
                    else
                    {
                        if (h_asf.joints[0].dof[i]=="RZ")
                        {
                            new_joint.dof.push_back("rz");
                        }
                    }
            }
    }

    h_asf.joints[child_index].parent=new_joint_name; // ustawienie nowego parenta

    for (int i = 0; i < h_asf.joints[parent_index].children.size(); ++i)
    {
        if (h_asf.joints[parent_index].children[i]==child_name)
        {
            h_asf.joints[parent_index].children[i]=new_joint_name;
        }
    }
    // h_asf.joints[parent_index].children.push_back(new_joint_name);

     h_asf.joints.push_back(new_joint);

     joint_data new_joint_data;
     new_joint_data.name=new_joint_name;
     new_joint_data.id=new_joint.id;
     for (int i = 0; i < new_joint.dof.size(); ++i)
     {
     new_joint_data.channels.push_back(0);
     }
     if (new_joint_data.channels.size()>0)
     {
         for (int i = 0; i < f_amc.frames.size(); ++i)
         {
             f_amc.frames[i].channel_data.push_back(new_joint_data);
         }
     }


}

void Data::asf_add_joint_below(hierarchy_asf &h_asf, frames_data &f_amc, string new_joint_name, string parent_name)
{
    joint_asf new_joint;
    int parent_index=find_joint_index_h_asf(h_asf,parent_name);
    int id=0;

    new_joint.name=new_joint_name;
    new_joint.direction.push_back(0);
    new_joint.direction.push_back(0);
    new_joint.direction.push_back(0);
    new_joint.length=0;
    new_joint.axis.push_back(0);
    new_joint.axis.push_back(0);
    new_joint.axis.push_back(0);
    new_joint.axis_order=h_asf.joints[0].axis_order;
    new_joint.parent=parent_name; //parent


    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        if (h_asf.joints[i].id>id)
        {
            id=h_asf.joints[i].id;
        }
    }
    new_joint.id=id+1;



    for (int i = 0; i < h_asf.joints[0].dof.size(); ++i) //dodanie kanałów do nowego jointa
    {
        if (h_asf.joints[0].dof[i]=="RX")
        {
            new_joint.dof.push_back("rx");
        }
            else
            {
                if (h_asf.joints[0].dof[i]=="RY")
                {
                    new_joint.dof.push_back("ry");
                }
                    else
                    {
                        if (h_asf.joints[0].dof[i]=="RZ")
                        {
                            new_joint.dof.push_back("rz");
                        }
                    }
            }
    }


     h_asf.joints[parent_index].children.push_back(new_joint_name);

     h_asf.joints.push_back(new_joint);

     joint_data new_joint_data;
     new_joint_data.name=new_joint_name;
     new_joint_data.id=new_joint.id;
     for (int i = 0; i < new_joint.dof.size(); ++i)
     {
     new_joint_data.channels.push_back(0);
     }
     if (new_joint_data.channels.size()>0)
     {
         for (int i = 0; i < f_amc.frames.size(); ++i)
         {
             f_amc.frames[i].channel_data.push_back(new_joint_data);
         }
     }

}

void Data::asf_change_name(hierarchy_asf &h_asf, frames_data &f_amc, string old_name, string new_name)
{
    int hierarchy_joint_index=0;
    int frames_joint_index=0;
    int parent_index;

    hierarchy_joint_index= find_joint_index_h_asf(h_asf,old_name);

    frames_joint_index=find_joint_index_f_amc(f_amc,old_name);

    parent_index=find_joint_index_h_asf(h_asf,h_asf.joints[hierarchy_joint_index].parent);

    h_asf.joints[hierarchy_joint_index].name=new_name;//ustawineie nowej nazwy w parent

    for (int i = 0; i < h_asf.joints[parent_index].children.size(); ++i)
    {
        if (h_asf.joints[parent_index].children[i]==old_name)
        {
            h_asf.joints[parent_index].children[i]=new_name;
            break;
        }
    }

    for (int i = 0; i < h_asf.joints[hierarchy_joint_index].children.size(); ++i) //ustawienie nowej nazwy w dzieciach
    {
        h_bvh.joints[h_asf.joints[hierarchy_joint_index].child_index_joint[i]].parent=new_name;
    }

    for (int i = 0; i < f_amc.frames.size(); ++i) //ustawienie nowej nazwy w frame
    {
        f_amc.frames[i].channel_data[frames_joint_index].name=new_name;
    }
}

void Data::asf_remove_joint(hierarchy_asf &h_asf, frames_data &f_amc, string name)
{
    int joint_index=0;
    int parent_index=0;
    int f;
    string parent_name;

    joint_index=find_joint_index_h_asf(h_asf,name);
    if (joint_index==-1)
    {
        return;
    }

    parent_name=h_asf.joints[joint_index].parent;
    parent_index=find_joint_index_h_asf(h_asf,parent_name);


    for (int i = 0; i < h_asf.joints[parent_index].children.size(); ++i) // usunięcie podanego joint z dzieci parenta
    {
        if (h_asf.joints[parent_index].children[i]==name)
        {
            h_asf.joints[parent_index].children.erase(h_asf.joints[parent_index].children.begin()+i);
            break;
        }
    }

    for (int i = 0; i < h_asf.joints[joint_index].children.size(); ++i)
    {
        h_asf.joints[parent_index].children.push_back(h_asf.joints[joint_index].children[i]);//przypisanie dziecka do parenta
        int child_index=find_joint_index_h_asf(h_asf,h_asf.joints[joint_index].children[i]);
        h_asf.joints[child_index].parent=parent_name;//przypisanie parent do dzieci
    }

    if (h_asf.joints[joint_index].dof.size()>0)
    {
       f=find_joint_index_f_amc(f_amc,name); // znajdź indeks jointa w ramkach

        for (int i = 0; i < f_amc.frames.size(); ++i)//usunięcie danych joint z frame
        {
            f_amc.frames[i].channel_data.erase(f_amc.frames[i].channel_data.begin()+f);
        }
    }

    h_asf.joints.erase(h_asf.joints.begin()+joint_index); //usunięcie samego jointa
}

void Data::asf_change_parent(hierarchy_asf &h_asf, frames_data &f_amc, string joint_name, string new_parent_name)
{
    int new_parent_index=find_joint_index_h_asf(h_asf,new_parent_name);
    int joint_index=find_joint_index_h_asf(h_asf,joint_name);
    int old_parent_index=find_joint_index_h_asf(h_asf,h_asf.joints[joint_index].parent);

    if (new_parent_index==-1||joint_index==-1||old_parent_index==-1)
    {
        return;
    }

    h_asf.joints[joint_index].parent=new_parent_name;
    h_asf.joints[new_parent_index].children.push_back(joint_name);

    for (int i = 0; i < h_asf.joints[old_parent_index].children.size(); ++i)
    {
        if (h_asf.joints[old_parent_index].children[i]==joint_name)
        {
            h_asf.joints[old_parent_index].children.erase(h_asf.joints[old_parent_index].children.begin()+i);
        }
    }

}

int Data::find_joint_index_h_bvh(hierarchy_bvh &h_bvh, string name)
{
    for (int i = 0; i < h_bvh.joints.size(); ++i)
    {
        if (h_bvh.joints[i].name==name)
        {
            return i;
        }
    }
    return -1;
}

int Data::find_joint_index_h_asf(hierarchy_asf &h_asf, string name)
{
    for (int i = 0; i < h_asf.joints.size(); ++i)
    {
        if (h_asf.joints[i].name==name)
        {
            return i;
        }
    }
    return -1;
}

int Data::find_joint_index_f_bvh(frames_data &f_bvh, string name)
{
    for (int i = 0; i < f_bvh.frames[0].channel_data.size(); ++i)
    {
        if (f_bvh.frames[0].channel_data[i].name==name)
        {
            return i;
        }
    }
    return -1;
}

int Data::find_joint_index_f_amc(frames_data &f_amc, string name)
{
    for (int i = 0; i < f_amc.frames[0].channel_data.size(); ++i)
    {
        if (f_amc.frames[0].channel_data[i].name==name)
        {
            return i;
        }
    }
    return -1;
}

void Data::change_offset_bvh(hierarchy_bvh &h_bvh,std::string name, float x, float y, float z)
{
    int index=find_joint_index_h_bvh(h_bvh,name);
    h_bvh.joints[index].offset[0]=x;
    h_bvh.joints[index].offset[1]=y;
    h_bvh.joints[index].offset[2]=z;

}

void Data::change_offset_asf(hierarchy_asf &h_asf, std::string name, float x, float y, float z)
{
    float dx,dy,dz,l;
    if (name==h_asf.joints[0].name)
    {
        h_asf.joints[0].direction[0]=x;
        h_asf.joints[0].direction[1]=y;
        h_asf.joints[0].direction[2]=z;
    }
    else
    {
        int index=find_joint_index_h_asf(h_asf,name);
        l=sqrt(((x*x)+(y*y)+(z*z)));
        dx=x/l;
        dy=y/l;
        dz=z/l;
        h_asf.joints[index].length=l;
        h_asf.joints[index].direction[0]=dx;

        h_asf.joints[index].direction[1]=dy;
        h_asf.joints[index].direction[2]=dz;
    }
}

void Data::amc_rad_to_deg(hierarchy_asf &h_asf, frames_data &f_amc)
{
    int RX=0, RY=0, RZ=0;
    bool x=false, y=false, z=false;
    for (int d = 0; d < h_asf.joints[0].dof.size(); ++d)
    {
        if (h_asf.joints[0].dof[d]=="RX")
        {
            RX=d;
            x=true;
        }
        if (h_asf.joints[0].dof[d]=="RY")
        {
            RY=d;
            y=true;
        }
        if (h_asf.joints[0].dof[d]=="RZ")
        {
            RZ=d;
            z=true;
        }
    }
    int f_index=0;
    for (int f=0; f<f_amc.frames.size(); ++f)
    {
        if (x)
        {
            f_amc.frames[f].channel_data[f_index].channels[RX]=57.2957795*f_amc.frames[f].channel_data[f_index].channels[RX];
        }
        if (y)
        {
            f_amc.frames[f].channel_data[f_index].channels[RY]=57.2957795*f_amc.frames[f].channel_data[f_index].channels[RY];
        }
        if (z)
        {
            f_amc.frames[f].channel_data[f_index].channels[RZ]=57.2957795*f_amc.frames[f].channel_data[f_index].channels[RZ];
        }


    }

    for (int i = 1; i < h_asf.joints.size(); ++i)
    {
        int rx=0, ry=0, rz=0;
        bool x=false, y=false, z=false;
        for (int d = 0; d < h_asf.joints[i].dof.size(); ++d)
        {
            if (h_asf.joints[i].dof[d]=="rx")
            {
                rx=d;
                x=true;
            }
            if (h_asf.joints[i].dof[d]=="ry")
            {
                ry=d;
                y=true;
            }
            if (h_asf.joints[i].dof[d]=="rz")
            {
                rz=d;
                z=true;
            }
        }
        int f_index=find_joint_index_f_amc(f_amc,h_asf.joints[i].name);
        for (int f=0; f<f_amc.frames.size(); ++f)
        {
            if (x)
            {
                f_amc.frames[f].channel_data[f_index].channels[rx]=57.2957795*f_amc.frames[f].channel_data[f_index].channels[rx];
            }
            if (y)
            {
                f_amc.frames[f].channel_data[f_index].channels[ry]=57.2957795*f_amc.frames[f].channel_data[f_index].channels[ry];
            }
            if (z)
            {
                f_amc.frames[f].channel_data[f_index].channels[rz]=57.2957795*f_amc.frames[f].channel_data[f_index].channels[rz];
            }


        }
    }
}
