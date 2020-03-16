//
//  Matrix.cpp
//  
//
//  Created by Wang ZhiPeng on 12-12-12.
//
//

#include "Matrix.h"
#include <time.h>
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#define WALL_IMAGE ("\xA1\xF6") //("■")
#define SPACE_IMAGE ("  ")
#define PLAYER_IMAGE ("\xA1\xE2") //("♀")
#else
#include <pthread.h>
#include <unistd.h>
#define WALL_IMAGE ("\xE2\x96\x89\xE2\x96\x89") //("▉▉")
#define SPACE_IMAGE ("  ")
#define PLAYER_IMAGE ("\xE2\x99\x80\xE2\x99\x80") //("♀♀")
#endif

using namespace std;

Matrix::Matrix(const int x_size, const int y_size):m_x_size(x_size), m_y_size(y_size), wall_image(WALL_IMAGE), space_image(SPACE_IMAGE), player_image(PLAYER_IMAGE)
{
    if (m_x_size <= 2) {
        m_x_size = 3;
    }
    if (m_y_size <= 2) {
        m_y_size = 3;
    }
    long data_size = m_x_size * m_y_size;
    m_data = new char[data_size];
    // 默认将迷宫矩阵初始化为全部是墙构成 //
    memset(m_data, Wall, data_size * sizeof(char));
}

Matrix::~Matrix()
{
    if (m_data != NULL) {
        delete m_data;
        m_data = NULL;
    }
}

void Matrix::showMatrix(unsigned int millisecond)
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
    for (int y = 0; y < m_y_size; ++y) {
        for (int x = 0; x < m_x_size; ++x) {
            if (m_data[y * m_x_size + x] == Space) {
                cout<<space_image;
            } else if (m_data[y * m_x_size + x] == Wall) {
                cout<<wall_image;
            }
        }
        cout<<endl;
    }
#if defined(_WIN32) || defined(_WIN64)
    Sleep(millisecond);
#else
    usleep(millisecond * 1000);
#endif
}

void Matrix::showMatrixWithPlayer(unsigned int millisecond)
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
    for (int y = 0; y < m_y_size; ++y) {
        for (int x = 0; x < m_x_size; ++x) {
            if (m_data[y * m_x_size + x] == Space) {
                if (x == m_player_pos.x_pos && y == m_player_pos.y_pos) {
                    cout<<player_image;
                } else {
                    cout<<space_image;
                }
            } else if (m_data[y * m_x_size + x] == Wall) {
                if (x == m_player_pos.x_pos && y == m_player_pos.y_pos) {
                    cout<<player_image;
                } else {
                    cout<<wall_image;
                }
            }
        }
        cout<<endl;
    }
#if defined(_WIN32) || defined(_WIN64)
    Sleep(millisecond);
#else
    usleep(millisecond * 1000);
#endif
}

void Matrix::generateNewMatrix(Point& in_pos, Point& out_pos)
{
    checkBounder(in_pos);
    checkBounder(out_pos);
    
    setPointState(in_pos, Space);
    setPointState(out_pos, Space);
    
    PathNode seed_node;
    seed_node.point = in_pos;
    
    createPuzzle(seed_node);
    
    if (out_pos.x_pos == 0) {
        PathNode e_node;
        e_node.point = out_pos;
        while (!haveSpaceAroundNode(e_node)) {
            e_node.point.x_pos += 1;
            setPointState(e_node.point, Space);
        }
    } else if (out_pos.x_pos == (m_x_size - 1)) {
        PathNode e_node;
        e_node.point = out_pos;
        while (!haveSpaceAroundNode(e_node)) {
            e_node.point.x_pos -= 1;
            setPointState(e_node.point, Space);
        }
    } else if (out_pos.y_pos == 0) {
        PathNode e_node;
        e_node.point = out_pos;
        while (!haveSpaceAroundNode(e_node)) {
            e_node.point.y_pos += 1;
            setPointState(e_node.point, Space);
        }
    } else if (out_pos.y_pos == (m_y_size - 1)) {
        PathNode e_node;
        e_node.point = out_pos;
        while (!haveSpaceAroundNode(e_node)) {
            e_node.point.y_pos -= 1;
            setPointState(e_node.point, Space);
        }
    }
}

