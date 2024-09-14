package edu.java.lab01;

import java.util.Random;
import java.util.Arrays;

/**
 * @author Baimuhamedov Rafael 3311
 */

public class lab_01 {

    /**
     * Main method of app. It generates an array of 10 random integers,
     * after that sorts them via bubble sort algorithm and displays it in
     * ascending and descending order.
     * @param args no arguments
     */
    public static void main(String[] args) {
        int[] Arr = new int[10];
        Random rand = new Random();

        for (int i = 0; i < Arr.length; i++) {
            Arr[i] = rand.nextInt(100);
        }

        System.out.println("Original array: " + Arrays.toString(Arr));
        bubble_sort(Arr, true);
        System.out.println("Array in ascending order: " + Arrays.toString(Arr));
        bubble_sort(Arr, false);
        System.out.println("Array in descending order: "+Arrays.toString(Arr));
    }

    /**
     * Sorts an array in both direction using bubble sort algorithm
     * The method compares two nearby elements and swap them if they take wrong place
     * @param array the array of integers, that should be sorted
     * @param direction true or false = ascending or descending order to sort the array
     */
    public static void bubble_sort(int[] array, boolean direction){
        int n = array.length;
        boolean swapped = false;
        for(int i=0; i<(n-1);i++){
            for(int j=0; j<(n-i-1);j++){
                if((direction && array[j]>array[j+1]) || (!direction && array[j]<array[j+1])){
                    int temp = array[j+1];
                    array[j+1] = array[j];
                    array[j] = temp;
                    swapped = true;
                }
            }
            if(!swapped){
                break;
            }
        }
    }
    }