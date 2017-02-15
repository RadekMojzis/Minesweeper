#ifndef TILE_INCLUDED
#define TILE_INCLUDED

class tile{
    private:
      SDL_Rect box;    // Size and offset of a tile
      SDL_Rect* clip;  // size and offset of its texture
      int minecount;
      int state;
			int type;
			bool isflag;
    public:
      button();
			void settile(int x, int y, int w, int h);

      void render();

      void reveal();
      void plantmine();
      void addmine();
      void changeflag();
      void changestate(int newstate);
      
      int getminecount();
      bool ismine();
      bool isrevealed();
      bool isflag();
};

#endif 
