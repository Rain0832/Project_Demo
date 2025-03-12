#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>

using namespace std;

// C/C++:
// -------------------- 数据设计 --------------------
/*
    棋盘:
        map[i][j]表示坐标(i,j)的值
        0表示空地
        1表示黑子
        2表示白子
    如: map[3][6] = 1  表示(3,6)的位置是黑子
*/
int map[19][19];

// map的行数和列数
const int map_row = sizeof(map) / sizeof(map[0]);
const int map_col = sizeof(map[0]) / sizeof(map[0][0]);

// 表示当前回合数  偶数表示黑棋落子  奇数表示白棋落子
// 如: flag = 20 表示当前是第[20]次落子  由黑方落子
int flag;

// -------------------- 数据设计 --------------------

// -------------------- service --------------------
/*
    负责人: Rain
    功能: init: 初始化游戏数据
        将棋盘的值初始化为0
        当前回合设为黑棋(flag设为0)
    参数: void
    返回值: void
*/
void init();

/*
    *难点1
    负责人: Rain
    功能: isWin: 根据传入的坐标(map对应位置)和flag值 判断落点后是否获胜
    参数:
        x: 当前回合落子的x坐标
        y: 当前回合落子的y坐标
    返回值:
        0表示没有获胜
        1表示黑子胜利
        2表示白子胜利
*/
int isWin(int x, int y);

/*
    负责人: Rain
    功能: dfs: 根据输入的map和起始坐标进行深度优先搜索 以及 连子计数
    参数:
        map: 棋盘
        cur_row: 当前位置x坐标
        cur_col: 当前位置y坐标
        visit: 访问数组
        prevNode: 当前棋子颜色
        win_cnt: 连子计数
        direction: 方向
    返回值:
        void
*/
void dfs(int (&map)[map_row][map_col], int cur_row, int cur_col, vector<vector<bool>> &visit, int prevNode, int &win_cnt, int direction);

/*
    负责人: Rain
    功能: playerMove: 在指定位置落子
        如果map[x][y]是空地 则修改map[x][y]的值:改为相应颜色(flag对应颜色)        否则不操作
    参数:
        x: 当前回合落子的x坐标
        y: 当前回合落子的y坐标
    返回值:
        0表示落子失败 (棋盘已经有子)
        1表示落子成功
*/
int playerMove(int x, int y);
// -------------------- service --------------------

// -------------------- view --------------------
/*
    负责人: Rain
    功能: menuView: 展示选项, 玩家可以在这里选择进入游戏, 进入设置或退出游戏
        进入游戏: 调用游戏界面函数gameView();
        进入设置: 敬请期待...
        退出游戏: 调用exit(0);
    参数: void
    返回值: void
*/
void menuView();

/*
    负责人: Rain
    功能: gameView_ShowMap: 根据map数组 打印游戏棋盘
    参数: void
    返回值: void
*/
void gameView_ShowMap();

/*
    负责人: Rain
    功能: winView: 根据flag的值  打印游戏胜利界面  用户可以按任意键回到主菜单
    参数: void
    返回值: void
*/
void winView();

/*
    *难点2
    负责人: Rain
    功能: gameView: 游戏界面整合
        初始化游戏数据(调用函数init())
        while(1){
            打印游戏界面(调用函数gameView_ShowMap())
            接收玩家坐标输入

            落子(调用落子函数playerMove())
                (如果落子失败 重新开始循环)

            判断游戏是否胜利(调用胜利判断函数isWin())
                (如果游戏胜利 调用胜利界面函数 然后结束当前界面)
            切换玩家(修改flag值)
        }
    参数: void
    返回值: void
*/
void gameView();
// -------------------- view --------------------

int main()
{
    menuView(); // 调用菜单界面函数
    return 0;
}

// 初始化游戏数据
void init()
{
    // init map
    for (auto &row : map) // 遍历棋盘的每一行
    {
        for (auto &element : row) // 遍历当前行的每一个元素
        {
            element = 0; // 将元素初始化为0，表示空地
        }
    }

    // init flag
    flag = 0; // 初始化回合数为0，表示黑棋先落子

    return;
}

/*
    思路提示: 使用DFS算法 从落子点开始 向8个方向搜索 统计连子数量
*/
// 判断当前落子是否获胜
int isWin(int x, int y)
{
    // 返回值，默认棋子落子失败
    int r_val = 0;

    // 从落子点开始进行深度优先搜索
    for (int dir = 1; dir <= 4; dir++)
    {
        // 访问标记
        vector<vector<bool>> visit(map_row, vector<bool>(map_col, false));
        // 连子计数
        int win_cnt = 0;
        dfs(map, x, y, visit, map[x][y], win_cnt, dir);

        // 如果连子数量达到5，表示获胜
        if (win_cnt >= 5)
        {
            r_val = map[x][y]; // 返回获胜方的棋子颜色
            break;
        }
    }

    return r_val;
}

