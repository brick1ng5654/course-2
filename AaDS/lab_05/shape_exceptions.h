#pragma once

#include <stdexcept>
#include <string>

// Базовый класс для всех исключений, связанных с фигурами
class ShapeException : public std::runtime_error {
public:
    explicit ShapeException(const std::string& message) : std::runtime_error(message) {}
};

// Исключение для точек вне экрана
class PointOutOfScreenException : public ShapeException {
public:
    explicit PointOutOfScreenException(const std::string& message) : ShapeException(message) {}
};

// Исключение для некорректных параметров фигуры
class InvalidShapeParametersException : public ShapeException {
public:
    explicit InvalidShapeParametersException(const std::string& message) : ShapeException(message) {}
};

// Исключение для недостаточного места на экране
class NotEnoughSpaceException : public ShapeException {
public:
    explicit NotEnoughSpaceException(const std::string& message) : ShapeException(message) {}
};

// Исключение для повторного преобразования
class AlreadyTransformedException : public ShapeException {
public:
    explicit AlreadyTransformedException(const std::string& message) : ShapeException(message) {}
}; 