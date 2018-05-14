#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include <cmath>
#include <list>
#include <ctime>

using namespace sf;
using namespace std;

struct Figure
{
public:
	int type;
	float x;
	float y;
	float fi;
	float dx;
	float dy;
	float dfi;
	float speed;
	int w;
	int h;
}typedef Figure;
////////////////////////////////////Общий класс родитель//////////////////////////
class Entity {
public:
	Figure fig;
	float moveTimer;//добавили переменную таймер для будущих целей
	int health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	int name;

	virtual void update(float time) = 0;
	Entity(Image &image, float X, float Y, int W, int H, int Name) {
		fig.x = X; fig.y = Y; fig.w = W; fig.h = H; name = Name; moveTimer = 0;
		fig.speed = 0; health = 100; fig.dx = 0; fig.dy = 0; fig.fi = 0; fig.dfi = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(fig.w / 2, fig.h / 2);
		//cout << X << "  " << Y << "  " << (float)W << " " << H << endl;
	}
};


class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;//добавляем тип перечисления - состояние объекта
	int playerScore;//эта переменная может быть только у игрока

	Player(Image &image, float X, float Y, int W, int H, int Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay;
		if (name == 1) {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
		}
	}

	//operator = 

	void control() {
		if (Keyboard::isKeyPressed) {//если нажата клавиша
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//а именно левая
				state = left; fig.dfi = -0.01;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; fig.dfi = 0.01;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//если нажата клавиша вверх и мы на земле, то можем прыгать
				fig.speed = 0.1; //увеличил высоту прыжка
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				fig.speed = -0.1;
			}
		}
	}

	/*void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)//проходимся по элементам карты
			for (int j = fig.x / 32; j<(fig.x + fig.w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
				{
					if (Dy>0) { fig.y = i * 32 - fig.h;  fig.dy = 0; onGround = true; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy<0) { fig.y = i * 32 + 32;  fig.dy = 0; }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx>0) { fig.x = j * 32 - fig.w; }//с правым краем карты
					if (Dx<0) { fig.x = j * 32 + 32; }// с левым краем карты
				}
				//else { onGround = false; }//надо убрать т.к мы можем находиться и на другой поверхности или платформе которую разрушит враг
			}
	}
	*/
	void update(float time)
	{
		control();//функция управления персонажем
				  /*switch (state)//тут делаются различные действия в зависимости от состояния
				  {
				  case right:dx = speed; break;//состояние идти вправо
				  case left:dx = -speed; break;//состояние идти влево
				  case up: break;//будет состояние поднятия наверх (например по лестнице)
				  case down: dx = 0; break;//будет состояние во время спуска персонажа (например по лестнице)
				  case stay: break;//и здесь тоже
				  }*/
		fig.dx = sin(fig.fi) * fig.speed;
		fig.dy = -cos(fig.fi) * fig.speed;
		fig.fi += fig.dfi * time;
		//dfi = 0;
		sprite.setRotation(fig.fi * 180 / 3.1415);//поворачиваем спрайт на эти градусы
		fig.dfi = 0;
		fig.x += fig.dx*time;
		//checkCollisionWithMap(fig.dx, 0);//обрабатываем столкновение по Х
		fig.y += fig.dy*time;
		//checkCollisionWithMap(0, fig.dy);//обрабатываем столкновение по Y
		sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); //задаем позицию спрайта в место его центра
		if (health <= 0) { life = false; }
		if (!isMove) { fig.speed = 0; }
		//if (!onGround) { dy = dy + 0.0015*time; }//убираем и будем всегда притягивать к земле
		if (life) { setPlayerCoordinateForView(fig.x, fig.y); }
		//dy = dy + 0.0015*time;//постоянно притягиваемся к земле
	}
};
class Asteroid :public Entity {
public:
	Asteroid(Image &image, float X, float Y, int D, int Name) :Entity(image, X, Y, D, D, Name) {
		sprite.setTextureRect(IntRect(0, 0, 150, 150)); //<- нужно заменить константы на переменные
		//cout << (float)D / 150 << endl;
		sprite.setScale((float)fig.w / 150 , (float)fig.h / 150);
		
		//fig.dx = 0.1;
	}
	void update(float time) {
		sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h);
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, int Name) :Entity(image, X, Y, W, H, Name) {
		if (name == 2) {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			fig.dx = 0.1;//даем скорость.этот объект всегда двигается
		}
	}

	/*void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)//проходимся по элементам карты
			for (int j = fig.x / 32; j<(fig.x + fig.w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
				{
					if (Dy>0) { fig.y = i * 32 - fig.h; }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy<0) { fig.y = i * 32 + 32; }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx>0) { fig.x = j * 32 - fig.w; fig.dx = -0.1; sprite.scale(-1, 1); }//с правым краем карты
					if (Dx<0) { fig.x = j * 32 + 32; fig.dx = 0.1; sprite.scale(-1, 1); }// с левым краем карты
				}
			}
	}*/

	void update(float time)
	{
		if (name == 2) {//для персонажа с таким именем логика будет такой

								  //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			//checkCollisionWithMap(fig.dx, 0);//обрабатываем столкновение по Х
			fig.x += fig.dx*time;
			sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); //задаем позицию спрайта в место его центра
			if (health <= 0) { life = false; }
		}
	}
};