void Matrix::createPuzzle(const PathNode& seed_node)
{
    path_stack.clear();
    
    time_t t;
    time(&t);
    tm* s_t = localtime(&t);
    srand(s_t->tm_sec);
    
    collectWallAroundNode(seed_node);
    
    while (path_stack.size() > 0) {
        size_t size = path_stack.size();
        int pos = rand() % size;
        PathNode wall_node = path_stack.at(pos);
        vector<PathNode>::iterator it = path_stack.begin();
        it += pos;
        path_stack.erase(it);
        switch (wall_node.curr_dir) {
            case DirUp:
            {
                PathNode node = wall_node;
                node.point.y_pos -= 1;
                if (whatInPos(node.point) == Wall && !isOnBounder(node.point)) {
                    setPointState(node.point, Space);
                    setPointState(wall_node.point, Space);
                    collectWallAroundNode(node);
                }
                break;
            }
            case DirRight:
            {
                PathNode node = wall_node;
                node.point.x_pos += 1;
                if (whatInPos(node.point) == Wall && !isOnBounder(node.point)) {
                    setPointState(node.point, Space);
                    setPointState(wall_node.point, Space);
                    collectWallAroundNode(node);
                }
                break;
            }
            case DirDown:
            {
                PathNode node = wall_node;
                node.point.y_pos += 1;
                if (whatInPos(node.point) == Wall && !isOnBounder(node.point)) {
                    setPointState(node.point, Space);
                    setPointState(wall_node.point, Space);
                    collectWallAroundNode(node);
                }
                break;
            }
            case DirLeft:
            {
                PathNode node = wall_node;
                node.point.x_pos -= 1;
                if (whatInPos(node.point) == Wall && !isOnBounder(node.point)) {
                    setPointState(node.point, Space);
                    setPointState(wall_node.point, Space);
                    collectWallAroundNode(node);
                }
                break;
            }
            default:
                break;
        }
    }
}

void Matrix::randomNewMatrix()
{
    resetMatrix();
    
    PathNode seed_node;
    seed_node.point.x_pos = rand() % (m_x_size - 2) + 1;
    seed_node.point.y_pos = rand() % (m_y_size - 2) + 1;
    setPointState(seed_node.point, Space);
    
    createPuzzle(seed_node);
    
    time_t t;
    time(&t);
    tm* s_t = localtime(&t);
    srand(s_t->tm_sec);
    
    int in_edge = rand() % 4; // 获得起点的边 //
    int out_edge = (in_edge + 1) % 4; // 获得终点的边 //
    
    int in_x, in_y;
    int out_x, out_y;
    
    switch (in_edge) {
        case DirUp: // up edge
        {
            in_x = rand() % (m_x_size - 2) + 1;
            in_y = 0;
            PathNode s_node;
            s_node.point.x_pos = in_x;
            s_node.point.y_pos = in_y;
            while (!haveSpaceAroundNode(s_node)) {
                s_node.point.y_pos += 1;
                setPointState(s_node.point, Space);
            }
            break;
        }
        case DirRight: // right edge
        {
            in_x = m_x_size - 1;
            in_y = rand() % (m_y_size - 2) + 1;
            PathNode s_node;
            s_node.point.x_pos = in_x;
            s_node.point.y_pos = in_y;
            while (!haveSpaceAroundNode(s_node)) {
                s_node.point.x_pos -= 1;
                setPointState(s_node.point, Space);
            }
            break;
        }
        case DirDown: // down edge
        {
            in_x = rand() % (m_x_size - 2) + 1;
            in_y = m_y_size - 1;
            PathNode s_node;
            s_node.point.x_pos = in_x;
            s_node.point.y_pos = in_y;
            while (!haveSpaceAroundNode(s_node)) {
                s_node.point.y_pos -= 1;
                setPointState(s_node.point, Space);
            }
            break;
        }
        case DirLeft: // left edge
        {
            in_x = 0;
            in_y = rand() % (m_y_size - 2) + 1;
            PathNode s_node;
            s_node.point.x_pos = in_x;
            s_node.point.y_pos = in_y;
            while (!haveSpaceAroundNode(s_node)) {
                s_node.point.x_pos += 1;
                setPointState(s_node.point, Space);
            }
            break;
        }
        default:
            break;
    }
    
    switch (out_edge) {
        case DirUp: // up edge
        {
            out_x = rand() % (m_x_size - 2) + 1;
            out_y = 0;
            PathNode e_node;
            e_node.point.x_pos = out_x;
            e_node.point.y_pos = out_y;
            while (!haveSpaceAroundNode(e_node)) {
                e_node.point.y_pos += 1;
                setPointState(e_node.point, Space);
            }
            break;
        }
        case DirRight: // right edge
        {
            out_x = m_x_size - 1;
            out_y = rand() % (m_y_size - 2) + 1;
            PathNode e_node;
            e_node.point.x_pos = out_x;
            e_node.point.y_pos = out_y;
            while (!haveSpaceAroundNode(e_node)) {
                e_node.point.x_pos -= 1;
                setPointState(e_node.point, Space);
            }
            break;
        }
        case DirDown: // down edge
        {
            out_x = rand() % (m_x_size - 2) + 1;
            out_y = m_y_size - 1;
            PathNode e_node;
            e_node.point.x_pos = out_x;
            e_node.point.y_pos = out_y;
            while (!haveSpaceAroundNode(e_node)) {
                e_node.point.y_pos -= 1;
                setPointState(e_node.point, Space);
            }
            break;
        }
        case DirLeft: // left edge
        {
            out_x = 0;
            out_y = rand() % (m_y_size - 2) + 1;
            PathNode e_node;
            e_node.point.x_pos = out_x;
            e_node.point.y_pos = out_y;
            while (!haveSpaceAroundNode(e_node)) {
                e_node.point.x_pos += 1;
                setPointState(e_node.point, Space);
            }
            break;
        }
        default:
            break;
    }
    start.x_pos = in_x;
    start.y_pos = in_y;
    end.x_pos = out_x;
    end.y_pos = out_y;
    setPointState(start, Space);
    setPointState(end, Space);
}

