#pragma once

/* Needed for boggle.cpp */
#include "grid.h"
#include "lexicon.h"
void scoreBoardRec(Grid<char>& board, const Lexicon& lex, Vector<GridLocation> &path, Set<std::string> &allFindedWords, int &score);
int scoreBoard(Grid<char>& board, Lexicon& lex);
int points(std::string str);
std::string vecToString(Grid<char>& board, Vector<GridLocation> &path);
Set<GridLocation> generateValidMoves(Grid<char> &board, const Vector<GridLocation> &path);
Set<GridLocation> vectorToSet(const Vector<GridLocation> &path);

/* Needed for voting.cpp */
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);

