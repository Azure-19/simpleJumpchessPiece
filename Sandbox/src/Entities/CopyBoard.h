#pragma once
enum Color {
    White, Black, Yellow, Green, Blue, Purple, Red
};


class CopyBoard {

public:

    explicit CopyBoard();
    ~CopyBoard();
    bool SetRaw(int r);
    bool SetColumn(int c);
    void SetColor(Color c);
    int getRaw();
    int getCol();
    Color getColor();


private:
    int raw;       
    int col;       
    Color color;   
};
