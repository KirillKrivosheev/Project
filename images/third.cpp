﻿#include <SFML/Graphics.hpp>
#include "map.h"
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include <cmath>
#include <list>
#include <ctime>
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
	Vector velocity;
	Vector acceleration;
	int w;
	int h;
}typedef Figure;

bool checkIntersection(Figure fig, Figure Enemy)
{
	//CircleShape shape2(5, 100);
	//shape2.setFillColor(sf::Color::Green);
	Vector e1;
	e1.x = sin(fig.fi);
	e1.y = -cos(fig.fi);
	Vector e2;
	e2.x = -cos(fig.fi);
	e2.y = -sin(fig.fi);
	Vector r;
	r.x = Enemy.x + 0.5*Enemy.w - fig.x - 0.5*fig.w;
	r.y = Enemy.y + 0.5*Enemy.h - fig.y - 0.5*fig.h;

	//cout << Enemy.h << endl << Enemy.y << endl;
	///////////////////////////////////////////////////
	if (sqrt((r.x)*(r.x) + (r.y)*(r.y)) > sqrt((0.5*fig.h)*(0.5*fig.h) + (0.5*fig.w)*(0.5*fig.w)) + 0.5*Enemy.h)
	{
		//shape2.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
		//shape2.setRadius(0);
		//return shape2;
		return false;
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

	if (Scalar_product(e1, r) < 0.5*fig.h && Scalar_product(e1, r) > -0.5*fig.h && Scalar_product(e2, r) < -0.5*fig.w)
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

	//shape2.setPosition(closest.x, closest.y);

	if ((0.5*Enemy.h)*(0.5*Enemy.h) < (Enemy.x + 0.5*Enemy.w - closest.x)*(Enemy.x + 0.5*Enemy.w - closest.x) + (Enemy.y + 0.5*Enemy.h - closest.y)*(Enemy.y + 0.5*Enemy.h - closest.y))
	{
		//return shape2;
		return false;
	}

	return true;
}

bool checkCollisionWithFriendForBullet(Figure fig, Figure Friend)
{
	Vector r;
	r.x = Friend.x + 0.5*Friend.w - fig.x - 0.5*fig.w;
	r.y = Friend.y + 0.5*Friend.h - fig.y - 0.5*fig.h;

	///////////////////////////////////////////////////
	if (sqrt((r.x)*(r.x) + (r.y)*(r.y)) > 0.5*30*fig.h/7 + 0.5*Friend.h)
	{
		return false;
	}

	return true;
}

bool checkCollisionWithFriend(Figure fig, Figure Friend)
{
	Vector r;
	r.x = Friend.x + 0.5*Friend.w - fig.x - 0.5*fig.w;
	r.y = Friend.y + 0.5*Friend.h - fig.y - 0.5*fig.h;

	///////////////////////////////////////////////////
	if (sqrt((r.x)*(r.x) + (r.y)*(r.y)) > 0.5 * fig.h + 0.5*Friend.h)
	{
		return false;
	}

	return true;
}

class Entity {
public:
	Figure fig;
	float moveTimer;
	int health;
	bool life, isMove;
	Texture texture;
	Sprite sprite;
	int name;
	virtual void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) = 0;
	virtual void draw(RenderWindow &window) {
		window.draw(sprite);
	}
	Entity(Image &image, float X, float Y, int W, int H, int Name) {
		fig.x = X; fig.y = Y; fig.w = W; fig.h = H; name = Name; moveTimer = 0;
		fig.speed = 0; health = 100; fig.dx = 0; fig.dy = 0; fig.fi = 0; fig.dfi = 0;
		fig.velocity.x = 0; fig.velocity.y = 0; fig.acceleration.x = 0; fig.acceleration.y = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(fig.w / 2, fig.h / 2);
		//cout << X << "  " << Y << "  " << (float)W << " " << H << endl;
	}
};

