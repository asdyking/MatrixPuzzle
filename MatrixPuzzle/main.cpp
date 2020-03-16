//
//  main.cpp
//  MatrxPuzzle
//
//  Created by Wang ZhiPeng on 12-12-12.
//  Copyright (c) 2012年 Wang ZhiPeng. All rights reserved.
//

#include "Matrix.h"
#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#endif

using namespace std;

const int map_size = 400;
const char map[map_size] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,1,
    1,0,1,1,1,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1,
    1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,
    1,0,0,0,0,0,0,1,0,1,1,1,1,1,0,1,1,1,0,1,
    1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,1,0,1,
    1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,
    1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,1,0,1,
    1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
    1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    // 默认迷宫后半段 //
    1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,
    1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,1,1,0,1,
    1,0,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,
    1,0,0,0,0,1,0,0,0,0,0,1,1,0,1,0,0,0,1,1,
    1,0,1,1,0,1,1,1,0,1,1,1,1,0,1,0,1,0,1,1,
    1,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,
    1,0,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,0,1,1,
    1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

int main(int argc, const char * argv[])
{
    Matrix* matrix = new Matrix(20, 20);
    matrix->initializeMatrixWithData(map, map_size);
    matrix->start.x_pos = 18;
    matrix->start.y_pos = 1;
    matrix->setPlayerPos(matrix->start);
    matrix->end.x_pos = 19;
    matrix->end.y_pos = 18;
    matrix->showMatrixWithPlayer();
    do {
#if defined(_WIN32) || defined(_WIN64)
		char cmd = getch();
#else
        char cmd = getchar();
#endif
        bool exit = false;
        switch (cmd) {
            case 'w': //up
                matrix->playerMoveUp();
                break;
            case 's': //down
                matrix->playerMoveDown();
                break;
            case 'a': //left
                matrix->playerMoveLeft();
                break;
            case 'd': //right
                matrix->playerMoveRight();
                break;
            case 'm': //auto find path
                matrix->playerAutoMove();
                break;
            case 'r': //reset
                matrix->resetPlayer();
                break;
            case 'n': //new
                matrix->randomNewMatrix();
                matrix->resetPlayer();
                break;
            case 'b': //new with SEU
                matrix->resetMatrix();
                matrix->initializeMatrixWithData(map, map_size / 2);
                matrix->start.x_pos = 18;
                matrix->start.y_pos = 1;
                matrix->end.x_pos = 19;
                matrix->end.y_pos = 18;
                Point start_p;
                start_p.x_pos = 1;
                start_p.y_pos = 10;
                matrix->generateNewMatrix(start_p, matrix->end);
                matrix->resetPlayer();
                break;
            case 'q': //quit
                exit = true;
                break;
            default:
                break;
        }
        if (exit) {
            break;
        }
    }while(1);
    if (matrix != NULL) {
        delete matrix;
        matrix = NULL;
    }
    return 0;
}
