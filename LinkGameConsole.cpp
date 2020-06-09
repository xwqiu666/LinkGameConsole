// LinkGameConsole.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <atlstr.h>

using namespace std;

//水平个数,列数
const int horizontalCount = 19;
//垂直个数，行数
const int verticalCount = 11;

byte chess[11][19] = { 0 };
/*********************************************************************************
1.取出原点棋子四周的棋子坐标类
POINT pold; //当前的棋子坐标
POINT up;   //当前的棋子坐标上面的棋子坐标
POINT down; //当前的棋子坐标下面面的棋子坐标
POINT left; //当前的棋子坐标左边的棋子坐标
POINT right;//当前的棋子坐标右边的棋子坐标
*********************************************************************************/
class CChessPoint
{
public:
    POINT pold; //当前的棋子坐标
    POINT up;   //当前的棋子坐标上面的棋子坐标
    POINT down; //当前的棋子坐标下面面的棋子坐标
    POINT left; //当前的棋子坐标左边的棋子坐标
    POINT right;//当前的棋子坐标右边的棋子坐标

    CChessPoint(POINT pxy);

    virtual ~CChessPoint();

};
//head file

     /************************************************************************
2     //是否在棋盘上 的2个点之前是否有一条全为0的直线，如有true，否则false
3     这是两种特殊的情况 同一行 或 同一列
4     ************************************************************************/

bool CheckLine(POINT p1, POINT p2) {
    //定义循环变量
    int x, y, t;
    //1.当两个相同的点在一起的时候就返回true
    if ((p1.x == p2.x) && (p1.y == p2.y) && (chess[p1.y][p1.x] == 0) && (chess[p2.y][p2.x] == 0)) { return  true; }
    else
        //判断是否是棋盘棋子的有效数据
        if ((p1.x < 0) || (p1.x > 18) || (p1.y < 0) || (p1.y > 10) ||
            (p2.x < 0) || (p2.x > 18) || (p2.y < 0) || (p2.y > 10)) {
            return  false;
        }

    if (p1.y == p2.y) //当y相等的时候就遍历以x做为循环变量  同行
    {
        if (p1.x > p2.x)
        {
            t = p1.x;
            p1.x = p2.x;
            p2.x = t;
        }
        for (x = p1.x; x <= p2.x; x++)
        {
            if ((chess[p1.y][x]) != 0) { return false; }
        }

    }

    if (p1.x == p2.x)//当x相同的时候就遍历以y做为循环变量   同列
    {
        if (p1.y > p2.y)
        {
            t = p1.y;
            p1.y = p2.y;
            p2.y = t;
        }
        for (y = p1.y; y <= p2.y; y++)
        {
            if ((chess[y][p1.x]) != 0) { return false; }
        }

    }

    return true;

}


/************************************************************************************************
    检查两点是否可以消除     桩模块
    a、在这一对棋子间找相通路径的原理
    b、(Check2p函数)框架代码
    c、(CheckLine函数)检测2点是否有连通

    LineNull(p1,p2); //是否在棋盘上 的2个点之前是否有一条全为0的直线，如有true，否则false
    1、剪贴游戏图；
    Y坐标相同的情况下 p1,p2
    lineNull(p1.right,p2.left) //可消除
    X坐标相同的情况下 p1,p2
    LineNull(p1.down,p2.up)    //可消除
    ************************************************************************************************/
