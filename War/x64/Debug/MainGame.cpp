#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

struct Body
{
	int x=0;
	int y=0;
	bool life = false;
	Body()
	{
		x = 0;
		y = 0;
		life = false;
	}
};

double speedx = 0.3;
double speedy = 0.3;
const int H = 15;
const int W = 30;
bool A1 = false;
bool A2 = false;
String MainMap[W]{
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"                             ",
	"S                            ",
	"S                            ",
	"S                            ",
	"SSSSSSSSSSSSSSSSSSSSSSSSSSSSS",
};
bool GamePlay = true;

class PLAYER {  
public:
	Text* VS;
	string Name;
	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;
	char apple;
	int count =0;
	int score = 0;
	Body body[50];

	PLAYER(Texture &image, char c,string name)
	{
		Name = name;

		sprite.setTexture(image);
		rect = FloatRect(300, 150, 40, 40);

		apple = c;

		dx = dy = 0;
		currentFrame = 0;
		sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, 40,40));

		body[0].life = true;
		body[0].x = rect.left;
		body[0].y = rect.top;

	}
	PLAYER()
	{

	}
	void update(float time)
	{
		rect.left += dx * time;
		Collision(0);
		rect.top += dy * time;
		Collision(1);
		sprite.setPosition(rect.left, rect.top);

		body[0].x = rect.left;
		body[0].y = rect.top;

	}

	void Collision(int dir)
	{
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
		{
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
			{
				if(i<15 && j<30)
				if (MainMap[i][j] == 'S')
				{
					if ((dx > 0) && (dir == 0)) { GamePlay = false;  VS->setString("Проиграл " + Name);}// rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) { GamePlay = false;  VS->setString("Проиграл " + Name); }// rect.left = j * 32 + 32;

					if ((dy > 0) && (dir == 1)) { GamePlay = false;  VS->setString("Проиграл " + Name); }// { rect.top = i * 32 - rect.height;  dy = 0; }
					if ((dy < 0) && (dir == 1)) { GamePlay = false;  VS->setString("Проиграл " + Name); }// { rect.top = i * 32 + 32; dy = 0; }
				}
				else
					if (MainMap[i][j] == apple)
					{
						MainMap[i][j] = ' ';
						count++;
						score++;
						if (score == 50) 
						{
							GamePlay = false;
							VS->setString("Победил "+Name);
						}
						body[count].life = true;
						body[count].x = body[count-1].x;
						body[count].y = body[count-1].y;

					}
					else if (MainMap[i][j] != ' ')
					{
						MainMap[i][j] = ' ';
						score++;
						if (score == 50)
						{
							GamePlay = false;
							VS->setString("Победил " + Name);
						}
						body[count].life = false;
						count--;
						if (count < 0) count = 0;
					}
			}
		}
	}
	void MoveBody() 
	{
		for (int i = 49; i > 0; i--)
		{
			if (body[i].life) 
			{
				body[i].x = body[i-1].x;
				body[i].y = body[i-1].y;
			}
		}
	};
};

void spawnA(char apple)
{
	int a = 0;
	for (int i = 0; i < H; i++)
	{
		for (int u = 0; u < W; u++)
		{
			if (MainMap[i][u] == apple) a++;
		}
	}
	if (a == 0)
	{
		MainMap[rand() % 12 + 2][rand() % 28+1] = apple;
	}
}

void triger(PLAYER p, PLAYER p2,Text& txt) 
{
	for (int i = 0; i < 40; i++)
	{
		if (p2.body[i].life)
			if ((p.rect.top < p2.body[i].y && p.rect.top + p.rect.height > p2.body[i].y) &&
				((p.rect.left < p2.body[i].x && p.rect.left + p.rect.width > p2.body[i].x)
					|| (p.rect.left < p2.body[i].x + p.rect.width && p.rect.left + p.rect.width > p2.body[i].x + p.rect.width)))

			{
				GamePlay = false;
				txt.setString("Победил "+p2.Name);
				break;
			}
		if ((p.rect.top < p2.body[i].y + p2.rect.height && p.rect.top + p.rect.height > p2.body[i].y + p2.rect.height) &&
			((p.rect.left < p2.body[i].x && p.rect.left + p.rect.width > p2.body[i].x)
				|| (p.rect.left < p2.body[i].x + p2.rect.width && p.rect.left + p.rect.width > p2.body[i].x + p2.rect.width)))
		{
			GamePlay = false;
			txt.setString("Победил " + p2.Name);
			break;
		}
	}
}

void spavnMap(int o, int c)
{
	int u = 0;
	MainMap[o] = "";
	for (int i = 0; i < W; i++)
	{
		int b = 0;
		if (u == 0) b = 1;
		u = rand() % c - b;
		if (u == 0 || u < 0) MainMap[o] += "S";
		else
		{
			if (u >= 1 && u <= 7) MainMap[o] += " ";
			else
			{
				MainMap[o] += "0";
			}
		}

	}
}
int main()

