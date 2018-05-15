/*#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include <cmath>
#include <list>
#include <vector>

using namespace sf;
using namespace std;

struct Vector
{
	float x;
	float y;
}typedef Vector;

float Scalar_product(Vector a, Vector b)
{
	return (a.x)*b.x + (a.y)*b.y;
}

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

	/////////////////////////new//////////////////////////////////////
	Vector velocity;
	Vector acceleration;
	///////////////////////////////////////////////////////////////////////

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
	String name;//враги могут быть разные, мы не будем делать другой класс для различающегося врага.всего лишь различим врагов по имени и дадим каждому свое действие в update в зависимости от имени
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		fig.x = X; fig.y = Y; fig.w = W; fig.h = H; name = Name; moveTimer = 0;
		fig.speed = 0; health = 100; fig.dx = 0; fig.dy = 0; fig.fi = 0; fig.dfi = 0; 
		
		
		/////////////////new//////////////////////////////////////
		fig.velocity.x = 0;
		fig.velocity.y = 0;
		fig.acceleration.x = 0;
		fig.acceleration.y = 0;
		//////////////////////////////////////////////////////////////////


		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(fig.w / 2, fig.h / 2);
	}
};


////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player :public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;

	Player(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay;
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
		}
	}

	void control() {
		if (Keyboard::isKeyPressed) {//если нажата клавиша
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//а именно левая
				state = left; fig.dfi = -0.005;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; fig.dfi = 0.005;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {
				fig.speed = 0.3; 
			}

			//if (Keyboard::isKeyPressed(Keyboard::Down)) {
				//fig.speed = -0.1;
			//}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)//проходимся по элементам карты
			for (int j = fig.x / 32; j < (fig.x + fig.w) / 32; j++)
			{
				if (TileMap[i][j] == '0' || TileMap[i][j] == '1')//если элемент наш тайлик земли? то
				{
					if (Dy > 0) { fig.y = i * 32 - fig.h;  }//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) { fig.y = i * 32 + 32;  }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) { fig.x = j * 32 - fig.w;  }//с правым краем карты
					if (Dx < 0) { fig.x = j * 32 + 32;  }// с левым краем карты
				}
			}
	}

	CircleShape checkCollisionWithEnemy(Figure Enemy)
	{
		CircleShape shape2(5, 100);
		shape2.setFillColor(sf::Color::Green);
		Vector e1;
		e1.x = sin(fig.fi);
		e1.y = -cos(fig.fi);
		Vector e2;
		e2.x = -cos(fig.fi);
		e2.y = -sin(fig.fi);
		Vector r;
		r.x = Enemy.x + 0.5*Enemy.w - fig.x - 0.5*fig.w;
		r.y = Enemy.y + 0.5*Enemy.h - fig.y - 0.5*fig.h;

		///////////////////////////////////////////////////
		if (sqrt((r.x)*(r.x) + (r.y)*(r.y)) > sqrt((0.5*fig.h)*(0.5*fig.h) + (0.5*fig.w)*(0.5*fig.w)) + 0.5*Enemy.h)
		{
			shape2.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
			shape2.setRadius(0);
			return shape2;
		}
        ////////////////////////////////////////////////////////////////
		Vector closest;
		closest.x = 0;
		closest.y = 0;
		Vector projection;
		Vector normal;
		float penetration;
		if (Scalar_product(e1, r) >= 0.5*fig.h && Scalar_product(e2, r) > 0.5*fig.w || Scalar_product(e1, r) > 0.5*fig.h && Scalar_product(e2, r) >= 0.5*fig.w)
		{
			closest.x = fig.x + 0.5*(fig.h)*e1.x + 0.5*(fig.w)*e2.x + 0.5*fig.w;
			closest.y = fig.y + 0.5*(fig.h)*e1.y + 0.5*(fig.w)*e2.y + 0.5*fig.h;
		}

		if ((Scalar_product(e1, r) >= 0.5*fig.h && Scalar_product(e2, r) < -0.5*fig.w) || (Scalar_product(e1, r) > 0.5*fig.h && Scalar_product(e2, r) <= -0.5*fig.w))
		{
			closest.x = fig.x + 0.5*(fig.h)*e1.x - 0.5*(fig.w)*e2.x + 0.5*fig.w;
			closest.y = fig.y + 0.5*(fig.h)*e1.y - 0.5*(fig.w)*e2.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) <= -0.5*fig.h && Scalar_product(e2, r) > 0.5*fig.w || Scalar_product(e1, r) < -0.5*fig.h && Scalar_product(e2, r) >= 0.5*fig.w)
		{
			closest.x = fig.x - 0.5*(fig.h)*e1.x + 0.5*(fig.w)*e2.x + 0.5*fig.w;
			closest.y = fig.y - 0.5*(fig.h)*e1.y + 0.5*(fig.w)*e2.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) <= -0.5*fig.h && Scalar_product(e2, r) < -0.5*fig.w || Scalar_product(e1, r) < -0.5*fig.h && Scalar_product(e2, r) <= -0.5*fig.w)
		{
			closest.x = fig.x - 0.5*(fig.h)*e1.x - 0.5*(fig.w)*e2.x + 0.5*fig.w;
			closest.y = fig.y - 0.5*(fig.h)*e1.y - 0.5*(fig.w)*e2.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) > 0.5*fig.h && Scalar_product(e2, r) < 0.5*fig.w && Scalar_product(e2, r) > -0.5*fig.w)
		{
			projection.x = (e2.x)*Scalar_product(e2, r);
			projection.y = (e2.y)*Scalar_product(e2, r);
			closest.x = fig.x + 0.5*(fig.h)*e1.x + projection.x + 0.5*fig.w;
			closest.y = fig.y + 0.5*(fig.h)*e1.y + projection.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) < -0.5*fig.h && Scalar_product(e2, r) < 0.5*fig.w && Scalar_product(e2, r) > -0.5*fig.w)
		{
			projection.x = (e2.x)*Scalar_product(e2, r);
			projection.y = (e2.y)*Scalar_product(e2, r);
			closest.x = fig.x - 0.5*(fig.h)*e1.x + projection.x + 0.5*fig.w;
			closest.y = fig.y - 0.5*(fig.h)*e1.y + projection.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) < 0.5*fig.h && Scalar_product(e1, r) > -0.5*fig.h && Scalar_product(e2, r) > 0.5*fig.w)
		{
			projection.x = (e1.x)*Scalar_product(e1, r);
			projection.y = (e1.y)*Scalar_product(e1, r);
			closest.x = fig.x + 0.5*(fig.w)*e2.x + projection.x + 0.5*fig.w;
			closest.y = fig.y + 0.5*(fig.w)*e2.y + projection.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) < 0.5*fig.h && Scalar_product(e1, r) > -0.5*fig.h && Scalar_product(e2, r) < - 0.5*fig.w)
		{
			projection.x = (e1.x)*Scalar_product(e1, r);
			projection.y = (e1.y)*Scalar_product(e1, r);
			closest.x = fig.x - 0.5*(fig.w)*e2.x + projection.x + 0.5*fig.w;
			closest.y = fig.y - 0.5*(fig.w)*e2.y + projection.y + 0.5*fig.h;
		}

		if (Scalar_product(e1, r) <= 0.5*fig.h && Scalar_product(e1, r) >= -0.5*fig.h && Scalar_product(e2, r) <= 0.5*fig.w && Scalar_product(e2, r) >= -0.5*fig.w)
		{
			closest.x = Enemy.x + 0.5*Enemy.w;
			closest.y = Enemy.y + 0.5*Enemy.h;
		}

		shape2.setPosition(closest.x, closest.y);

		if ((0.5*Enemy.h)*(0.5*Enemy.h) < (Enemy.x + 0.5*Enemy.w - closest.x)*(Enemy.x + 0.5*Enemy.w - closest.x) + (Enemy.y + 0.5*Enemy.h - closest.y)*(Enemy.y + 0.5*Enemy.h - closest.y))
		{
			return shape2;
		}

		normal.x = Enemy.x + 0.5*Enemy.w - closest.x;
		normal.y = Enemy.y + 0.5*Enemy.h - closest.y;//к центру круга
		penetration = (0.5*Enemy.h) - sqrt((Enemy.x + 0.5*Enemy.w - closest.x)*(Enemy.x + 0.5*Enemy.w - closest.x) + (Enemy.y + 0.5*Enemy.h - closest.y)*(Enemy.y + 0.5*Enemy.h - closest.y));

		if (Scalar_product(e1, r) <= 0.5*fig.h && Scalar_product(e1, r) >= -0.5*fig.h && Scalar_product(e2, r) <= 0.5*fig.w && Scalar_product(e2, r) >= -0.5*fig.w)
		{
			normal.x = r.x;
			normal.y = r.y;
		}
		//cout << penetration << endl;

		float normal_length;
		normal_length = (normal.x)*(normal.x) + (normal.y)*(normal.y);
		if (normal_length != 0)
		{
			normal.x = (normal.x) / sqrt(normal_length);
			normal.y = (normal.y) / sqrt(normal_length);
		}
	
		Vector relative_speed;
		relative_speed.x = (Enemy.speed)*sin(Enemy.fi) - (fig.speed)*sin(fig.fi);
		relative_speed.y = -(Enemy.speed)*cos(Enemy.fi) + (fig.speed)*cos(fig.fi);
	    if (Scalar_product(relative_speed, normal) > 0)
		{
			//cout << "I am here";
			return shape2;
		}
		float power = sqrt(penetration)*penetration*0.01*Scalar_product(relative_speed, normal);
		fig.acceleration.x = power*normal.x;
		fig.acceleration.y = power*normal.y;
		if (fig.acceleration.x > 0.25)
		{ 
			fig.acceleration.x = 0.25;
		}
		if (fig.acceleration.x < -0.25)
		{
			fig.acceleration.x = -0.25;
		}
		if (fig.acceleration.y < -0.25)
		{
			fig.acceleration.y = -0.25;
		}
		return shape2;
	}

	CircleShape update(float time, Figure enemy)
	{
		CircleShape shape3;
		control();
				  
		//////////////////////////old//////////////////////////////////////////
		//fig.dx = sin(fig.fi) * fig.speed;//fig speed_x // fig.speed_y
		//fig.dy = -cos(fig.fi) * fig.speed;
		//////////////////////////////////////////////////////////////////////

		//////////////////////////new///////////////////////////////////////
		fig.velocity.x = sin(fig.fi) * fig.speed;
		fig.velocity.y = -cos(fig.fi) * fig.speed;
		shape3 = checkCollisionWithEnemy(enemy);
		fig.velocity.x += fig.acceleration.x * time;
		fig.velocity.y += fig.acceleration.y * time;
		fig.acceleration.x = 0;
		fig.acceleration.y = 0;
		if (fig.velocity.y < 0)
		{
			fig.fi = atan(-fig.velocity.x / (fig.velocity.y));
		}
		if (fig.velocity.y > 0)
		{
			fig.fi = acos(-1) + atan(-fig.velocity.x / (fig.velocity.y));
		}
		fig.dx = (fig.velocity.x);
		fig.dy = (fig.velocity.y);
		fig.fi += fig.dfi * time;
		sprite.setRotation(fig.fi * 180 / 3.1415);//поворачиваем спрайт на эти градусы
		fig.dfi = 0;
		fig.x += fig.dx*time;
		checkCollisionWithMap(fig.dx, 0);//обрабатываем столкновение по 
		fig.y += fig.dy*time;
		checkCollisionWithMap(0, fig.dy);//обрабатываем столкновение по Y
		sprite.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h); //задаем позицию спрайта в место его центра
		if (health <= 0) { life = false; }
		if (!isMove) { fig.speed = 0.98*fig.speed; }
		if (life) { setPlayerCoordinateForView(fig.x, fig.y); }
		return shape3;
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			fig.fi = acos(0);
			fig.speed = 0.2;
			fig.dfi = 0;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)
			for (int j = fig.x / 32; j < (fig.x + fig.w) / 32; j++)
			{
				if (TileMap[i][j] == '0')
				{
					if (Dy < 0) { fig.y = i * 32 + 32; fig.dy = -Dy; }
					if (Dy > 0) { fig.y = i * 32 - fig.h; fig.dy = -Dy; }
				}
				if (TileMap[i][j] == '1')
				{
					if (Dx > 0) { fig.x = j * 32 - fig.w; fig.dx = -Dx;  }
					if (Dx < 0) { fig.x = j * 32 + 32; fig.dx = -Dx; }
				}
			}
	}

	CircleShape checkCollisionWithFriend(Figure Friend)
	{
		CircleShape shape2(5, 100);
		shape2.setFillColor(sf::Color::Cyan);
		shape2.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
		shape2.setRadius(0);

		Vector r;
		r.x = Friend.x + 0.5*Friend.w - fig.x - 0.5*fig.w;
		r.y = Friend.y + 0.5*Friend.h - fig.y - 0.5*fig.h;

		///////////////////////////////////////////////////
		if (sqrt((r.x)*(r.x) + (r.y)*(r.y)) > 0.5*fig.h + 0.5*Friend.h)
		{
			return shape2;
		}

		Vector normal;
		float penetration;
		penetration = -sqrt((r.x)*(r.x) + (r.y)*(r.y)) + 0.5*fig.h + 0.5*Friend.h;

		normal.x = r.x;
		normal.y = r.y;
		cout << penetration << endl;

		float normal_length;
		normal_length = (normal.x)*(normal.x) + (normal.y)*(normal.y);
		if (normal_length != 0)
		{
			normal.x = (normal.x) / sqrt(normal_length);
			normal.y = (normal.y) / sqrt(normal_length);
		}

		Vector relative_speed;
		relative_speed.x = (Friend.speed)*sin(Friend.fi) - (fig.speed)*sin(fig.fi);
		relative_speed.y = -(Friend.speed)*cos(Friend.fi) + (fig.speed)*cos(fig.fi);

		if (Scalar_product(relative_speed, normal) > 0)
		{
			return shape2;
		}

		float power = sqrt(penetration)*penetration*0.01*Scalar_product(relative_speed, normal);
		fig.acceleration.x = power*normal.x;
		fig.acceleration.y = power*normal.y;

		return shape2;
	}

	void update(float time, Figure Friend)
	{
		if (name == "EasyEnemy") {
        	checkCollisionWithFriend(Friend);
			fig.velocity.x = sin(fig.fi) * fig.speed;
			fig.velocity.y = -cos(fig.fi) * fig.speed;
			fig.velocity.x += fig.acceleration.x * time;
			fig.velocity.y += fig.acceleration.y * time;
			fig.acceleration.x = 0;
			fig.acceleration.y = 0;
			//if (fig.velocity.y < 0)
			//{
				//fig.fi = atan(-fig.velocity.x / (fig.velocity.y));
			//}
			//if (fig.velocity.y > 0)
			//{
				//fig.fi = acos(-1) + atan(-fig.velocity.x / (fig.velocity.y));
			//}
			fig.dx = (fig.velocity.x);
			fig.dy = (fig.velocity.y);
			checkCollisionWithMap(fig.dx, fig.dy);
			fig.velocity.x = fig.dx;
			fig.velocity.y = fig.dy;
			if (fig.velocity.y < 0)
			{
			    fig.fi = atan(-fig.velocity.x / (fig.velocity.y));
			}
			if (fig.velocity.y > 0)
			{
			    fig.fi = acos(-1) + atan(-fig.velocity.x / (fig.velocity.y));
			}
			if (fig.velocity.y == 0 && fig.velocity.x > 0)
			{
				fig.fi = acos(0);
			}
			if (fig.velocity.y == 0 && fig.velocity.x < 0)
			{
				fig.fi = -acos(0);
			}
			sprite.setRotation(fig.fi * 180 / 3.1415);//поворачиваем спрайт на эти градусы
			fig.x += fig.dx*time;
			fig.y += fig.dy*time;
			sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2); 
			if (health <= 0) { life = false; }
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(640, 480), "Space");
	view.reset(FloatRect(0, 0, 640, 480));


	Image map_image;
	map_image.loadFromFile("images/background.jpg");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);

	Image heroImage;
	heroImage.loadFromFile("images/Bus1.jpg");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/Bill.jpg");
	easyEnemyImage.createMaskFromColor(Color(100, 100, 255));//сделали маску по цвету.но лучше изначально иметь прозрачную картинку


	Image easyEnemyImage2;
	easyEnemyImage2.loadFromFile("images/Bill.jpg");
	easyEnemyImage2.createMaskFromColor(Color(100, 100, 255));

	CircleShape shape(5, 100);
	shape.setFillColor(sf::Color::Red);
	CircleShape shape1(5, 100);
	shape1.setFillColor(sf::Color::Blue);
	CircleShape shape4;

	Player p(heroImage, 750, 300, 100, 200, "Player1");//объект класса игрока
	Enemy easyEnemy(easyEnemyImage, 850, 391, 150, 150, "EasyEnemy");//простой враг, объект класса врага
	Enemy easyEnemy2(easyEnemyImage, 300, 387, 150, 150, "EasyEnemy");
	
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
		shape4 = p.update(time, easyEnemy.fig);// Player update function	
		easyEnemy.update(time, easyEnemy2.fig);//easyEnemy update function
		easyEnemy2.update(time, easyEnemy.fig);
		shape.setPosition(p.fig.x + 0.5*p.fig.w, p.fig.y + 0.5*p.fig.h);
		shape1.setPosition(easyEnemy.fig.x + 0.5*easyEnemy.fig.w, easyEnemy.fig.y + 0.5*easyEnemy.fig.h);
		window.setView(view);
		window.clear();

		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
				if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
				if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == '1')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
				if ((TileMap[i][j] == 'f')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
				if ((TileMap[i][j] == 'h')) s_map.setTextureRect(IntRect(128, 0, 32, 32));
				s_map.setPosition(j * 32, i * 32);

				window.draw(s_map);
			}
		window.draw(easyEnemy.sprite);
		window.draw(easyEnemy2.sprite);
		window.draw(p.sprite);
		window.draw(shape);
		window.draw(shape1);
		window.draw(shape4);
		window.display();
	}
	return 0;
}*/