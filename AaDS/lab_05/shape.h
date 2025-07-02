#include <list>
#include <algorithm>  // для std::swap
#include "shape_exceptions.h"
using std::list;
//==1. Поддержка экрана в форме матрицы символов ==
char screen[YMAX] [XMAX];
enum color { black = '*', white = '.' };
void screen_init( )
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y])  x = white;
}
void screen_destroy( )
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto &x : screen[y])  x = black;
}
bool on_screen(int a, int b) // проверка попадания точки на экран
{ return 0 <= a && a < XMAX && 0 <= b && b < YMAX; }
void put_point(int a, int b)
{ if (!on_screen(a,b)) {
        throw PointOutOfScreenException("Point (" + std::to_string(a) + ", " + std::to_string(b) + ") is out of screen");
    }
    screen[b] [a] = black; }
void put_line(int x0, int y0, int x1, int y1)
/* Алгоритм Брезенхэма для прямой:
рисование отрезка прямой от (x0, y0) до (x1, y1).
Уравнение прямой: b(x–x0) + a(y–y0) = 0.
Минимизируется величина abs(eps), где eps = 2*(b(x–x0)) + a(y–y0).  */
{
    if (!on_screen(x0, y0) || !on_screen(x1, y1)) {
        throw PointOutOfScreenException("Line endpoint is out of screen");
    }
    int dx = 1;
    int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
    int dy = 1;
    int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
    int two_a = 2*a;
    int two_b = 2*b;
    int xcrit = -b + two_a;
    int eps = 0;
    for (;;) { //Формирование прямой линии по точкам
        try {
            put_point(x0, y0);
        } catch (const PointOutOfScreenException& e) {
            // Skip points outside the screen
            // This allows drawing lines that partially go off-screen
        }
        if (x0 == x1 && y0 == y1) break;
        if (eps <= xcrit) x0 += dx, eps += two_b;
        if (eps >= a || a < b) y0 += dy, eps -= two_a;
    }
}
void screen_clear( ) { screen_init( ); } //Очистка экрана
void screen_refresh( ) // Обновление экрана
{
    for (int y = YMAX-1; 0 <= y; --y) { // с верхней строки до нижней
        for (auto x : screen[y])                 // от левого столбца до правого
            std::cout << x;
        std::cout << '\n';
    }
}
//== 2. Библиотека фигур ==
struct shape {   // Виртуальный базовый класс «фигура»
    static list<shape*> shapes;       // Список фигур (один на все фигуры!)
    shape( ) { shapes.push_back(this); } //Фигура присоединяется к списку
    virtual point north( ) const = 0;  //Точки для привязки
    virtual point south( ) const = 0;
    virtual point east( ) const = 0;
    virtual point west( ) const = 0;
    virtual point neast( ) const = 0;
    virtual point seast( ) const = 0;
    virtual point nwest( ) const = 0;
    virtual point swest( ) const = 0;
    virtual void draw( ) = 0;		//Рисование
    virtual void move(int, int) = 0;	//Перемещение
    virtual void resize(double) = 0;    	//Изменение размера
    virtual ~shape( ) { shapes.remove(this); } //Деструктор
};
list<shape*> shape::shapes;   // Размещение списка фигур
void shape_refresh( )    // Перерисовка всех фигур на экране
{
    screen_clear( );
    for (auto p : shape :: shapes) {
        try {
            p->draw( );
        } catch (const ShapeException& e) {
            // Draw error symbol instead of the shape
            std::cerr << "Error drawing shape: " << e.what() << std::endl;
            // Draw an X as error symbol
            try {
                point center = p->north();
                int x = center.x;
                int y = center.y;
                put_line(x - 2, y - 2, x + 2, y + 2);
                put_line(x - 2, y + 2, x + 2, y - 2);
            } catch (...) {
                // If even error symbol can't be drawn, just skip
            }
        }
    }
    screen_refresh( );
}

