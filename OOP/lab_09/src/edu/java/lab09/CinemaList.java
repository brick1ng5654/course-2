package edu.java.lab09;

import java.awt.*;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.event.*;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.io.FileReader;
import java.awt.FileDialog;
import java.io.File;
import java.io.FileOutputStream;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Element;
import org.xml.sax.SAXException;
import org.junit.Assert;
import org.junit.Test;

public class CinemaList {
    private JFrame cinemaList;
    private DefaultTableModel modelMovies, modelSessions;
    private JButton save, add, delete, upload, print, edit, saveXML, loadXML, saveHTML, savePDF;
    private JToolBar toolBar;
    private JScrollPane scrollMovies, scrollSessions;
    private JTable tableMovies, tableSessions;
    private JComboBox director;
    private JTextField wordToFind;
    private JButton filter;
    private String fileNameOpen = null;

    public void show() {
        initializeComponents();
        cinemaList.setVisible(true);
    }

    public void initializeComponents() {
        cinemaList = new JFrame("Список фильмов");
        cinemaList.setSize(1000, 600);
        cinemaList.setLocation(100, 100);
        cinemaList.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        cinemaList.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                int confirm = JOptionPane.showConfirmDialog(cinemaList, "Вы уверены, что хотите выйти?");
                if (confirm == JOptionPane.YES_OPTION) {
                    cinemaList.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                } else {
                    cinemaList.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
                }
            }
        });

        toolBar = new JToolBar("Панель инструментов");
        cinemaList.setLayout(new BorderLayout());
        cinemaList.add(toolBar, BorderLayout.NORTH);

        save = new JButton(new ImageIcon("src/img/save.png"));
        save.setToolTipText("Сохранить данные");
        toolBar.add(save);
        save.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                saveToFile();
            }
        });

        add = new JButton(new ImageIcon("src/img/add.png"));
        add.setToolTipText("Добавить данные");
        toolBar.add(add);

        delete = new JButton(new ImageIcon("src/img/delete.png"));
        delete.setToolTipText("Удалить данные");
        toolBar.add(delete);

        upload = new JButton(new ImageIcon("src/img/upload.png"));
        upload.setToolTipText("Загрузить данные");
        toolBar.add(upload);
        upload.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadFromFile();
            }
        });

        edit = new JButton(new ImageIcon("src/img/edit.png"));
        edit.setToolTipText("Редактировать данные");
        toolBar.add(edit);

        saveXML = new JButton(new ImageIcon("src/img/xml_save.png"));
        saveXML.setToolTipText("Сохранить данные в XML");
        toolBar.add(saveXML);
        saveXML.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                saveToXML();
            }
        });

        loadXML = new JButton(new ImageIcon("src/img/xml_upload.png"));
        loadXML.setToolTipText("Загрузить данные из XML");
        toolBar.add(loadXML);
        loadXML.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                loadFromXML();
            }
        });

        savePDF = new JButton(new ImageIcon("src/img/pdf_upload.png"));
        savePDF.setToolTipText("Сохранить данные в PDF-отчёт");
        toolBar.add(savePDF);

        saveHTML = new JButton(new ImageIcon("src/img/html_upload.png"));
        saveHTML.setToolTipText("Сохранить данные в HTML-отчёт");
        toolBar.add(saveHTML);

        String [] columnsMovies = {"Год", "Фильм", "Режиссёр", "Жанр"};
        String [][] dataMovies = {};
        modelMovies=  new DefaultTableModel(dataMovies, columnsMovies);
        tableMovies = new JTable(modelMovies);
        scrollMovies = new JScrollPane(tableMovies);

        String [] columnsSessions = {"Фильм", "Дата", "Время", "Продано билетов"};
        String [][] dataSessions = {};
        modelSessions=  new DefaultTableModel(dataSessions, columnsSessions);
        tableSessions = new JTable(modelSessions);
        scrollSessions = new JScrollPane(tableSessions);

        JTabbedPane tabbedPane = new JTabbedPane();
        tabbedPane.addTab("Фильмы", scrollMovies);
        tabbedPane.addTab("Сеансы", scrollSessions);
        cinemaList.add(tabbedPane, BorderLayout.CENTER);

        director = new JComboBox(new String[]{"Фильмы", "Сеансы"});

        wordToFind = new JTextField("Ключевое слово", 20);
        wordToFind.addFocusListener(new FocusAdapter() {
            public void focusGained(FocusEvent e) {
                if (wordToFind.getText().equals("Ключевое слово")) {
                    wordToFind.setText("");
                }
            }
            public void focusLost(FocusEvent e) {
                if (wordToFind.getText().isEmpty()) {
                    wordToFind.setText("Ключевое слово");
                }
            }
        });

        filter = new JButton("Поиск");
        filter.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent arg0) {
                try {
                    checkName(wordToFind);
                }
                catch(NullPointerException ex) {
                    JOptionPane.showMessageDialog(cinemaList, ex.toString());
                }
                catch(InvalidFindException myEx) {
                    JOptionPane.showMessageDialog(null, myEx.getMessage());
                }
            }
        });

        JPanel filterPanel = new JPanel();
        filterPanel.add(director);
        filterPanel.add(wordToFind);
        filterPanel.add(filter);
        cinemaList.add(filterPanel, BorderLayout.SOUTH);

        add.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (tabbedPane.getSelectedIndex() == 0) {
                    addMovie();
                } else if (tabbedPane.getSelectedIndex() == 1) {
                    addSession();
                }
            }
        });
        delete.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                int selectedTab = tabbedPane.getSelectedIndex();

                if (selectedTab == 0) {
                    int selectedRow = tableMovies.getSelectedRow();
                    if (selectedRow != -1) {
                        modelMovies.removeRow(selectedRow);
                    } else {
                        JOptionPane.showMessageDialog(cinemaList, "Пожалуйста, выберите фильм для удаления.");
                    }
                } else if (selectedTab == 1) {
                    int selectedRow = tableSessions.getSelectedRow();
                    if (selectedRow != -1) {
                        modelSessions.removeRow(selectedRow);
                    } else {
                        JOptionPane.showMessageDialog(cinemaList, "Пожалуйста, выберите сеанс для удаления.");
                    }
                }
            }
        });
    }

    private class InvalidFindException extends Exception {
        public InvalidFindException(String message) {
            super (message);
        }
    }
    private void checkName (JTextField bName) throws InvalidFindException,NullPointerException {
        String sName = bName.getText();
        if (sName.contains("Ключевое слово"))
            throw new InvalidFindException("Вы не ввели слова для поиска");
        if (sName.isEmpty()) throw new NullPointerException();
    }

    class InvalidDateException extends Exception {
        public InvalidDateException(String message) {
            super(message);
        }
    }
    private void checkDate(String date) throws InvalidDateException {
        if (!date.matches("\\d{2}\\.\\d{2}\\.\\d{4}")) {
            throw new InvalidDateException("Неверный формат даты! Ожидается формат: дд.мм.гггг.");
        }
    }

    class InvalidTimeException extends Exception {
        public InvalidTimeException(String message) {
            super(message);
        }
    }
    private void checkTime(String time) throws InvalidTimeException {
        if (!time.matches("\\d{2}:\\d{2}")) {
            throw new InvalidTimeException("Неверный формат времени! Ожидается формат: чч:мм.");
        }
    }

    class InvalidTicketCountException extends Exception {
        public InvalidTicketCountException(String message) {
            super(message);
        }
    }
    private void checkTicketCount(String ticketsSold) throws InvalidTicketCountException, NumberFormatException {
        try {
            int count = Integer.parseInt(ticketsSold);
            if (count < 0) {
                throw new InvalidTicketCountException("Количество проданных билетов не может быть отрицательным!");
            }
        } catch (NumberFormatException ex) {
            throw new InvalidTicketCountException("Количество проданных билетов должно быть числом!");
        }
    }

    private void addMovie() {
        JPanel inputPanel = new JPanel(new GridLayout(4, 2));
        JTextField yearField = new JTextField();
        JTextField movieField = new JTextField();
        JTextField directorField = new JTextField();
        JTextField genreField = new JTextField();

        inputPanel.add(new JLabel("Год:"));
        inputPanel.add(yearField);
        inputPanel.add(new JLabel("Фильм:"));
        inputPanel.add(movieField);
        inputPanel.add(new JLabel("Режиссер:"));
        inputPanel.add(directorField);
        inputPanel.add(new JLabel("Жанр:"));
        inputPanel.add(genreField);
        boolean flag = false;

        while(!flag){
            int result = JOptionPane.showConfirmDialog(cinemaList, inputPanel,
                    "Добавить новый фильм", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);

            if (result == JOptionPane.OK_OPTION) {
                if (yearField.getText().isEmpty() || movieField.getText().isEmpty() ||
                        directorField.getText().isEmpty() || genreField.getText().isEmpty()) {
                    JOptionPane.showMessageDialog(cinemaList, "Все поля должны быть заполнены",
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                } else {
                    modelMovies.addRow(new Object[]{yearField.getText(), movieField.getText(),
                            directorField.getText(), genreField.getText()});
                    flag = true;
                }
            } else {
                flag = true;
            }
        }
    }

    private void addSession() {
        JPanel inputPanel = new JPanel(new GridLayout(4, 2));
        JTextField movieField = new JTextField();
        JTextField dateField = new JTextField();
        JTextField timeField = new JTextField();
        JTextField ticketsSoldField = new JTextField();

        inputPanel.add(new JLabel("Фильм:"));
        inputPanel.add(movieField);
        inputPanel.add(new JLabel("Дата:"));
        inputPanel.add(dateField);
        inputPanel.add(new JLabel("Время:"));
        inputPanel.add(timeField);
        inputPanel.add(new JLabel("Продано билетов:"));
        inputPanel.add(ticketsSoldField);
        boolean flag = false;

        while (!flag) {
            int result = JOptionPane.showConfirmDialog(cinemaList, inputPanel,
                    "Добавить новый сеанс", JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE);

            if (result == JOptionPane.OK_OPTION) {
                if (dateField.getText().isEmpty() || movieField.getText().isEmpty() ||
                        timeField.getText().isEmpty() || ticketsSoldField.getText().isEmpty()) {
                    JOptionPane.showMessageDialog(cinemaList, "Все поля должны быть заполнены",
                            "Ошибка", JOptionPane.ERROR_MESSAGE);
                } else {
                    try {
                        checkDate(dateField.getText());
                        checkTime(timeField.getText());
                        checkTicketCount(ticketsSoldField.getText());

                        modelSessions.addRow(new Object[]{
                                movieField.getText(),
                                dateField.getText(),
                                timeField.getText(),
                                ticketsSoldField.getText()
                        });
                        flag = true;

                    } catch (InvalidDateException | InvalidTimeException | InvalidTicketCountException ex) {
                        JOptionPane.showMessageDialog(cinemaList, ex.getMessage(),
                                "Ошибка", JOptionPane.ERROR_MESSAGE);
                    }
                }
            } else {
                flag = true;
            }
        }
    }

    private void saveToFile() {
        FileDialog saveDialog = new FileDialog(cinemaList, "Сохранение данных", FileDialog.SAVE);
        saveDialog.setFile("*.txt");
        saveDialog.setVisible(true);

        String directory = saveDialog.getDirectory();
        String filename = saveDialog.getFile();
        if (directory == null || filename == null) return;

        String filePath = directory + filename;

        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filePath))) {
            for (int i = 0; i < modelMovies.getRowCount(); i++) {
                for (int j = 0; j < modelMovies.getColumnCount(); j++) {
                    writer.write(modelMovies.getValueAt(i, j).toString());
                    writer.write(";");
                }
                writer.newLine();
            }
            writer.write("---");
            writer.newLine();
            for (int i = 0; i < modelSessions.getRowCount(); i++) {
                for (int j = 0; j < modelSessions.getColumnCount(); j++) {
                    writer.write(modelSessions.getValueAt(i, j).toString());
                    writer.write(";");
                }
                writer.newLine();
            }
            JOptionPane.showMessageDialog(cinemaList, "Данные успешно сохранены.");
        } catch (IOException ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Ошибка при сохранении файла.");
        }
    }

    private void loadFromFile() {
        FileDialog loadDialog = new FileDialog(cinemaList, "Загрузка данных", FileDialog.LOAD);
        loadDialog.setFile("*.txt");
        loadDialog.setVisible(true);

        String directory = loadDialog.getDirectory();
        String filename = loadDialog.getFile();
        if (directory == null || filename == null) return;

        String filePath = directory + filename;

        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            modelMovies.setRowCount(0);
            modelSessions.setRowCount(0);

            String line;
            boolean isSessionsTable = false;
            while ((line = reader.readLine()) != null) {
                if (line.equals("---")) {
                    isSessionsTable = true;
                    continue;
                }
                String[] data = line.split(";");
                if (!isSessionsTable) {
                    modelMovies.addRow(data);
                } else {
                    modelSessions.addRow(data);
                }
            }
            JOptionPane.showMessageDialog(cinemaList, "Данные успешно загружены.");
        } catch (FileNotFoundException ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Файл не найден.");
        } catch (IOException ex) {
            ex.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Ошибка при чтении файла.");
        }
    }

    private void saveToXML() {
        FileDialog saveXMLDialog = new FileDialog(cinemaList, "Сохранение данных в XML", FileDialog.SAVE);
        saveXMLDialog.setFile("*.xml");
        saveXMLDialog.setVisible(true);

        String directory = saveXMLDialog.getDirectory();
        String filename = saveXMLDialog.getFile();
        if (directory == null || filename == null) return;

        String filePath = directory + filename;

        try {
            Document doc = getDocument();
            Node booklist = doc.createElement("booklist");
            doc.appendChild(booklist);
            for (int i = 0; i < modelMovies.getRowCount(); i++) {
                Element book = doc.createElement("book");
                booklist.appendChild(book);
                book.setAttribute("author", (String) modelMovies.getValueAt(i, 2));
                book.setAttribute("nameBook", (String) modelMovies.getValueAt(i, 1));
                book.setAttribute("year", (String) modelMovies.getValueAt(i, 0));
                book.setAttribute("presence", (String) modelMovies.getValueAt(i, 3));
            }
            try {
                Transformer trans = TransformerFactory.newInstance().newTransformer();
                trans.setOutputProperty(OutputKeys.METHOD, "xml");
                trans.setOutputProperty(OutputKeys.INDENT, "yes");
                trans.transform(new DOMSource(doc), new StreamResult(new FileOutputStream(filePath)));
                JOptionPane.showMessageDialog(cinemaList, "Данные успешно сохранены в XML.");
            } catch (TransformerConfigurationException e) {
                e.printStackTrace();
            } catch (TransformerException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } catch (Exception e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Ошибка при сохранении XML файла.");
        }
    }

    private void loadFromXML() {
        FileDialog loadXMLDialog = new FileDialog(cinemaList, "Загрузка данных из XML", FileDialog.LOAD);
        loadXMLDialog.setFile("*.xml");
        loadXMLDialog.setVisible(true);

        String directory = loadXMLDialog.getDirectory();
        String filename = loadXMLDialog.getFile();
        if (directory == null || filename == null) return;

        String filePath = directory + filename;

        try {
            Document doc = getDocument(filePath);
            modelMovies.setRowCount(0);

            doc.getDocumentElement().normalize();
            NodeList nlBooks = doc.getElementsByTagName("book");
            for (int temp = 0; temp < nlBooks.getLength(); temp++) {
                Node elem = nlBooks.item(temp);
                NamedNodeMap attrs = elem.getAttributes();
                String author = attrs.getNamedItem("author").getNodeValue();
                String title = attrs.getNamedItem("nameBook").getNodeValue();
                String year = attrs.getNamedItem("year").getNodeValue();
                String genre = attrs.getNamedItem("presence").getNodeValue();
                modelMovies.addRow(new String[]{year, title, author, genre});
            }
            JOptionPane.showMessageDialog(cinemaList, "Данные успешно загружены из XML.");
        } catch (SAXException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Ошибка при чтении XML файла.");
        } catch (IOException e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Ошибка при чтении XML файла.");
        } catch (Exception e) {
            e.printStackTrace();
            JOptionPane.showMessageDialog(cinemaList, "Ошибка при чтении XML файла.");
        }
    }

    private static Document getDocument(String filePath) throws Exception {
        try {
            DocumentBuilderFactory f = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = f.newDocumentBuilder();
            return builder.parse(new File(filePath));
        } catch (Exception exception) {
            throw new Exception("XML parsing error!");
        }
    }

    private static Document getDocument() throws Exception {
        try {
            DocumentBuilderFactory f = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = f.newDocumentBuilder();
            return builder.newDocument();
        } catch (Exception exception) {
            throw new Exception("XML parsing error!");
        }
    }

    public boolean intt() {
        return "Введите данные".equals(wordToFind.getText());
    }

    public boolean wert() {
        return fileNameOpen == "fail";
    }

    public JTextField getWordToFind() {
        return wordToFind;
    }

    public String getFileNameOpen() {
        return fileNameOpen;
    }

    public static void main(String[] args) {
        new CinemaList().show();
    }
}