class Drop :public Entity {
public:
	//float minrad;
	//float alfa;
	float rotate;
	bool isInTag;
	Drop(Image &image, float X, float Y) : Entity(image, X, Y, 150, 150, 6) {
		//minrad = 16000;
		//	alfa = 0.1;
		rotate = 0.05;
		isInTag = false;
	}
	/*void collect(Player * target) {
		Figure tfig = target->fig;
		///////////////////////////////////////////////////////////////////////////
		float distx = tfig.x - fig.x - fig.w / 2 * sin(fig.fi);
		float disty = tfig.y - fig.y + fig.h / 2 * cos(fig.fi);
		float dist = distx * distx + disty * disty;
		if (dist < minrad) {
		////////////////////////////////////////////////////////////////////////////
		if (checkIntersection(tfig, fig)) {
			if (0) {
				life = false;
				target->playerScore += 10;
				cout << target->playerScore << endl;
			}
			if (isInTag == false)
			{

				//isInTag = true;//here I should add this thing to the list <<Tag>> in class Player
				//////////////////////////////////////////////////(target->Tag).push_back();

				//fig.dx += alfa * (tfig.x - fig.x);// + (1 - alfa) * fig.x;
				//fig.dy += alfa * (tfig.y - fig.y);// +(1 - alfa) * fig.y;
				//	cout << "I am here" << endl;
				rotate = 0;
				fig.fi = tfig.fi;
				/////////////////////////////////////////////////////////////////////////
				fig.x = tfig.x + fig.w / 2 * sin(fig.fi);// fig.dx;
				fig.y = tfig.y - fig.h / 2 * cos(fig.fi);//fig.dy;
				////////////////////////////////////////////////////////////////////////
				fig.x = -(0.5)*(1 + sin(fig.fi))*fig.w - 0.5*(tfig.h)*(sin(tfig.fi)) + tfig.x + 0.5*tfig.w;
				fig.y = -(0.5)*(1 - cos(fig.fi))*fig.h + 0.5*(tfig.h)*(1 + cos(tfig.fi)) + tfig.y;
			}
			//////////////////////////////////////////////////////////////////////////////
		}
	}*/
	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) {
		if (isInTag == false) {
			fig.x += 5;
		    fig.dfi += rotate;
		    fig.fi += fig.dfi;
		    fig.dfi = 0;
			if (fig.x > WIDTH_MAP + fig.w + 100) {
				life = false;
				//cout << "outofrange" << endl;
			}
		}
		//else
		//{

		//}
		//else
				//cout << fig.fi << endl;
		sprite.setRotation(180*fig.fi/3.1415);
		sprite.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
	}
};

class Bullet :public Entity {
public:
	Bullet(Image &image, float X, float Y, float speed, float fi, int Name) : Entity(image, X, Y, 7, 7, Name) {
		fig.speed = speed;
		fig.fi = fi;
		sprite.setTextureRect(IntRect(0, 0, 7, 7));
	    sprite.setScale(30 / 7, 30 / 7);
	}
	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) {
		if (fig.x + 0.5*fig.w> 0)
		{
			fig.x += fig.speed * sin(fig.fi)* time;
			fig.y += -fig.speed * cos(fig.fi) *time;
			sprite.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
			if ((fig.x - fig.w > WIDTH_MAP) ||
				(fig.x + fig.w < 0) ||
				(fig.y - fig.h > HEIGHT_MAP) ||
				(fig.y + fig.h < 0)
				)
				life = false;
		}
	}
};

class Item :public Entity {
public:
	Figure * holder;
	float subx;
	float suby;
	float passtime; 
	int ammos;
	Bullet * bullets;
	Image bullet_image;
	void control(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) {
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		fig.fi = (atan2(pos.y - (fig.y), pos.x - (fig.x))) + 3.1415 / 2;
		sprite.setRotation(fig.fi);
		if (passtime < 0)
			passtime = 0;
		else
			passtime -= time;
		if (event.type == Event::MouseButtonPressed)
			if (event.key.code == Mouse::Left)
				if (passtime == 0) {
					if (ammos > 0) {
						bullets = new Bullet(bullet_image, fig.x + fig.w / 2 + sin(fig.fi) * 75, fig.y + fig.h / 2 - cos(fig.fi) * 75, 3, fig.fi, 5);
						pobjects->push_back(bullets);
						passtime = 1000;
						ammos -= 1;
					}
				}
	}
	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) {
		fig.x = holder->x + subx;
		fig.y = holder->y + suby;
		control(time, window, event, pobjects);
		sprite.setRotation(fig.fi * 180 / 3.1415);
		sprite.setPosition(fig.x, fig.y);
	}
	Item(Image &image, float subX, float subY, int W, int H, Figure* substrat, int Name) :Entity(image, substrat->x + subX, substrat->y + subY, W, H, Name) {
		if (name == 4) {
			passtime = 0;
			holder = substrat;
			subx = subX;
			suby = subY;
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			sprite.setOrigin(fig.w / 2, fig.h * 2 / 3);
			ammos = 3;
			bullet_image.loadFromFile("images/Bullet.png");
			bullet_image.createMaskFromColor(Color(255, 255, 255));
		}
	}
};

