#include <iostream>
#include "Grille.h"
#include <cassert>
#include <vector>


Grille::Grille() {
    this->dimX = 4;
    this->dimY = 4;

    this->grid = new Case[this->dimX * this->dimY];
}

unsigned int Grille::getDimX() const {
    return dimX;
}

unsigned int Grille::getDimY() const {
    return dimY;
}

Case & Grille::getCase(unsigned int x, unsigned int y) {
    assert(x<dimX);
    assert(y<dimY);
    return grid[y*dimX+x];
}

void Grille::setCase(unsigned int x, unsigned int y, Pion * p) {
    if (x < dimX && y < dimY) grid[y*dimX+x].setPion(p);
}

bool Grille::haveOneCommonCharacteristic(std::vector<Pion*> pions)
{
    bool canCheck = true;

    // making sure there's no nulls pions before checking their characteristics
    for(int i = 0; i < 4; i ++)
    {
        if(pions.at(i) == nullptr)
        {
            canCheck = false;
        }
    }
    if(canCheck)
    {
        return ((pions.at(0)->getTrou()) && (pions.at(1)->getTrou()) && (pions.at(2)->getTrou()) &&
                (pions.at(3)->getTrou()))
               // les pions n'ont pas de trou
               || (!(pions.at(0)->getTrou()) && !(pions.at(1)->getTrou()) && !(pions.at(2)->getTrou()) &&
                   !(pions.at(3)->getTrou()))
               // les pions sont bleu
               || ((pions.at(0)->getCouleur()) && (pions.at(1)->getCouleur()) && (pions.at(2)->getCouleur()) &&
                   (pions.at(3)->getCouleur()))
               // les pions sont rouges
               || (!(pions.at(0)->getCouleur()) && !(pions.at(1)->getCouleur()) && !(pions.at(2)->getCouleur()) &&
                   !(pions.at(3)->getCouleur()))
               // les pions sont rectangulaires
               || ((pions.at(0)->getForme()) && (pions.at(1)->getForme()) && (pions.at(2)->getForme()) &&
                   (pions.at(3)->getForme()))
               // les pions sont ronds
               || (!(pions.at(0)->getForme()) && !(pions.at(1)->getForme()) && !(pions.at(2)->getForme()) &&
                   !(pions.at(3)->getForme()))
               // les pions sont grands
               || ((pions.at(0)->getTaille()) && (pions.at(1)->getTaille()) && (pions.at(2)->getTaille()) &&
                   (pions.at(3)->getTaille()))
               // les pions sont petits
               || (!(pions.at(0)->getTaille()) && !(pions.at(1)->getTaille()) && !(pions.at(2)->getTaille()) &&
                   !(pions.at(3)->getTaille()));
    }
    else
    {
        return false;
    }
}

bool Grille::horizontalOrVerticalWin()
{
    std::vector<Pion*> horizontalPions;
    std::vector<Pion*> verticalPions;
    for(unsigned int i = 0; i < this->dimX; i++)
    {
        horizontalPions.clear();
        verticalPions.clear();
        for(unsigned int j = 0; j < this->dimX; j++)
        {
            verticalPions.push_back(this->getCase(i, j).getPion());
            horizontalPions.push_back(this->getCase(j, i).getPion());
        }
        if(this->haveOneCommonCharacteristic(horizontalPions) || this->haveOneCommonCharacteristic(verticalPions))
        {
            return true;
        }
    }
    return false;
}

bool Grille::diagonalOrReverseDiagonalWin()
{
    unsigned int i;
    unsigned int j;
    std::vector<Pion*> diagonalPions;
    std::vector<Pion*> reverseDiagonalPions;
    for(i = 0, j = 3; (i < this->dimX) && (j >= 0); i++, j--)
    {
        diagonalPions.push_back(this->getCase(i, j).getPion());
        reverseDiagonalPions.push_back(this->getCase(i, i).getPion());
    }
    return this->haveOneCommonCharacteristic(diagonalPions) || this->haveOneCommonCharacteristic(reverseDiagonalPions);
}

bool Grille::win()
{
    return ( (this->horizontalOrVerticalWin()) || this->diagonalOrReverseDiagonalWin() );
}

bool Grille::full() {
    for (unsigned int i = 0; i < dimX; ++i) {
        for (unsigned int j = 0; j < dimY; ++j) {
            if (getCase(i, j).getPion() == nullptr) return false;
        }
    }
    return true;
}

int Grille::heuristicValueHorizontal(unsigned int indice) {
    int value, bestValue = 0;
    std::vector<Pion *> tab;
    bool equals;
    for (unsigned int ind1 = 0; ind1 < dimX; ++ind1) {
        tab.clear();
        if (this->getCase(indice, ind1).getPion() != nullptr) {
            tab.push_back(this->getCase(indice, ind1).getPion());
            for (unsigned int ind2 = ind1; ind2 < dimY; ++ind2) {
                if (this->getCase(indice, ind2).getPion() != nullptr) {
                    equals = true;
                    for (auto &k : tab) {
                        if (k != nullptr) {
                            equals = k->equals(this->getCase(indice, ind2).getPion());
                        }
                    }
                    if (equals) tab.push_back(this->getCase(indice, ind2).getPion());
                }
            }
            value = (int)tab.size();
            if (value > bestValue) bestValue = value;
        }
    }

    return bestValue;
}

