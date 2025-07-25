#pragma once
#include "Piece.h"

class CBishop : public CPiece
{
public:
    CBishop(PLAYER_ID _ID);
    ~CBishop();

    void Draw();
    void Update();

private:

};

