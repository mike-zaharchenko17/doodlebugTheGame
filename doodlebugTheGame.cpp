#include <iostream>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cstdlib>
#include <string>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32

using namespace std;

class Slot;

class Entity {
public:
    Entity();
    Entity(Slot* toOccupy);
    Slot* getOccupies();
    int getClassification();
protected:
    Slot* occupies;
    int classification;
};

class Slot {
public:
    Slot() { repr = '-'; occupied = false; occupiedBy = nullptr; }

    Slot(int posX, int posY) : repr('-'), posX(posX), posY(posY), occupied(false), occupiedBy(nullptr) {}

    void printSlot() { cout<<repr<<"\t"; }

    void emptySlot() { repr = '-'; occupied = false; }

    void fillSlot(char newRepr, Entity* occupant) { repr = newRepr; occupied = true; occupiedBy = occupant; }

    bool isOccupied() { return occupied; }

    Entity* getOccupant() { return occupiedBy; }

    int getX() { return posX; }

    int getY() { return posY; }

private:
    bool occupied;
    char repr;
    int posX;
    int posY;
    Entity* occupiedBy;
};

Entity::Entity() { occupies = nullptr; classification = 0; }
Entity::Entity(Slot* toOccupy) { occupies = toOccupy; classification = 0; }
Slot* Entity::getOccupies() { return occupies; }
int Entity::getClassification() { return classification; }

class NPC : public Entity {
public:
    NPC() : Entity() { classification = 1; repr = 'o'; }
    NPC(Slot* toOccupy) : Entity(toOccupy) { classification = 1; repr = 'o'; occupies->fillSlot(repr, this); }
    void move(Slot* toOccupy) { occupies->emptySlot(); occupies = toOccupy; occupies->fillSlot(repr, this); }
private:
    char repr;
};

class playerChar : public Entity {
public:
    playerChar() : Entity() { classification = 2; repr = 'X'; }
    playerChar(Slot* toOccupy) : Entity(toOccupy) { classification = 2; repr = 'X'; occupies->fillSlot(repr, this); }
    void move(Slot* toOccupy) { occupies->emptySlot(); occupies = toOccupy; occupies->fillSlot(repr, this); } 
private:
    char repr;
};

class Gameboard {
public:
    Gameboard() : player(nullptr), starvationCounter(0), score(0), turnCounter(0) {
        srand(time(NULL));
        for (int i = 0; i < 5; i++) {
            vector <Slot*> row;
            for (int j = 0; j < 5; j++) {
                Slot* slotPtr = new Slot(i, j);
                row.push_back(slotPtr);
            }
            board.push_back(row);
        }
    }

    ~Gameboard() {
        delete player;
        for (NPC* i : npcVec) {
            delete i;
        }
        for (vector <Slot*> i : board) {
            for (Slot* k : i) {
                delete k;
            }
        }
    }

    void spawnEntities() {
        player = new playerChar(board[2][2]);

        int spawnArr[4] = {0, 1, 3, 4};
        int spawnArr2[5] = {0, 1, 2, 3, 4};
        int k = 0;

        while (k < 3) {
            int randNum1 = rand() % 4;
            int randNum2 = rand() % 5;

            if (!board[spawnArr[randNum1]][spawnArr2[randNum2]]->isOccupied()) {
                NPC* tempPtr = new NPC(board[spawnArr[randNum1]][spawnArr2[randNum2]]);
                npcVec.push_back(tempPtr);
                ++k;
            }
        }

        starvationCounter = 5;
        score = 0;
        turnCounter = 0;
    }

    void spawnNewNPCs() {

        int spawnArr[4] = {0, 1, 3, 4};
        int spawnArr2[5] = {0, 1, 2, 3, 4};
        int k = 0;

        while (k < 3) {
            int randNum1 = rand() % 4;
            int randNum2 = rand() % 5;
            
            if (!board[spawnArr[randNum1]][spawnArr2[randNum2]]->isOccupied()) {
                NPC* tempPtr = new NPC(board[spawnArr[randNum1]][spawnArr2[randNum2]]);
                npcVec.push_back(tempPtr);
                ++k;
            }
        }
    }   