void setPlayerCoordinateForView(float x, float y) 
{
	float tempX = x; float tempY = y;

	//if (x < 320) tempX = 320;
	//if (y < 240) tempY = 240;//верхнюю сторону
	if (y > 624) tempY = 624;//нижнюю стороню.для новой карты

	view.setCenter(tempX, tempY);
}

class Map_value {
public:
	float ** value;
	int map_width;
	int map_height;
	int block_size;
	void fill() {
		int i, j;
		for (i = 0; i < WIDTH_MAP; ++i)
			for (j = 0; j < HEIGHT_MAP; ++j) {
				value[i][j] += 1;
			}
	}
	void feed(int x, int y) {
		int i, j;
		int tx, ty;
		cout << " old x = " << x << " y = " << y << " " << value[x][y] << endl;
		float tmpm = value[x][y];
		for (i = 0; i <= tmpm; ++i)
			for (j = 1; j + i <= tmpm; ++j)
			{
				tx = ((x - i) > 0 ? x - i : 0);
				ty = ((y - j) > 0 ? y - j : 0);
				if ((x != tx) || (y != ty)) {
					value[x][y] += value[tx][ty];
					//cout << "\tf  x " << tx << " f  y " << ty << " znach " << value[x][y] << endl;
					value[tx][ty] = 0;
				}
				tx = ((x - i) > 0 ? x - i : 0);
				ty = ((y + j) < (map_height - 1) ? y + j : map_height - 1);
				if ((x != tx) || (y != ty)) {
					value[x][y] += value[tx][ty];
					value[tx][ty] = 0;
				}
				tx = ((x + i) < (map_width - 1) ? x + i : map_width - 1);
				ty = ((y - j) > 0 ? y - j : 0);
				if ((x != tx) || (y != ty)) {
					value[x][y] += value[tx][ty];
					value[tx][ty] = 0;
				}
				tx = ((x + i) < (map_width - 1) ? x + i : map_width - 1);
				ty = ((y + j) < (map_height - 1) ? y + j : map_height - 1);
				if ((x != tx) || (y != ty)) {
					value[x][y] += value[tx][ty];
					//cout << "\tse x " << (tx) << " se y " << (ty) << " znach " << value[x][y] << endl;
					value[tx][ty] = 0;
				}

			}
		value[x][y] = tmpm + (value[x][y] - tmpm) / (4 * tmpm);
		//value[x][y] = log2(value[x][y]);
		cout << " x = " << x << " y = " << y << " "<<  value[x][y] << endl;
	}
	Map_value(int width, int height, int Block_size, int crowd, int massiv) {
		map_width = width;
		map_height = height;
		block_size = Block_size;
		int i, j;
		int tmpx, tmpy;
		int oldx, oldy;
		srand(time(0));
		oldx = rand() % (map_width - 1);
		srand(time(0));
		oldy = rand() % (map_height - 1);
		tmpx = oldx;
		tmpy = oldy;
		value = (float **)malloc(map_width * sizeof(float *));
		for (i = 0; i < map_width; ++i)
			value[i] = (float *)malloc(map_height * sizeof(float));
		for (i = 0; i < width; ++i)
			for (j = 0; j < height; ++j) {
				value[i][j] = 0;
			}
		for (i = 0; i < massiv; ++i) {
			fill();
			for (j = 0; j < crowd; ++j){
				while ((oldx == tmpx) && (oldy == tmpy)) {
					srand(time(0));
					tmpx = rand() % (map_width - 1);
					srand(time(0));
					tmpy = rand() % (map_height - 1);
					//cout << "\n repeated \n " << endl;
				}
				cout << "oldx " << oldx << "oldy " << oldy << "tmpx " << tmpx << "tmpy " << tmpy << endl;
				oldx = tmpx;
				oldy = tmpy;
				feed(tmpx, tmpy);
			}
		}
	}

};

class Scene
{
public:
	int map_width;
	int map_height;

	//Player *pplayer;//<-костыль

