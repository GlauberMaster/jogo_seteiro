struct Nuvem{
	int x;
	int y;
	int speed;
	bool live;
};

struct Player{
	int x;
	int y;
	void inicializa(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	void desenhaPlayer(ALLEGRO_BITMAP *playerImg)
	{
		al_draw_scaled_bitmap(playerImg, 0, 0, 100, 132, this->x, this->y, 80, 100, 0);
	}
};

struct Alvo
{
	int x;
	int y;
	int centro;
	int raioX; //36
	int raioY; //38
	int resizeX;
	int resizeY;
	
	void desenha(ALLEGRO_BITMAP *imgAlvo, int x, int y)
	{
		this->x = x;
		this->y = y;
		al_draw_scaled_bitmap(imgAlvo, 0, 0, 40, 40, this->x, this->y, this->resizeX, this->resizeY, 0);
	}
	
	void inicializa(int x, int y, int resizeX, int resizeY)
	{
		this->x = x;
		this->y = y;
		this->resizeX = resizeX;
		this->resizeY = resizeY;
		this->centro = (15*resizeX) / 40;
		this->raioX = this->centro;
		this->raioY = this->centro;
	}
	
};

struct Seta{
	int x;
	int y;
	double angulo;
	int speedX;
	int speedY;
	bool live;
	int gravidade;
	bool colidiu;
	
	//Métodos
	void inicializa(Player &player)
	{
		this->x = player.x + 50;
		this->y = player.y + 50;
		this->speedX = 0;
		this->speedY = 0;
		this->angulo = 0;
		this->live = false;
		this->gravidade = 0;
		this->colidiu = false;
	};
	
	void desenha(ALLEGRO_BITMAP *flechaImg)
	{
		if(this->live)
			al_draw_rotated_bitmap(flechaImg, 16, 4 ,this->x, this->y, this->angulo, 0);
	};
	
	void dispara(double valorAngulo,int forca)
	{
		if(!this->live)
		{
			this->live = true;
			this->speedX = forca / valorAngulo;
			this->speedY = -forca * valorAngulo;
		}	
	};

};


