package edu.java.lab02;

// Подключение графических библиотек
import java.awt.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;

public class MovieList {
    // Объявления графических компонентов
    private JFrame movieList;
    private DefaultTableModel modelMovies;
    private JButton save, add, delete, upload, print, edit;
    private JToolBar toolBar;
    private JScrollPane scroll;
    private JTable movies;
    private JComboBox director;
    private JTextField movieName;
    private JButton filter;
    public void show() {
        // Создание окна
        movieList = new JFrame("Список фильмов"); // Название приложения
        movieList.setSize(1000, 600); // Ширина и высота окна
        movieList.setLocation(100, 100); // Начальное положение
        movieList.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Создание кнопок, прикрепление иконок и настройка подсказок для кнопок
        save = new JButton(new ImageIcon("src/img/save.png"));
        save.setToolTipText("Сохранить список фильмов");

        add = new JButton(new ImageIcon("src/img/add.png"));
        add.setToolTipText("Добавить фильм");

        delete = new JButton(new ImageIcon("src/img/delete.png"));
        delete.setToolTipText("Удалить фильм");

        upload = new JButton(new ImageIcon("src/img/upload.png"));
        upload.setToolTipText("Загрузить данные");

        print = new JButton(new ImageIcon("src/img/print.png"));
        print.setToolTipText("Распечатать");

        edit = new JButton(new ImageIcon("src/img/edit.png"));
        edit.setToolTipText("Редактировать данные");

        // Добавление кнопок на панель инструментов
        toolBar = new JToolBar("Панель инструментов");
        toolBar.add(save);
        toolBar.add(add);
        toolBar.add(delete);
        toolBar.add(edit);
        toolBar.add(upload);
        toolBar.add(print);

        // Размещение панели инструментов
        movieList.setLayout(new BorderLayout());
        movieList.add(toolBar, BorderLayout.NORTH); // Начальное положение панели инструментов

        // Создание таблицы с данными
        String [] columns = {"Режиссёр", "Фильм", "Просмотрен"};
        String [][] data = {{"Квентин Тарантино", "Бешеные псы", "Да"},
                {"Мартин Скорсезе", "Отступники", "Нет"}};
        modelMovies=  new DefaultTableModel(data, columns);
        movies = new JTable(modelMovies);
        scroll = new JScrollPane(movies);

        // Размещение таблицы с данными
        movieList.add(scroll, BorderLayout.CENTER);

        // Подготовка компонентов поиска
        director = new JComboBox(new String[]{"Режиссёр", "лошара", "Квентин Тарантино", "Мартин Скорсезе"});
        movieName = new JTextField("Название фильма");
        filter = new JButton("Поиск");
        // Добавление компонентов на панель
        JPanel filterPanel = new JPanel();
        filterPanel.add(director); // Добавление на панель поиска
        filterPanel.add(movieName);
        filterPanel.add(filter);

        // Размещение  панели поиска внизу окна
        movieList.add(filterPanel, BorderLayout.SOUTH);

        // Визуализация экранной формы
        movieList.setVisible(true);
    }

    public static void main(String[] args) {
        // Создание и отображение экранной формы
        new MovieList().show();
    }
}
