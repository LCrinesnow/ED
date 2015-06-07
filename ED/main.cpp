//
//  main.cpp
//  ED
//
//  Created by Rinesnow on 15/6/6.
//  Copyright (c) 2015年 Rinesnow. All rights reserved.
//

/***********************************************************************/
/************************* DISCLAIMER **********************************/
/***********************************************************************/
/** This UCR Suite software is copyright protected � 2012 by          **/
/** Thanawin Rakthanmanon, Bilson Campana, Abdullah Mueen,            **/
/** Gustavo Batista and Eamonn Keogh.                                 **/
/**                                                                   **/
/** Unless stated otherwise, all software is provided free of charge. **/
/** As well, all software is provided on an "as is" basis without     **/
/** warranty of any kind, express or implied. Under no circumstances  **/
/** and under no legal theory, whether in tort, contract,or otherwise,**/
/** shall Thanawin Rakthanmanon, Bilson Campana, Abdullah Mueen,      **/
/** Gustavo Batista, or Eamonn Keogh be liable to you or to any other **/
/** person for any indirect, special, incidental, or consequential    **/
/** damages of any character including, without limitation, damages   **/
/** for loss of goodwill, work stoppage, computer failure or          **/
/** malfunction, or for any and all other damages or losses.          **/
/**                                                                   **/
/** If you do not agree with these terms, then you you are advised to **/
/** not use this software.                                            **/
/***********************************************************************/
/***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
#include <cmath>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include<algorithm>
#include <vector>

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
#define dist(x,y) ((x-y)*(x-y))

#define INF 1e20       //Pseudo Infitinte number for this code

using namespace std;

/// Data structure for sorting the query.
typedef struct Index
{   double value;
    int    index;
} Index;

typedef struct SAX
{
    char word[3];
    int index;
    int count;
} SAX;

const int sonnum=3,base='1';
typedef struct Trie
{
    int num;//记录多少单词途径该节点，即多少单词拥有以该节点为末尾的前缀
    bool terminal;//若terminal==true，该节点没有后续节点
    int count;//记录单词的出现次数，此节点即一个完整单词的末尾字母
    struct Trie *son[sonnum];//后续节点
    vector<int> Index;//索引数组
    
} Trie;

typedef struct DistLoc
{
    double distance;
    int loc;
    
} DL;

/*********************************
 创建一个新节点
 *********************************/
Trie *NewTrie()
{
    Trie *temp=new Trie;
    temp->num=1;
    temp->terminal=false;
    temp->count=0;
    for(int i=0;i<sonnum;++i)temp->son[i]=NULL;
    return temp;
}
/*********************************
 插入一个新词到字典树
 pnt:树根
 s  :新词
 len:新词长度
 *********************************/
void Insert(Trie *pnt,char *s,int len,int position, SAX sax[])
{
    Trie *temp=pnt;
    for(int i=0;i<len;++i)
    {
        if(temp->son[s[i]-base]==NULL){
            temp->son[s[i]-base]=NewTrie();
        }
        else {temp->son[s[i]-base]->num++;temp->terminal=false;}
        temp=temp->son[s[i]-base];
    }
    temp->terminal=true;
    temp->count++;
    temp->Index.push_back(position);
    
    for (int j=0;j<=temp->count-1;j++) {//更新countarray的值，让countarray 做到第0个112和第7个112都是4
        sax[temp->Index.at(j)].count=temp->count;
    }
    
    
}
/*********************************
 删除整棵树
 pnt:树根
 *********************************/
void Delete(Trie *pnt)
{
    if(pnt!=NULL)
    {
        for(int i=0;i<sonnum;++i)if(pnt->son[i]!=NULL)Delete(pnt->son[i]);
        delete pnt;
        pnt=NULL;
    }
}
/*********************************
 查找单词在字典树中的末尾节点
 pnt:树根
 s  :单词
 len:单词长度
 *********************************/
Trie *Find(Trie *pnt,char *s,int len)
{
    Trie *temp=pnt;
    for(int i=0;i<len;++i)
        if(temp->son[s[i]-base]!=NULL){
            temp=temp->son[s[i]-base];
            cout<<"count值"<<temp->count<<endl;
            cout<<"num值"<<temp->num<<endl;
            
        }
        else return NULL;
    //每个被查找的字符串对应的Index们
    for (int j=0;j<=temp->count-1;j++) {
        cout<<"Index:"<<temp->Index.at(j)<<endl;
    }
    return temp;
}

/********************************
 寻找count最小的p集合（SAXword）
 并以此寻找q集合（Index）
 *******************************/
int Outter(int countarray[],int actualength)
{
    
    return 0;
}
/********************************
 qsort的compare函数
 *******************************/

int compareSAX(const void *a, const void* b)
{   SAX* x = (SAX*)a;
    SAX* y = (SAX*)b;
    return x->count - y->count;   // high to low
}
/*------------------------------
 LC SAX Discord Structs&Methods
 -------------------------------*/