{
	setlocale(LC_ALL, "Russian");

	for (int i = 1; i < H; i++)
	{
		MainMap[i][0] = 'S';
		MainMap[i][W - 1] = 'S';
	}
	for (int i = 0; i < W - 1; i++)
	{
		MainMap[0][i] = ' ';
		MainMap[1][i] = 'S';
	}


	RenderWindow window(VideoMode(W*32, H*32), "SFMLworks");

	Texture t;
	t.loadFromFile("1.jpg");

	PLAYER p(t, 'A',"Первый");
	PLAYER p2(t, 'B',"Второй");

	Font font;
	font.loadFromFile("CyrilicOld.TTF");


	Text nom1;
	nom1.setFont(font);
	nom1.setString('1');
	nom1.setCharacterSize(24);
	nom1.setFillColor(Color::Black);

	Text nom2 = nom1;
	nom2.setString('2');

	Text score1;
	score1.setFont(font);
	score1.setString(to_string(p.score));
	score1.setCharacterSize(24);
	score1.setFillColor(Color::Black);

	Text score2;
	score2 = score1;
	score2.setString(to_string(p2.score));
	score2.setPosition(900,0);

	Text VS;
	VS.setFont(font);
	VS.setString("VS");
	VS.setCharacterSize(24);
	VS.setFillColor(Color::Black);
	VS.setPosition(450, 0);

	p.VS = &VS;
	p2.VS = &VS;

	p2.rect.left = 700;
	p2.rect.top = 150;
	Clock clock;

	Image map_image;
	map_image.loadFromFile("map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);
	
	Text* MenuText[3];

	Text One_player;
	One_player.setFont(font);
	One_player.setString("Один игрок");
	One_player.setCharacterSize(30);
	One_player.setFillColor(Color::Black);
	One_player.setPosition(450, 150);

	Text Two_player;
	Two_player.setFont(font);
	Two_player.setString("Два игрока");
	Two_player.setCharacterSize(30);
	Two_player.setFillColor(Color::Black);
	Two_player.setPosition(450, 200);

	Text ExitGame;
	ExitGame.setFont(font);
	ExitGame.setString("Выход");
	ExitGame.setCharacterSize(30);
	ExitGame.setFillColor(Color::Black);
	ExitGame.setPosition(450, 250);

	float stime=0;

	MenuText[0] = &One_player;
	MenuText[1] = &Two_player;
	MenuText[2] = &ExitGame;

	bool Menu = true;
	int Menu_Ch =0;
	bool solo = false;

	bool key = false;

	while (window.isOpen())
	{
		window.clear(Color::White);
		if (Menu)
		{
			Event event;
			while (window.pollEvent(event))

			{

				if (event.type == Event::Closed)

					window.close();

			}

			if (key == false) 
			{
				if (Keyboard::isKeyPressed(Keyboard::Down))
				{
					Menu_Ch++;
					if (Menu_Ch == 3) Menu_Ch = 0;
					key = true;
				}
				if (Keyboard::isKeyPressed(Keyboard::Up))
				{
					Menu_Ch -= 1;
					if (Menu_Ch < 0) Menu_Ch = 2;
					key = true;
				}
				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					switch (Menu_Ch)
					{
					case 0: Menu = false; solo = true; GamePlay = true;
						p.dx = 0;
						p.dy = 0;

						VS.setString("");
						p.rect.left = 400;
						p.rect.top = 150;

						for (int i = 0; i < 50; i++)
						{
							p.body[i].life = false;
							p.body[i].x = 0;
							p.body[i].y = 0;
						}

						p.count = 0;
						p.score = 0;

						score1.setString(to_string(p.score));

						p.VS = &VS;
						break;
					case 1: Menu = false; solo = false; GamePlay = true;
						p.dx = 0;
						p.dy = 0;
						p2.dx = 0;
						p2.dy = 0;

						VS.setString("VS");
						 p.rect.left = 400;
						 p.rect.top = 150;

						 p2.rect.left = 700;
						 p2.rect.top = 150;

						 for (int i = 0; i < 50; i++)
						 {
							 p.body[i].life = false;
							 p2.body[i].life = false;
							 p.body[i].x = 0;
							 p.body[i].y = 0;
						 }

						 p.count = 0;
						 p.score = 0;

						 p2.count = 0;
						 p2.score = 0;

						score1.setString(to_string(p.score));
						score2.setString(to_string(p2.score));

						p.VS = &VS;
						p2.VS = &VS;

						p2.rect.left = 700;
						p2.rect.top = 150;
					
						break;
						case 2: window.close(); break;
					default:
						break;
					}
				}

				
			}
			else if (event.type == Event::KeyReleased) key = false;

			
			MenuText[0]->setFillColor(Color::Black);
			MenuText[1]->setFillColor(Color::Black);
			MenuText[2]->setFillColor(Color::Black);
			
			MenuText[Menu_Ch]->setFillColor(Color::Red);

			
			
			window.draw(One_player);
			window.draw(Two_player);
			window.draw(ExitGame);
			//Menu = false;
		}
		else if (solo) 
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				GamePlay = false;
				Menu = true;
				solo = false;
			}
			VS.setString("");

			if (!GamePlay)
			{
				p.dx = 0;
				p.dy = 0;
			}
			float time = clock.getElapsedTime().asMicroseconds();
			Event event;
			clock.restart();
			time = time / 1000;

			while (window.pollEvent(event))

			{

				if (event.type == Event::Closed)

					window.close();

			}
			if (GamePlay) {
				if (Keyboard::isKeyPressed(Keyboard::Left))
				{
					if (p.dx <= 0) {
						p.dx = -1 * speedx;
						p.dy = 0;
					}
				}
				else
					if (Keyboard::isKeyPressed(Keyboard::Right))
					{
						if (p.dx >= 0) {
							p.dx = speedx;
							p.dy = 0;
						}
					}
					else
						if (Keyboard::isKeyPressed(Keyboard::Up))
						{
							if (p.dy <= 0) {
								p.dy = -1 * speedy;
								p.dx = 0;
							}
						}
						else
							if (Keyboard::isKeyPressed(Keyboard::Down))
							{
								if (p.dy >= 0) {
									p.dx = 0;
									p.dy = speedy;
								}
							}
				stime += time;

				if (stime > 40) {
					p.MoveBody();
					stime = 0;
				}
			}

			/*if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				solo = false;
				GamePlay = false;
				Menu = true;
			}*/

			for (int i = 0; i < H; i++)
			{
				for (int j = 0; j < W; j++)
				{
					if (MainMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); 
					if (MainMap[i][j] == 'S')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
					if ((MainMap[i][j] == 'A' || MainMap[i][j] == 'B')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


					s_map.setPosition(j * 32, i * 32);

					window.draw(s_map);
				}
			}




			p.update(time);

			spawnA('A');

			window.draw(p.sprite);



			for (int i = 0; i < 49; i++)
			{
				if (p.body[i].life) {
					p.sprite.setPosition(p.body[i].x, p.body[i].y);
					window.draw(p.sprite);
				}
			}


			score1.setString(to_string(p.score));

			window.draw(score1);
		}
		else{
			
		if (!GamePlay)
		{
			p.dx = 0;
			p.dy = 0;
			p2.dx = 0;
			p2.dy = 0;
		}
		float time = clock.getElapsedTime().asMicroseconds();
		Event event;
		clock.restart();
		time = time / 1000;

		while (window.pollEvent(event))

		{

			if (event.type == Event::Closed)

				window.close();

		}
		if (GamePlay) {
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				if (p.dx <= 0) {
					p.dx = -1 * speedx;
					p.dy = 0;
				}
			}
			else
				if (Keyboard::isKeyPressed(Keyboard::Right))
				{
					if (p.dx >= 0) {
						p.dx = speedx;
						p.dy = 0;
					}
				}
				else
					if (Keyboard::isKeyPressed(Keyboard::Up))
					{
						if (p.dy <= 0) {
							p.dy = -1 * speedy;
							p.dx = 0;
						}
					}
					else
						if (Keyboard::isKeyPressed(Keyboard::Down))
						{
							if (p.dy >= 0) {
								p.dx = 0;
								p.dy = speedy;
							}
						}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				if (p2.dx <= 0) {
					p2.dx = -1 * speedx;
					p2.dy = 0;
				}
			}
			else
				if (Keyboard::isKeyPressed(Keyboard::D))
				{
					if (p2.dx >= 0) {
						p2.dx = speedx;
						p2.dy = 0;
					}
				}
				else
					if (Keyboard::isKeyPressed(Keyboard::W))
					{
						if (p2.dy <= 0) {
							p2.dy = -1 * speedy;
							p2.dx = 0;
						}
					}
					else
						if (Keyboard::isKeyPressed(Keyboard::S))
						{
							if (p2.dy >= 0) {
								p2.dx = 0;
								p2.dy = speedy;
							}
						}
			stime += time;

			if (stime > 40) {
				p.MoveBody();
				p2.MoveBody();
				stime = 0;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			GamePlay = false;
			Menu = true;
		}

		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				if (MainMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
				if (MainMap[i][j] == 'S')  s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
				if ((MainMap[i][j] == 'A' || MainMap[i][j] == 'B')) s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик


				s_map.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

				window.draw(s_map);
			}
		}




		p.update(time);
		p2.update(time);

		spawnA('A');
		spawnA('B');

		window.draw(p.sprite);
		window.draw(p2.sprite);



		for (int i = 0; i < 49; i++)
		{
			if (p.body[i].life) {
				p.sprite.setPosition(p.body[i].x, p.body[i].y);
				window.draw(p.sprite);
			}
		}
		for (int i = 0; i < 49; i++)
		{
			if (p2.body[i].life) {
				p2.sprite.setPosition(p2.body[i].x, p2.body[i].y);
				window.draw(p2.sprite);
			}
		}


		nom1.setPosition(p.rect.left + 10, p.rect.top);
		window.draw(nom1);

		nom2.setPosition(p2.rect.left + 10, p2.rect.top);
		window.draw(nom2);

		score1.setString(to_string(p.score));
		score2.setString(to_string(p2.score));

		window.draw(score1);
		window.draw(score2);

		window.draw(VS);

		if (GamePlay) {
			triger(p, p2, VS);
			triger(p2, p, VS);
		}
	}
	
		window.display();

	}

	return 0;

}