class Player :public Entity {
public:
	int playerScore;
	float deltaPlaytime;
	//bool seek_for_drop;
	Item* items;
	//int ammos;
	vector<Drop*> Tag;
	Player(Image &image, float X, float Y, int W, int H, int Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0;
		if (name == 1) {
			deltaPlaytime = 0;
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			//seek_for_drop = true;
			Image item_image;
			item_image.loadFromFile("images/Gun.png");
			item_image.createMaskFromColor(Color(255, 255, 255));
			items = new Item(item_image, fig.w / 2, fig.h / 2, 32, 100, &fig, 4);
		}
	}
	void draw(RenderWindow &window) {
		window.draw(sprite);
		window.draw(items->sprite);
	}
	void control() {
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				fig.dfi = -0.005;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				fig.dfi = 0.005;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {
				fig.speed = 0.5;
			}

			//	if (Keyboard::isKeyPressed(Keyboard::Down)) {
			//			fig.speed = -0.5;
			//			}
		}
	}

	/*void checkCollisionWithMap(float Dx, float Dy)
	{
	for (int i = fig.y / 32; i < (fig.y + fig.h) / 32; i++)
	for (int j = fig.x / 32; j<(fig.x + fig.w) / 32; j++)
	{
	if (TileMap[i][j] == '0')
	{
	if (Dy>0) { fig.y = i * 32 - fig.h;  fig.dy = 0;}
	if (Dy<0) { fig.y = i * 32 + 32;  fig.dy = 0; }
	if (Dx>0) { fig.x = j * 32 - fig.w; }
	if (Dx<0) { fig.x = j * 32 + 32; }
	}
	}
	*/
	void collect(Drop * target) 
	{
		Figure* tfig = &(target->fig);
		/*//////////////////////////////////////////////////////////////////////////
		float distx = tfig.x - fig.x - fig.w / 2 * sin(fig.fi);
		float disty = tfig.y - fig.y + fig.h / 2 * cos(fig.fi);
		float dist = distx * distx + disty * disty;
		if (dist < minrad) {
		/*///////////////////////////////////////////////////////////////////////////
		if (checkIntersection(fig, *tfig)) {
		//	if (0) {
			//	life = false;
				//playerScore += 10;
				//cout << target->playerScore << endl;
			//}
			if (target->isInTag == false)
			{

				target->isInTag = true;//here I should add this thing to the list <<Tag>> in class Player
			    Tag.push_back(target);
				items->ammos++;
				deltaPlaytime += 10000;
				playerScore += 100;
				//seek_for_drop = true;
				//fig.dx += alfa * (tfig.x - fig.x);// + (1 - alfa) * fig.x;
				//fig.dy += alfa * (tfig.y - fig.y);// +(1 - alfa) * fig.y;
				//	cout << "I am here" << endl;
				target->rotate = 0;
		//		tfig->fi = fig.fi;
				/*////////////////////////////////////////////////////////////////////////
				fig.x = tfig.x + fig.w / 2 * sin(fig.fi);// fig.dx;
				fig.y = tfig.y - fig.h / 2 * cos(fig.fi);//fig.dy;
				/*///////////////////////////////////////////////////////////////////////
		//		tfig->x = -(0.5)*(1 + sin(tfig->fi))*tfig->w - 0.5*(fig.h)*(sin(fig.fi)) + fig.x + 0.5*fig.w;
		//   	tfig->y = -(0.5)*(1 - cos(tfig->fi))*tfig->h + 0.5*(fig.h)*(1 + cos(fig.fi)) + fig.y;
			}
			//////////////////////////////////////////////////////////////////////////////
		}
	}

	void checkCollisionWithEnemy(Figure Enemy)
	{
		//CircleShape shape2(5, 100);
		//shape2.setFillColor(sf::Color::Green);
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
			//shape2.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
			//shape2.setRadius(0);
			//return shape2;
			return;
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

		if (Scalar_product(e1, r) < 0.5*fig.h && Scalar_product(e1, r) > -0.5*fig.h && Scalar_product(e2, r) < -0.5*fig.w)
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

		//shape2.setPosition(closest.x, closest.y);

		if ((0.5*Enemy.h)*(0.5*Enemy.h) < (Enemy.x + 0.5*Enemy.w - closest.x)*(Enemy.x + 0.5*Enemy.w - closest.x) + (Enemy.y + 0.5*Enemy.h - closest.y)*(Enemy.y + 0.5*Enemy.h - closest.y))
		{
			//return shape2;
			return;
		}

		normal.x = Enemy.x + 0.5*Enemy.w - closest.x;
		normal.y = Enemy.y + 0.5*Enemy.h - closest.y;//ê öåíòðó êðóãà
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
			//return shape2;
			return;
		}
		float power = sqrt(penetration)*penetration*0.01*Scalar_product(relative_speed, normal);
		fig.acceleration.x += power*normal.x;
		fig.acceleration.y += power*normal.y;
		/*if (fig.acceleration.x > 0.25)
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
		}*/
		//return shape2;
		return;
	}

	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects)
	{
		///////////////////////////////////////////////////////////////
		//control();
		////////////////////////////////////////////////////////
		/*switch (state)
		{
		case right:dx = speed; break;
		case left:dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case stay: break;
		}*/

		//////////////////////////////////////////////////////////////////////////////
		fig.velocity.x = sin(fig.fi) * fig.speed;
		fig.velocity.y = -cos(fig.fi) * fig.speed;
		//shape3 = checkCollisionWithEnemy(enemy);
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
		sprite.setRotation(fig.fi * 180 / 3.1415);//
		fig.dfi = 0;
		fig.x += fig.dx*time;
		//checkCollisionWithMap(fig.dx, 0);//
		fig.y += fig.dy*time;
		//checkCollisionWithMap(0, fig.dy);//
		sprite.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h); //
		if (health <= 0) { life = false; }
		if (!isMove) { fig.speed = 0.98*fig.speed; }
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Vector attachPoint;
		Vector fromNext;
		Figure* Current;
		float removal;
		Vector e;
		attachPoint.x = -0.5*(fig.h)*(sin(fig.fi)) + fig.x + 0.5*fig.w;
		attachPoint.y = 0.5*(fig.h)*(1 + cos(fig.fi)) + fig.y;
		for (std::vector<Drop*>::iterator it = Tag.begin(); it != Tag.end(); ++it)
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      	(&((*it)->fig))->fi = fig.fi;
			/*////////////////////////////////////////////////////////////////////////
			fig.x = tfig.x + fig.w / 2 * sin(fig.fi);// fig.dx;
			fig.y = tfig.y - fig.h / 2 * cos(fig.fi);//fig.dy;
			/*///////////////////////////////////////////////////////////////////////
