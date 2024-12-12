package edu.java.lab09;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;


public class CinemaListTest {
    private CinemaList cinemaList;

    @Before
    public void setUp() {
        cinemaList = new CinemaList();
        cinemaList.initializeComponents();
    }

    @Test
    public void testintt() {
        cinemaList.getWordToFind().setText("Введите данные");
        Assert.assertTrue(cinemaList.intt());
    }

    @Test
    public void testwert() {
        Assert.assertTrue(cinemaList.wert());
    }
}