int Grille::heuristicValueVertical(unsigned int indice) {
    int value, bestValue = 0;
    std::vector<Pion *> tab;
    bool equals;
    for (unsigned int ind1 = 0; ind1 < dimX; ++ind1) {
        tab.clear();
        if (this->getCase(ind1, indice).getPion() != nullptr) {
            tab.push_back(this->getCase(ind1, 3-ind1).getPion());
            for (unsigned int ind2 = ind1; ind2 < dimY; ++ind2) {
                if (this->getCase(ind2, indice).getPion() != nullptr) {
                    equals = true;
                    for (auto &k : tab) {
                        if (k != nullptr) {
                            equals = k->equals(this->getCase(ind2, indice).getPion());
                        }
                    }
                    if (equals) tab.push_back(this->getCase(ind2, indice).getPion());
                }
            }
            value = (int)tab.size();
            if (value > bestValue) bestValue = value;
        }
    }

    return bestValue;
}

int Grille::heuristicValueDiagonal() {
    int value, bestValue = 0;
    std::vector<Pion *> tab;
    bool equals;
    for (unsigned int ind1 = 0; ind1 < dimX; ++ind1) {
        tab.clear();
        if (this->getCase(ind1, ind1).getPion() != nullptr) {
            tab.push_back(this->getCase(ind1, 3-ind1).getPion());
            for (unsigned int ind2 = ind1; ind2 < dimY; ++ind2) {
                if (this->getCase(ind2, ind2).getPion() != nullptr) {
                    equals = true;
                    for (auto &k : tab) {
                        if (k != nullptr) {
                            equals = k->equals(this->getCase(ind2, ind2).getPion());
                        }
                    }
                    if (equals) tab.push_back(this->getCase(ind2, ind2).getPion());
                }
            }
            value = (int)tab.size();
            if (value > bestValue) bestValue = value;
        }
    }

    return bestValue;
}

int Grille::heuristicValueAntiDiagonal() {
    int value, bestValue = 0;
    std::vector<Pion *> tab;
    bool equals;
    for (unsigned int ind1 = 0; ind1 < dimX; ++ind1) {
        tab.clear();
        if (this->getCase(ind1, 3-ind1).getPion() != nullptr) {
            tab.push_back(this->getCase(ind1, 3-ind1).getPion());
            for (unsigned int ind2 = ind1; ind2 < dimY; ++ind2) {
                if (this->getCase(ind2, 3-ind2).getPion() != nullptr) {
                    equals = true;
                    for (auto &k : tab) {
                        if (k != nullptr) {
                            equals = k->equals(this->getCase(ind2, 3-ind2).getPion());
                        }
                    }
                    if (equals) tab.push_back(this->getCase(ind2, 3-ind2).getPion());
                }
            }
            value = (int)tab.size();
            if (value > bestValue) bestValue = value;
        }
    }

    return bestValue;
}

int Grille::heuristicValue() {
    int value, bestValue = 0;
    // Comme dimX = dimY on peut faire une seule boucle
    for (unsigned int indice = 0; indice < dimX; ++indice) {
        value = heuristicValueHorizontal(indice);
        if (value > bestValue) bestValue = value;

        value = heuristicValueVertical(indice);
        if (value > bestValue) bestValue = value;
    }

    value = heuristicValueDiagonal();
    if (value > bestValue) bestValue = value;

    value = heuristicValueAntiDiagonal();
    if (value > bestValue) bestValue = value;

    return bestValue;
}

bool Grille::win(Motif motif)
{
    switch(motif)
    {
        case BATON : return this->batonWin();
        case L_NORMAL : return this->l_normalWin();
        case L_INVERSE : return this->l_inverseWin();
        case BLOC : return this->blocWin();
        case BIAIS_NORMAL : return this->biais_normalWin();
        case T : return this->tWin();
        case BIAIS_INVERSE : return this->biais_inverseWin();
        case NONE : return false;
        default : return false;
    }
}

bool Grille::batonWin()
{
    return this->horizontalOrVerticalWin();
}