//	        (&((*it)->fig))->x = -(0.5)*(1 + sin((&((*it)->fig))->fi))*(&((*it)->fig))->w - 0.5*(fig.h)*(sin(fig.fi)) + fig.x + 0.5*fig.w;
//          (&((*it)->fig))->y = -(0.5)*(1 - cos((&((*it)->fig))->fi))*(&((*it)->fig))->h + 0.5*(fig.h)*(1 + cos(fig.fi)) + fig.y;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			Current = &((*it)->fig);
			if ((attachPoint.x - Current->x - 0.5*Current->w)*(attachPoint.x - Current->x - 0.5*Current->w) + (attachPoint.y - Current->y - 0.5*Current->h)*(attachPoint.y - Current->y - 0.5*Current->h) < (0.5*Current->w)*(0.5*Current->w))
			{
				attachPoint.x = Current->x + 0.5*Current->w - 0.5*(Current->w)*sin(Current->fi);
				attachPoint.y = Current->y + 0.5*Current->h + 0.5*(Current->h)*cos(Current->fi);
				continue;
			}
			//if attach point inside the next circle ---> continue only to get new attach point 
			Current->x = Current->x - 0.5*(Current->w)*sin(Current->fi);
			Current->y = Current->y + 0.5*(Current->h)*cos(Current->fi);
			fromNext.x = attachPoint.x - Current->x - 0.5*Current->w;
			fromNext.y = attachPoint.y - Current->y - 0.5*Current->h;
			
			if (fromNext.y < 0)
			{
				Current->fi = atan(-fromNext.x / (fromNext.y));
			}
			if (fromNext.y > 0)
			{
				Current->fi = acos(-1) + atan(-fromNext.x / (fromNext.y));
			}
			if ((fromNext.y == 0) && (fromNext.x > 0))
			{
				Current->fi = acos(0);
			}
			if ((fromNext.y == 0) && (fromNext.x < 0))
			{
				Current->fi = -acos(0);
			}
			//cout << Current->fi << endl;
			e.x = sin(Current->fi);
			e.y = -cos(Current->fi);
			removal = Scalar_product(e, fromNext);
			Current->x += (removal - 0.5*fig.w)*sin(Current->fi);
			Current->y -= (removal - 0.5*fig.w)*cos(Current->fi);
			attachPoint.x = Current->x + 0.5*Current->w - 0.5*(Current->w)*sin(Current->fi);
			attachPoint.y = Current->y + 0.5*Current->h + 0.5*(Current->h)*cos(Current->fi);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		items->update(time, window, event, pobjects);
	}
};