bool Check2p(POINT a, POINT b)
{
    CChessPoint p1(a), p2(b);
    POINT  pa, pb;//转角点
    int x, y;

    // 如果2点为同一点 则返回假
    if ((a.x == b.x) && (a.y == b.y)) { return false; }
    else
        if ((chess[a.y][a.x] == 0) || (chess[b.y][b.x] == 0))
        {
            return false;
        }
        else
            if (chess[a.y][a.x] != chess[b.y][b.x])
            {
                return false;
            }

    pa = a;pb = b;
    // 在横向一条线上 y坐标 相同
    if (a.y == b.y)
    {     // 2点在左右相邻  //问题所在
        if ((p1.right.x == p2.pold.x) || (p1.left.x == p2.pold.x)) { return true; }
        //检测 这条线是否有一条路径相通

        if (CheckLine(p1.right, p2.left)) { return true; }
        //检测 上下
        //y 上
        pa = a;pb = b;
        if ((p1.up.y >= 0) && (p1.up.y <= 10))
            for (y = 0;y <= p1.up.y;y++)
            {
                pa.y = y;pb.y = y;
                if (CheckLine(pa, p1.up) && CheckLine(pb, p2.up) && CheckLine(pa, pb)) { return true; }
            }
        // y下
        pa = a;pb = b;
        if ((p1.down.y >= 0) && (p1.down.y <= 10))
            for (y = p1.down.y;y <= 10;y++)
            {
                pa.y = y;pb.y = y;
                if (CheckLine(pa, p1.down) && CheckLine(pb, p2.down) && CheckLine(pa, pb)) { return true; }
            }

        //检测 左右    因为 y轴相等，所以不存在左右路径

    }
    else
        //纵向一条线  x 坐标 相同
        if (a.x == b.x)
        {
            //x下上 相邻不 //???????????a
        //if ((p1.down.y==p2.up.y ) || (p1.up.y==p2.up.y))
            if ((p1.down.y == p2.pold.y) || (p1.up.y == p2.pold.y))  //我在这里错过
            {
                return true;
            }
            //检测 这条线是否有一条路径相通
            if (CheckLine(p1.down, p2.up)) { return true; }
            //检测 上下   国为x 轴相等 所以不存在路径

            //检测 左右
             //x左
            pa = a;pb = b;
            for (x = 0;x <= p1.left.x;x++)
            {
                pa.x = x;
                pb.x = x;
                if (CheckLine(pa, p1.left) && CheckLine(pb, p2.left) && CheckLine(pa, pb)) { return true; }
            }
            //x右

            pa = a;pb = b;
            for (x = p1.right.x;x <= 18;x++)
            {
                pa.x = x;
                pb.x = x;
                if (CheckLine(pa, p1.right) && CheckLine(pb, p2.right) && CheckLine(pa, pb)) { return true; }
            }
        }
        else

            //xy 坐标 都不相同 {{{{{{
        {
            pa = a;pb = b;

            if (a.x > b.x) {   // p2点 在 左 left
            // 找x轴路径
                for (x = 0;x <= p2.left.x;x++)
                {
                    pa.x = x;pb.x = x;
                    if (CheckLine(pa, p1.left) && CheckLine(pa, pb) && CheckLine(pb, p2.left))
                    {
                        return true;
                    }


                } // end for

                for (x = p2.right.x;x <= p1.left.x;x++)
                {
                    pa.x = x;pb.x = x;
                    if (CheckLine(p2.right, pb) && CheckLine(pa, pb) && CheckLine(pa, p1.left)) { return true; }

                }
                for (x = p2.right.x;x <= 18;x++)
                {
                    pa.x = x;pb.x = x;
                    if (CheckLine(p1.right, pa) && CheckLine(p2.right, pb) && CheckLine(pa, pb)) { return true; }




                }
                /////////////////yyyyyyyyyyyyyyyyyyyy 找y轴路径 由于是从上向下 搜索 所以p1.y>p2.y
                pa.x = a.x;   pb.x = b.x; //初始化坐标 y渐变
                for (y = 0;y <= p1.up.y;y++)    //1段
                {
                    pa.y = y;pb.y = y;
                    if (CheckLine(pb, pa) && CheckLine(pa, p1.up) && CheckLine(pb, p2.up)) { return true; }


                }
                ////////////////////////
                for (y = p1.down.y;y <= p2.up.y;y++)//2段
                {
                    pa.y = y;pb.y = y;
                    if (CheckLine(pb, pa) && CheckLine(p1.down, pa) && CheckLine(pb, p2.up))
                    {
                        return true;
                    }


                }
                ///////////////////////
                for (y = p2.down.y;y <= 10;y++) //3段
                {
                    ///////////////////////////////
                    pa.y = y;pb.y = y;
                    if (CheckLine(pb, pa) && CheckLine(p1.down, pa) && CheckLine(p2.down, pb)) { return true; }
                }

            }
            else
                ////////////p2点  在 右 right a.x>b.x
            {
                pa.y = a.y;   pb.y = b.y; //初始化坐标
                for (x = 0;x <= p1.left.x;x++);
                {
                    pa.x = x;pb.x = x;
                    if (CheckLine(pa, pb) && CheckLine(pa, p1.left) && CheckLine(pb, p2.left))
                    {
                        return true;
                    }


                }
                /////////////////////

                for (x = p1.right.x;x <= p2.left.x;x++)
                {
                    pa.x = x;pb.x = x;
                    if (CheckLine(pa, pb) && CheckLine(p1.right, pa) && CheckLine(pb, p2.left))
                    {
                        return true;

                    }
                }
                ///////////////////////

                for (x = p2.right.x;x <= 18;x++)
                {
                    pa.x = 0;pb.x = x;
                    if (CheckLine(pa, pb) && CheckLine(p1.right, pa) && CheckLine(p2.right, pb)) { return true; }



                }
                ///////////////////////yyyyyyyyyyyyyyyyyy   y轴渐变
                pa.x = a.x;   pb.x = b.x; //初始化坐标
                if ((p1.up.y >= 0) && (p1.up.y <= 10))
                {
                    for (y = 0;y <= p1.up.y;y++)    //1段
                    {
                        pa.y = y;pb.y = y;
                        if (CheckLine(pa, pb) && CheckLine(pa, p1.up) && CheckLine(pb, p2.up)) { return true; }


                    }
                }
                //////
                pa.x = a.x;   pb.x = b.x; //初始化坐标
                if ((p1.down.y <= 10) && (p2.up.y >= 0))
                {
                    for (y = p1.down.y;y <= p2.up.y;y++)  //2段
                    {
                        pa.y = y;pb.y = y;
                        if (CheckLine(pa, pb) && CheckLine(p1.down, pa) && CheckLine(pb, p2.up)) {
                            return true;
                        }
                    }
                }
                ////
                pa.x = a.x;   pb.x = b.x; //初始化坐标
                if (p2.down.y <= 10)
                {
                    for (y = p2.down.y;y <= 10;y++)           //3段
                    {
                        pa.y = y;pb.y = y;
                        if (CheckLine(pa, pb) && CheckLine(p1.down, pa) && CheckLine(p2.down, pb)) {
                            return true;
                        }
                    }
                }
            }
        }    //xy 坐标 都不相同 }}}}}}}}}
    return false;
}//Cick2p() end

