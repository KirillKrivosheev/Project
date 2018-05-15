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

	float speedx;
	float speedy;
	float speedfi;

	int w;
	int h;
}typedef Figure;
////////////////////////////////////����� ����� ��������//////////////////////////
class Entity {
public:
	Figure fig;
	float moveTimer;//�������� ���������� ������ ��� ������� �����
	int health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	int name;

	virtual void update(float time) = 0;
	Entity(Image &image, float X, float Y, int W, int H, int Name) {
		fig.x = X; fig.y = Y; fig.w = W; fig.h = H; name = Name; moveTimer = 0;
		fig.speed = 0; health = 100; fig.dx = 0; fig.dy = 0; fig.fi = 0; fig.dfi = 0;

		fig.speedx = 0; fig.speedy = 0; fig.speedfi = 0;

		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(fig.w / 2, fig.h / 2);
		//cout << X << "  " << Y << "  " << (float)W << " " << H << endl;
	}
};


class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������

	Player(Image &image, float X, float Y, int W, int H, int Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay;
		if (name == 1) {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
		}
	}

	//operator = 

	void control() {
		if (Keyboard::isKeyPressed) {//���� ������ �������
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//� ������ �����
				state = left; fig.dfi = -0.01;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; fig.dfi = 0.01;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
				fig.speed = 0.5; //�������� ������ ������
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				fig.speed = -0.5;
			}
		}
	}

	/*void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)//���������� �� ��������� �����
			for (int j = fig.x / 32; j<(fig.x + fig.w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//���� ������� ��� ������ �����? ��
				{
					if (Dy>0) { fig.y = i * 32 - fig.h;  fig.dy = 0; onGround = true; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy<0) { fig.y = i * 32 + 32;  fig.dy = 0; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx>0) { fig.x = j * 32 - fig.w; }//� ������ ����� �����
					if (Dx<0) { fig.x = j * 32 + 32; }// � ����� ����� �����
				}
				//else { onGround = false; }//���� ������ �.� �� ����� ���������� � �� ������ ����������� ��� ��������� ������� �������� ����
			}
	}
	*/
	void update(float time)
	{
		control();//������� ���������� ����������
				  /*switch (state)//��� �������� ��������� �������� � ����������� �� ���������
				  {
				  case right:dx = speed; break;//��������� ���� ������
				  case left:dx = -speed; break;//��������� ���� �����
				  case up: break;//����� ��������� �������� ������ (�������� �� ��������)
				  case down: dx = 0; break;//����� ��������� �� ����� ������ ��������� (�������� �� ��������)
				  case stay: break;//� ����� ����
				  }*/
		fig.dx = sin(fig.fi) * fig.speed;
		fig.dy = -cos(fig.fi) * fig.speed;
		fig.fi += fig.dfi * time;
		//dfi = 0;
		sprite.setRotation(fig.fi * 180 / 3.1415);//������������ ������ �� ��� �������
		fig.dfi = 0;
		fig.x += fig.dx*time;
		//checkCollisionWithMap(fig.dx, 0);//������������ ������������ �� �
		fig.y += fig.dy*time;
		//checkCollisionWithMap(0, fig.dy);//������������ ������������ �� Y
		sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); //������ ������� ������� � ����� ��� ������
		if (health <= 0) { life = false; }
		if (!isMove) { fig.speed = 0; }
		//if (!onGround) { dy = dy + 0.0015*time; }//������� � ����� ������ ����������� � �����
		//dy = dy + 0.0015*time;//��������� ������������� � �����
	}
};
class Asteroid :public Entity {
public:
	bool spawned;
	Asteroid(Image &image, float X, float Y, int D, float gr_rotation,  int Name) :Entity(image, X, Y, D, D, Name) {
		spawned = false;
		sprite.setTextureRect(IntRect(0, 0, 150, 150)); //<- ����� �������� ��������� �� ����������
		sprite.setScale((float)fig.w / 150 , (float)fig.h / 150);
		sprite.setOrigin(150 / 2, 150 / 2);
		fig.fi = gr_rotation / 180 * 3.1415;
		//fig.dx = 0.1;
	}
	void update(float time) {
		/*fig.dx = sin(fig.fi) * fig.speed;
		fig.dy = -cos(fig.fi) * fig.speed;*/
		if (life == false) {
			//cout << "deleted" << endl;
			return;
		}
		if ((spawned) &&
			 ((fig.x - fig.w > WIDTH_MAP) ||
				(fig.x + fig.w < 0) ||
				(fig.y - fig.h > HEIGHT_MAP) ||
				(fig.y + fig.h < 0)
				)
			)
				life = false;
		else {
			if ( !(spawned) && !((fig.x - fig.w > WIDTH_MAP) ||
				(fig.x + fig.w < 0) ||
				(fig.y - fig.h > HEIGHT_MAP) ||
				(fig.y + fig.h < 0)
				)) {
				spawned = true;
				//cout << "spawned" << endl;
			}
		}
		
		fig.dx = fig.speedx; fig.dy = fig.speedy; fig.dfi = fig.speedfi;
		fig.x += fig.dx * time;
		fig.y += fig.dy * time;
		fig.fi += fig.dfi * time;
		fig.dx = 0; fig.dy = 0; fig.dfi = 0;
		sprite.setPosition(fig.x + (float)fig.w / 2, fig.y + (float)fig.h / 2);
		sprite.setRotation(fig.fi * 180 / 3.1415);
		//sprite.setPosition(fig.x, fig.y);
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, int Name) :Entity(image, X, Y, W, H, Name) {
		if (name == 2) {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			fig.dx = 0.1;//���� ��������.���� ������ ������ ���������
		}
	}

