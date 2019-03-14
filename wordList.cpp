#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stack>
#include <algorithm>
#include<string.h> 
using namespace std;

#define wordNumMax 3001
#define FINITY 9999

int wordNum=0;    //记录单词数量
int maxLen = 0;    //最长路径长度
string wordArr[wordNumMax];    //存储所有的单词的数组
int arc[wordNumMax][wordNumMax];    //存储邻接矩阵中边的权值
int arcTemp[wordNumMax][wordNumMax];    //供操作的邻接矩阵
int indegree[wordNumMax];    //入度矩阵
int orderArr[wordNumMax];    //经过拓扑排序后的顶点序列
int maxPath[wordNumMax];    //存储到顶点i的最长路径
int point[wordNumMax];    //存储最长路线的选择点
int path[wordNumMax][wordNumMax];    //存储最长路径的矩阵

void init()
{
    for (int i = 0; i < wordNumMax;i++)
    {
        indegree[i] = 0;
        maxPath[i] = 0;
        point[i] = 0;
        for (int j = 0; j < wordNumMax;j++){
            path[i][j] = 0;
        }
    }
}

void getWord(string fileName)    //从文本中得到所有的单词 
{
    fstream wordTxt;
    char singleChar;
    bool flag = false;    //用于标识读入的字符是否为字母
    bool ifNewWord = false;
    wordTxt.open(fileName, ios::in);
    wordTxt >> noskipws;
    while(!wordTxt.eof())
    {
        wordTxt >> singleChar;
        //cout << singleChar << endl;
        if(wordTxt.fail())
        {
            break;
        }
        if(wordNum>=wordNumMax)
        {
            cout << "too many words!!!" << endl;
            break;
        }
        if(singleChar>='A'&&singleChar<='Z')
        {
            singleChar = singleChar - 'A' + 'a';
        }
        if(singleChar>='a'&&singleChar<='z')
        {
            if(!flag)
            {
                flag = true;
            }
            wordArr[wordNum] += singleChar;
        }
        else
        {
            if(flag)
            {
                flag = false;
                ifNewWord = true;
                for (int i = 0; i < wordNum;i++)
                {
                    if(wordArr[wordNum]==wordArr[i])
                    {
                        ifNewWord = false;
                        wordArr[wordNum].clear();
                        break;
                    }
                }
                if(ifNewWord)
                {
                    wordNum++;
                }
            }
        }
    }
}

void createMatrix()    //创建邻接矩阵
{
    for (int i = 0; i < wordNum;i++)
    {
        for (int j = 0; j < wordNum;j++)
        {
            if(j==i)
            {
                arc[i][j] = FINITY;
                arcTemp[i][j] = FINITY;
                continue;
            }
            if(wordArr[i][wordArr[i].length()-1]==wordArr[j][0])
            {
                arc[i][j] = wordArr[j].length();
                arcTemp[i][j] = wordArr[j].length();
                indegree[j]++;
            }
            else
            {
                arc[i][j] = FINITY;
                arcTemp[i][j] = FINITY;
            }
        }
    }
}

bool topologicalSort()    //拓扑排序
{
    stack<int> s;    //存放顶点的栈
    int temp;    //当前出栈元素
    int count = 0;    //记录经过排序后顶点的数量，即经拓扑排序后序列的顶点数量
    for (int i = 0; i < wordNum;i++)
    {
        if(indegree[i]==0)
        {
            s.push(i);
            indegree[i] = -1;    //对已经入栈的点进行标记
        }
    }
        while (!s.empty())
        {
            temp = s.top();
            s.pop(); 
            orderArr[count] = temp;
            count++;
            for (int i = 0; i < wordNum; i++)
            {
                if(arcTemp[temp][i]!=FINITY)
                {
                    arcTemp[temp][i] = FINITY;
                    indegree[i]--;
                    if (indegree[i] == 0)
                    {
                        s.push(i);
                        indegree[temp] = -1;    //对已经出栈的点进行标记
                    }
                }
                
            }
        }
    if(count<wordNum)
    {
        return false;    //返回false代表该图有环
    }
    return true;
}

