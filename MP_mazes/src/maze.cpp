/* Your code here! */
#include <memory>
#include <maze.h>
#include <algorithm>
using namespace std;
using namespace cs225;
SquareMaze::SquareMaze(){
// leave empty?
}


void SquareMaze::makeMaze(int width, int height){ 
    
    width_ = width;
    height_ = height;
    walls_.resize(width_);
    for (int col = 0; col < width_; ++col) {
        walls_[col].resize(height_);
    }

    DisjointSets mazeCells;
    mazeCells.addelements(width_ * height_);

    for (int r = 0; r< height_; ++r) {
        for (int c = 0; c < width_; ++c) {
            setWall(c, r, 0, true); 
            setWall(c, r, 1, true); 
        }
    }

    int wallsremoved = 0;
    while (wallsremoved < width_ * height_ - 1) {
        int randRow = rand() % height_;
        int randCol = rand() % width_;
        int cellIndex = randRow * width_ + randCol; // Row-major 
        int wallDirection = rand() % 2; 

        if (wallDirection == 1) { 
            if (randCol != width_ - 1 && mazeCells.find(cellIndex) != mazeCells.find(cellIndex + 1)) {
                mazeCells.setunion(mazeCells.find(cellIndex), mazeCells.find(cellIndex + 1));
                setWall(randCol, randRow, 0, false); 
                wallsremoved++;
            }
        } else { 
            if (randRow != height_ - 1 && mazeCells.find(cellIndex) != mazeCells.find(cellIndex + width_)) {
            mazeCells.setunion(mazeCells.find(cellIndex), mazeCells.find(cellIndex + width_));
             setWall(randCol, randRow, 1, false); // Remove 
             wallsremoved++;
            }
        }
    }
}

bool SquareMaze::canTravel(int currentX, int currentY, int direction) const {
    switch (direction) {
        case 0: // right 
            return currentX != width_ - 1 && !walls_[currentX][currentY].right;
        case 1: // down
            return currentY != height_ - 1 && !walls_[currentX][currentY].down;
        case 2: // left
            return currentX != 0 && !walls_[currentX - 1][currentY].right;
        case 3: // up
            return currentY != 0 && !walls_[currentX][currentY - 1].down;
        default:
            return false;
    }


}


void SquareMaze::setWall(int x, int y, int 	dir, bool exists){
    if (dir == 0) {
        walls_[x][y].right = exists;
    } else {
        walls_[x][y].down = exists;
    }
}

vector<int> SquareMaze::solveMaze() {
   queue<pair<int, int>> explorationQueue;
    vector<vector<bool>> isCellVisited(width_, vector<bool>(height_, false));
    vector<vector<pair<int, int>>> pathFromCell(width_, vector<pair<int, int>>(height_));
    vector<int> pathLengthToEndRow(width_);
    explorationQueue.push({0, 0});

    isCellVisited[0][0] = true;
    pair<int, int> currentCell;
    int stepCount = 0;
    while (!explorationQueue.empty()) {
        currentCell = explorationQueue.front();
        explorationQueue.pop();
        int currentX = currentCell.first;
        int currentY = currentCell.second;

        
        for (int dir = 0; dir < 4; ++dir) {
            int nextX = currentX + (dir == 0) - (dir == 2);
            int nextY = currentY + (dir == 1) - (dir == 3);
            if (canTravel(currentX, currentY, dir) && !isCellVisited[nextX][nextY]) {
                explorationQueue.push({nextX, nextY});
                isCellVisited[nextX][nextY] = true;
                pathFromCell[nextX][nextY] = currentCell;
                if (nextY == height_ - 1) {
                    pathLengthToEndRow[nextX] = stepCount;
                }
            }
        }
        stepCount++;
    }

    int destinationX = distance(pathLengthToEndRow.begin(), max_element(pathLengthToEndRow.begin(), pathLengthToEndRow.end()));
    vector<int> solutionDirection;
    currentCell = {destinationX, height_ - 1};

    while (currentCell != make_pair(0,0)) {
        pair<int, int> previousCell = pathFromCell[currentCell.first][currentCell.second];
        int direction = (currentCell.first > previousCell.first) ? 0 :
                        (currentCell.first < previousCell.first) ? 2 :
                        (currentCell.second > previousCell.second) ? 1 : 3;
        solutionDirection.push_back(direction);
        currentCell = previousCell;
    }
    reverse(solutionDirection.begin(), solutionDirection.end());
    return solutionDirection;
}

cs225::PNG * SquareMaze::drawMaze()const{ 
    const int pixelWidth = width_ * 10 + 1;
    const int pixelHeight = height_ * 10 + 1;
    std::unique_ptr<cs225::PNG> mazeGraphic(new cs225::PNG(pixelWidth, pixelHeight));

   
    for (int yPos = 0; yPos < pixelHeight; ++yPos) {
        mazeGraphic->getPixel(0, yPos).l = 0; // left edge 
    }
    for (int xPos = 0; xPos < pixelWidth; ++xPos) {
        mazeGraphic->getPixel(xPos, 0).l = 0; // top edge 
    }
    
    for (int entrancePos = 1; entrancePos < 10; ++entrancePos) {
        mazeGraphic->getPixel(entrancePos, 0).l = 1;
    }


 for (int gridRow = 0; gridRow < height_; ++gridRow) {
    for (int gridCol = 0; gridCol < width_; ++gridCol) {
       
        int baseX = gridCol * 10, baseY = gridRow * 10;

      
        if (walls_[gridCol][gridRow].down) {
            for (int offset = 0; offset <= 10; ++offset) {
                mazeGraphic->getPixel(baseX + offset, baseY + 10).l = 0;
            }
        }

    
        if (walls_[gridCol][gridRow].right) {
            for (int offset = 0; offset <= 10; ++offset) {
                mazeGraphic->getPixel(baseX + 10, baseY + offset).l = 0;
            }
        }
    }
 }
    return mazeGraphic.release();

}


cs225::PNG * SquareMaze::drawMazeWithSolution(){
PNG* mazeImage = drawMaze();
vector<int> solutionPath = solveMaze();
int currentX = 5;
int currentY = 5;
int pathLength = solutionPath.size();

auto setPathColor = [&](int posX, int posY) {
    mazeImage->getPixel(posX, posY).h = 0;
    mazeImage->getPixel(posX, posY).s = 1;
    mazeImage->getPixel(posX, posY).l = 0.5;
    mazeImage->getPixel(posX, posY).a = 1;
};

for (int i = 0; i < pathLength; i++) {
    switch (solutionPath[i]) {
        case 0: // Right
            for (int j = 0; j < 11; j++) setPathColor(currentX + j, currentY);
            currentX += 10;
            break;
        case 1: // Down
            for (int j = 0; j < 11; j++) setPathColor(currentX, currentY + j);
            currentY += 10;
            break;
        case 2: // Left
            for (int j = 0; j < 11; j++) setPathColor(currentX - j, currentY);
            currentX -= 10;
            break;
        case 3: // Up
            for (int j = 0; j < 11; j++) setPathColor(currentX, currentY - j);
            currentY -= 10;
            break;
    }
}
// need to be AJUSTED 
currentX -= 5;

for (int j = 1; j < 10; j++) { // start from 1
    mazeImage->getPixel(currentX + j, height_ * 10).l = 1; // Lighten the path 
}
return mazeImage;
}