void Matrix::initializeMatrixWithData(const char* data, const int length)
{
    memcpy(m_data, data, length * sizeof(char));
}

void Matrix::setPlayerPos(const Point &pos)
{
    m_player_pos = pos;
}

void Matrix::checkBounder(Point &point)
{
    if (point.x_pos < 0) {
        point.x_pos = 0;
    }
    if (point.x_pos >= m_x_size) {
        point.x_pos = m_x_size - 1;
    }
    
    if (point.y_pos < 0) {
        point.y_pos = 0;
    }
    if (point.y_pos >= m_y_size) {
        point.y_pos = m_y_size - 1;
    }
}

bool Matrix::checkInner(const Point& point)
{
    bool ret = false;
    do {
        if (point.x_pos < 0) {
            break;
        }
        if (point.x_pos >= m_x_size) {
            break;
        }
        
        if (point.y_pos < 0) {
            break;
        }
        if (point.y_pos >= m_y_size) {
            break;
        }
        ret = true;
    }while(0);
    return ret;
}

bool Matrix::checkNodeDuplicate(const PathNode& node)
{
    bool ret = false;
    do {
        vector<PathNode>::iterator it;
        for (it = path_stack.begin(); it != path_stack.end(); ++it) {
            if (it->point == node.point) {
                ret = true;
                break;
            }
        }
    }while(0);
    return ret;
}

bool Matrix::checkValidePath(const PathNode &node)
{
    bool ret = false;
    do {
        if (checkNodeDuplicate(node)) {
            break;
        }
        if (node.point.x_pos == 0 || node.point.x_pos == (m_x_size - 1)) {
            break;
        }
        if (node.point.y_pos == 0 || node.point.y_pos == (m_y_size - 1)) {
            break;
        }
        Point point = node.point;
        point.x_pos -= 1;
        if (whatInPos(point) == Space) {
            break;
        }
        point = node.point;
        point.x_pos += 1;
        if (whatInPos(point) == Space) {
            break;
        }
        point = node.point;
        point.y_pos -= 1;
        if (whatInPos(point) == Space) {
            break;
        }
        point = node.point;
        point.y_pos += 1;
        if (whatInPos(point) == Space) {
            break;
        }
        ret = true;
    }while(0);
    return ret;
}