    void printBoard() {
        cout<<"Score: "<<score<<endl;
        cout<<"Starvation Counter: "<<starvationCounter<<endl<<endl;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                board[i][j]->printSlot();
            }
            cout<<endl;
        }
    }

    void moveNPCs() {
        for (int i = 0; i < npcVec.size(); i++) {

            int currentRow = npcVec[i]->getOccupies()->getX();
            int currentSlot = npcVec[i]->getOccupies()->getY();

            vector <Slot*> possMoves;

            if (currentRow > 0 && currentRow < 4) {
                if (currentSlot > 0 && currentSlot < 4) {
                    possMoves.push_back(board[currentRow + 1][currentSlot]);
                    possMoves.push_back(board[currentRow - 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot + 1]);
                    possMoves.push_back(board[currentRow][currentSlot - 1]);
                } else if (currentSlot == 0) {
                    possMoves.push_back(board[currentRow + 1][currentSlot]);
                    possMoves.push_back(board[currentRow - 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot + 1]);
                } else if (currentSlot == 4) {
                    possMoves.push_back(board[currentRow + 1][currentSlot]);
                    possMoves.push_back(board[currentRow - 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot - 1]);
                } else {
                    cout<<"Invalid slot number."<<endl;
                    exit(1);
                }
            } else if (currentRow == 0) {
                if (currentSlot > 0 && currentSlot < 4) {
                    possMoves.push_back(board[currentRow + 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot + 1]);
                    possMoves.push_back(board[currentRow][currentSlot - 1]);
                } else if (currentSlot == 0) {
                    possMoves.push_back(board[currentRow + 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot + 1]);
                } else if (currentSlot == 4) {
                    possMoves.push_back(board[currentRow + 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot - 1]);
                } else {
                    cout<<"Invalid slot number."<<endl;
                    exit(1);
                }
            } else if (currentRow == 4) {
                if (currentSlot > 0 && currentSlot < 4) {
                    possMoves.push_back(board[currentRow - 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot + 1]);
                    possMoves.push_back(board[currentRow][currentSlot - 1]);
                } else if (currentSlot == 0) {
                    possMoves.push_back(board[currentRow - 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot + 1]);
                } else if (currentSlot == 4) {
                    possMoves.push_back(board[currentRow - 1][currentSlot]);
                    possMoves.push_back(board[currentRow][currentSlot - 1]);
                } else {
                    cout<<"Invalid slot number"<<endl;
                    exit(1);
                }
            } else {
                cout<<"Invalid row number. "<<endl;
                exit(1);
            }
            
            possMoves.erase(remove_if(possMoves.begin(), possMoves.end(), [] (Slot* currSlot) { return currSlot->isOccupied(); }), possMoves.end());

            if (!possMoves.empty()) {
                int randNum = rand() % possMoves.size();
                Slot* selectedSlot = possMoves[randNum];
                board[currentRow][currentSlot]->emptySlot();
                npcVec[i]->move(selectedSlot);
            }
        }
    }

    void gameplayLoop() {

        cout<<"Welcome to Doodlebug The Game!"<<endl;
        cout<<"Eat ants to reset your starvation counter."<<endl;
        cout<<"If it drops below zero, it's game over!"<<endl<<endl;

        cout<<"Controls:"<<endl;
        cout<<"The arrow keys will move your doodlebug."<<endl;
        cout<<"Pressing the space bar will let you wait and delay your starvation counter."<<endl;

        spawnEntities();
        printBoard();

        bool flag = true;
        int currX = 2;
        int currY = 2;
        int waitCounter = 0;

        while (flag) {

            if ((turnCounter % 2 == 0 && turnCounter > 0)) {
                moveNPCs();
                system("cls");
                printBoard();
                cout<<endl; 
            }

            bool isWaiting = false;

            int move = getch();

            int currXOG = currX;
            int currYOG = currY;

            if (starvationCounter >= 0) {
                switch (move) {
                    case KEY_UP:
                        if (currX > 0 && currX <= 4) {
                            currX--;
                            break;
                        } else {
                            break;
                        }
                    case (KEY_DOWN):
                        if (currX >= 0 && currX < 4) {
                            currX++;
                            break;
                        } else {
                            break;
                        }
                    case KEY_LEFT:
                        if (currY > 0 && currY <= 4) {
                            currY--;
                            break;
                        } else {
                            break;
                        }
                    case KEY_RIGHT:
                        if (currY >= 0 && currY < 4) {
                            currY++;
                            break;
                        } else {
                            break;
                        }
                    case KEY_SPACE:
                        isWaiting = true;
                        break;
                    default:
                        break;
                }

                if (currXOG != currX || currYOG != currY) {
                    if (!board[currX][currY]->isOccupied()) {
                        board[currXOG][currYOG]->emptySlot();
                        player->move(board[currX][currY]);

                        starvationCounter--;
                    } else {

                        npcVec.erase(remove(npcVec.begin(), npcVec.end(), board[currX][currY]->getOccupant()), npcVec.end());

                        delete board[currX][currY]->getOccupant();

                        board[currX][currY]->emptySlot();
                        board[currXOG][currYOG]->emptySlot();

                        player->move(board[currX][currY]);

                        score++;
                        starvationCounter = 5;
                    }
                }

                if ((currXOG == currX && currYOG == currY) && isWaiting) {
                    waitCounter++;
                    turnCounter++;
                    if (waitCounter % 2 == 0 && waitCounter > 0) {
                        starvationCounter--;
                    }
                }

                if ((npcVec.size() <= 1) || (npcVec.size() == 2 && starvationCounter < 3)) {
                    spawnNewNPCs();
                }

                turnCounter++;
            } else {
                cout<<"GAME OVER"<<endl;
                cout<<"Your score was: "<<score<<endl;
                flag = false;
            } 
        }
    }

private:
    vector <vector <Slot*>> board;
    vector <NPC*> npcVec;
    playerChar* player;
    int score;
    int starvationCounter;
    int turnCounter;
};

int main() {
    Gameboard g1;
    g1.gameplayLoop();
    return 0;
}