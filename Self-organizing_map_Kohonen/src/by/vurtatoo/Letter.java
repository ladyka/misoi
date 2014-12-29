/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package by.vurtatoo;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author user
 */
public class Letter {
    
    private String name;
    private List<ArrayLetterView> arrayLetterViews;

    public List<ArrayLetterView> getArrayLetterViews() {
        return arrayLetterViews;
    }

    public void addArrayLetterViews(ArrayLetterView arrayLetterView) {
        if (null == arrayLetterViews) {
            arrayLetterViews = new ArrayList<>();
        }
        arrayLetterViews.add(arrayLetterView);
    }
    
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    void addArrayLetterViews(int[][] letterMatrix,String imagePath) {
        ArrayLetterView arrayLetterView = new ArrayLetterView(letterMatrix,imagePath);
        addArrayLetterViews(arrayLetterView);
    }
    
    class ArrayLetterView{
        
        private int[][] array;

        private String imagePath;

        public String getImagePath() {
            return imagePath;
        }
        
        public int[][] getArray() {
            return array;
        }

        public ArrayLetterView(int[][] array,String imagePath) {
            this.array = array;
            this.imagePath = imagePath;
        }
    }
} 