void Matrix::resetMatrix()
{
    long data_size = m_x_size * m_y_size;
    memset(m_data, Wall, data_size * sizeof(char));
}

int Matrix::getMatrixWidth()
{
    return m_x_size;
}

int Matrix::getMatrixHeight()
{
    return m_y_size;
}

void Matrix::setPointState(Point &point, MatrixState state)
{
    checkBounder(point);
    m_data[point.y_pos * m_x_size + point.x_pos] = state;
    showMatrix();
}

MatrixState Matrix::whatInPos(const Point &point)
{
    MatrixState ret = Invalid;
    do {
        if (point.x_pos < 0) {
            break;
        }
        if (point.x_pos >= m_x_size) {
            break;
        }
        
        if (point.y_pos < 0) {
            break;
        }
        if (point.y_pos >= m_y_size) {
            break;
        }
        ret = (MatrixState)m_data[point.y_pos * m_x_size + point.x_pos];
    }while(0);
    return ret;
}

Point Matrix::playerMoveUp()
{
    Point ret = m_player_pos;
    ret.y_pos -= 1;
    if (whatInPos(ret) == Space) {
        m_player_pos = ret;
    }
    showMatrixWithPlayer();
    return m_player_pos;
}

Point Matrix::playerMoveDown()
{
    Point ret = m_player_pos;
    ret.y_pos += 1;
    if (whatInPos(ret) == Space) {
        m_player_pos = ret;
    }
    showMatrixWithPlayer();
    return m_player_pos;
}

Point Matrix::playerMoveLeft()
{
    Point ret = m_player_pos;
    ret.x_pos -= 1;
    if (whatInPos(ret) == Space) {
        m_player_pos = ret;
    }
    showMatrixWithPlayer();
    return m_player_pos;
}

Point Matrix::playerMoveRight()
{
    Point ret = m_player_pos;
    ret.x_pos += 1;
    if (whatInPos(ret) == Space) {
        m_player_pos = ret;
    }
    showMatrixWithPlayer();
    return m_player_pos;
}

void Matrix::playerAutoMove()
{
    showMatrixWithPlayer();
    path_stack.clear();
    PathNode node;
    node.point = m_player_pos;
    node.curr_dir = DirUp;
    node.dir_count = 4;
    path_stack.push_back(node);
    
    while (node.point != end && path_stack.size() > 0) {
        node = findPath();
        m_player_pos = node.point;
        showMatrixWithPlayer();
    }
}

void Matrix::resetPlayer()
{
    m_player_pos = start;
    showMatrixWithPlayer();
}

PathNode Matrix::findPath()
{
    PathNode node = path_stack.back();
    path_stack.pop_back();
    
    while (node.dir_count <= 0 && path_stack.size() > 0) {
        node = path_stack.back();
        m_player_pos = node.point;
        showMatrixWithPlayer();
        path_stack.pop_back();
    }
    
    if (node.dir_count <= 0) {
        return node;
    }
    
    PathNode next = node;
    switch (node.curr_dir % 4) {
        case DirUp:
        {
            --node.dir_count;
            ++node.curr_dir;
            next.point = node.point;
            next.point.y_pos -= 1;
            if (whatInPos(next.point) == Space && !checkNodeDuplicate(next)) {
                next.curr_dir = DirLeft;
                next.dir_count = 3;
                path_stack.push_back(node);
                path_stack.push_back(next);
                break;
            }
            if (node.dir_count <= 0) {
                next.point = node.point;
                break;
            }
        }
        case DirRight:
        {
            --node.dir_count;
            ++node.curr_dir;
            next.point = node.point;
            next.point.x_pos += 1;
            if (whatInPos(next.point) == Space && !checkNodeDuplicate(next)) {
                next.curr_dir = DirUp;
                next.dir_count = 3;
                path_stack.push_back(node);
                path_stack.push_back(next);
                break;
            }
            if (node.dir_count <= 0) {
                next.point = node.point;
                break;
            }
        }
        case DirDown:
        {
            --node.dir_count;
            ++node.curr_dir;
            next.point = node.point;
            next.point.y_pos += 1;
            if (whatInPos(next.point) == Space && !checkNodeDuplicate(next)) {
                next.curr_dir = DirRight;
                next.dir_count = 3;
                path_stack.push_back(node);
                path_stack.push_back(next);
                break;
            }
            if (node.dir_count <= 0) {
                next.point = node.point;
                break;
            }
        }
        case DirLeft:
        {
            --node.dir_count;
            ++node.curr_dir;
            next.point = node.point;
            next.point.x_pos -= 1;
            if (whatInPos(next.point) == Space && !checkNodeDuplicate(next)) {
                next.curr_dir = DirDown;
                next.dir_count = 3;
                path_stack.push_back(node);
                path_stack.push_back(next);
                break;
            }
            if (node.dir_count <= 0) {
                next.point = node.point;
                break;
            }
        }
        default:
            path_stack.push_back(node);
            next.point = node.point;
            break;
    }
    return next;
}

