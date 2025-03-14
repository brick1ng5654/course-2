#include <list>
#include <algorithm>  // для std::swap
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
{ if (on_screen(a,b)) screen[b] [a] = black; }
void put_line(int x0, int y0, int x1, int y1)
/* Алгоритм Брезенхэма для прямой:
рисование отрезка прямой от (x0, y0) до (x1, y1).
Уравнение прямой: b(x–x0) + a(y–y0) = 0.
Минимизируется величина abs(eps), где eps = 2*(b(x–x0)) + a(y–y0).  */
{
    int dx = 1;
    int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
    int dy = 1;
    int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
    int two_a = 2*a;
    int two_b = 2*b;
    int xcrit = -b + two_a;
    int eps = 0;
    for (;;) { //Формирование прямой линии по точкам
        put_point(x0, y0);
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
    for (auto p : shape :: shapes) p->draw( ); //Динамическое связывание!!!
    screen_refresh( );
}

// Класс фигур, пригодных к повороту
class rotatable : virtual public shape {
protected:
    enum class rotated { left, no, right };
    rotated state;           // Текущее состояние поворота
public:
    rotatable(rotated r = rotated::no) : state(r) { }
    void rotate_left() { state = rotated::left; }
    void rotate_right() { state = rotated::right; }
};

//----------------------------------------------------------
// Класс фигур, пригодных к зеркальному отражению
class reflectable : virtual public shape {
protected:
    bool hor, vert;         // Текущее состояние отражения
public:
    reflectable(bool h = false, bool v = false) : hor(h), vert(v) { }
    // Исправлены имена методов (ранее была опечатка)
    virtual void flip_horizontally() { hor = !hor; }
    virtual void flip_vertically() { vert = !vert; }
};

class line : public shape {        // ==== Прямая линия ====
/* отрезок прямой ["w", "e"].
   north( ) определяет точку «выше центра отрезка и так далеко
   на север, как самая его северная точка», и т. п. */
protected:
    point w, e;
public:
    line(point a, point b) : w(a), e(b) { }; //Произвольная линия (по двум точкам)
    line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  }; //Горизонтальная линия
    point north( ) const { return point((w.x+e.x)/2, e.y<w.y? w.y : e.y); }
    point south( ) const { return point((w.x+e.x)/2, e.y<w.y? e.y : w.y); }
    point east( ) const { return point(e.x<w.x? w.x : e.x, (w.y+e.y)/2); }
    point west( ) const { return point(e.x<w.x? e.x : w.x, (w.y+e.y)/2); }
    point neast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? w.y : e.y); }
    point seast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? e.y : w.y); }
    point nwest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? w.y : e.y); }
    point swest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? e.y : w.y); }
    void move(int a, int b) 	{ w.x += a; w.y += b; e.x += a; e.y += b; }
    void draw( ) { put_line(w, e); }
    void resize(double d)                // Изменение длины линии в (d) раз
    { e.x = w.x + (e.x - w.x) * d; e.y = w.y + (e.y - w.y) * d; }
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
    rectangle(point a, point b) :  sw(a), ne(b) { }
    point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
    point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
    point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast( ) const { return ne; }
    point seast( ) const { return point(ne.x, sw.y); }
    point nwest( ) const { return point(sw.x, ne.y); }
    point swest( ) const { return sw; }
    void rotate_right( )           // Поворот вправо относительно se
    { int w = ne.x - sw.x, h = ne.y - sw.y; // (учитывается масштаб по осям)
        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;	}
    void rotate_left() // Поворот влево относительно sw
    { int w = ne.x - sw.x, h = ne.y - sw.y;
        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2; }
    void move(int a, int b)
    { sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
    void resize(int d)
    { ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d; }
    void resize(double d)
    {
        ne.x = sw.x + (ne.x - sw.x) * d;    ne.y = sw.y + (ne.y - sw.y) * d;
    }
    void draw( )
    {
        put_line(nwest( ), ne);   put_line(ne, seast( ));
        put_line(seast( ), sw);   put_line(sw, nwest( ));
    }
};

// Класс right_triangle
class right_triangle : public rotatable, public reflectable{
protected:
    point sw; // вершина A (нижний лечный угол, прямой угол)
    point ne; // верхний левый угол опорного прямоугольника
public:
    right_triangle(point a, point b, point /*dummy*/)
            : sw(a), ne(b) { } // параметр dummy не используется, оставляем для совместимости
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
        ne.x = sw.x + (ne.x - sw.x) * d;
        ne.y = sw.y + (ne.y - sw.y) * d;
    }
    void rotate_right()  {
        int dx = ne.x - sw.x;
        int dy = ne.y - sw.y;
        point A = sw;
        point new_sw(A.x, A.y + dx);
        point new_ne(A.x + dy, A.y);
        sw = new_sw;
        ne = new_ne;
    }

    void rotate_left()  {
        // Поворот против часовой стрелки на 90° относительно sw
        int dx = ne.x - sw.x;
        int dy = ne.y - sw.y;
        point A = sw; // сохраняем исходную точку A
        point new_sw(A.x, A.y);
        point new_ne(A.x - dy, A.y + dx);
        sw = new_sw;
        ne = new_ne;
    }


    void flip_vertically()  {
        int cx = (sw.x + ne.x) / 2;
        sw.x = 2 * cx - sw.x;
        ne.x = 2 * cx - ne.x;
        if (sw.x < ne.x)
            std::swap(ne.x,sw.x);
    }

    void flip_horizontally()  {
        point old_sw = sw;
        point old_ne = ne;
        sw = point(old_sw.x, old_ne.y);
        ne = point(old_ne.x, old_sw.y);
        if (sw.x < ne.x)
            std::swap(ne.x,sw.x);
    }
    void draw( )
    {
        put_line(swest(), nwest()); // сторона AB
        put_line(swest(), seast()); // сторона AC
        put_line(nwest(), seast()); // гипотенуза BC
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
            // Рисуем контур шляпы
            rectangle::draw();
            // Рисуем косой крест внутри шляпы с правильным наклоном
            int centerX = (sw.x + ne.x) / 2;
            int centerY = (sw.y + ne.y) / 2;
            int length = (ne.x - sw.x) / 3; // Оптимальная длина
            put_line(centerX - length, centerY - length, centerX + length, centerY + length);
            put_line(centerX + length, centerY - length, centerX - length, centerY + length);
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
    parallelogram(point a, point b, point c) 
        : p1(a), p2(b), p3(c) {
        // Рассчитываем 4-ю точку для параллелограмма
        p4 = point(p1.x + (p3.x - p2.x), p1.y + (p3.y - p2.y));
    }

    void draw() override {
        put_line(p1, p2);
        put_line(p2, p3);
        put_line(p3, p4);
        put_line(p4, p1);
        put_line(p1, p4); // Ensure all sides are drawn

        put_line(p1, p4); // Ensure all sides are drawn

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
        put_line(centerX - length, centerY - length, centerX + length, centerY + length);
        put_line(centerX + length, centerY - length, centerX - length, centerY + length);

    }
};
