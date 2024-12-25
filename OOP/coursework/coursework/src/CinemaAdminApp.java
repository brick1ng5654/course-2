import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.io.File;
import java.io.IOException;
import java.sql.*;
import java.util.logging.*;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CinemaAdminApp {
    private static Connection dbConnection;
    private static DefaultTableModel filmsModel;
    private static DefaultTableModel repertoireModel;
    private static DefaultTableModel sessionsModel;
    private static DefaultTableModel ticketsModel;
    private static JTable currentTable;
    private static JTable filmsTable;
    private static JTable repertoireTable;
    private static JTable sessionsTable;
    private static JTable ticketsTable;

    // Хранит данные для каждой вкладки
    private static Map<String, List<String[]>> tempData = new HashMap<>();

    // Компоненты поиска
    private static JComboBox<String> director;
    private static JTextField wordToFind;

    // Флаг для редактирования
    private static boolean isEditingEnabled = false;

    private static final Logger logger = Logger.getLogger(CinemaAdminApp.class.getName());

    static {
        try {
            FileHandler fileHandler = new FileHandler("src/log.txt", true);
            fileHandler.setFormatter(new SimpleFormatter());
            logger.addHandler(fileHandler);
            logger.setLevel(Level.ALL);
        } catch (IOException e) {
            System.err.println("Не удалось настроить логирование: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        JButton filter;
        JButton filterClear;

        // Проверка авторизации
        logger.info("Приложение запущено");
        if (!showLoginDialog()) {
            logger.info("Приложение закрыто");
            System.exit(0);
        }

        // Создаем главное окно приложения
        JFrame frame = new JFrame("Cinema Admin App");
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setSize(800, 600);

        // Обработчик закрытия окна
        frame.addWindowListener(new java.awt.event.WindowAdapter() {
            @Override
            public void windowClosing(java.awt.event.WindowEvent windowEvent) {
                handleExit(frame);
            }
        });

        // Создаем панель инструментов
        JToolBar toolBar = new JToolBar();

        JButton saveButton = new JButton(new ImageIcon("src/img/save.png"));
        saveButton.setToolTipText("Сохранить список фильмов");
        saveButton.addActionListener(e -> saveDataToDatabase(frame));

        JButton addButton = new JButton(new ImageIcon("src/img/add.png"));
        addButton.setToolTipText("Добавить фильм");
        addButton.addActionListener(e -> {
            JTabbedPane tabbedPane = (JTabbedPane) frame.getContentPane().getComponent(1);
            int selectedIndex = tabbedPane.getSelectedIndex();
            handleAddData(selectedIndex, frame);
        });

        JButton deleteButton = new JButton(new ImageIcon("src/img/delete.png"));
        deleteButton.setToolTipText("Удалить фильм");
        deleteButton.addActionListener(e -> {
            handleDeleteData(frame);
        });

        JButton editButton = new JButton(new ImageIcon("src/img/edit.png"));
        editButton.setToolTipText("Редактировать фильм");
        editButton.addActionListener(e -> {
            isEditingEnabled = !isEditingEnabled;
            updateTableEditing(isEditingEnabled);
        });

        JButton loadButton = new JButton(new ImageIcon("src/img/upload.png"));
        loadButton.setToolTipText("Загрузить список фильмов");
        loadButton.addActionListener(e -> loadDatabaseFile(frame));

        JButton logsButton = new JButton(new ImageIcon("src/img/log.png"));
        logsButton.setToolTipText("Открыть логи");

        JButton exitButton = new JButton(new ImageIcon("src/img/exit.png"));
        exitButton.setToolTipText("Выйти из приложения");

        // Добавляем кнопки на панель инструментов
        toolBar.add(saveButton);
        toolBar.add(loadButton);
        toolBar.addSeparator();
        toolBar.add(addButton);
        toolBar.add(deleteButton);
        toolBar.add(editButton);
        toolBar.addSeparator();
        toolBar.add(logsButton);
        toolBar.add(exitButton);

        // Добавляем обработчик для кнопки "Логи"
        logsButton.addActionListener(e -> {
            File logFile = new File("src/log.txt");
            if (logFile.exists()) {
                try {
                    Desktop.getDesktop().open(logFile);
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(frame, "Ошибка открытия log.txt: " + ex.getMessage(),
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                }
            } else {
                JOptionPane.showMessageDialog(frame, "Файл log.txt не найден.",
                        "Ошибка", JOptionPane.ERROR_MESSAGE);
            }
        });

        // Добавляем обработчик для кнопки "Выход"
        exitButton.addActionListener(e -> handleExit(frame));

        // Создаем панель вкладок
        JTabbedPane tabbedPane = new JTabbedPane();

        // Вкладка "Фильмы"
        JPanel filmsPanel = new JPanel(new BorderLayout());
        filmsTable = new JTable();
        filmsModel = new DefaultTableModel() {
            @Override
            public boolean isCellEditable(int row, int column) {
                return column != 0 && isEditingEnabled;
            }
        };
        filmsTable.setModel(filmsModel);
        filmsPanel.add(new JScrollPane(filmsTable), BorderLayout.CENTER);
        tabbedPane.addTab("Фильмы", filmsPanel);

        // Вкладка "Репертуар"
        JPanel repertoirePanel = new JPanel(new BorderLayout());
        repertoireTable = new JTable();
        repertoireModel = new DefaultTableModel() {
            @Override
            public boolean isCellEditable(int row, int column) {
                return column != 0 && isEditingEnabled;
            }
        };
        repertoireTable.setModel(repertoireModel);
        repertoirePanel.add(new JScrollPane(repertoireTable), BorderLayout.CENTER);
        tabbedPane.addTab("Репертуар", repertoirePanel);

        // Вкладка "Сеансы"
        JPanel sessionsPanel = new JPanel(new BorderLayout());
        sessionsTable = new JTable();
        sessionsModel = new DefaultTableModel() {
            @Override
            public boolean isCellEditable(int row, int column) {
                return column != 0 && isEditingEnabled;
            }
        };
        sessionsTable.setModel(sessionsModel);
        sessionsPanel.add(new JScrollPane(sessionsTable), BorderLayout.CENTER);
        tabbedPane.addTab("Сеансы", sessionsPanel);

        // Вкладка "Билеты"
        JPanel ticketsPanel = new JPanel(new BorderLayout());
        ticketsTable = new JTable();
        ticketsModel = new DefaultTableModel() {
            @Override
            public boolean isCellEditable(int row, int column) {
                return column != 0 && isEditingEnabled;
            }
        };
        ticketsTable.setModel(ticketsModel);
        ticketsPanel.add(new JScrollPane(ticketsTable), BorderLayout.CENTER);
        tabbedPane.addTab("Билеты", ticketsPanel);

        // Подготовка компонентов поиска
        director = new JComboBox<>();

        wordToFind = new JTextField("Ключевое слово", 20);
        wordToFind.addFocusListener(new FocusAdapter() {
            public void focusGained(FocusEvent e) {
                if (wordToFind.getText().equals("Ключевое слово")) {
                    wordToFind.setText(""); // Очистить поле при получении фокуса
                }
            }
            public void focusLost(FocusEvent e) {
                if (wordToFind.getText().isEmpty()) {
                    wordToFind.setText("Ключевое слово"); // Вернуть текст, если поле пустое
                }
            }
        });

        filterClear = new JButton("Сброс");
        filterClear.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                loadDataFromDatabase("movies", filmsTable);
                loadDataFromDatabase("rep", repertoireTable);
                loadDataFromDatabase("sessions", sessionsTable);
                loadDataFromDatabase("tickets", ticketsTable);
                wordToFind.setText("Ключевое слово");
            }
        });

        filter = new JButton("Поиск");
        filter.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                try {
                    checkName(wordToFind);
                    performSearch(frame);
                } catch (NullPointerException ex) {
                    JOptionPane.showMessageDialog(frame, ex.toString(), "Ошибка", JOptionPane.ERROR_MESSAGE);
                } catch (InvalidFindException myEx) {
                    JOptionPane.showMessageDialog(frame, myEx.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
                }
            }
        });

        // Панель поиска
        JPanel searchPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        searchPanel.add(new JLabel("Поиск по:"));
        searchPanel.add(director);
        searchPanel.add(wordToFind);
        searchPanel.add(filter);
        searchPanel.add(filterClear);

        // Устанавливаем расположение элементов в главном окне
        frame.setLayout(new BorderLayout());
        frame.add(toolBar, BorderLayout.NORTH);
        frame.add(tabbedPane, BorderLayout.CENTER);
        frame.add(searchPanel, BorderLayout.SOUTH);

        // Подключение к базе данных
        connectToDatabase();

        // Загрузка данных из базы в таблицы
        loadDataFromDatabase("movies", filmsTable);
        loadDataFromDatabase("rep", repertoireTable);
        loadDataFromDatabase("sessions", sessionsTable);
        loadDataFromDatabase("tickets", ticketsTable);

        // Обновление полей поиска при смене вкладки
        tabbedPane.addChangeListener(e -> updateSearchFields(tabbedPane.getSelectedIndex()));

        // Отображаем окно
        frame.setVisible(true);
    }

    /**
     * Обработчик закрытия приложения.
     * @param frame Главное окно приложения.
     */
    private static void handleExit(JFrame frame) {
        int choice = JOptionPane.showConfirmDialog(frame,
                "Вы уверены, что хотите выйти из приложения?",
                "Подтверждение",
                JOptionPane.YES_NO_OPTION);
        if (choice == JOptionPane.YES_OPTION) {
            logger.info("Приложение закрыто");
            if (dbConnection != null) {
                try {
                    dbConnection.close();
                } catch (SQLException e) {
                    logger.warning("Ошибка при закрытии соединения с базой данных: " + e.getMessage());
                }
            }
            System.exit(0);
        }
    }

    /**
     * Загрузка базы данных из файла.
     * @param frame Главное окно приложения.
     */
    private static void loadDatabaseFile(JFrame frame) {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new javax.swing.filechooser.FileNameExtensionFilter("SQLite Database Files", "db"));
        int returnValue = fileChooser.showOpenDialog(frame);

        if (returnValue == JFileChooser.APPROVE_OPTION) {
            File selectedFile = fileChooser.getSelectedFile();
            String dbUrl = "jdbc:sqlite:" + selectedFile.getAbsolutePath();

            try {
                if (dbConnection != null && !dbConnection.isClosed()) {
                    dbConnection.close();
                }
                dbConnection = DriverManager.getConnection(dbUrl);
                JOptionPane.showMessageDialog(frame, "База данных успешно загружена.", "Успех", JOptionPane.INFORMATION_MESSAGE);
                logger.info("База данных успешно загружена из файла: " + dbUrl);

                // Перезагрузка данных в таблицы
                loadDataFromDatabase("movies", filmsTable);
                loadDataFromDatabase("rep", repertoireTable);
                loadDataFromDatabase("sessions", sessionsTable);
                loadDataFromDatabase("tickets", ticketsTable);
            } catch (SQLException e) {
                JOptionPane.showMessageDialog(frame, "Ошибка подключения к базе данных: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
                logger.severe("Ошибка подключения к базе данных: " + e.getMessage());
            }
        }
    }

    /**
     * Отображение диалога авторизации.
     * @return true, если авторизация успешна, иначе false.
     */
    private static boolean showLoginDialog() {
        JPanel panel = new JPanel(new GridLayout(3, 2));
        JLabel userLabel = new JLabel("Логин:");
        JTextField userField = new JTextField();
        JLabel passLabel = new JLabel("Пароль:");
        JPasswordField passField = new JPasswordField();

        panel.add(userLabel);
        panel.add(userField);
        panel.add(passLabel);
        panel.add(passField);

        JOptionPane optionPane = new JOptionPane(panel, JOptionPane.PLAIN_MESSAGE, JOptionPane.OK_CANCEL_OPTION);
        JDialog dialog = optionPane.createDialog("Вход в систему");
        dialog.setModalityType(Dialog.ModalityType.APPLICATION_MODAL);
        dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

        while (true) {
            dialog.setVisible(true);

            int result = (Integer) optionPane.getValue();
            if (result == JOptionPane.OK_OPTION) {
                String username = userField.getText();
                String password = new String(passField.getPassword());
                if ("admin".equals(username) && "admin".equals(password)) {
                    logger.info("Авторизация успешно пройдена");
                    return true;
                } else {
                    JOptionPane.showMessageDialog(dialog, "Неправильный логин или пароль.",
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                    logger.info("Логин или пароль неправильно введены");
                    // Очистка полей логина и пароля
                    userField.setText("");
                    passField.setText("");
                }
            } else {
                return false;
            }
        }
    }

    /**
     * Подключение к базе данных.
     */
    private static void connectToDatabase() {
        String url = "jdbc:sqlite:cinema.db";
        try {
            dbConnection = DriverManager.getConnection(url);
            logger.info("Соединение с базой данных успешно установлено");
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(null, "Ошибка подключения к базе данных: " + e.getMessage(),
                    "Ошибка", JOptionPane.ERROR_MESSAGE);
            logger.severe("Ошибка подключения к базе данных: " + e.getMessage());
        }
    }

    /**
     * Загрузка данных из базы данных в таблицу.
     * @param tableName Имя таблицы в базе данных.
     * @param table Таблица для отображения данных.
     */
    private static void loadDataFromDatabase(String tableName, JTable table) {
        try (Statement stmt = dbConnection.createStatement()) {
            ResultSet rs = stmt.executeQuery("SELECT * FROM " + tableName);
            ResultSetMetaData metaData = rs.getMetaData();

            // Установка модели таблицы с динамическими колонками
            DefaultTableModel model = (DefaultTableModel) table.getModel();
            model.setColumnCount(0);
            model.setRowCount(0);
            int columnCount = metaData.getColumnCount();
            for (int i = 1; i <= columnCount; i++) {
                model.addColumn(metaData.getColumnName(i));
            }

            while (rs.next()) {
                Object[] rowData = new Object[columnCount];
                for (int i = 0; i < columnCount; i++) {
                    rowData[i] = rs.getObject(i + 1);
                }
                model.addRow(rowData);
            }
            logger.info("Данные из таблицы " + tableName + " успешно загружены");
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(null, "Ошибка загрузки данных из таблицы " + tableName + ": " + e.getMessage(),
                    "Ошибка", JOptionPane.ERROR_MESSAGE);
            logger.warning("Ошибка загрузки данных из таблицы " + tableName + ": " + e.getMessage());
        }
    }

    /**
     * Получение следующего ID для новой записи в таблице.
     * @param tableName Имя таблицы.
     * @return Следующий ID.
     * @throws SQLException Если происходит ошибка при выполнении SQL-запроса.
     */
    private static int getNextId(String tableName) throws SQLException {
        String idColumn = getIdColumnName(tableName);
        if (idColumn != null) {
            try (Statement stmt = dbConnection.createStatement();
                 ResultSet rs = stmt.executeQuery("SELECT MAX(" + idColumn + ") FROM " + tableName)) {
                if (rs.next()) {
                    return rs.getInt(1) + 1;
                }
            }
        }
        return 1; // Если таблица пуста или столбец не найден, начинаем с 1
    }

    /**
     * Получение имени столбца ID для таблицы.
     * @param tableName Имя таблицы.
     * @return Имя столбца ID.
     * @throws SQLException Если происходит ошибка при выполнении SQL-запроса.
     */
    private static String getIdColumnName(String tableName) throws SQLException {
        try (Statement stmt = dbConnection.createStatement();
             ResultSet rs = stmt.executeQuery("PRAGMA table_info(" + tableName + ")")) {
            while (rs.next()) {
                String columnName = rs.getString("name");
                if (columnName.matches(".*_id")) {
                    return columnName;
                }
            }
        }
        return null;
    }

    /**
     * Извлечение числа из строки в скобках.
     * @param input Входная строка.
     * @return Число в скобках или null, если число не найдено.
     */
    private static String extractNumberFromString(String input) {
        Pattern pattern = Pattern.compile("\\((\\d+)\\)");  // Регулярное выражение для числа в скобках
        Matcher matcher = pattern.matcher(input);
        if (matcher.find()) {
            return matcher.group(1);  // Возвращаем только число
        }
        return null;  // Если числа в скобках нет, возвращаем null
    }

    /**
     * Обработчик для кнопки "Добавить данные".
     * @param tabIndex Индекс текущей вкладки.
     * @param frame Главное окно приложения.
     */
    private static void handleAddData(int tabIndex, JFrame frame) {
        String tableName = getTableNameByTabIndex(tabIndex);
        if (tableName == null) {
            JOptionPane.showMessageDialog(null, "Неизвестная вкладка.", "Ошибка", JOptionPane.ERROR_MESSAGE);
            return;
        }

        JPanel inputPanel = new JPanel(new GridLayout(0, 2));
        try (Statement stmt = dbConnection.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM " + tableName + " LIMIT 1")) {
            ResultSetMetaData metaData = rs.getMetaData();
            int columnCount = metaData.getColumnCount();
            JTextField[] inputFields = new JTextField[columnCount - 1]; // Исключаем поле id

            for (int i = 2; i <= columnCount; i++) {
                String columnName = metaData.getColumnName(i);
                inputPanel.add(new JLabel(getUserFriendlyName(columnName)));

                if (columnName.equalsIgnoreCase("rep_id") || columnName.equalsIgnoreCase("movie_id") || columnName.equalsIgnoreCase("session_id")) {
                    JComboBox<String> comboBox = new JComboBox<>();
                    loadComboBoxData(comboBox, columnName);
                    inputPanel.add(comboBox);
                } else {
                    inputFields[i - 2] = new JTextField();
                    inputPanel.add(inputFields[i - 2]);
                }
            }

            JOptionPane optionPane = new JOptionPane(inputPanel, JOptionPane.PLAIN_MESSAGE, JOptionPane.OK_CANCEL_OPTION);
            JDialog dialog = optionPane.createDialog("Добавить запись в " + tableName);
            dialog.setModalityType(Dialog.ModalityType.APPLICATION_MODAL);
            dialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);

            while (true) {
                dialog.setVisible(true);

                int result = (Integer) optionPane.getValue();
                if (result == JOptionPane.OK_OPTION) {
                    String[] rowData = new String[columnCount];
                    int nextId = getNextId(tableName);
                    rowData[0] = String.valueOf(nextId);

                    for (int i = 1; i < columnCount; i++) {
                        Component fieldComponent = inputPanel.getComponent(i * 2 - 1); // Поле ввода/выбор

                        if (fieldComponent instanceof JTextField) {
                            JTextField textField = (JTextField) fieldComponent;
                            rowData[i] = textField.getText();  // Получаем текст из поля ввода
                        } else if (fieldComponent instanceof JComboBox) {
                            JComboBox<?> comboBox = (JComboBox<?>) fieldComponent;
                            try {
                                rowData[i] = comboBox.getSelectedItem() != null ? comboBox.getSelectedItem().toString() : null;  // Получаем выбранное значение из JComboBox
                            } catch (Exception e) {
                                System.out.println("Ошибка при получении значения из JComboBox: " + e.getMessage());
                            }
                        }
                    }

                    // Проверка валидности данных
                    try {
                        validateData(tableName, rowData);
                        if (!tempData.containsKey(tableName)) {
                            tempData.put(tableName, new ArrayList<>());
                        }
                        tempData.get(tableName).add(rowData);

                        // Отладочные сообщения
                        StringBuilder debugMessage = new StringBuilder("Добавление строки в модель таблицы " + tableName + ": ");
                        for (String data : rowData) {
                            debugMessage.append(data).append(", ");
                        }
                        logger.info(debugMessage.toString());

                        DefaultTableModel model = getTableModelByTabIndex(tabIndex);
                        model.addRow(rowData);
                        logger.info("Запись успешно добавлена в интерфейс таблицы " + tableName);
                        saveDataToDatabase(frame);

                        break; // Выход из цикла, если данные корректны
                    } catch (Exception e) {
                        JOptionPane.showMessageDialog(dialog, e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
                    }
                } else {
                    break; // Выход из цикла, если пользователь отменил ввод
                }
            }
        } catch (SQLException e) {
            logger.warning("Ошибка добавления записи в интерфейс таблицы " + tableName + ": " + e.getMessage());
            JOptionPane.showMessageDialog(null, "Ошибка добавления записи: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Сохранение данных в базу данных.
     * @param frame Главное окно приложения.
     */
    private static void saveDataToDatabase(JFrame frame) {
        try (Statement stmt = dbConnection.createStatement()) {
            for (Map.Entry<String, List<String[]>> entry : tempData.entrySet()) {
                String tableName = entry.getKey();
                for (String[] values : entry.getValue()) {
                    System.out.println(values);
                    saveRowToDatabase(tableName, values, stmt);
                }
            }
            tempData.clear(); // Очищаем временный список после сохранения
            saveEditedRowsToDatabase(frame);
            logger.info("Данные успешно сохранены в базу данных");
            JOptionPane.showMessageDialog(frame, "Данные успешно сохранены!", "Успех", JOptionPane.INFORMATION_MESSAGE);
        } catch (SQLException e) {
            logger.warning("Ошибка сохранения данных в базу данных: " + e.getMessage());
            JOptionPane.showMessageDialog(frame, "Ошибка сохранения данных: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Сохранение строки в базу данных.
     * @param tableName Имя таблицы.
     * @param values Значения для сохранения.
     * @param stmt Statement для выполнения SQL-запросов.
     * @throws SQLException Если происходит ошибка при выполнении SQL-запроса.
     */
    private static void saveRowToDatabase(String tableName, String[] values, Statement stmt) throws SQLException {
        System.out.println(values);
        StringBuilder query = new StringBuilder("INSERT INTO " + tableName + " (");
        ResultSet rs = stmt.executeQuery("SELECT * FROM " + tableName + " LIMIT 1");
        ResultSetMetaData metaData = rs.getMetaData();
        int columnCount = metaData.getColumnCount();

        // Формируем список столбцов, исключая автоинкрементируемое поле
        for (int i = 2; i <= columnCount; i++) { // Начинаем с 1, чтобы включить все столбцы
            String columnName = metaData.getColumnName(i);
            query.append(columnName);
            if (i < columnCount) query.append(", ");
        }
        query.append(") VALUES (");
        for (int i = 1; i < values.length; i++) {
            System.out.println(values[1]);
            query.append("'").append(values[i]).append("'");
            if (i < values.length - 1) query.append(", ");
        }
        query.append(")");
        System.out.println(query.toString()); // Для отладки
        stmt.executeUpdate(query.toString());
    }

    /**
     * Обработчик для кнопки "Удалить данные".
     * @param frame Главное окно приложения.
     */
    private static void handleDeleteData(JFrame frame) {
        JTabbedPane tabbedPane = (JTabbedPane) frame.getContentPane().getComponent(1);
        int selectedIndex = tabbedPane.getSelectedIndex();
        Component component = tabbedPane.getComponentAt(selectedIndex);
        if (component instanceof JPanel) {
            JPanel panel = (JPanel) component;
            JScrollPane scrollPane = (JScrollPane) panel.getComponent(0); // Предполагаем, что JScrollPane первый компонент в JPanel
            JTable selectedTable = (JTable) scrollPane.getViewport().getView();
            String tableName = getTableNameByTabIndex(selectedIndex);

            // Устанавливаем текущую таблицу
            setCurrentTable(selectedTable, tableName);

            if (currentTable == null || currentTable.getSelectedRow() == -1) {
                JOptionPane.showMessageDialog(null, "Выберите строку для удаления.", "Ошибка", JOptionPane.ERROR_MESSAGE);
                return;
            }

            int selectedRow = currentTable.getSelectedRow();
            String idColumn = null;
            switch (tableName) {
                case "movies":
                    idColumn = "movie_id";
                    break;
                case "rep":
                    idColumn = "rep_id";
                    break;
                case "sessions":
                    idColumn = "session_id";
                    break;
                case "tickets":
                    idColumn = "ticket_id";
                    break;
            }

            String id = currentTable.getValueAt(selectedRow, 0).toString(); // Предполагается, что ID в первом столбце

            int confirm = JOptionPane.showConfirmDialog(null,
                    "Вы уверены, что хотите удалить запись с ID: " + id + "?",
                    "Подтверждение удаления", JOptionPane.YES_NO_OPTION);

            if (confirm != JOptionPane.YES_OPTION) {
                return;
            }

            try (Statement stmt = dbConnection.createStatement()) {
                int rowsAffected = stmt.executeUpdate("DELETE FROM " + tableName + " WHERE " + idColumn + " = " + id);

                if (rowsAffected > 0) {
                    JOptionPane.showMessageDialog(null, "Запись успешно удалена.", "Успех", JOptionPane.INFORMATION_MESSAGE);
                    ((DefaultTableModel) currentTable.getModel()).removeRow(selectedRow); // Удаляем строку из таблицы
                    logger.info("Запись с ID " + id + " удалена из таблицы " + tableName);
                } else {
                    JOptionPane.showMessageDialog(null, "Запись с указанным ID не найдена.", "Ошибка", JOptionPane.ERROR_MESSAGE);
                    logger.warning("Запись с ID " + id + " не найдена в таблице " + tableName);
                }
            } catch (SQLException e) {
                logger.warning("Ошибка удаления записи из таблицы " + tableName + ": " + e.getMessage());
                JOptionPane.showMessageDialog(null, "Ошибка удаления записи: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
            }
        } else {
            JOptionPane.showMessageDialog(frame, "Неверный компонент вкладки.", "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    /**
     * Вспомогательный метод для обновления текущей таблицы.
     * @param table Таблица для обновления.
     * @param tableName Имя таблицы.
     */
    public static void setCurrentTable(JTable table, String tableName) {
        currentTable = table;
        currentTable.putClientProperty("tableName", tableName);
    }

    /**
     * Получение имени таблицы по индексу вкладки.
     * @param tabIndex Индекс вкладки.
     * @return Имя таблицы или null, если вкладка неизвестна.
     */
    private static String getTableNameByTabIndex(int tabIndex) {
        switch (tabIndex) {
            case 0: return "movies";
            case 1: return "rep";
            case 2: return "sessions";
            case 3: return "tickets";
            default: return null;
        }
    }

    /**
     * Получение модели таблицы по индексу вкладки.
     * @param tabIndex Индекс вкладки.
     * @return Модель таблицы или null, если вкладка неизвестна.
     */
    private static DefaultTableModel getTableModelByTabIndex(int tabIndex) {
        switch (tabIndex) {
            case 0: return filmsModel;
            case 1: return repertoireModel;
            case 2: return sessionsModel;
            case 3: return ticketsModel;
            default: return null;
        }
    }

    /**
     * Получение пользовательского имени столбца.
     * @param columnName Имя столбца.
     * @return Пользовательское имя столбца.
     */
    private static String getUserFriendlyName(String columnName) {
        switch (columnName.toLowerCase()) {
            case "movie_name": return "Название фильма";
            case "director": return "Режиссер";
            case "movie_year": return "Год выпуска";
            case "genre": return "Жанр";
            case "date": return "Дата";
            case "time": return "Время";
            case "host": return "Зал";
            case "movie_id": return "ID фильма";
            case "rep_id": return "ID репертуара";
            case "numb_seats": return "Количество мест";
            case "numb_sold_tickets": return "Количество проданных билетов";
            case "ticket_id": return "ID билета";
            case "session_id": return "ID сеанса";
            case "numb_of_seat": return "Номер места";
            case "status": return "Статус";
            case "date_bought": return "Дата покупки";
            default: return columnName;
        }
    }

    // Исключения для проверки валидности данных
    private static class InvalidFindException extends Exception {
        public InvalidFindException(String message) {
            super(message);
        }
    }

    private static class InvalidDateException extends Exception {
        public InvalidDateException(String message) {
            super(message);
        }
    }

    private static class InvalidTimeException extends Exception {
        public InvalidTimeException(String message) {
            super(message);
        }
    }

    private static class InvalidTicketCountException extends Exception {
        public InvalidTicketCountException(String message) {
            super(message);
        }
    }

    // Методы проверки валидности данных
    private static void checkName(JTextField bName) throws InvalidFindException, NullPointerException {
        String sName = bName.getText();
        if (sName.contains("Ключевое слово"))
            throw new InvalidFindException("Вы не ввели слова для поиска");
        if (sName.isEmpty()) throw new NullPointerException();
    }

    private static void checkDate(String date) throws InvalidDateException {
        if (!date.matches("\\d{2}\\.\\d{2}\\.\\d{4}")) {
            throw new InvalidDateException("Неверный формат даты! Ожидается формат: дд.мм.гггг.");
        }
    }

    private static void checkTime(String time) throws InvalidTimeException {
        if (!time.matches("\\d{2}:\\d{2}")) {
            throw new InvalidTimeException("Неверный формат времени! Ожидается формат: чч:мм.");
        }
    }

    private static void checkTicketCount(String ticketsSold) throws InvalidTicketCountException, NumberFormatException {
        try {
            int count = Integer.parseInt(ticketsSold);
            if (count < 0) {
                throw new InvalidTicketCountException("Количество проданных билетов не может быть отрицательным!");
            }
        } catch (NumberFormatException ex) {
            throw new InvalidTicketCountException("Количество проданных билетов должно быть числом!");
        }
    }

    // Метод для проверки валидности данных перед добавлением
    private static void validateData(String tableName, String[] rowData) throws Exception {
        for (int i = 1; i < rowData.length; i++) {
            String columnName = getColumnNameByIndex(tableName, i);
            String value = rowData[i];
            switch (columnName.toLowerCase()) {
                case "date":
                    checkDate(value);
                    break;
                case "time":
                    checkTime(value);
                    break;
                case "numb_sold_tickets":
                    checkTicketCount(value);
                    break;
                // Добавьте другие проверки по мере необходимости
            }
        }
    }

    // Вспомогательный метод для получения имени столбца по индексу
    private static String getColumnNameByIndex(String tableName, int index) throws SQLException {
        try (Statement stmt = dbConnection.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM " + tableName + " LIMIT 1")) {
            ResultSetMetaData metaData = rs.getMetaData();
            return metaData.getColumnName(index + 1); // Индексы столбцов начинаются с 1
        }
    }

    // Метод для выполнения поиска
    private static void performSearch(JFrame frame) {
        JTabbedPane tabbedPane = (JTabbedPane) frame.getContentPane().getComponent(1);
        int selectedIndex = tabbedPane.getSelectedIndex();
        String tableName = getTableNameByTabIndex(selectedIndex);
        String searchField = director.getSelectedItem() != null ? director.getSelectedItem().toString() : "";
        String searchKeyword = wordToFind.getText();

        if (tableName == null || searchField.isEmpty() || searchKeyword.isEmpty()) {
            JOptionPane.showMessageDialog(frame, "Выберите поле для поиска и введите ключевое слово.", "Ошибка", JOptionPane.ERROR_MESSAGE);
            return;
        }

        try (Statement stmt = dbConnection.createStatement()) {
            String query = "SELECT * FROM " + tableName + " WHERE " + searchField + " LIKE ?";
            PreparedStatement pstmt = dbConnection.prepareStatement(query);
            pstmt.setString(1, "%" + searchKeyword + "%");
            ResultSet rs = pstmt.executeQuery();

            DefaultTableModel model = getTableModelByTabIndex(selectedIndex);
            model.setRowCount(0); // Очищаем текущую таблицу

            ResultSetMetaData metaData = rs.getMetaData();
            int columnCount = metaData.getColumnCount();

            while (rs.next()) {
                Object[] rowData = new Object[columnCount];
                for (int i = 0; i < columnCount; i++) {
                    rowData[i] = rs.getObject(i + 1);
                }
                model.addRow(rowData);
            }

            JOptionPane.showMessageDialog(frame, "Поиск завершен.", "Успех", JOptionPane.INFORMATION_MESSAGE);
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(frame, "Ошибка выполнения поиска: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
            logger.warning("Ошибка выполнения поиска: " + e.getMessage());
        }
    }

    // Метод для обновления полей поиска при смене вкладки
    private static void updateSearchFields(int selectedIndex) {
        String tableName = getTableNameByTabIndex(selectedIndex);
        if (tableName == null) {
            return;
        }

        try (Statement stmt = dbConnection.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT * FROM " + tableName + " LIMIT 1")) {
            ResultSetMetaData metaData = rs.getMetaData();
            int columnCount = metaData.getColumnCount();
            String[] columnNames = new String[columnCount];

            for (int i = 1; i <= columnCount; i++) {
                columnNames[i - 1] = metaData.getColumnName(i);
            }

            director.setModel(new DefaultComboBoxModel<>(columnNames));
        } catch (SQLException e) {
            JOptionPane.showMessageDialog(null, "Ошибка обновления полей поиска: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
            logger.warning("Ошибка обновления полей поиска: " + e.getMessage());
        }
    }

    // Метод для обновления редактирования таблицы
    private static void updateTableEditing(boolean isEditingEnabled) {
        filmsModel.fireTableDataChanged();
        repertoireModel.fireTableDataChanged();
        sessionsModel.fireTableDataChanged();
        ticketsModel.fireTableDataChanged();
    }

    // Метод для загрузки данных в раскрывающийся список
    private static void loadComboBoxData(JComboBox<String> comboBox, String columnName) {
        try (Statement stmt = dbConnection.createStatement()) {
            String query = "";
            if (columnName.equalsIgnoreCase("movie_id")) {
                query = "SELECT movie_name, movie_id FROM movies";
            } else if (columnName.equalsIgnoreCase("rep_id")) {
                query = "SELECT rep_id FROM rep";
            } else if (columnName.equalsIgnoreCase("session_id")) {
                query = "SELECT session_id FROM sessions";
            }
            ResultSet rs = stmt.executeQuery(query);
            while (rs.next()) {
                String item = columnName.equalsIgnoreCase("movie_id") ?
                        rs.getString("movie_name") + " (" + rs.getString("movie_id") + ")" :
                        rs.getString(1);
                comboBox.addItem(item);
                System.out.println("Добавлен элемент в JComboBox: " + item);  // Логирование
            }
        } catch (SQLException e) {
            logger.warning("Ошибка загрузки данных в раскрывающийся список: " + e.getMessage());
            JOptionPane.showMessageDialog(null, "Ошибка загрузки данных в раскрывающийся список: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    // Метод для сохранения измененных строк в базе данных
    private static void saveEditedRowsToDatabase(JFrame frame) {
        try (Statement stmt = dbConnection.createStatement()) {
            JTabbedPane tabbedPane = (JTabbedPane) frame.getContentPane().getComponent(1);
            int selectedIndex = tabbedPane.getSelectedIndex();
            String tableName = getTableNameByTabIndex(selectedIndex);
            DefaultTableModel model = getTableModelByTabIndex(selectedIndex);

            for (int row = 0; row < model.getRowCount(); row++) {
                String idColumn = null;
                switch (tableName) {
                    case "movies":
                        idColumn = "movie_id";
                        break;
                    case "rep":
                        idColumn = "rep_id";
                        break;
                    case "sessions":
                        idColumn = "session_id";
                        break;
                    case "tickets":
                        idColumn = "ticket_id";
                        break;
                }

                String id = model.getValueAt(row, 0).toString(); // Предполагается, что ID в первом столбце
                StringBuilder query = new StringBuilder("UPDATE " + tableName + " SET ");
                boolean isRowChanged = false;
                StringBuilder setClause = new StringBuilder(); // Для накопления изменений в строках

                for (int col = 1; col < model.getColumnCount(); col++) {
                    String columnName = model.getColumnName(col);
                    String value = model.getValueAt(row, col).toString();

                    // Сравнение текущего значения с оригинальным
                    if (!value.equals(getOriginalValue(tableName, id, columnName))) {
                        isRowChanged = true;
                        if (setClause.length() > 0) {
                            setClause.append(", "); // Добавляем запятую между парами "column = value"
                        }
                        setClause.append(columnName).append(" = '").append(value).append("'");
                    }
                }

                if (isRowChanged) {
                    // Формируем финальный запрос
                    query.append(setClause).append(" WHERE ").append(idColumn).append(" = ").append(id);
                    System.out.println(query.toString()); // Для отладки
                    stmt.executeUpdate(query.toString());
                }
            }

            logger.info("Измененные строки успешно сохранены в базе данных");
        } catch (SQLException e) {
            logger.warning("Ошибка сохранения измененных строк в базе данных: " + e.getMessage());
            JOptionPane.showMessageDialog(frame, "Ошибка сохранения измененных строк: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    private static String getOriginalValue(String tableName, String id, String columnName) throws SQLException {
        try (Statement stmt = dbConnection.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT " + columnName + " FROM " + tableName + " WHERE " + getIdColumn(tableName) + " = " + id)) {
            if (rs.next()) {
                return rs.getString(columnName);
            }
        }
        return null;
    }

    private static String getIdColumn(String tableName) {
        switch (tableName) {
            case "movies":
                return "movie_id";
            case "rep":
                return "rep_id";
            case "sessions":
                return "session_id";
            case "tickets":
                return "ticket_id";
            default:
                return null;
        }
    }
}
