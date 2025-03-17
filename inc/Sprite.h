typedef enum {dead,alive} status_t;

class Sprite {

private:
  int x; // x coordinate
  int y; // y coordinate
  const unsigned short *image; // ptr->image
  status_t life; // dead/alive
  int needDraw;

public:
  Sprite();
  Sprite(const unsigned short* img);
  Sprite(int x, int y, const unsigned short* img, status_t liv);
  void Sprite_Init();
  int getX();
  int getY();
  void setX(int x);
  void setY(int y);
  void setLife(status_t liv);
  void setDraw(int dr);
  void setImage(unsigned const short* img);
  void Move_Up();
  void Move_Down();
  void Move_Right(int x);
  void Move_Left(int x);
  void Draw(int oldx, int oldy, int w, int h);
  void DrawFish(int w, int h);
  void DrawBubbles(int w, int h);
};