void getMaxPathLen(int arg, char first, char last) //获取最长路径  arg=0:单词数目最多  arg=1:单词长度最长; first:指定的首字母，没有输入0  last：指定的尾字母，没有输入0
{   
    int v1, v2;
    int length=1;
    bool firstFlag, lastFlag;    //true:找到了符合的开始/结尾  false:没有找到符合的开始/结尾
    bool headFlag;    //标记是否指定开始字母  true:找到  false:没找到

    if(first==0)
    {
        firstFlag = true;
        headFlag = false;
    }
    else if(first>='a'&&first<='z')
    {
        firstFlag = false;
        headFlag = true;
    }
    else if(first>='A'&&first<='Z')
    {
        firstFlag = false;
        headFlag = true;
        first = first - 'A' + 'a';
    }
    else
    {
        cout << "illegimate input as first char: " << first << endl;
        return;
    }

    if(last==0)
    {
        lastFlag = true;
    }
    else if(last>='a'&&last<='z')
    {
        lastFlag = false;
    }
    else if(last>='A'&&last<='Z')
    {
        lastFlag = false;
        last = last - 'A' + 'a';
    }
    else
    {
        cout << "illegimate input as last char: " << last << endl;
        return;
    }

    for (int i = 0; i < wordNum;i++)
    {
        v2 = orderArr[i];
        if(!firstFlag)
        {
            if(wordArr[v2][0]==first)
            {
                firstFlag = true;
                if(arg==0)
                {
                    maxPath[v2] = 1;
                }
                else
                {
                    maxPath[v2] = wordArr[v2].length();
                }
                continue;
            }
            else
            {
                continue;
            }
        }
        if(maxPath[v2]==0)
        {
            if(arg==0)
            {
                maxPath[v2] = 1;
            }
            else
            {
                maxPath[v2] = wordArr[v2].length();
            }
        }
        for (int j = 0; j < i;j++)
        {
            v1 = orderArr[j];
            if(arc[v1][v2]!=FINITY)    //表示v1可达v2，进行判断
            {
                if(arg==1)
                {
                    length = arc[v1][v2];
                }
                if(headFlag&&maxPath[v1]==0)
                {
                    continue;
                }
                path[v1][v2] = maxLen;
                if(maxPath[v1]+length>maxPath[v2])
                {
                    maxPath[v2] = maxPath[v1] + length;
                }
                if(maxPath[v2]>maxLen)
                {
                    maxLen = maxPath[v2];
                    path[v1][v2] = maxLen;
                }
                //cout << "v1: " << v1 << " v2: " << v2 << " maxLen: " << maxLen << endl;
                //cout << wordArr[v1] << "   " << wordArr[v2] << endl;
            }
        }
    }
    if(lastFlag)
    {
        cout <<"maxLen: " << maxLen << endl;
    }
}

void getMaxPath()    //获取最长路径
{
    stack<int> s;    //记录路线的栈
    int max;    //记录当前最长路径长度
    int stackSize;    //栈的长度
    //bool flag = false;    //标识是不是第一次遇到链里的单词
    int iStart=0,jStart=0;    //记录开始的单词是哪一个
    for (int i = 0; i < wordNum;i++)
    {
        for (int j = 0; j < wordNum;j++)
        {
            if(path[i][j]>path[iStart][jStart])
            {
                iStart = i;
                jStart = j;
            }
        }
    }
    for (int j = jStart; j > 0;)
    {
        for (int i = 0; i < wordNum;i++)
        {
            point[i] = path[i][j];
        }
        sort(point, point + wordNum);
        max = point[wordNum - 1]; //排序后的最大值
        if(max==0)
        {
            s.push(j);
            break;
        }
        for (int i = 0; i < wordNum;i++)
        {
            if(path[i][j]==max)
            {
                s.push(j);
                j = i;
            }
        }
    }

    /*
    for (int i = jStart; i > 0;)
        {
            for (int j = 0; j < wordNum; j++)
            {
                point[j] = path[j][i];
            }
            sort(point, point + wordNum);
            max = point[wordNum - 1]; //排序后的最大值
            if (max != 0)
            {
                flag = true;
                for (int j = 0; j < wordNum; j++)
                {
                    if (path[j][i] == max)
                    {
                        s.push(i);
                        i = j;
                        if (i == 0)
                        {
                            s.push(i);
                        }
                        break;
                    }
                }
            }
            else
            {
                if (flag)
                {
                    s.push(i);
                    flag = false;
                }
                i--;
            }
        }
    */

    stackSize = s.size();
    cout << "max path is: ";
    for (int i = 0; i < stackSize;i++)
    {
        if(i!=0)
        {
            cout << " -> ";
        }
        cout << wordArr[s.top()];
        s.pop();
    }
}

