#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
using namespace std;

// <file Path> .... maybe could be more..,
const char* img_file1 = "/home/4_1sub/0.jpg"; // after the code, this name should turn into "original"
const char* img_file1_new = "/home/4_1sub/original.jpg";
const char* img_file2 = "/home/4_1sub/bright_50.jpg";
const char* img_file3 = "/home/4_1sub/dark_50.jpg";

const char* txt_file1 = "/home/4_1sub/0.txt";
const char* txt_file1_new = "/home/4_1sub/original.txt";
const char* txt_file2 = "/home/4_1sub/bright_50.txt";
const char* txt_file3 = "/home/4_1sub/dark_50.txt";

Mat img_imp;
Mat img_sub , img_plus;
vector<Mat> channels(3);

int row, col;
int B(0),G(0),R(0);

void on_change(int val,bool sign,int i ,void* userdata){

    int SIGN_;
    if (sign == true) SIGN_ = 1;
    else {SIGN_ = -1;}

    Mat src = *(Mat*)userdata;
    channels[i] = src + (SIGN_ * val); 
    
    cout << "mat ["<< i<< "] changed" <<endl;

}

void copy_txt(const char* origin,const char* cloned ){
    ifstream in(origin);
    ofstream out(cloned);
    out << in.rdbuf();
    out.close();
    in.close();
}

int main(int argc, char* argv[]){

    img_imp = imread(img_file1,1);
    cout << "IMG reaed" <<endl;

    if(img_imp.empty()) {
    cout << "Ew! file is empty" <<endl;    
    return 1;
    }

    row = img_imp.rows;
    col = img_imp.cols;
    cout << "IMG identified" <<endl;

    // zerolize Mat:  Mat::zeros(row, col, input.type()); 
    img_sub  = Mat::zeros(row, col, img_imp.type()); img_plus = Mat::zeros(row, col, img_imp.type()); 
    cout << "OUTPUT Mat zerolized" <<endl;
    
    // Write down add/sub value to BGR channel 
    cin >> B >> G >> R ;
    Scalar V_(B, G, R);    

    cout << "+ or - VALUE: [B = "<< V_[0] <<"] [G = "<< V_[1] <<"] [R = "<< V_[2] <<"] "<< endl; 

    img_plus = img_imp.clone(); img_sub = img_imp.clone(); 
    cout << "Both cloned "<< endl;    
    
    // <Plus>
    split (img_plus, channels); 
    cout << "splited" <<endl;
    on_change(V_[0],1,0, &channels[0]); // Blue channel calculate  == // on_change(B,true,0, &channels[0]);
    on_change(V_[1],1,1, &channels[1]); // Green channel calculate == // on_change(G,true ,1, &channels[1]);
    on_change(V_[2],1,2, &channels[2]); // Red channel calculate == // on_change(R,true ,2, &channels[2]);    

    merge(channels, img_plus);
    cout << "merged " <<endl;

    imshow("IMG_P",img_plus);

    //<Sub>
    split (img_sub, channels); 
    cout << "splited" <<endl;
    on_change(V_[0],0,0, &channels[0]);
    on_change(V_[1],0,1, &channels[1]);
    on_change(V_[2],0,2, &channels[2]);
    
    merge(channels, img_sub);
    cout << "merged " <<endl;

    imshow("IMG_S",img_sub);

    waitKey(1);

    // destroyAllWindows();

    // <file Writing>
    imwrite(img_file2,img_plus); cout << "img_plus wrote" <<endl;
    // imwrite(file1_new,img_imp);
    rename(img_file1, img_file1_new); cout << "img_imp renamed" <<endl;
    imwrite(img_file3,img_sub); cout << "img_sub wrote" <<endl;

    copy_txt(txt_file1, txt_file2); cout << "txt_plus wrote" <<endl;
    copy_txt(txt_file1, txt_file3); cout << "txt_plus wrote" <<endl;
    rename(txt_file1, txt_file1_new); cout << "txt_0 renamed" <<endl;


    return 0;
}