void ReadBoard()
{
    HWND gameHandle = FindWindow(NULL, CString("QQ游戏 - 连连看角色版"));
    //更新
}

HWND gameHandle;

bool updateChess() {
    gameHandle = FindWindow(NULL, CString("QQ游戏 - 连连看角色版"));
    //00199F68
    if (NULL != gameHandle)//先判断是否进行了初始化
    {
        //获取进程id
        DWORD pid;
        GetWindowThreadProcessId(gameHandle, &pid);
        //基址
        DWORD BaseAddress = 0X00199F5C;
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);//打开进程 
        //DWORD data[horizontalCount][verticalCount] = { 0 };
        DWORD dwSize = 0;

        cout << "---------------  这个是读取棋盘时的数据  ---------------------" << endl;
        for (int i = 0; i < 11; i++)
        {
            for (int j = 0; j < 19; j++)
            {

                ReadProcessMemory(hProcess, (LPVOID)BaseAddress, &chess[i][j], 1, &dwSize);
                //if (chess[i][j] >= 0 && chess[i][j] <= 15)
                //{
                //}
                printf("%x ", chess[i][j]);
                //printf("%x ", BaseAddress);
                BaseAddress++;
            }
            printf("\n");
        }

    }
    else
    {
        cout << "未找到游戏" << endl;
        return FALSE;
    }

    return TRUE;
}

bool Click2P(POINT p1, POINT p2) {
         
         //点击p1
         //HWND hwnd = FindWindow(NULL, gamecaption);
         HWND hwnd = FindWindow(NULL, CString("QQ游戏 - 连连看角色版"));
         int lparam;
         lparam = ((p1.y * 35 + 192) << 16) + (p1.x * 31 + 21);
         SendMessage(hwnd, WM_LBUTTONDOWN, 0, lparam);
         SendMessage(hwnd, WM_LBUTTONUP, 0, lparam);
         //点击p2
         cout << "clickTwo" << endl;
          lparam = ((p2.y * 35 + 192) << 16) + (p2.x * 31 + 21);
         SendMessage(hwnd, WM_LBUTTONDOWN, 0, lparam);
         SendMessage(hwnd, WM_LBUTTONUP, 0, lparam);
         return true;
    
}