class Asteroid :public Entity {
public:
	bool spawned;
	Asteroid(Image &image, float X, float Y, int D, float gr_rotation, int Name) :Entity(image, X, Y, D, D, Name) {
		spawned = false;
		sprite.setTextureRect(IntRect(0, 0, 150, 150));
		sprite.setScale((float)fig.w / 150, (float)fig.h / 150);
		sprite.setOrigin(150 / 2, 150 / 2);
		fig.fi = gr_rotation / 180 * 3.1415;
		//fig.dx = 0.1;
	}
	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) {
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
			if (!(spawned) && !((fig.x - fig.w > WIDTH_MAP) ||
				(fig.x + fig.w < 0) ||
				(fig.y - fig.h > HEIGHT_MAP) ||
				(fig.y + fig.h < 0)
				)) {
				spawned = true;
				//cout << "spawned" << endl;
			}
		}
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
		sprite.setRotation(fig.fi * 180 / 3.1415);
		fig.x += fig.dx*time;
		fig.y += fig.dy*time;
		sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2);

		/*//////////////////////////////////////////////////////////////////////////////////////
		fig.dx = fig.velocity.x; fig.dy = fig.velocity.y;
		fig.x += fig.dx * time;
		fig.y += fig.dy * time;
		fig.fi += fig.dfi * time;
		fig.dx = 0; fig.dy = 0; fig.dfi = 0;
		sprite.setPosition(fig.x + (float)fig.w / 2, fig.y + (float)fig.h / 2);
		sprite.setRotation(fig.fi * 180 / 3.1415);
		///*//////////////////////////////////////////////////////////////////////////////////////
		//sprite.setPosition(fig.x, fig.y);
	}
};


class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, int Name) :Entity(image, X, Y, W, H, Name) {
		if (name == 2) {
			sprite.setTextureRect(IntRect(0, 0, fig.w, fig.h));
			fig.dx = 0.1;
		}
	}
	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects)
	{
		if (name == 2) {

			//checkCollisionWithMap(fig.dx, 0);
			fig.x += fig.dx*time;
			sprite.setPosition(fig.x + fig.w / 2, fig.y + fig.h / 2);
			if (health <= 0) { life = false; }
		}
	}
};

/*class Drop :public Entity {
public:
	//float minrad;
	//float alfa;
	float rotate;
	bool isInTag;
	Drop(Image &image, float X, float Y) : Entity(image, X, Y, 150, 150, 6) {
		//minrad = 16000;
	//	alfa = 0.1;
		rotate = 0.2;
	    isInTag = false;
	}
	void collect(Player * target) {
		Figure tfig = target->fig;
		///////////////////////////////////////////////////////////////////////////
		float distx = tfig.x - fig.x - fig.w / 2 * sin(fig.fi);
		float disty = tfig.y - fig.y + fig.h / 2 * cos(fig.fi);
		float dist = distx * distx + disty * disty;
		if (dist < minrad) {
		////////////////////////////////////////////////////////////////////////////
		if(checkIntersection(tfig, fig)){
			if (0) {
				life = false;
				target->playerScore += 10;
				cout << target->playerScore << endl;
			}
			if (isInTag == false)
			{

			    //isInTag = true;//here I should add this thing to the list <<Tag>> in class Player
				//////////////////////////////////////////////////(target->Tag).push_back();

				//fig.dx += alfa * (tfig.x - fig.x);// + (1 - alfa) * fig.x;
				//fig.dy += alfa * (tfig.y - fig.y);// +(1 - alfa) * fig.y;
			//	cout << "I am here" << endl;
				rotate = 0;
				fig.fi = tfig.fi;
				////////////////////////////////////////////////////////////////////////
				fig.x = tfig.x + fig.w / 2 * sin(fig.fi);// fig.dx;
				fig.y = tfig.y - fig.h / 2 * cos(fig.fi);//fig.dy;
				///////////////////////////////////////////////////////////////////////
				fig.x = -(0.5)*(1 + sin(fig.fi))*fig.w - 0.5*(tfig.h)*(sin(tfig.fi)) + tfig.x + 0.5*tfig.w;
				fig.y = -(0.5)*(1 - cos(fig.fi))*fig.h + 0.5*(tfig.h)*(1 + cos(tfig.fi)) + tfig.y;
			}
			//////////////////////////////////////////////////////////////////////////////
		}
	}
	void update(float time, RenderWindow &window, Event event, list<Entity *> * pobjects) {
		//if (isInTag == false) {
			fig.dfi += rotate;
			fig.fi += fig.dfi;
			fig.dfi = 0;
	//	}
		//else
		//{

		//}
		sprite.setRotation(fig.fi);
		sprite.setPosition(fig.x + 0.5*fig.w, fig.y + 0.5*fig.h);
	}
};*/

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
				if (value[i][j] == 0)
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
		for (i = 0; i < massiv; ++i) {
			fill();
			for (j = 0; j < crowd; ++j) {
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
				if (value[tmpx][tmpy] != 0)
					feed(tmpx, tmpy);
			}
		}
	}

};

