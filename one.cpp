#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include <cmath>
#include <list>

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
////////////////////////////////////����� ����� ��������//////////////////////////
class Entity {
public:
	Figure fig;
	float moveTimer;//�������� ���������� ������ ��� ������� �����
	int health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;//����� ����� ���� ������, �� �� ����� ������ ������ ����� ��� �������������� �����.����� ���� �������� ������ �� ����� � ����� ������� ���� �������� � update � ����������� �� �����
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		fig.x = X; fig.y = Y; fig.w = W; fig.h = H; name = Name; moveTimer = 0;
		fig.speed = 0; health = 100; fig.dx = 0; fig.dy = 0; fig.fi = 0; fig.dfi = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(fig.w / 2, fig.h / 2);
	}
};

class Scene
{
public:
}typedef Scene;
////////////////////////////////////////////////////����� ������////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;//��������� ��� ������������ - ��������� �������
	int playerScore;//��� ���������� ����� ���� ������ � ������

	Player(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay;
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
		}
	}

	void control() {
		if (Keyboard::isKeyPressed) {//���� ������ �������
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//� ������ �����
				state = left; fig.dfi = -0.01;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; fig.dfi = 0.01;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
				fig.speed = 0.1; //�������� ������ ������
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				fig.speed = -0.1;
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
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
		checkCollisionWithMap(fig.dx, 0);//������������ ������������ �� �
		fig.y += fig.dy*time;
		checkCollisionWithMap(0, fig.dy);//������������ ������������ �� Y
		sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); //������ ������� ������� � ����� ��� ������
		if (health <= 0) { life = false; }
		if (!isMove) { fig.speed = 0; }
		//if (!onGround) { dy = dy + 0.0015*time; }//������� � ����� ������ ����������� � �����
		if (life) { setPlayerCoordinateForView(fig.x, fig.y); }
		//dy = dy + 0.0015*time;//��������� ������������� � �����
	}
};


class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			fig.dx = 0.1;//���� ��������.���� ������ ������ ���������
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
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
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {//��� ��������� � ����� ������ ������ ����� �����

								  //moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			checkCollisionWithMap(fig.dx, 0);//������������ ������������ �� �
			fig.x += fig.dx*time;
			sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); //������ ������� ������� � ����� ��� ������
			if (health <= 0) { life = false; }
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(640, 480), "Lesson 21. kychka-pc.ru");
	view.reset(FloatRect(0, 0, 640, 480));


	Image map_image;
	map_image.loadFromFile("images/map.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Image heroImage;
	heroImage.loadFromFile("images/BUs1.jpg");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/Bill.jpg");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));//������� ����� �� �����.�� ����� ���������� ����� ���������� ��������


	Player p(heroImage, 750, 300, 100, 200, "Player1");//������ ������ ������
	Enemy easyEnemy(easyEnemyImage, 850, 471, 150, 150, "EasyEnemy");//������� ����, ������ ������ �����

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
		p.update(time);// Player update function	
		easyEnemy.update(time);//easyEnemy update function
		window.setView(view);
		window.clear();


		for (int i = 0; i < HEIGHT_MAP; i++)
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
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}