int Qscomp(const void *a, const void* b)
{
    return ( *(int*)a - *(int*)b );
}


int loc2it(int orign,int m,int EPOCH)
{
    //    return (orign-i+m-1)/EPOCH+m-1;
    return (orign-m+1)/(EPOCH-m+1);
}
int loc2i(int orign,int it,int m,int EPOCH)

{
    return orign-1-(it*(EPOCH-m+1))+m;//减了个m
}



/// Comparison function for sorting the query.
/// The query will be sorted by absolute z-normalization value, |z_norm(Q[i])| from high to low.
int comp(const void *a, const void* b)
{   Index* x = (Index*)a;
    Index* y = (Index*)b;
    return abs(y->value) - abs(x->value);
}


/// Main function for calculating ED distance between the query, Q, and current data, T.
/// Note that Q is already sorted by absolute z-normalization value, |z_norm(Q[i])|
//double distance(const double * const Q, const double * const T , const int& j , const int& m , const double& mean , const double& std , const int* const order, const double& bsf)
double distance(double * Q, double * T , const int& j , const int& m , const double& mean , const double& std , int* order, const double& bsf)
{
    int i;
    double sum = 0;
    for ( i = 0 ; i < m && sum < bsf ; i++ )
    {
        double x = (T[(order[i]+j)]-mean)/std;
        sum += (x-Q[i])*(x-Q[i]);
    }
    return sum;
}


/// If serious error happens, terminate the program.
void error(int id)
{
    if(id==1)
        printf("ERROR : Memory can't be allocated!!!\n\n");
    else if ( id == 2 )
        printf("ERROR : File not Found!!!\n\n");
    else if ( id == 3 )
        printf("ERROR : Can't create Output File!!!\n\n");
    else if ( id == 4 )
    {
        printf("ERROR: Invalid Number of Arguments!!!\n");
        printf("Command Usage:   UCR_ED.exe  data_file  query_file   m   \n");
        printf("For example  :   UCR_ED.exe  data.txt   query.txt   128  \n");
    }
    exit(1);
}