	list<Entity *> objects;
	void asteroid_field_generate(Image heroImage, int crowd, int massiv) {
		Map_value cur_map(map_width, map_height, 50, crowd, massiv);
		int i, j;
		Image asteroid_image;
		asteroid_image.loadFromFile("images/Bill.jpg");
		asteroid_image.createMaskFromColor(Color(255, 255, 255));
		for (i = 0; i < cur_map.map_width; ++i)
			for (j = 0; j < cur_map.map_height; ++j) {
				if (cur_map.value[i][j] == 0)
					continue;
				Asteroid *aster = new Asteroid(asteroid_image, (float) (i * cur_map.block_size), (float)(j * cur_map.block_size), cur_map.value[i][j]  * cur_map.block_size / 2, 3);
				//cout << aster->fig.x << aster->fig.y << endl;
				//cout << cur_map.value[i][j] * cur_map.block_size << endl;
				objects.push_front(aster);
			}
	}
	void enemy_generate(float X, float Y) {
		Image EnemyImage;
		EnemyImage.loadFromFile("images/Bus1.jpg");
		EnemyImage.createMaskFromColor(Color(0, 0, 0));
		Enemy *enemy = new Enemy(EnemyImage, X, Y, 100, 200, 2);
		objects.push_front(enemy);
	}
	void player_generate(float X, float Y, Image heroImage) {
		//Image heroImage;
		//heroImage.loadFromFile("images/BUs1.jpg");
		Player *player1 = new Player(heroImage, X, Y, 100, 200, 1);
		//objects.push_front(player1);//<- костыль
		
		//pplayer = (Player*)malloc(sizeof(Player));

		
		//cout << sizeof(*applayer) << "\n";
		/*applayer->fig = player1.fig;

		applayer->moveTimer = player1.moveTimer;//добавили переменную таймер для будущих целей
		applayer->health = player1.health;
		//bool life, isMove, onGround;
		applayer->texture = player1.texture;
		applayer->sprite = player1.sprite;
		cout << 0 << "\n";
		applayer->name = player1.name;*/
		//applayer->name = (char *)malloc(100);
		//*pplayer = player1;
		//cout << 1 << "\n";
		//cout << applayer->fig.x << "\n";
		//*applayer = player1;
		//cout << 11 << "\n";
		//cout << sizeof(*applayer) << "\n";
		objects.push_front(player1);
	}
	void colisions(Figure &f1, Figure &f2) {
	}
	void update(float time) {
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); it1++) {
			//pplayer->update(time);
			if ((*it1)->name == 1) {
				(*it1)->update(time);
			} //нужно определить преобразние
			for (list<Entity*>::iterator it2 = it1; it2 != objects.end(); it2++) {
				if (it1 == it2)
					continue;
				colisions((*it1)->fig, (*it2)->fig);
			}
				
		}
	}
	void draw(RenderWindow *window) {
		for (list<Entity*>::iterator it = objects.begin(); it != objects.end(); it++) {
			window->draw((*it)->sprite);
		}
	}
	void RandomGenerate(Image heroImage) {
		asteroid_field_generate(heroImage, 10, 1);

		//Map_value cur_map(map_width, map_height, 100, 0, 1);
		//int i, j;
		//Asteroid *aster1;
		//asteroid_image.loadFromFile("images/Bus1.jpg");//("images/Bill.jpg");
		//asteroid_image.createMaskFromColor(Color(0, 0, 0));
		/*for (i = 2; i < 2; ++i)
			for (j = 1; j < 2; ++j) {
				if (cur_map.value[i][j] == 0)
					continue;
				 aster1 = new Asteroid(heroImage asteroid_image, 100, 100, cur_map.value[i][j]  * cur_map.block_size, 3);
				//cout << aster->fig.x << aster->fig.y << endl;
				//cout << cur_map.value[i][j] * cur_map.block_size << endl;
				objects.push_front(aster1);
			}*/

		enemy_generate(150, 150);
		//Enemy * enemy = new Enemy(heroImage, 150, 150, 100, 200, 2);
		//objects.push_front(enemy);

		//Asteroid * aster = new Asteroid(heroImage, 150, 150, 100, 3);
		//objects.push_front(aster);
		//Enemy * enemy = new Enemy(heroImage, 50, 50, 100, 200, 2);
		//objects.push_front(enemy);
		player_generate(150, 150, heroImage);
		
	}
	Scene(int Map_width, int Map_height) {
		map_width = Map_width;
		map_height = Map_height;
	}
};


int main()
{
	RenderWindow window(VideoMode(640, 480), "gamelike");
	Scene level1(WIDTH_MAP, HEIGHT_MAP);
	
	

	Image heroImage;
	heroImage.loadFromFile("images/BUs1.jpg");
	level1.RandomGenerate(heroImage);
	//level1.player_generate(200, 200, heroImage);

	view.reset(FloatRect(0, 0, 640, 480));
	Clock clock;
	while (window.isOpen())
	{

		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}	
		window.setView(view);
		//level1.update(time);
		window.clear();


		/*for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}
		window.draw(easyEnemy.sprite);
		window.draw(p.sprite);*/
		//level1.draw(&window);//<- здеся видимо создаётся бесконечное кол-во окон
		/*for (list<Entity>::iterator it = level1.objects.begin(); 
			it != level1.objects.end(); 
			it++) {
			window.draw(it->sprite);
		}*/
		//player1.update(time);
		//window.draw(player1.sprite);
		/*
		(*level1.objects.begin())->update(time);
		window.draw((*level1.objects.begin())->sprite);
		*/
		//cout << level1.pplayer->fig.x;

		//(*level1.objects.begin())->update(time);
		//level1.update(time);
		for (list<Entity*>::iterator it = level1.objects.begin();
			it != level1.objects.end();
			it++) {
			(*it)->update(time);
			window.draw((*it)->sprite);
		}


		//player1.update(time);
		//window.draw(player1.sprite);//<- здеся скорее всего всё крашится нафиг
		window.display();
	}
	return 0;
}