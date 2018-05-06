#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) {
	float tempX = x; float tempY = y;

	//if (x < 320) tempX = 320;
	//if (y < 240) tempY = 240;//верхнюю сторону
	if (y > 624) tempY = 624;//нижнюю стороню.дл€ новой карты

	view.setCenter(tempX, tempY);
}
/*#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;//объ€вили sfml объект "вид", который и €вл€етс€ камерой

void getplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и провер€ем их, чтобы убрать кра€

	if (x < 320) tempX = 320;//убираем из вида левую сторону
	if (y < 240) tempY = 240;//верхнюю сторону
	if (y > 554) tempY = 554;//нижнюю сторону	

	view.setCenter(tempX, tempY); //следим за игроком, передава€ его координаты. 

}


void viewmap(float time) { //функци€ дл€ перемещени€ камеры по карте. принимает врем€ sfml


	if (Keyboard::isKeyPressed(Keyboard::D)) {
		view.move(0.1*time, 0);//скроллим карту вправо (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}

	if (Keyboard::isKeyPressed(Keyboard::S)) {
		view.move(0, 0.1*time);//скроллим карту вниз (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		view.move(-0.1*time, 0);//скроллим карту влево (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		view.move(0, -0.1*time);//скроллим карту вправо (см урок, когда мы двигали геро€ - всЄ тоже самое)
	}

}

void changeview() {


	if (Keyboard::isKeyPressed(Keyboard::U)) {
		view.zoom(1.0100f); //масштабируем, уменьшение
							//view.zoom(1.0006f); //тоже самое помедленнее соответственно
	}

	if (Keyboard::isKeyPressed(Keyboard::R)) {
		//view.setRotation(90);//сразу же задает поворот камере
		view.rotate(1);//постепенно поворачивает камеру (отрицательное значение - в обратную сторону)
	}


	if (Keyboard::isKeyPressed(Keyboard::I)) {
		view.setSize(640, 480);//устанавливает размер камеры (наш исходный)
	}

	if (Keyboard::isKeyPressed(Keyboard::P)) {
		view.setSize(540, 380);//например другой размер
	}


	if (Keyboard::isKeyPressed(Keyboard::Q)) {
		view.setViewport(sf::FloatRect(0, 0, 0.5f, 1));//таким образом делаетс€ раздельный экран дл€ игры на двоих. нужно только создать ещЄ один объект View и прив€зывать к нему координаты игрока 2.
	}

}*/