bool Matrix::isOnBounder(const Point &point)
{
    bool ret = false;
    do {
        if (point.x_pos == 0) {
            ret = true;
            break;
        }
        if (point.x_pos == (m_x_size - 1)) {
            ret = true;
            break;
        }
        if (point.y_pos == 0) {
            ret = true;
            break;
        }
        if (point.y_pos == (m_y_size - 1)) {
            ret = true;
            break;
        }
    }while(0);
    return ret;
}

void Matrix::collectWallAroundNode(const PathNode &node)
{
    PathNode wall_node = node;
    wall_node.point.x_pos -= 1; //Left
    if (whatInPos(wall_node.point) == Wall && !isOnBounder(wall_node.point) && !haveSpaceAroundNodeWithoutDir(wall_node, DirRight)) {
        wall_node.curr_dir = DirLeft;
        path_stack.push_back(wall_node);
    }
    wall_node = node;
    wall_node.point.x_pos += 1; //Right
    if (whatInPos(wall_node.point) == Wall && !isOnBounder(wall_node.point) && !haveSpaceAroundNodeWithoutDir(wall_node, DirLeft)) {
        wall_node.curr_dir = DirRight;
        path_stack.push_back(wall_node);
    }
    wall_node = node;
    wall_node.point.y_pos -= 1; //Up
    if (whatInPos(wall_node.point) == Wall && !isOnBounder(wall_node.point) && !haveSpaceAroundNodeWithoutDir(wall_node, DirDown)) {
        wall_node.curr_dir = DirUp;
        path_stack.push_back(wall_node);
    }
    wall_node = node;
    wall_node.point.y_pos += 1; //Down
    if (whatInPos(wall_node.point) == Wall && !isOnBounder(wall_node.point) && !haveSpaceAroundNodeWithoutDir(wall_node, DirUp)) {
        wall_node.curr_dir = DirDown;
        path_stack.push_back(wall_node);
    }
}

bool Matrix::haveSpaceAroundNode(const PathNode &node)
{
    bool ret = true;
    do {
        PathNode around_node = node;
        around_node.point.x_pos -= 1; //Left
        if (whatInPos(around_node.point) == Space) {
            break;
        }
        around_node = node;
        around_node.point.x_pos += 1; //Right
        if (whatInPos(around_node.point) == Space) {
            break;
        }
        around_node = node;
        around_node.point.y_pos -= 1; //Up
        if (whatInPos(around_node.point) == Space) {
            break;
        }
        around_node = node;
        around_node.point.y_pos += 1; //Down
        if (whatInPos(around_node.point) == Space) {
            break;
        }
        ret = false;
    }while(0);
    return ret;
}

bool Matrix::haveSpaceAroundNodeWithoutDir(const PathNode& node, const DirectionType& dir)
{
    bool ret = true;
    do {
        PathNode around_node = node;
        around_node.point.x_pos -= 1; //Left
        if (whatInPos(around_node.point) == Space && dir != DirLeft) {
            break;
        }
        around_node = node;
        around_node.point.x_pos += 1; //Right
        if (whatInPos(around_node.point) == Space && dir != DirRight) {
            break;
        }
        around_node = node;
        around_node.point.y_pos -= 1; //Up
        if (whatInPos(around_node.point) == Space && dir != DirUp) {
            break;
        }
        around_node = node;
        around_node.point.y_pos += 1; //Down
        if (whatInPos(around_node.point) == Space && dir != DirDown) {
            break;
        }
        ret = false;
    }while(0);
    return ret;
}