int main()
{
    int array_size = 1024; // 需要随着不同的datapoint长度随时调整，比如ECG数据Datapoint时3571被11整除以后《1024，可以用define the size of character array
    char * array = new char[array_size]; // allocating an array of 1kb
    int position = 0; //this will be used incremently to fill characters in the array
    
    ifstream fin("/Users/rinesnow/Github/ED/ED/FirstSymbol.txt");
    //opening an input stream for file test.txt
    /*checking whether file could be opened or not. If file does not exist or don't have read permissions, file
     stream could not be opened.*/
    if(fin.is_open())
    {
        //file opened successfully so we are here
        cout << "File Opened successfully!!!. Reading data from file into array" << endl;
        //this loop run until end of file (eof) does not occur
        while(!fin.eof() && position < array_size)
        {
            
            fin.get(array[position]); //reading one character from file to array
            position++;
            
        }
        array[position-1] = '\0'; //placing character array terminating character
        
        cout << "Displaying Array..." << endl << endl;
        //this loop display all the charaters in array till \0
        for(int ii = 0; array[ii] != '\0'; ii++)
        {
            cout << array[ii];
        }
    }
    else //file could not be opened
    {
        cout << "File could not be opened." << endl;
    }
    cout<<"abc"<<endl;
    //以每三个char为一行
    //   string *wordarray =new string[array_size/3];
    int jj=0;
    int iw=0;//write i
    SAX *sax= new SAX[array_size/3];
    
    while (array[iw]!='\0') {
        sax[jj].word[0]=array[iw];
        sax[jj].word[1]=array[iw+1];
        sax[jj].word[2]=array[iw+2];
        sax[jj].index=jj+1;
        cout<<"排序qian"<<sax[jj].index<<" "<<sax[jj].word[0]<<sax[jj].word[1]<<sax[jj].word[2]<<endl;
        jj++;
        iw+=3;
    }
    cout<<jj<<endl;
    int actualen=jj-1;//wordarray中实际数组的大小
    cout<<actualen<<endl;
    sax[actualen].word[0]='E';
    
    /*********************************
     二维数组编写结束
     下面将二维数组录入Trie中
     *********************************/
    //    Trie *temp;
    Trie *saxtemp;
    //    temp=NewTrie();
    saxtemp=NewTrie();
    int kk=0;
    while (sax[kk].word[0]!='E') {
        Insert(saxtemp, sax[kk].word,3,kk,sax);
        kk++;
        cout<<"kk:"<<kk<<endl;
    }
    /********************************
     sax根据count 由小到大排序
     确定了outter的顺序count的p集合（sax）
     *******************************/
    qsort(sax,actualen,sizeof(SAX),compareSAX);
    int n;
    for (n=0;n<actualen;n++)
        cout<<n<<"排序后"<<sax[n].index<<" "<<sax[n].word[0]<<sax[n].word[1]<<sax[n].word[2]<<" "<<sax[n].count<<endl;
    /********************************
     根据p找q
     开始Discord Detection的代码
     *******************************/

    int compressionrate=16;


        
        
        
    
    FILE *fp;              // the input file pointer
    FILE *qp;              // the query file pointer
    
    double *Q;             // query array
    double *T;             // array of current data
    int *order;            // ordering of query by |z(q_i)|
    double bsf;            // best-so-far
    int m;                 // length of query
    long long loc = 0;     // answer: location of the best-so-far match
    
    double d;
    long long i , j ;
    double ex , ex2 , mean, std;
    
    double t1,t2;

    t1 = clock();
    
    
    string dataPath, queryPath;

    bsf = INF;
    i = 0;
    j = 0;
    ex = ex2 = 0;
    
    m = 240;
    
    /// Array for keeping the query data
    Q = (double *)malloc(sizeof(double)*m);
    if( Q == NULL )
        error(1);
    int frontbound;//各个it的前界
    int backbound;//各个it的后界
    bool ifoverwhelm=false;
    int exceptionbackbound;//暂存的特殊it的后界
    
    long int orign;  //数据要时长的话需要扩大成longlongint
    
    DL disloc[actualen];
    
    
    for (int outter=0; outter<actualen; outter++) {
        
        int countifall=0;
        
        /// Read the query data from input file and calculate its statistic such as mean, std
        fp = fopen("/Users/rinesnow/Github/ED/ED/First.txt","r");//若想变成输入式参考上边代码
        if( fp == NULL )
            error(2);
        orign=sax[outter].index*compressionrate*3;
        i=0;
        int w=0;
        while(fscanf(fp,"%lf",&d) != EOF)
        {
            if ((i>orign-2)&&(i<orign+m)){
                Q[w] = d;
//                              cout<<d<<endl;
                w++;
            }
            i++;
            
        
        }
        cout<<"原始序列"<<orign<<" "<<Q[0]<<Q[1]<<Q[2]<<Q[77]<<Q[78]<<Q[79]<<endl;
        fclose(fp);
        
        i=0;
        while(i<m)
        {
            d = Q[i];
            cout<<i<<" ";
            cout<<Q[i]<<endl;
            
            ex += d;
            ex2 += d*d;
            i++;
        }
        mean = ex/m;
        std = ex2/m;
        std = sqrt(std-mean*mean);

        /// Do z_normalixation on query data
        for( i = 0 ; i < m ; i++ )
            Q[i] = (Q[i] - mean)/std;

        /// Sort the query data
        order = (int *)malloc(sizeof(int)*m);
        if( order == NULL )
            error(1);
        Index *Q_tmp = (Index *)malloc(sizeof(Index)*m);
        if( Q_tmp == NULL )
            error(1);
        for( i = 0 ; i < m ; i++ )
        {
            Q_tmp[i].value = Q[i];
            Q_tmp[i].index = i;
        }
        qsort(Q_tmp, m, sizeof(Index),comp);
        for( i=0; i<m; i++)
        {   Q[i] = Q_tmp[i].value;
            order[i] = Q_tmp[i].index;
        }
        free(Q_tmp);



        /// Array for keeping the current data; Twice the size for removing modulo (circulation) in distance calculation
        T = (double *)malloc(sizeof(double)*2*m);
        if( T == NULL )
            error(1);

        double dist = 0;
        i = 0;
        j = 0;
        ex = ex2 = 0;
        qp = fopen("/Users/rinesnow/Github/ED/ED/First.txt","r");//若想变成输入式参考上边代码
        if( qp == NULL )
            error(2);
        /// Read data file, one value at a time
        while(fscanf(qp,"%lf",&d) != EOF )
        {
            ex += d;
            ex2 += d*d;
            T[i%m] = d;
            T[(i%m)+m] = d;
            
            /// If there is enough data in T, the ED distance can be calculated
            if( i >= m-1 )
            {
                /// the current starting location of T
                j = (i+1)%m;
                
                /// Z_norm(T[i]) will be calculated on the fly
                mean = ex/m;
                std = ex2/m;
                std = sqrt(std-mean*mean);
                
                /// Calculate ED distance
                dist = distance(Q,T,j,m,mean,std,order,bsf);
                if( dist < bsf )
                {
                    bsf = dist;
                    loc = i-m+1;
                }
                ex -= T[j];
                ex2 -= T[j]*T[j];
            }
            i++;
        }
        fclose(qp);
    }
    
        t2 = clock();

        cout << "Location : " << loc << endl;
        cout << "Distance : " << sqrt(bsf) << endl;
        cout << "Data Scanned : " << i << endl;
        cout << "Total Execution Time : " << (t2-t1)/CLOCKS_PER_SEC << " sec" << endl;
    
}
