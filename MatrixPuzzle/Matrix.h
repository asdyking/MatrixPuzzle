//
//  Matrix.h
//  
//
//  Created by Wang ZhiPeng on 12-12-12.
//
//

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 迷宫矩阵中放置的物体，可以是墙Wall，可以是空间Space，以后可以增加更多类型，比如宝箱Chest，Invalid代表无效的空间，一般指超出了Matrix范围的空间 //
typedef enum{
    Invalid = -1,
    Space,
    Wall
} MatrixState;

// 基本数据结构封装，Point点，代表在Mtraix中的位置 //
struct Point {
    int x_pos;
    int y_pos;
    // 自定义比较函数，用于比较两个Point是否相等 //
    bool operator == (const Point& point)
    {
        return (point.x_pos == x_pos && point.y_pos == y_pos);
    }
    // 自定义比较函数，用于比较两个Point是否不等 //
    bool operator != (const Point& point)
    {
        return (point.x_pos != x_pos || point.y_pos != y_pos);
    }
};
    
typedef enum {
    DirUp = 0,
    DirRight,
    DirDown,
    DirLeft
} DirectionType; // 移动方向枚举，判断顺序为顺时针，上->右->下->左 //
    
struct PathNode {
    Point point; // 节点位置 //
    int curr_dir;// 当前判断的方向 //
    int dir_count;// 还需要判断的方向统计计数 //
}; // 路径节点 //

// 迷宫矩阵类 //
class Matrix {
public:
    // 构造矩阵必须指定大小 //
    Matrix(const int x_size, const int y_size);
    ~Matrix();
public:
    // 在命令行中打印显示当前矩阵 //
    void showMatrix(unsigned int millisecond = 50);
    // 在命令行中打印显示当前矩阵并显示主角的位置 //
    void showMatrixWithPlayer(unsigned int millisecond = 50);
    // 根据指定的进口和出口位置，自动生成新的迷宫矩阵 //
    void generateNewMatrix(Point& in_pos, Point& out_pos);
    // 随机生成新的迷宫矩阵 //
    void randomNewMatrix();
    // 根据指定的数据初始化矩阵，比如可以指定矩阵上半部设置为特定的路径SEU //
    void initializeMatrixWithData(const char* data, const int length);
    // 设置主角当前的位置 //
    void setPlayerPos(const Point& pos);
    Point playerMoveUp();
    Point playerMoveDown();
    Point playerMoveLeft();
    Point playerMoveRight();
    // 角色自动寻找路径 //
    void playerAutoMove();
    // 重置角色位置到起点 //
    void resetPlayer();
    // 重置迷宫矩阵 //
    void resetMatrix();
    // 获得迷宫矩阵的宽度 //
    int getMatrixWidth();
    // 获得迷宫矩阵的高度 //
    int getMatrixHeight();
    // 设置指定位置的矩阵类型 //
    void setPointState(Point& point, MatrixState state);
protected:
    // 检查指定参数点是否超出边界，如果超出边界，把参数修改为边界上的点 //
    void checkBounder(Point& point);
    // 检查指定参数点是否在矩阵内部，边界上的点不算在矩阵内部 //
    bool checkInner(const Point& point);
    // 返回指定参数点上设置的矩阵物体类型，是Wall还是Space //
    MatrixState whatInPos(const Point& point);
    // 寻找路径，并返回找出的下一个路径节点 //
    PathNode findPath();
    // 在当前矩阵环境中创造迷宫 //
    void createPuzzle(const PathNode& seed_node);
    // 检查指定节点是否已经在路径栈中出现，如果是，返回true，如果不是，返回false //
    bool checkNodeDuplicate(const PathNode& node);
    // 检查指定节点是否是一个合法的路径节点，如果是，返回true，如果不是，返回false //
    bool checkValidePath(const PathNode& node);
    // 判断节点是否在边界上 //
    bool isOnBounder(const Point& point);
    // 将指定节点的周边非边界和非空间的墙体区域添加到指定队列中 //
    void collectWallAroundNode(const PathNode& node);
    // 返回指定节点周边是否有Space节点 //
    bool haveSpaceAroundNode(const PathNode& node);
    // 返回指定节点周边除了参数说明的方向外，是否有Space节点 //
    bool haveSpaceAroundNodeWithoutDir(const PathNode& node, const DirectionType& dir);
public:
    Point start;            // 起点 //
    Point end;              // 终点 //
protected:
    int m_x_size;   // 迷宫矩阵的横向大小 //
    int m_y_size;   // 迷宫矩阵的纵向大小 //
    char* m_data;    // 迷宫矩阵数据 //
    const string wall_image;  // 在命令行中Wall类型需要显示的内容 //
    const string space_image; // 在命令行中Space类型需要显示的内容 //
    const string player_image;// 在命令行中Player需要显示的内容 //
    Point m_player_pos;       // 主角的位置 //
    vector<PathNode> path_stack; // 路径栈 //
};