bool Grille::l_normalWin()
{
    // (i, j), (i, j-1), (i+1, j), (i+2, j)
    // (i, j), (i-1, j), (i, j-1), (i, j-2)
    // (i, j), (i-1, j), (i-2, j), (i, j+1)
    // (i, j), (i+1, j), (i, j+1), (i, j+2)

    int offsets1 [] = {0, 0, 0, -1, 1, 0, 2, 0};
    int offsets2 [] = {0, 0, -1, 0, 0, -1, 0, -2};
    int offsets3 [] = {0, 0, -1, 0, -2, 0, 0, 1};
    int offsets4 [] = {0, 0, 1, 0, 0, 1, 0, 2};
    return ( (this->winMotifTetris(offsets1)) || (this->winMotifTetris(offsets2)) || (this->winMotifTetris(offsets3)) || (this->winMotifTetris(offsets4)) );
}

bool Grille::l_inverseWin()
{
    // (i, j), (i, j-1), (i-1, j), (i-2, j)
    // (i, j), (i+1, j), (i+2, j), (i, j+1)
    // (i, j), (i, j-2), (i, j-1), (i+1, j)
    // (i, j), (i-1, j), (i, j+1), (i, j+2)

    int offsets1 [] = {0, 0, 0, -1, -1, 0, -2, 0};
    int offsets2 [] = {0, 0, 1, 0, 2, 0, 0, 1};
    int offsets3 [] = {0, 0, 0, -2, 0, -1, 1, 0};
    int offsets4 [] = {0, 0, -1, 0, 0, 1, 0, 2};
    return ( (this->winMotifTetris(offsets1)) || (this->winMotifTetris(offsets2)) || (this->winMotifTetris(offsets3)) || (this->winMotifTetris(offsets4)) );
}

bool Grille::blocWin()
{
    // square : (i,j), (i+1,j), (i+1, j+1), (i, j+1)
    int offsets [] = {0, 0, 1, 0, 1, 1, 0, 1};
    return this->winMotifTetris(offsets);
}

bool Grille::biais_normalWin()
{
    //  (i,j), (i-1, j), (i, j-1), (i+1, j-1)
    // or : (i, j), (i-1, j), (i-1, j-1), (i, j+1)

    int offsets1 [] = {0, 0, -1, 0, 0, -1, 1, -1};
    int offsets2 [] = {0, 0, -1, 0, -1, -1, 0, 1};

    return ( (this->winMotifTetris(offsets1)) || (this->winMotifTetris(offsets2)) );
}

bool Grille::tWin()
{
    // T : (i,j), (i-1,j), (i+1,j), (i,j-1) _|_
    // or : (i,j), (i,j-1), (i+1,j), (i,j+1) |-
    // or : (i,j), (i-1,j), (i+1,j), (i,j+1)  T
    // or : (i,j), (i-1,j), (i, j-1), (i, j+1) -|

    int offsets1 [] = {0, 0, -1, 0, 1, 0, 0, -1};
    int offsets2 [] = {0, 0, 0, -1, 1, 0, 0, 1};
    int offsets3 [] = {0, 0, -1, 0, 1, 0, 0, 1};
    int offsets4 [] = {0, 0, -1, 0, 0, -1, 0, 1};
    return ( (this->winMotifTetris(offsets1)) || (this->winMotifTetris(offsets2)) || (this->winMotifTetris(offsets3)) || (this->winMotifTetris(offsets4)) );
}

bool Grille::biais_inverseWin()
{
    //  (i,j), (i, j-1), (i-1, j-1), (i+1, j)
    // or : (i, j), (i-1, j), (i-1, j+1), (i, j-1)

    int offsets1 [] = {0, 0, 0, -1, -1, -1, 1, 0};
    int offsets2 [] = {0, 0, -1, 0, -1, 1, 0, -1};

    return ( (this->winMotifTetris(offsets1)) || (this->winMotifTetris(offsets2)) );
}

bool Grille::checkPionsWith4Coordinates(const std::vector<std::pair<int, int>>& coordinates)
{
    std::vector <Pion*> blockPions;
    blockPions.reserve(coordinates.size());
    for(auto & coordinate : coordinates) {
        blockPions.push_back(this->getCase(coordinate.first, coordinate.second).getPion());
    }
    return this->haveOneCommonCharacteristic(blockPions);
}

bool Grille::outOfGrid(const std::vector<std::pair<int, int>>& coordinates)
{
    for(auto & coordinate : coordinates)
    {
        if
        (
                (coordinate.first < 0)
            ||  (coordinate.first > 3)
            ||  (coordinate.second < 0)
            ||  (coordinate.second > 3)
        )
        {
            return true;
        }
    }
    return false;
}

bool Grille::winMotifTetris(int offsets[7])
{
    for(int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            std::vector <std::pair<int, int>> block;
            block.emplace_back(std::make_pair(i+offsets[0], j+offsets[1]));
            block.emplace_back(std::make_pair(i+offsets[2], j+offsets[3]));
            block.emplace_back(std::make_pair(i+offsets[4], j+offsets[5]));
            block.emplace_back(std::make_pair(i+offsets[6], j+offsets[7]));
            if(!this->outOfGrid(block))
            {
                if(this->checkPionsWith4Coordinates(block))
                {
                    return true;
                }
            }
        }
    }
    return false;
}