	/*void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)//���������� �� ��������� �����
			for (int j = fig.x / 32; j<(fig.x + fig.w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//���� ������� ��� ������ �����, ��
				{
					if (Dy>0) { fig.y = i * 32 - fig.h; }//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy<0) { fig.y = i * 32 + 32; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx>0) { fig.x = j * 32 - fig.w; fig.dx = -0.1; sprite.scale(-1, 1); }//� ������ ����� �����
					if (Dx<0) { fig.x = j * 32 + 32; fig.dx = 0.1; sprite.scale(-1, 1); }// � ����� ����� �����
				}
			}
	}*/

	void update(float time)
	{
		if (name == 2) {//��� ��������� � ����� ������ ������ ����� �����

								  //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			//checkCollisionWithMap(fig.dx, 0);//������������ ������������ �� �
			fig.x += fig.dx*time;
			sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
	}
};
class Map_value {
public:
	float ** value;
	int map_width;
	int map_height;
	int block_size;
	void fill() {
		int i, j;
		for (i = 0; i < map_width; ++i)
			for (j = 0; j < map_height; ++j) {
				if(value[i][j] == 0)
					value[i][j] += 1;
			}
	}
	void feed(int x, int y) {
		int i, j;
		int tx, ty;
	//	cout << " x = " << x << " y = " << y << " " << value[x][y];
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
		//cout << " x = " << x << " y = " << y << " "<<  value[x][y] << endl;
		//cout << value[x][y] << endl;
	}
	void distruct() {
		int i;
		for (i = 0; i < map_width; ++i)
			free(value[i]);
		free(value);
	}
	Map_value(int width, int height, int Block_size, int crowd, int massiv) {
		map_width = width;
		map_height = height;
		block_size = Block_size;
		int i, j;
		int tmpx, tmpy;
		int oldx, oldy;
		Clock clock;
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
		//feed(3, 3);
		//feed(3, 3);
		for (i = 0; i < massiv; ++i) {
			fill();
			for (j = 0; j < crowd; ++j){
				while ((oldx == tmpx) && (oldy == tmpy)) {
					srand(time(0) + clock.getElapsedTime().asMicroseconds());
					tmpx = rand() % (map_width - 1);
					srand(time(0) + clock.getElapsedTime().asMicroseconds());
					tmpy = rand() % (map_height - 1);
					//cout << "\n repeated \n " << endl;
				}
				//cout << "oldx " << oldx << "oldy " << oldy << "tmpx " << tmpx << "tmpy " << tmpy << endl;
				oldx = tmpx;
				oldy = tmpy;
				if(value[tmpx][tmpy] != 0)
					feed(tmpx, tmpy);
			}
		}
	}

};

class Scene
{
public:
	float passtime;
	int map_width;
	int map_height;
	float vew_size_x;
	float vew_size_y;
	Texture map_texture;
	Sprite map_sprite;
	float asteroid_speedx;
	list<Entity *> objects;
	void asteroid_field_generate(float X, float Y, int crowd, int massiv, int block_size, int rad_size) {
		Map_value cur_map(map_width / block_size, map_height / block_size, block_size, crowd, massiv);
		int i, j;
		int tmpx, tmpy, tmpfi;
		Clock clock;
		Image asteroid_image;
		asteroid_image.loadFromFile("images/Bill.jpg");
		asteroid_image.createMaskFromColor(Color(255, 255, 255));
		for (i = 0; i < cur_map.map_width; ++i)
			for (j = 0; j < cur_map.map_height; ++j) {
				if (cur_map.value[i][j] == 0)
					continue;
				srand(time(0) + clock.getElapsedTime().asMicroseconds());
				tmpx = (float)(X + i * cur_map.block_size) + rand() % (block_size / 2);
				srand(time(0) + clock.getElapsedTime().asMicroseconds());
				tmpy = (float)(Y + j * cur_map.block_size) + rand() % (block_size / 2);
				srand(time(0) + clock.getElapsedTime().asMicroseconds());
				tmpfi = rand() % 360;
				Asteroid *aster = new Asteroid(asteroid_image, tmpx, tmpy, cur_map.value[i][j]  * rad_size, tmpfi, 3);
				aster->fig.speedx = asteroid_speedx;
				//aster->fig.speedy = 0.01;
				//aster->fig.speedfi = 0.01;
				//cout << aster->fig.x << aster->fig.y << endl;
				//cout << cur_map.value[i][j] * cur_map.block_size << endl;
				objects.push_front(aster);
			}
		cur_map.distruct();
	}
	void enemy_generate(float X, float Y) {
		Image EnemyImage;
		EnemyImage.loadFromFile("images/Bus1.jpg");
		EnemyImage.createMaskFromColor(Color(0, 0, 0));
		Enemy *enemy = new Enemy(EnemyImage, X, Y, 100, 200, 2);
		objects.push_front(enemy);
	}
	void player_generate(float X, float Y, Image heroImage) {
		Player *player1 = new Player(heroImage, X, Y, 100, 200, 1);
		objects.push_front(player1);
	}
	void colisions(Figure &f1, Figure &f2) {
	}
	void set_vew(float x, float y) {
		float old_vew_size_x = vew_size_x;
		float old_vew_size_y = vew_size_y;
		if (Keyboard::isKeyPressed(Keyboard::U)) {
			vew_size_x = vew_size_x * 1.01;
			vew_size_y = vew_size_y * 1.01;
		}
		if (Keyboard::isKeyPressed(Keyboard::H)) {
			vew_size_x = vew_size_x * 0.99;
			vew_size_y = vew_size_y * 0.99;
		}
		if (Keyboard::isKeyPressed(Keyboard::I)) {
			vew_size_x = 640 * 2;
			vew_size_y = 480 * 2;
		}
		if ((vew_size_x > max(map_width, 640 * 2)) || (vew_size_y > max(map_height, 480 * 2))){
			vew_size_x = old_vew_size_x;
			vew_size_y = old_vew_size_y;
		}
		if ((vew_size_x < 160) || (vew_size_y < 120)) {
			vew_size_x = old_vew_size_x;
			vew_size_y = old_vew_size_y;
		}
		view.setSize(vew_size_x, vew_size_y);
		float tmpx = x; float tmpy = y;
		if (x < vew_size_x / 2)
			tmpx = vew_size_x / 2;
		if (x > map_width - vew_size_x / 2)
			tmpx = map_width - vew_size_x / 2;
		if (y > map_height - vew_size_y / 2)
			tmpy = map_height - vew_size_y / 2;
		if (y < vew_size_y / 2)
			tmpy = vew_size_y / 2;
		view.setCenter(tmpx, tmpy);

	}
	void update(float time) {
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); it1++) {
			if ((*it1)->name == 1) {
				(*it1)->update(time);
				set_vew((*it1)->fig.x + (*it1)->fig.w / 2, (*it1)->fig.y + (*it1)->fig.h / 2);
			}
		}
		//list<Entity*>::iterator oldit1 = objects.begin();
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); ) {
			if ((*it1)->name != 1) {
				(*it1)->update(time);
			} 
			if (!((*it1)->life)) {
				it1 = objects.erase(it1);
				continue;
			}
			for (list<Entity*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++) {
				if (it1 == it2)
					continue;
				colisions((*it1)->fig, (*it2)->fig);
			}
			it1++;
		}
	}
	void draw(RenderWindow &window){
		window.draw(map_sprite);
		for (list<Entity*>::iterator it = objects.begin(); it != objects.end(); it++) {
			window.draw((*it)->sprite);
		}
	}
	void Refresh(float time) {
		passtime += time;
		if (map_width < asteroid_speedx * passtime) {
			//cout << "refresh" << endl;
			asteroid_field_generate(-map_width, 0, 100, 5, BLOCK_SIZE, RAD_SIZE);
			passtime = 0;
		}
	}
	void RandomGenerate(Image heroImage) {
		asteroid_field_generate(0, 0, 100, 5, BLOCK_SIZE, RAD_SIZE);
		asteroid_field_generate( -map_width, 0, 100, 5, BLOCK_SIZE, RAD_SIZE);
		enemy_generate(150, 150);
		player_generate(150, 150, heroImage);
	}
	void distruct() {
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); it1++) {
			delete *it1;
		}
	}
	Scene(int Map_width, int Map_height) {
		map_width = Map_width;
		map_height = Map_height;
		passtime = 0;
		vew_size_x = 640;
		vew_size_y = 480;
		asteroid_speedx = 0.3;//0.01;
		Image map_image;
		map_image.loadFromFile("images/legends_space.png");
		map_texture.loadFromImage(map_image);
		map_sprite.setTexture(map_texture);
		map_sprite.setTextureRect(IntRect(0, 0, 4096, 2304));
		float mapx = map_texture.getSize().x; //4096 ;
		float mapy = map_texture.getSize().y; //2304;
		map_sprite.setScale((float)map_width / mapx, (float)map_height / mapy);
		map_sprite.setOrigin(mapx / 2, mapy / 2);
		map_sprite.setPosition(map_width / 2, map_height / 2);
	}
};


int main()
{
	RenderWindow window(VideoMode(640, 480), "gamelike");
	Scene level1(WIDTH_MAP, HEIGHT_MAP);
	Image heroImage;
	heroImage.loadFromFile("images/BUs1.jpg");
	level1.RandomGenerate(heroImage);
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
		window.clear();
		/*for (list<Entity*>::iterator it = level1.objects.begin();
			it != level1.objects.end();
			it++) {
			(*it)->update(time);
			window.draw((*it)->sprite);
		}*/
		level1.Refresh(time);
		level1.update(time);
		level1.draw(window);
		window.display();
	}
	level1.distruct();
//	int k;
//	cin >> k;
	return 0;
}