/************************************************************************************************
模拟点击相同的两点     桩模块
就是一个单一的动作就是模拟点击两点

    *********************************************************
***************************************/
bool Click2pP(POINT p1, POINT p2) {
    cout << "p1 : " << p1.x << ","<< p1.y << "; p2 : " << p2.x << "," << p2.y << endl;
    //点击p1
    HWND hwnd = FindWindow(CString("#32770 (对话框)"), CString("QQ游戏 - 连连看角色版"));
    //HWND hwnd = FindWindow(NULL, CString("QQ游戏 - 连连看角色版"));
    int lparam;
    lparam = ((p1.y * 35 + 192) << 16) + (p1.x * 31 + 21);
    SendMessage(hwnd, WM_LBUTTONDOWN, 0, lparam);
    SendMessage(hwnd, WM_LBUTTONUP, 0, lparam);
    //点击p2
    lparam = ((p2.y * 35 + 192) << 16) + (p2.x * 31 + 21);
    SendMessage(hwnd, WM_LBUTTONDOWN, 0, lparam);
    SendMessage(hwnd, WM_LBUTTONUP, 0, lparam);
    return true;
}

bool Click2PAction(POINT p1, POINT p2) {
    //点击p1
    //HWND hwnd = FindWindow(CString("#32770 (对话框)"), CString("QQ游戏 - 连连看角色版"));
    //HWND hwnd = FindWindow(NULL, CString("QQ游戏 - 连连看角色版"));
    int lparam;
    lparam = ((p1.y * 35 + 192) << 16) + (p1.x * 31 + 21);
    SendMessage(gameHandle, WM_LBUTTONDOWN, 0, lparam);
    SendMessage(gameHandle, WM_LBUTTONUP, 0, lparam);
    //点击p2
    lparam = ((p2.y * 35 + 192) << 16) + (p2.x * 31 + 21);
    SendMessage(gameHandle, WM_LBUTTONDOWN, 0, lparam);
    SendMessage(gameHandle, WM_LBUTTONUP, 0, lparam);
    return true;
}

void showChess() {
    //cout << "----------------这个是存储后的数据--------------------" << endl;
   // int calCount = 0;
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 19; j++)
        {
            if (chess[i][j] >= 0 && chess[i][j] <= 15)
            {
                //cout << "0";
            }
            //cout << data[i][j] << " ";
            printf("%x ", chess[i][j]);
            /*calCount++;
            if (calCount % 8 == 0) {
                printf("\n");
            }*/
        }
        printf("\n");
    }

    // cout << "------------------------------------" << endl;
}

bool ClearChess() {
    //读取更新棋盘数据byte chess[11][19] 模式 a[y][x]
    if (!updateChess()) {
        return TRUE;
    };
    //showChess();
    //进行遍历找到相同的两个点
    POINT m_ponit1, m_ponit2;
    //临时变量
    int x1, y1, x2, y2;
    for (y1 = 0; y1 < 11; y1++)
        for (x1 = 0; x1 < 19; x1++)
        {
            for (y2 = y1; y2 < 11; y2++)
                for (x2 = 0; x2 < 19; x2++)
                {
                    //判断两个点是否相同
                    if ((chess[y1][x1] == chess[y2][x2]) && (!((x1 == x2) && (y1 == y2))))
                    //if ((chess[y1][x1] == chess[y2][x2]) && ((x1 != x2) || y1 != y2))
                    {
                        m_ponit1.x = x1;
                        m_ponit1.y = y1;
                        m_ponit2.x = x2;
                        m_ponit2.y = y2;
                        //检查两点是否可以消除
                        if (Check2p(m_ponit1, m_ponit2))
                        {
                            //cout << y1 << x1 << " --- " << y2 << x2 << endl;
                            //模拟点击相同的两点还没有实现的
                            Click2P(m_ponit1, m_ponit2);
                            return true;
                        }
                    }
                }


        }

    return    false;

}



CChessPoint::CChessPoint(POINT pxy)
{
    //初始化
    up = pxy;
    down = pxy;
    left = pxy;
    right = pxy;
    pold = pxy;
    //四周棋子的某一个方向的坐标改变了
    up.y = pxy.y - 1;
    down.y = pxy.y + 1;
    left.x = pxy.x - 1;
    right.x = pxy.x + 1;
}

CChessPoint::~CChessPoint()
{
}

int main()
{
    while (TRUE) {
        DWORD speed;
        cout << "输入速度继续......" << endl;
        cin >> speed;
        while (TRUE) {
            bool isNotClear = ClearChess();
            if (isNotClear == FALSE) {
                break;
            }
            Sleep(speed);
        }
    }
    //ClearChess();

    cout << "finish!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
