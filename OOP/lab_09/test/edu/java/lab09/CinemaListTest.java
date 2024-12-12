package edu.java.lab09;

import org.junit.Assert;
import org.junit.Test;

import javax.swing.*;

public class CinemaListTest {

    @Test
    public void testIntt() {
        // Подготовка
        MovieList movieList = new MovieList();
        JTextField testField = new JTextField("Введите данные");
        movieList.movieName = testField; // Устанавливаем поле

        // Тестирование
        Assert.assertTrue(movieList.intt());

        // Изменяем текст
        testField.setText("Другое значение");
        Assert.assertFalse(movieList.intt());
    }

    @Test
    public void testWert() {
        // Подготовка
        MovieList movieList = new MovieList();

        // Тестирование
        Assert.assertTrue(movieList.wert());

        // Устанавливаем значение fileNameOpen
        movieList.fileNameOpen = "example.txt";
        Assert.assertFalse(movieList.wert());
    }
}