// Класс фигур, пригодных к повороту
class rotatable : virtual public shape {
protected:
    enum class rotated { left, no, right };
    rotated state;           // Текущее состояние поворота
public:
    rotatable(rotated r = rotated::no) : state(r) { }
    virtual void rotate_left() {
        if (state != rotated::no) {
            throw AlreadyTransformedException("Shape is already rotated");
        }
        state = rotated::left; }
    virtual void rotate_right() {
        if (state != rotated::no) {
            throw AlreadyTransformedException("Shape is already rotated");
        }
        state = rotated::right; }
};

//----------------------------------------------------------
// Класс фигур, пригодных к зеркальному отражению
class reflectable : virtual public shape {
protected:
    bool hor, vert;         // Текущее состояние отражения
public:
    reflectable(bool h = false, bool v = false) : hor(h), vert(v) { }
    // Исправлены имена методов (ранее была опечатка)
    virtual void flip_horizontally() {
        if (hor) {
            throw AlreadyTransformedException("Shape is already flipped horizontally");
        }
        hor = !hor; }
    virtual void flip_vertically() {
        if (vert) {
            throw AlreadyTransformedException("Shape is already flipped vertically");
        }
        vert = !vert; }
};

class line : public shape {        // ==== Прямая линия ====
/* отрезок прямой ["w", "e"].
   north( ) определяет точку «выше центра отрезка и так далеко
   на север, как самая его северная точка», и т. п. */
protected:
    point p1, p2;
public:
    line() : p1(point(0, 0)), p2(point(0, 0)) {} // Конструктор по умолчанию
    line(point a, point b) : p1(a), p2(b) {
        if (a.x == b.x && a.y == b.y) {
            throw InvalidShapeParametersException("Line endpoints cannot be the same point");
        }
    }; //Произвольная линия (по двум точкам)
    line(point a, int L) : p1(point(a.x + L - 1, a.y)), p2(a) {
        if (L <= 0) {
            throw InvalidShapeParametersException("Line length must be positive");
        }
        if (!on_screen(p1.x, p1.y) || !on_screen(p2.x, p2.y)) {
            throw NotEnoughSpaceException("Line does not fit on screen");
        }
    }; //Горизонтальная линия
    point north( ) const { return point((p1.x+p2.x)/2, p2.y<p1.y? p1.y : p2.y); }
    point south( ) const { return point((p1.x+p2.x)/2, p2.y<p1.y? p2.y : p1.y); }
    point east( ) const { return point(p2.x<p1.x? p1.x : p2.x, (p1.y+p2.y)/2); }
    point west( ) const { return point(p2.x<p1.x? p2.x : p1.x, (p1.y+p2.y)/2); }
    point neast( ) const { return point(p2.x<p1.x? p1.x : p2.x, p2.y<p1.y? p1.y : p2.y); }
    point seast( ) const { return point(p2.x<p1.x? p1.x : p2.x, p2.y<p1.y? p2.y : p1.y); }
    point nwest( ) const { return point(p2.x<p1.x? p2.x : p1.x, p2.y<p1.y? p1.y : p2.y); }
    point swest( ) const { return point(p2.x<p1.x? p2.x : p1.x, p2.y<p1.y? p2.y : p1.y); }
    void move(int a, int b) 	{ p1.x += a; p1.y += b; p2.x += a; p2.y += b; }
    void draw( ) { put_line(p1, p2); }
    void resize(double d)                // Изменение длины линии в (d) раз
    {
        if (d <= 0) {
            throw InvalidShapeParametersException("Resize factor must be positive");
        }
        
        // Проверяем, не выйдет ли линия за пределы экрана при изменении размера
        point new_p2(p1.x + (p2.x - p1.x) * d, p1.y + (p2.y - p1.y) * d);
        if (!on_screen(new_p2.x, new_p2.y)) {
            throw NotEnoughSpaceException("Line would be resized out of screen");
        }
        
        p2.x = new_p2.x;
        p2.y = new_p2.y;
    }
};
class rectangle : public rotatable {      // ==== Прямоугольник ====
/* nw ------ n ------ ne
   |		          |
   |		          |
   w	   c            e
   |		          |
   |		          |
   sw ------- s ------ se */
protected:
    point sw, ne;
public:
    rectangle() : sw(point(0, 0)), ne(point(1, 1)) {} // Конструктор по умолчанию
    rectangle(point a, point b) :  sw(a), ne(b) {
        if (a.x >= b.x || a.y >= b.y) {
            throw InvalidShapeParametersException("Invalid rectangle coordinates");
        }
        if (!on_screen(sw.x, sw.y) || !on_screen(ne.x, ne.y)) {
            throw NotEnoughSpaceException("Rectangle does not fit on screen");
        }
    }
    point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
    point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
    point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast( ) const { return ne; }
    point seast( ) const { return point(ne.x, sw.y); }
    point nwest( ) const { return point(sw.x, ne.y); }
    point swest( ) const { return sw; }
    void rotate_right( )           // Поворот вправо относительно se
    {
        if (state != rotated::no) {
            throw AlreadyTransformedException("Rectangle is already rotated");
        }
        int w = ne.x - sw.x, h = ne.y - sw.y;
        if (!on_screen(ne.x - h * 2, sw.y + w / 2)) {
            throw NotEnoughSpaceException("Rotated rectangle would not fit on screen");
        }
        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;	state = rotated::right;	}
    void rotate_left() // Поворот влево относительно sw
    {
        if (state != rotated::no) {
            throw AlreadyTransformedException("Rectangle is already rotated");
        }
        int w = ne.x - sw.x, h = ne.y - sw.y;
        if (!on_screen(sw.x + h * 2, sw.y + w / 2)) {
            throw NotEnoughSpaceException("Rotated rectangle would not fit on screen");
        }
        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2; state = rotated::left; }
    void move(int a, int b)
    { sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
    void resize(double d)
    {
        if (d <= 0) {
            throw InvalidShapeParametersException("Resize factor must be positive");
        }
        ne.x = sw.x + (ne.x - sw.x) * d;    ne.y = sw.y + (ne.y - sw.y) * d;
        if (!on_screen(sw.x, sw.y) || !on_screen(ne.x, ne.y)) {
            throw NotEnoughSpaceException("Rectangle resized out of screen");
        }
    }
    void draw( )
    {
        try {
            put_line(nwest( ), ne);   put_line(ne, seast( ));
            put_line(seast( ), sw);   put_line(sw, nwest( ));
        } catch (const PointOutOfScreenException& e) {
            throw NotEnoughSpaceException("Cannot draw rectangle: " + std::string(e.what()));
        }
    }
};

// Класс right_triangle
class right_triangle : public rotatable, public reflectable{
protected:
    point sw; // вершина A (нижний лечный угол, прямой угол)
    point ne; // верхний левый угол опорного прямоугольника
public:
    right_triangle(point a, point b, point /*dummy*/)
            : sw(a), ne(b) {
        if (a.x >= b.x || a.y >= b.y) {
            throw InvalidShapeParametersException("Invalid triangle coordinates");
        }
        if (!on_screen(sw.x, sw.y) || !on_screen(ne.x, ne.y)) {
            throw NotEnoughSpaceException("Triangle does not fit on screen");
        }
    }
    // Якоря:
    point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
    point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
    point east( )  const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west( )  const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast( ) const { return ne; }
    point seast( ) const { return point(ne.x, sw.y); }
    point nwest( ) const { return point(sw.x, ne.y); }
    point swest( ) const { return sw; }
    void move(int a, int b)
    {
        sw.x += a; sw.y += b;
        ne.x += a; ne.y += b;
    }
    void resize(double d)
    {
        if (d <= 0) {
            throw InvalidShapeParametersException("Resize factor must be positive");
        }
        ne.x = sw.x + (ne.x - sw.x) * d;
        ne.y = sw.y + (ne.y - sw.y) * d;
    }
    void rotate_right()  {
        if (state != rotated::no) {
            throw AlreadyTransformedException("Triangle is already rotated");
        }
        int dx = ne.x - sw.x;
        int dy = ne.y - sw.y;
        point A = sw;
        point new_sw(A.x, A.y + dx);
        point new_ne(A.x + dy, A.y);
        sw = new_sw;
        ne = new_ne;
    }