class Scene
{
public:
	float passtime;
	float droptime;
	//bool DropCollected;
	Clock clock;
	int map_width;
	int map_height;
	float vew_size_x;
	float vew_size_y;
	float vew_centr_x;
	float vew_centr_y;
	float playtime;
	float maxplaytime;
	int nPlayerScore;
	bool pause;
	bool gamererun;
	Texture map_texture;
	Sprite map_sprite;
	float asteroid_speedx;
	//Player * pl;
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
				//tmpfi = rand() % 360;
				tmpfi = 90;
				Asteroid *aster = new Asteroid(asteroid_image, tmpx, tmpy, cur_map.value[i][j] * rad_size, tmpfi, 3);
				aster->fig.speed = asteroid_speedx;
				/*///////////////////////////////////////////////////////////////////////////////
				aster->fig.velocity.x = asteroid_speedx;
				/*/////////////////////////////////////////////////////////////////////////////////
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
		//pl = player1;
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
		if ((vew_size_x > max(map_width, 640 * 2)) || (vew_size_y > max(map_height, 480 * 2))) {
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
		vew_centr_x = tmpx;
		vew_centr_y = tmpy;
		view.setCenter(tmpx, tmpy);
	}
	/*void update(float time) {
	for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); it1++) {
	if ((*it1)->name == 1) {
	/////////////////////////////////////////////////////////////////
	//	(*it1)->update(time);
	//////////////////////////////////////////////////////////////////
	//((Player*)(*it1))->control();
	(*it1)->update(time);
	/////////////////////////////////////////////////////////////////////////////////////////
	//	for (list<Entity*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++)
	//{
	//if ((*it2)->name == 3)
	//{
	//((Player*)(*it1))->checkCollisionWithEnemy((*it2)->fig);
	//}
	//}
	/////////////////////////////////////////////////////////////////////////////////////
	set_vew((*it1)->fig.x + (*it1)->fig.w / 2, (*it1)->fig.y + (*it1)->fig.h / 2);
	}
	if ((*it1)->name = 3) /// and they must be different here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
	}
	}
	//list<Entity*>::iterator oldit1 = objects.begin();
	for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); ) {
	if ((*it1)->name != 1) {
	(*it1)->update(time);
	}
	////////////////////////////////////////////////////////
	if ((*it1)->name == 1)
	{
	set_vew((*it1)->fig.x + (*it1)->fig.w / 2, (*it1)->fig.y + (*it1)->fig.h / 2);
	}
	//////////////////////////////////////////////////////////
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
	}*/
	void update(float time, RenderWindow &window, Event event) {
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); it1++) {
			if ((*it1)->name == 5) {
				for (list<Entity*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++) {
					if ((*it2)->name == 3)
					{
						if (checkCollisionWithFriendForBullet((*it1)->fig, (*it2)->fig))
						{
							(*it2)->life = false;
							(*it1)->life = false;
						}
					}
				}
			}
		}
		bool flag = false;
		int i = 0;
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); it1++) {
			if ((*it1)->name == 1) {
				((Player*)(*it1))->control();
				for (list<Entity*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++) 
				{
					if ((*it2)->name == 6) 
					{
						((Player*)(*it1))->collect((Drop *)(*it2));
					}
				}
				for (list<Entity*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++)
				{
					if ((*it2)->name == 3)
					{
						((Player*)(*it1))->checkCollisionWithEnemy((*it2)->fig);
						for (std::vector<Drop*>::iterator it = ((Player*)(*it1))->Tag.begin(); it != ((Player*)(*it1))->Tag.end(); ++it)
						{
							if(checkCollisionWithFriend((*it2)->fig, (*it)->fig))
							{
								flag = true;
							}
							if (flag == true)
							{
								(*it)->life = false;
								i = i + 1;
							}
						}
						flag = false;
						if (i > 0)
						{
							for (int j = 0; j < i; j++)
							{
								((Player*)(*it1))->Tag.pop_back();
								((Player*)(*it1))->playerScore = ((Player*)(*it1))->playerScore - 50;
							}
						}
						i = 0;
					}
				}
				if ((*it1)->fig.acceleration.x > 0.25)
				{
					(*it1)->fig.acceleration.x = 0.25;
				}
				if ((*it1)->fig.acceleration.x < -0.25)
				{
					(*it1)->fig.acceleration.x = -0.25;
				}
				if ((*it1)->fig.acceleration.y < -0.25)
				{
					(*it1)->fig.acceleration.y = -0.25;
				}
				if ((*it1)->fig.acceleration.y > 0.25)
				{
					(*it1)->fig.acceleration.y = 0.25;
				}
				(*it1)->update(time, window, event, &objects);
				/*if (((Player*)(*it1))->seek_for_drop) {
					Image drop_image;
					drop_image.loadFromFile("images/Bill2.jpg");
					drop_image.createMaskFromColor(Color(255, 255, 255));
					//srand(clock.getElapsedTime().asMicroseconds());
					//int tmpx = rand() % (map_width - 1);
					srand(clock.getElapsedTime().asMicroseconds());
					int tmpy = rand() % (map_height - 1);
					Drop * dr = new Drop(drop_image, 0, tmpy);
					objects.push_back(dr);
					cout << "generated" << endl;
					((Player*)(*it1))->seek_for_drop = false;
				}*/
				set_vew((*it1)->fig.x + (*it1)->fig.w / 2, (*it1)->fig.y + (*it1)->fig.h / 2);
				playtime -= ((Player*)(*it1))->deltaPlaytime;
				((Player*)(*it1))->deltaPlaytime = 0;
				nPlayerScore = ((Player*)(*it1))->playerScore;
			}
			/*(*it1)->update(time, window, event, &objects);
			set_vew((*it1)->fig.x + (*it1)->fig.w / 2, (*it1)->fig.y + (*it1)->fig.h / 2);*/
			/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if ((*it1)->name == 6) {
				for (list<Entity*>::iterator it2 = objects.begin(); it2 != objects.end(); it2++) {
					if ((*it2)->name == 1) {
						((Drop*)(*it1))->collect((Player *)(*it2));
						//cout << 1 << endl;
					}
				}
			}
			/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (playtime > maxplaytime) {
				gamererun = true;
			}
		}

		//list<Entity*>::iterator oldit1 = objects.begin();
		for (list<Entity*>::iterator it1 = objects.begin(); it1 != objects.end(); ) {
			if ((*it1)->name != 1) {
				(*it1)->update(time, window, event, &objects);
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
		playtime += time;
	}
	void inventory(RenderWindow &window, Player* p1) {
		int i;
		Texture bullets_texture;
		bullets_texture.loadFromImage(p1->items->bullet_image);
		Sprite bullets_sprite;
		bullets_sprite.setTexture(bullets_texture);
		float bullets_sprite_x = 50 * vew_size_x / 640;
		float bullets_sprite_y = 50 * vew_size_y / 480;
		int sizex = 7;
		int sizey = 7;
		bullets_sprite.setScale(bullets_sprite_x / sizex, bullets_sprite_y / sizey);
		for (i = 0; i < p1->items->ammos; ++i) {
			bullets_sprite.setPosition(i * bullets_sprite_x * 1.1 + vew_centr_x - vew_size_x / 2, vew_centr_y - vew_size_y * 0.9 / 2 );
			window.draw(bullets_sprite);
		}
		bullets_sprite.setScale( vew_size_x * (1 - playtime / maxplaytime) / sizex, (10 / sizey) * vew_size_y / 480 );
		bullets_sprite.setPosition(vew_centr_x - vew_size_x / 2, vew_centr_y - vew_size_y / 2);
		window.draw(bullets_sprite);
	}
	void draw(RenderWindow &window) {
		window.draw(map_sprite);
		Player * p1 = NULL;
		for (list<Entity*>::iterator it = objects.begin(); it != objects.end(); it++) {
			if ((*it)->name == 1) {
				p1 = (Player*)(*it);
			}
			(*it)->draw(window);// window.draw((*it)->sprite);
		}
		if(p1 != NULL)
			inventory(window, p1);
	}
	
	void Refresh(float time) {
		droptime -= time;
		passtime += time;
		if (asteroid_speedx * passtime > map_width) {
			//cout << "refresh" << endl;
			asteroid_field_generate(-map_width, 0, 30, 5, BLOCK_SIZE, RAD_SIZE);
			passtime = 0;
		}
		if (droptime < 0) {
			Image drop_image;
			drop_image.loadFromFile("images/Bill2.jpg");
			drop_image.createMaskFromColor(Color(255, 255, 255));
			int tmpy = 100 + rand() % (map_height - 200);
			Drop * dr = new Drop(drop_image, 0, tmpy);
			objects.push_back(dr);
			//cout << "generated" << endl;
			droptime = 10000;
		}
		/*if (DropCollected) {
			Image drop_image;
			drop_image.loadFromFile("images/Bill2.jpg");
			drop_image.createMaskFromColor(Color(255, 255, 255));
			Drop * dr = new Drop(drop_image, 500, 500);
			objects.push_back(dr);
			cout << "generated" << endl;
		}*/
	}
	void RandomGenerate(Image heroImage) {
		asteroid_field_generate(0, 0, 30, 5, BLOCK_SIZE, RAD_SIZE);
		asteroid_field_generate(-map_width, 0, 30, 5, BLOCK_SIZE, RAD_SIZE);
		//enemy_generate(150, 150);
		player_generate(150, 150, heroImage);
		/*Image drop_image;
		drop_image.loadFromFile("images/Bill2.jpg");
		drop_image.createMaskFromColor(Color(255, 255, 255));
		Drop * dr = new Drop(drop_image, 500, 500);
		Drop * dr2 = new Drop(drop_image, 1500, 1500);
		Drop * dr3 = new Drop(drop_image, 2500, 1500);
		Drop * dr4 = new Drop(drop_image, 2500, 1500);
		Drop * dr5 = new Drop(drop_image, 2500, 1500);
		Drop *dr6[10];
		for (int i = 0; i < 10; ++i)
		{
			dr6[i] = new Drop(drop_image, 2500, 1500);
			objects.push_back(dr6[i]);
		}
		objects.push_back(dr);
		objects.push_back(dr2);
		objects.push_back(dr3);
		objects.push_back(dr4);
		objects.push_back(dr5);
		*/
	}
	void ShowGameResult(RenderWindow &window) {
		Font font;
		font.loadFromFile("images/CyrilicOld.ttf");
		Text text("", font, 20  * vew_size_x / 640);
		//			text.setColor(Color(200, 0, 0));
		text.setStyle(sf::Text::Bold | sf::Text::Underlined);
		ostringstream playerScoreString;
		playerScoreString << nPlayerScore;
		text.setString("Regame? PlayerScore:" + playerScoreString.str());
		text.setPosition(vew_centr_x, vew_centr_y);
		window.draw(text);
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
		droptime = 1000;
		playtime = 0;
		maxplaytime = 70000;
		nPlayerScore = 0;
		gamererun = false;
		pause = false;
		//DropCollected = false;
		vew_size_x = 640;
		vew_size_y = 480;
		asteroid_speedx = 0.25;//0.01;
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
		bool EscapeClicked = false;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if ((Keyboard::isKeyPressed(Keyboard::Escape)) && ! (EscapeClicked)) {
				level1.pause = !level1.pause;
				EscapeClicked = true;
			}
			if (!(Keyboard::isKeyPressed(Keyboard::Escape))) {
				EscapeClicked = false;
			}
		}
		if ((!(level1.pause)) && (! level1.gamererun)) {
			window.setView(view);
			window.clear();
			/*for (list<Entity*>::iterator it = level1.objects.begin();
			it != level1.objects.end();
			it++) {
			(*it)->update(time);
			window.draw((*it)->sprite);
			}*/
			level1.Refresh(time);
			level1.update(time, window, event);
			level1.draw(window);
			window.display();
			if (level1.gamererun) {
				level1.ShowGameResult(window);
				window.display();
			}
		}
	}
	level1.distruct();
	//	int k;
	//	cin >> k;
	return 0;
}