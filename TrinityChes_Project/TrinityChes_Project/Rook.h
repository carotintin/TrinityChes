#pragma once
#include "Piece.h"

class CRook : public CPiece
{
public:
    CRook(PLAYER_ID _ID);
    ~CRook();

    void Draw();
    void Update();

private:


};