void getMaxPathWithTail(int arg, char last)    //获取带尾部的最长路径  arg=0:单词数目最多  arg=1:单词长度最长;
{
    stack<int> s;    //记录路线的栈
    int max;    //记录当前最长路径长度
    int stackSize;    //栈的长度
    //bool flag = false;    //标识是不是第一次遇到链里的单词
    int iStart=0,jStart=0;    //记录开始的单词是哪一个
    string tempStr;    //记录可能会用到的单词
    for (int i = 0; i < wordNum;i++)
    {
        for (int j = 0; j < wordNum;j++)
        {
            if(path[i][j]>path[iStart][jStart])
            {
                iStart = i;
                jStart = j;
            }
        }
    }
    for (int j = jStart; j > 0;)
    {
        for (int i = 0; i < wordNum;i++)
        {
            point[i] = path[i][j];
        }
        sort(point, point + wordNum);
        max = point[wordNum - 1]; //排序后的最大值
        if(max==0)
        {
            s.push(j);
            break;
        }
        for (int i = 0; i < wordNum;i++)
        {
            if(path[i][j]==max)
            {
                s.push(j);
                j = i;
            }
        }
    }

    if(last>='a'&&last<='z')
    {
        //last = last;
    }
    else if(last>='A'&&last<='Z')
    {
        last = last - 'A' + 'a';
    }
    else
    {
        cout << "illegimate input as last char: " << last << endl;
        return;
    }

    maxLen = 0;
    stackSize = s.size();
    cout << "max path is: ";
    for (int i = 0; i < stackSize;i++)
    {
        if(i!=0)
        {
            cout << " -> ";
        }
        cout << wordArr[s.top()];
        tempStr = wordArr[s.top()];
        s.pop();
        if(arg==0)
        {
            maxLen++;
        }
        else
        {
            maxLen += tempStr.length();
        }
        if(tempStr[tempStr.length()-1]==last)
        {
            if(s.empty())
            {
                cout << endl << "maxLen: " << maxLen << endl;
                return;
            }
            else
            {
                if(wordArr[s.top()][wordArr[s.top()].length()-1]==last)
                {
                    cout << " -> ";
                    cout << wordArr[s.top()];
                    if(arg==0)
                    {
                        maxLen++;
                    }
                    else
                    {
                        maxLen += wordArr[s.top()].length();
                    }
                }
                cout << endl << "maxLen: " << maxLen << endl;
                return;
            }
        }
    }
}

int main()
{
    init();
    ofstream result("result.txt");
    string fileName;
    cin >> fileName;
    getWord(fileName);
    createMatrix();
    if(topologicalSort())
    {
        cout << "topologicalSort successed!"<< endl;
    }
    else
    {
        cout << "topologicalSort failed!" << endl;
    }
    getMaxPathLen(0,0,'b');
    //getMaxPath();
    getMaxPathWithTail(0,'b');
    for (int i = 0; i < wordNum;i++)
    {
        result << wordArr[i] << ":  ";
        for (int j = 0; j < wordNum;j++)
        {
            result << arc[i][j] << "  ";
        }
        result << endl;
    }
    result << endl;
    result << "wordNum: " << wordNum << endl;
    for (int i = 0; i < wordNum;i++)
    {
        result << wordArr[orderArr[i]] << "  ";
    }
    result << endl;
    for (int i = 0; i < wordNum;i++)
    {
        for (int j = 0; j < wordNum;j++)
        {
            result << path[i][j] << "  ";
        }
        result << endl;
    }
    return 0;
}

