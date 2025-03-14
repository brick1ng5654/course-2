#include <locale.h>
#include <iostream>
#include "screen.h"
#include "shape.h"

// -------------------------------------------------------------------------
// Модифицированный класс физиономии (лицо)
class myshape : public rectangle {
    int w, h;               // размеры лица
    line l_eye;  // левый глаз (линия)
    line r_eye;  // правый глаз (линия)
    parallelogram l_sideburn;  // Левая бакенбарда (параллелограмм)
    parallelogram r_sideburn;  // Правая бакенбарда (параллелограмм)

    line mouth;            // рот (оставляем линию)
    parallelogram beard_left;  // Левая часть бороды (параллелограмм)
    parallelogram beard_right; // Правая часть бороды (параллелограмм)

public:
    myshape(point, point);
    void draw();
    void move(int a, int b);
    void resize(double r) {
        rectangle::resize(r);
        w *= r; h *= r; // Обновляем размеры
        l_sideburn.resize(r);
        r_sideburn.resize(r);
        rectangle::resize(r);
        rectangle::move(w * (1 - r) * 0.5, h * (1 - r) * 0.5);
    }
    void rotate_left() { }
    void rotate_right() { }
};

myshape::myshape(point a, point b)
    : rectangle(a, b),
      w(neast().x - swest().x + 1),
      h(neast().y - swest().y + 1),
      l_sideburn(
        point(swest().x, seast().y + 5),  // Смещаем влево и ниже, увеличиваем высоту
        point(swest().x + w/6, swest().y +4),
        point(swest().x - w/12, swest().y)  // Удлиняем
    ),
    r_sideburn(
        point(seast().x, seast().y + 4),  // Смещаем вправо и ниже, увеличиваем высоту
        point(seast().x + w/6, seast().y +4),
        point(seast().x + w/12, swest().y )  // Удлиняем
    ),
    l_eye(point(swest().x + 2, swest().y + h * 3 / 4 - 1),
            point(swest().x + 4, swest().y + h * 3 / 4 - 1)),
    r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4 - 1),
            point(swest().x + w - 2, swest().y + h * 3 / 4 - 1)),
    mouth(point(swest().x + 2, swest().y + h / 4),
            point(swest().x + w - 3, swest().y + h / 4)),
    beard_left(point(swest().x + 0, swest().y + 0), 
    point(swest().x + 0, swest().y + 0), 
    point(swest().x + 0, swest().y +0)), // Левая часть бороды
    beard_right(point(seast().x - 8, swest().y - 5), 
    point(seast().x - 15, swest().y - 5), 
    point(seast().x - 8, swest().y + 0)) // Правая часть бороды
{ }

void myshape::draw() {
    // Рисуем контур лица
    rectangle::draw();
    // Рисуем глаза (составные объекты-треугольники)
    l_eye.draw();
    r_eye.draw();
    // Рисуем рот
    mouth.draw();
    // Рисуем бакенбарды
    l_sideburn.draw();
    r_sideburn.draw();
    // Рисуем бороду
    beard_left.draw();  // Рисуем левую часть бороды
    beard_right.draw(); // Рисуем правую часть бороды
    // Рисуем нос в центре лица

    int a = (swest().x + neast().x) / 2;
    int b = (swest().y + neast().y) / 2;
    put_point(point(a, b));
}

void myshape::move(int a, int b) {
    rectangle::move(a, b);
    l_eye.move(a, b);
    r_eye.move(a, b);
    mouth.move(a, b);
    l_sideburn.move(a, b); // Добавляем перемещение бакенбард
    r_sideburn.move(a, b);
    beard_left.move(a, b); // Добавляем перемещение бороды
    beard_right.move(a, b); // Добавляем перемещение бороды
}

// -------------------------------------------------------------------------
// main – программа последовательно демонстрирует:
// 1) Исходный набор фигур (Generated),
// 2) Результат подготовки фигур к сборке (Prepared),
// 3) Собранное изображение физиономии (Ready).
int main() {
    setlocale(LC_ALL, "Rus");
    screen_init();

    // --- 1. Исходный набор фигур ---
    myshape face(point(5, 10), point(17, 18));            // лицо
    hat_with_emblem hat(point(2, 5), point(16, 10));       // шляпа (перемещена выше лица)
    line brim(point(10, 12), 15);                           // поля шляпы (горизонтальная линия длиной 15)

    // Все фигуры автоматически добавляются в список
    shape_refresh();
    std::cout << "=== Generated... ===\n";
    //std::cin.get(); // Смотреть исходный набор


    //== 2. Подготовка к сборке ==
    hat.rotate_right();
    brim.resize(2.0);
    face.resize(1.0);
    
    face.move(25, 15);
    hat.resize(1.0);
    shape_refresh();
    std::cout << "=== Prepared... ===\n";
    //std::cin.get(); // Смотреть результат поворотов/отражений

    //== 3. Сборка изображения ==
    hat.move(25, 15);
    brim.move(25, 15);
    up(brim, face);
    up(hat, brim);
    
    shape_refresh();
    std::cout << "=== Ready! ===\n";
    std::cin.get(); // Смотреть результат

    screen_destroy();
    return 0;
}
