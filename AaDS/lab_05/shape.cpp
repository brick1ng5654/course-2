#include <locale.h>
#include <iostream>
#include "screen.h"
#include "shape.h"
#include "shape_exceptions.h"

// Face class
class myshape : public rectangle {
private:
    int w, h;               // Face dimensions
    rectangle l_eye;        // Left eye
    rectangle r_eye;        // Right eye

public:
    myshape(point, point);
    void draw() override;
    void move(int a, int b) override;
    
    void resize(double r) override {
        if (r <= 0) {
            throw InvalidShapeParametersException("Resize factor must be positive");
        }
        
        rectangle::resize(r);
        w *= r; 
        h *= r;
        
        try {
            l_eye.resize(r);
            r_eye.resize(r);
        } catch (const ShapeException& e) {
            throw NotEnoughSpaceException("Cannot resize face parts: " + std::string(e.what()));
        }
        
        rectangle::resize(r);
        rectangle::move(w * (1 - r) * 0.5, h * (1 - r) * 0.5);
    }
    
    void rotate_left() override {}
    void rotate_right() override {}
};

// Constructor
myshape::myshape(point a, point b)
    : rectangle(a, b),
      w(neast().x - swest().x + 1),
      h(neast().y - swest().y + 1)
{
    try {
        // Create eyes as small rectangles
        int eye_size = w / 6; // Размер глаза
        
        // Левый глаз
        l_eye = rectangle(
            point(swest().x + w/4, swest().y + h/3),
            point(swest().x + w/4 + eye_size, swest().y + h/3 + eye_size)
        );
        
        // Правый глаз
        r_eye = rectangle(
            point(swest().x + w*3/4 - eye_size, swest().y + h/3),
            point(swest().x + w*3/4, swest().y + h/3 + eye_size)
        );
    } catch (const ShapeException& e) {
        // If any part fails to create, remove this shape from the list
        shape::shapes.remove(this);
        throw; // Re-throw the exception
    }
}

// Draw face
void myshape::draw() {
    try {
        // Draw face outline
        rectangle::draw();
        
        // Draw eyes
        l_eye.draw();
        r_eye.draw();
    } catch (const ShapeException& e) {
        throw NotEnoughSpaceException("Cannot draw face: " + std::string(e.what()));
    }
}

// Move face
void myshape::move(int a, int b) {
    try {
        rectangle::move(a, b);
        l_eye.move(a, b);
        r_eye.move(a, b);
    } catch (const ShapeException& e) {
        throw NotEnoughSpaceException("Cannot move face: " + std::string(e.what()));
    }
}

// Main function
int main() {
    setlocale(LC_ALL, "Rus");
    screen_init();

    try {
        // Create shapes
        myshape face(point(5, 10), point(17, 18));      // Face
        hat_with_emblem hat(point(2, 5), point(16, 10)); // Hat
        line brim(point(10, 12), 15);                    // Hat brim

        // All shapes are automatically added to the list
        shape_refresh();
        std::cout << "=== Generated... ===\n";

        // Prepare shapes
        try {
            hat.rotate_right();
        } catch (const AlreadyTransformedException& e) {
            std::cerr << "Warning: " << e.what() << std::endl;
        }
        
        try {
            brim.resize(2.0);
        } catch (const InvalidShapeParametersException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // Use default size instead
            brim = line(point(10, 12), 15);
        }
        
        try {
            face.resize(1.0);
        } catch (const ShapeException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // Continue with original size
        }
        
        try {
            face.move(25, 15);
        } catch (const NotEnoughSpaceException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // Try moving to a different position
            face.move(15, 15);
        }
        
        try {
            hat.resize(1.0);
        } catch (const ShapeException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // Continue with original size
        }
        
        shape_refresh();
        std::cout << "=== Prepared... ===\n";

        // Assemble the image
        try {
            hat.move(25, 15);
            brim.move(25, 15);
            up(brim, face);
            up(hat, brim);
        } catch (const NotEnoughSpaceException& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            // Try alternative positioning
            hat.move(15, 15);
            brim.move(15, 15);
            up(brim, face);
            up(hat, brim);
        }
        
        shape_refresh();
        std::cout << "=== Ready! ===\n";
        
        // Test exception handling with invalid shapes
        std::cout << "\n=== Testing exception handling ===\n";
        
        try {
            // Test point out of screen
            line invalid_line(point(XMAX + 10, YMAX + 10), point(XMAX + 20, YMAX + 20));
            invalid_line.draw();
        } catch (const PointOutOfScreenException& e) {
            std::cerr << "Caught PointOutOfScreenException: " << e.what() << std::endl;
        }
        
        try {
            // Test invalid parameters
            rectangle invalid_rect(point(10, 10), point(5, 5));
            invalid_rect.draw();
        } catch (const InvalidShapeParametersException& e) {
            std::cerr << "Caught InvalidShapeParametersException: " << e.what() << std::endl;
        }
        
        try {
            // Test not enough space
            parallelogram large_para(point(1, 1), point(XMAX - 1, YMAX - 1), point(XMAX - 1, 1));
            // Проверяем, не выйдет ли параллелограмм за пределы экрана при изменении размера
            if (large_para.p1.x * 2 > XMAX || large_para.p1.y * 2 > YMAX ||
                large_para.p2.x * 2 > XMAX || large_para.p2.y * 2 > YMAX ||
                large_para.p3.x * 2 > XMAX || large_para.p3.y * 2 > YMAX ||
                large_para.p4.x * 2 > XMAX || large_para.p4.y * 2 > YMAX) {
                throw NotEnoughSpaceException("Parallelogram would be resized out of screen");
            }
            large_para.resize(2.0);
        } catch (const NotEnoughSpaceException& e) {
            std::cerr << "Caught NotEnoughSpaceException: " << e.what() << std::endl;
        }
        
        try {
            // Test already transformed
            right_triangle tri(point(30, 30), point(40, 40), point(0, 0));
            tri.rotate_right();
            tri.rotate_right(); // This should throw
        } catch (const AlreadyTransformedException& e) {
            std::cerr << "Caught AlreadyTransformedException: " << e.what() << std::endl;
        }
        
        std::cin.get();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
    }

    screen_destroy();
    return 0;
}
