#include "Sprite.h"
#include "../inc/ST7735.h"
#include "images/images.h"

Sprite::Sprite()
{
    x = 0;
    y = 0;
    image = nullptr;
    life = alive;
    needDraw = 1;
}

Sprite::Sprite(const unsigned short* img)
{
    x = 0;
    y = 0;
    image = img;
    life = alive;
    needDraw = 1;
}

Sprite::Sprite(int x, int y, const unsigned short* img, status_t liv)
{
    this->x = x;
    this->y = y;
    image = img;
    life = liv;
    needDraw = 1;
}

void Sprite::Sprite_Init()
{

}

int Sprite::getX()
{
    return x;
}

int Sprite::getY()
{
    return y;
}

void Sprite::setX(int x)
{
   this->x = x;
}

void Sprite::setY(int y)
{
    this->y = y;
}

void Sprite::setImage(unsigned const short* img)
{
    image = img;
}

void Sprite::setLife(status_t liv)
{
    life = liv;
}

void Sprite::setDraw(int dr)
{
    needDraw = dr;
}

void Sprite::Move_Up()
{
    if(y <= 36)
    {
        y = 156;
    }
    else
    {
        y -= 40;
    }
    needDraw = 1;
}

void Sprite::Move_Down()
{
    if(y < 156)
    {
        y += 40;
        needDraw = 1;
    }
    else
    {
        needDraw = 0;
    }
}

void Sprite::Move_Right(int x)
{
    if((this->x) + x < 165)
        {
            this->x += x;
        } else {
            this->x = -2;
        }
        needDraw = 1;

}

void Sprite::Move_Left(int x)
{
    if((this->x) - x > -43)
    {
        this->x -= x;
    } else {
        this->x = 128;
    }
    needDraw = 1;
}

void Sprite::Draw(int oldx, int oldy, int w, int h)
{
    if(needDraw == 1)
    {
       ST7735_DrawBitmap(oldx, oldy, blank2335, w, h);
        needDraw = 0;
    }
        ST7735_DrawBitmap(x, y, image, w, h);
}

void Sprite::DrawFish(int w, int h)
{
    ST7735_DrawBitmap(x, y, image, w, h);

}

void Sprite::DrawBubbles(int w, int h)
{
    ST7735_DrawBitmap(x, y, image, w, h);

}



