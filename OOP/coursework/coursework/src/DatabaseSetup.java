import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.Statement;

public class DatabaseSetup {
    public static void main(String[] args) {
        // Путь к базе данных
        String url = "jdbc:sqlite:cinema.db";

        try (Connection conn = DriverManager.getConnection(url);
             Statement stmt = conn.createStatement()) {

            // SQL-запросы для создания таблиц
            String createMoviesTable = """
                CREATE TABLE IF NOT EXISTS movies (
                    movie_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    movie_name TEXT NOT NULL,
                    director TEXT,
                    movie_year INTEGER,
                    genre TEXT
                );
            """;

            String createRepertoireTable = """
                CREATE TABLE IF NOT EXISTS rep (
                    rep_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    movie_id INTEGER NOT NULL,
                    date DATE NOT NULL,
                    time TIME NOT NULL,
                    host INTEGER NOT NULL,
                    FOREIGN KEY (movie_id) REFERENCES Фильмы (movie_id)
                );
            """;

            String createSessionsTable = """
                CREATE TABLE IF NOT EXISTS sessions (
                    session_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    rep_id INTEGER NOT NULL,
                    numb_seats INTEGER NOT NULL,
                    numb_sold_tickets INTEGER DEFAULT 0,
                    FOREIGN KEY (rep_id) REFERENCES Репертуар (rep_id)
                );
            """;

            String createTicketsTable = """
                CREATE TABLE IF NOT EXISTS tickets (
                    ticket_id INTEGER PRIMARY KEY AUTOINCREMENT,
                    session_id INTEGER NOT NULL,
                    number_of_seat INTEGER NOT NULL,
                    status TEXT NOT NULL CHECK (status IN ('свободен', 'продан')),
                    date_bought DATETIME DEFAULT CURRENT_TIMESTAMP,
                    FOREIGN KEY (session_id) REFERENCES Сеансы (session_id)
                );
            """;

            // Выполнение запросов
            stmt.execute(createMoviesTable);
            stmt.execute(createRepertoireTable);
            stmt.execute(createSessionsTable);
            stmt.execute(createTicketsTable);

            System.out.println("Таблицы успешно созданы!");

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