    void rotate_left()  {
        if (state != rotated::no) {
            throw AlreadyTransformedException("Triangle is already rotated");
        }
        int dx = ne.x - sw.x;
        int dy = ne.y - sw.y;
        point A = sw; // сохраняем исходную точку A
        point new_sw(A.x, A.y);
        point new_ne(A.x - dy, A.y + dx);
        sw = new_sw;
        ne = new_ne;
    }


    void flip_vertically()  {
        if (vert) {
            throw AlreadyTransformedException("Triangle is already flipped vertically");
        }
        int cx = (sw.x + ne.x) / 2;
        sw.x = 2 * cx - sw.x;
        ne.x = 2 * cx - ne.x;
        if (sw.x < ne.x)
            std::swap(ne.x,sw.x);
    }

    void flip_horizontally()  {
        if (hor) {
            throw AlreadyTransformedException("Triangle is already flipped horizontally");
        }
        point old_sw = sw;
        point old_ne = ne;
        sw = point(old_sw.x, old_ne.y);
        ne = point(old_ne.x, old_sw.y);
        if (sw.x < ne.x)
            std::swap(ne.x,sw.x);
    }
    void draw( )
    {
        try {
            put_line(swest(), nwest()); // сторона AB
            put_line(swest(), seast()); // сторона AC
            put_line(nwest(), seast()); // гипотенуза BC
        } catch (const PointOutOfScreenException& e) {
            throw NotEnoughSpaceException("Cannot draw triangle: " + std::string(e.what()));
        }
    }
    // --- Новый конструктор копирования с смещением:
    right_triangle(const right_triangle &other, int dx, int dy)
            : sw(other.sw), ne(other.ne)
    { move(dx, dy); }
};



//-------------------------------------------------------------------------------------
// Новый класс "шляпа с эмблемой", где шляпа содержит (has-a) треугольник-эмблему:
class hat_with_emblem : public rectangle {
    public:
        // Конструктор шляпы: a - нижний левый угол, b - верхний правый угол
        hat_with_emblem(point a, point b)
                : rectangle(a, b) { }
        // Отрисовка шляпы и её эмблема
        void draw() override {
            try {
                // Рисуем контур шляпы
                rectangle::draw();
                // Рисуем косой крест внутри шляпы с правильным наклоном
                int centerX = (sw.x + ne.x) / 2;
                int centerY = (sw.y + ne.y) / 2;
                int length = (ne.x - sw.x) / 3; // Оптимальная длина
                if (!on_screen(centerX - length, centerY - length) || 
                    !on_screen(centerX + length, centerY + length)) {
                    throw NotEnoughSpaceException("Emblem would not fit on screen");
                }
                put_line(centerX - length, centerY - length, centerX + length, centerY + length);
                put_line(centerX + length, centerY - length, centerX - length, centerY + length);
            } catch (const PointOutOfScreenException& e) {
                throw NotEnoughSpaceException("Cannot draw hat: " + std::string(e.what()));
            }
        }
    };

void up(shape& p, const shape& q) {
    point n = q.north(); // Получаем северную точку фигуры q
    point s = p.south(); // Получаем южную точку фигуры p
    p.move(0, n.y - s.y); // Перемещаем p над q
}


class parallelogram : public shape {
public:
    point p1, p2, p3, p4; // Добавляем 4-ю точку
public:
    parallelogram() : p1(point(0, 0)), p2(point(0, 0)), p3(point(0, 0)), p4(point(0, 0)) {} // Конструктор по умолчанию
    parallelogram(point a, point b, point c) 
        : p1(a), p2(b), p3(c) {
        // Рассчитываем 4-ю точку для параллелограмма
        p4 = point(p1.x + (p3.x - p2.x), p1.y + (p3.y - p2.y));
        
        if (p1.x == p2.x && p1.y == p2.y) {
            throw InvalidShapeParametersException("Parallelogram points cannot be the same");
        }
        
        if (!on_screen(p1.x, p1.y) || !on_screen(p2.x, p2.y) || 
            !on_screen(p3.x, p3.y) || !on_screen(p4.x, p4.y)) {
            throw NotEnoughSpaceException("Parallelogram does not fit on screen");
        }
    }