// 深度优先搜索函数
void dfs(int (&map)[map_row][map_col], int cur_row, int cur_col, vector<vector<bool>> &visit, int prevNode, int &win_cnt, int direction)
{
    // 越界、当前位置不是同色棋子、已经访问过、是空地或者连子数量达到5，返回
    if (cur_row < 0 || cur_col < 0 || cur_row == map_row || cur_col == map_col || map[cur_row][cur_col] != prevNode || visit[cur_row][cur_col] || prevNode == 0 || win_cnt == 5)
    {
        return;
    }

    win_cnt++;                      // 连子数量加1
    visit[cur_row][cur_col] = true; // 标记当前位置已访问

    // 向8个方向进行深度优先搜索
    if (direction == 1) // 上下搜索
    {
        dfs(map, cur_row + 1, cur_col, visit, map[cur_row][cur_col], win_cnt, direction); // 向下
        dfs(map, cur_row - 1, cur_col, visit, map[cur_row][cur_col], win_cnt, direction); // 向上
    }
    else if (direction == 2) // 左右搜索
    {
        dfs(map, cur_row, cur_col + 1, visit, map[cur_row][cur_col], win_cnt, direction); // 向右
        dfs(map, cur_row, cur_col - 1, visit, map[cur_row][cur_col], win_cnt, direction); // 向左
    }
    else if (direction == 3) // 左上右下搜索
    {
        dfs(map, cur_row - 1, cur_col - 1, visit, map[cur_row][cur_col], win_cnt, direction); // 左上
        dfs(map, cur_row + 1, cur_col + 1, visit, map[cur_row][cur_col], win_cnt, direction); // 右下
    }
    else if (direction == 4) // 左下右上搜索
    {
        dfs(map, cur_row - 1, cur_col + 1, visit, map[cur_row][cur_col], win_cnt, direction); // 右上
        dfs(map, cur_row + 1, cur_col - 1, visit, map[cur_row][cur_col], win_cnt, direction); // 左下
    }

    return;
}

// 玩家落子函数
int playerMove(int x, int y)
{
    // 落子结果，默认落子失败
    int r_val = 0;

    // 如果当前位置是空地
    if (map[x][y] == 0)
    {
        // 根据flag的值确定落子颜色
        map[x][y] = flag % 2 + 1;
        r_val = 1; // 落子成功
    }
    // 棋盘上已有棋子，落子失败
    else
    {
        r_val = 0;
    }
    return r_val;
}

// 菜单界面函数
void menuView()
{
    // 循环显示菜单
    while (true)
    {
        cout << "--------- 游戏主菜单 请选择 -------" << endl;
        cout << "---------      1. 进入游戏       -------" << endl;
        cout << "---------      2. 进入设置       -------" << endl;
        cout << "---------      3. 退出游戏       -------" << endl;
        cout << "                请输入你的选择: ";

        // 接收用户输入
        int select;
        cin >> select;
        switch (select)
        {
        // 进入游戏
        case 1:
            gameView();
            break;

        // 进入设置
        case 2:
            cout << "敬请期待" << endl;
            break;

        // 退出游戏
        case 3:
            exit(0);
            break;

        // 输入无效
        default:
            cout << "输入无效" << endl;
            break;
        }
    }

    return;
}

// 显示游戏棋盘
void gameView_ShowMap()
{
    // 显示当前回合数
    cout << "当前回合数: " << flag << endl;

    // 打印棋盘边界
    cout << "-----------------------------棋盘--------------------------" << endl;
    cout << "    0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18" << endl;
    int i = 0;
    for (auto &row : map) // 遍历棋盘的每一行
    {
        cout << setiosflags(ios::left) << setw(2) << i << "| "; // 打印行号
        for (auto element : row)                                // 遍历当前行的每一个元素
        {
            if (element == 0)
            {
                cout << " " << setiosflags(ios::left) << setw(2) << "|";
            }
            else if (element == 1)
            {
                cout << "X" << setiosflags(ios::left) << setw(2) << "|";
            }
            else if (element == 2)
            {
                cout << "O" << setiosflags(ios::left) << setw(2) << "|";
            }

            // {
            //     cout << element << setiosflags(ios::left) << setw(2) << "|"; // 打印元素
            // }
        }
        cout << endl;
        i++;
    }

    return;
}

// 显示胜利界面
void winView()
{
    // 根据flag的值判断获胜方
    if ((flag) % 2 == 0)
    {
        gameView_ShowMap();
        cout << "黑棋玩家获胜!" << endl;
    }
    else if ((flag) % 2 == 1)
    {
        gameView_ShowMap();
        cout << "白棋玩家获胜!" << endl;
    }
    return;
}

// 游戏界面整合函数
void gameView()
{
    // 初始化游戏数据
    init();

    // 游戏循环
    while (true)
    {
        int x, y;
        gameView_ShowMap(); // 显示游戏棋盘

        // 提示用户输入落子坐标
        cout << "请输入落子坐标(格式: x y, 按回车确认): " << endl;
        cin >> x >> y;

        // 检查坐标合法性
        if (x < 0 || x >= map_row || y < 0 || y >= map_col)
        {
            cout << "输入的坐标不合法，请重新输入" << endl;
            continue;
        }

        // 落子
        if (playerMove(x, y) == 0)
        {
            cout << "落子失败，请重新落子" << endl;
            // 落子失败，重新开始循环
            continue;
        }
        // 落子成功
        cout << "落子成功" << endl;

        // 判断游戏是否胜利
        if (isWin(x, y) == 1) // 黑子胜利
        {
            winView(); // 显示胜利界面
            system("pause");
            break;
        }
        else if (isWin(x, y) == 2) // 白子胜利
        {
            winView(); // 显示胜利界面
            system("pause");
            break;
        }

        // 游戏未结束，继续循环
        flag++; // 回合数加1
    }
    return;
}