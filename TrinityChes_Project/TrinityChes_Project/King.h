#pragma once
#include "Piece.h"


class CKing : public CPiece
{
public:
    CKing(PLAYER_ID);
    ~CKing();

    void Draw();
    void Update();

private:


};