    void draw() override {
        try {
            put_line(p1, p2);
            put_line(p2, p3);
            put_line(p3, p4);
            put_line(p4, p1);
        } catch (const PointOutOfScreenException& e) {
            throw NotEnoughSpaceException("Cannot draw parallelogram: " + std::string(e.what()));
        }
    }
    
    point north() const override { 
        double y = std::max({p1.y, p2.y, p3.y, p4.y});
        double x = (p1.x + p2.x + p3.x + p4.x) / 4;
        return point(x, y);
    }
    point south() const override { 
        double y = std::min({p1.y, p2.y, p3.y, p4.y});
        double x = (p1.x + p2.x + p3.x + p4.x) / 4;
        return point(x, y);
    }
    point east() const override { 
        double x = std::max({p1.x, p2.x, p3.x, p4.x});
        double y = (p1.y + p2.y + p3.y + p4.y) / 4;
        return point(x, y);
    }
    point west() const override { 
        double x = std::min({p1.x, p2.x, p3.x, p4.x});
        double y = (p1.y + p2.y + p3.y + p4.y) / 4;
        return point(x, y);
    }
    point neast() const override { 
        return point(std::max({p1.x, p2.x, p3.x, p4.x}), std::max({p1.y, p2.y, p3.y, p4.y}));
    }
    point nwest() const override { 
        return point(std::min({p1.x, p2.x, p3.x, p4.x}), std::max({p1.y, p2.y, p3.y, p4.y}));
    }
    point swest() const override { 
        return point(std::min(p1.x, p4.x), std::max(p1.y, p4.y)); 
    }
    point seast() const override { 
        return point(std::max(p2.x, p3.x), std::max(p2.y, p3.y)); 
    }

    
    void move(int dx, int dy) override {
        p1.x += dx; p1.y += dy;
        p2.x += dx; p2.y += dy;
        p3.x += dx; p3.y += dy;
        p4.x += dx; p4.y += dy;
    }

    
    void resize(double scale) override {
        if (scale <= 0) {
            throw InvalidShapeParametersException("Resize factor must be positive");
        }
        point center((p1.x + p2.x + p3.x + p4.x)/4, (p1.y + p2.y + p3.y + p4.y)/4);
        p1.x = center.x + (p1.x - center.x) * scale;
        p1.y = center.y + (p1.y - center.y) * scale;
        p2.x = center.x + (p2.x - center.x) * scale;
        p2.y = center.y + (p2.y - center.y) * scale;
        p3.x = center.x + (p3.x - center.x) * scale;
        p3.y = center.y + (p3.y - center.y) * scale;
        p4.x = center.x + (p4.x - center.x) * scale;
        p4.y = center.y + (p4.y - center.y) * scale;
    }

    void draw_cross() { // Реализация метода для рисования креста
        int centerX = (p1.x + p3.x) / 2;
        int centerY = (p1.y + p3.y) / 2 + 1; // Поднимаем крест на уровень глаз
        int length = (p3.x - p1.x) / 4; // Оптимальная длина
        if (!on_screen(centerX - length, centerY - length) || 
            !on_screen(centerX + length, centerY + length)) {
            throw NotEnoughSpaceException("Cross would not fit on screen");
        }
        try {
            put_line(centerX - length, centerY - length, centerX + length, centerY + length);
            put_line(centerX + length, centerY - length, centerX - length, centerY + length);
        } catch (const PointOutOfScreenException& e) {
            throw NotEnoughSpaceException("Cannot draw cross: " + std::string(e.what()));
        }
    }
};
