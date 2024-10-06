package edu.java.lab03;

// Подключение графических библиотек
import java.awt.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.event.*;


public class MovieList {
    // Объявления графических компонентов
    private JFrame cinemaList;
    private DefaultTableModel modelMovies, modelSessions;
    private JButton save, add, delete, upload, print, edit;
    private JToolBar toolBar;
    private JScrollPane scrollMovies, scrollSessions;
    private JTable tableMovies, tableSessions;
    private JComboBox director;
    private JTextField movieName;
    private JButton filter;
    private JTabbedPane tabbedPane;
    public void show() {
        // Создание окна
        cinemaList = new JFrame("Список фильмов"); // Название приложения
        cinemaList.setSize(1000, 600); // Ширина и высота окна
        cinemaList.setLocation(100, 100); // Начальное положение
        cinemaList.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        cinemaList.addWindowListener(new WindowAdapter() { // Слушатель на закрытие окна с подтвердждением выхода
            public void windowClosing(WindowEvent e) {
                int confirm = JOptionPane.showConfirmDialog(cinemaList, "Вы уверены, что хотите выйти?");
                if (confirm == JOptionPane.YES_OPTION) {
                    cinemaList.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                } else {
                    cinemaList.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
                }
            }
        });


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
        cinemaList.setLayout(new BorderLayout());
        cinemaList.add(toolBar, BorderLayout.NORTH); // Начальное положение панели инструментов

        // Создание таблицы для фильмов
        String [] columnsMovies = {"Год", "Фильм", "Режиссёр", "Жанр"};
        String [][] dataMovies = {{"1990","Бешеные псы", "Квентин Тарантино", "Детектив"},
                {"2010","Отступники","Мартин Скорсезе", "Триллер"}};
        modelMovies=  new DefaultTableModel(dataMovies, columnsMovies);
        tableMovies = new JTable(modelMovies);
        scrollMovies = new JScrollPane(tableMovies);

        // Создание таблицы для сеансов
        String [] columnsSessions = {"Фильм", "Дата", "Время", "Продано билетов"};
        String [][] dataSessions = {{"Бешеные псы","21.10.2024", "19:00", "31"},
                {"Отступники","05.07.2023","12:00", "5"}};
        modelSessions=  new DefaultTableModel(dataSessions, columnsSessions);
        tableSessions = new JTable(modelSessions);
        scrollSessions = new JScrollPane(tableSessions);

        // Создание вкладок с таблицами
        JTabbedPane tabbedPane = new JTabbedPane();
        tabbedPane.addTab("Фильмы", scrollMovies);
        tabbedPane.addTab("Сеансы", scrollSessions);

        // Размещение таблицы с данными
        cinemaList.add(tabbedPane, BorderLayout.CENTER);

        // Подготовка компонентов поиска
        director = new JComboBox(new String[]{"Режиссёр", "Дэвид Финчер", "Квентин Тарантино", "Мартин Скорсезе"});

        movieName = new JTextField("Название фильма");
        movieName.addFocusListener(new FocusAdapter() {
            public void focusGained(FocusEvent e) {
                movieName.setText(""); // Очистить поле при получении фокуса
            }
            public void focusLost(FocusEvent e) {
                if (movieName.getText().isEmpty()) {
                    movieName.setText("Название фильма"); // Вернуть текст, если поле пустое
                }
            }
        });

        filter = new JButton("Поиск");
        filter.addActionListener (new ActionListener() { // Добавление слушателя нажатия на кнопку
            public void actionPerformed (ActionEvent event){
                JOptionPane.showMessageDialog (cinemaList, "Проверка нажатия на кнопку");
            }
        });
        // Добавление компонентов на панель
        JPanel filterPanel = new JPanel();
        filterPanel.add(director); // Добавление на панель поиска
        filterPanel.add(movieName);
        filterPanel.add(filter);

        // Размещение  панели поиска внизу окна
        cinemaList.add(filterPanel, BorderLayout.SOUTH);

        // Визуализация экранной формы
        cinemaList.setVisible(true);
    }


    public static void main(String[] args) {
        // Создание и отображение экранной формы
        new MovieList().show();
    }
}
