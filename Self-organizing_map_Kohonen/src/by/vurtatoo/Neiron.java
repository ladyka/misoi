/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package by.vurtatoo;

import com.sun.javafx.Utils;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

/**
 *
 * @author user
 */
public class Neiron {
    
    final int w = 25;
    final int h = 25;
    
    private double[][] _weights;
    public double accuracy;

    public double betta;
    private int[] frec;
    private int ClassCount;
    private int InputNeuronCount;
    private int OutputNeuronCount;

    public Neiron(int countClass, int size) {
        InputNeuronCount = size;
        OutputNeuronCount = countClass;
        ClassCount = countClass;
        betta = 1;
        frec = new int[OutputNeuronCount];
        Arrays.fill(frec, 1);
        accuracy = 0.1;
        _weights = new double[InputNeuronCount][OutputNeuronCount];
    }

    public double getAccuracy() {
        return accuracy;
    }

    public void setAccuracy(double accuracy) {
        this.accuracy = accuracy;
    }
    
    public void learn(List<Letter> letters) {
        List<Integer[]>  listVectors = getVectors(letters);
        

        List<Double> errors = new ArrayList<>();
        while(true) {
            listVectors.stream().forEach((listVector) -> {
                //double[] y = calculate(listVector);
                List<Double> d = new ArrayList<>(OutputNeuronCount);
                for (int j = 0; j < OutputNeuronCount; j++) {
                    d.add(distanceEuclid(getW(j),listVector)*frec[j]);
                }
                int dwinIndex = getMinIndex(d);
                for(int j = 0 ;j < InputNeuronCount; j++) {
                    _weights[j][dwinIndex] += betta*listVector[j] - _weights[j][dwinIndex];
                }
                frec[dwinIndex]++;
                errors.add(distanceEuclid(getW(dwinIndex), listVector));
            });
            if (errors.get(getMaxIndex(errors)) < accuracy) 
                break;
        }
    }

    private List<Integer[]> getVectors(List<Letter> letters) {
        List<Integer[]> vectors = new ArrayList<>();
        letters.stream().forEach((letter) -> {
            letter.getArrayLetterViews().stream().forEach((a) -> {
                vectors.add(convertArray(a.getArray()));
            });
        });
        return vectors;
    }

    public Integer[] convertArray(int[][] arr) {
        Integer[] array;
        array = new Integer[w*h];
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                array[i*h+j] = arr[i][j];
            }
        }
        return array;
    }
    
    public double[] calculate(Integer[] source) {
        double[] y = new double[OutputNeuronCount];

        for (int j = 0; j < OutputNeuronCount; j++) {
            for (int i = 0; i < InputNeuronCount; i++) {
                y[j] += source[i] * _weights[i][j];
            }
        }

        return y;
    }
    
    /**
     * return Enumerable.Range(0,InputNeuronCount).Select(i => ).ToArray();
     */
    private double[] getW(int j) {
        double[] wtf1Question = new double[InputNeuronCount];
        for (int i = 0; i < InputNeuronCount; i++) {
            wtf1Question[i] = _weights[i][j];
        }
        return wtf1Question;
    }

    /**
     * return Math.Sqrt(vec1.Select((x,i) => Math.Pow(vec1[i]-vec2[i],2)).Sum());
     * @param w
     * @param listVector
     * @return 
     */
    private double distanceEuclid(double[] a, Integer[] b) {
        double returnValue = 0;
        if (a.length == b.length) {
            for (int i = 0; i < a.length; i++) {
                returnValue += Math.pow((a[i]-b[i]), 2);
            }
        } else {
            throw new RuntimeException("a.length != b.length");
        }
        return Math.sqrt(returnValue);
    }

    private int getMinIndex(List<Double> d) {
        int minIndex = 0;
        double minValue = d.get(minIndex);
        for (int i = 1; i < d.size(); i++) {
            if (minValue > d.get(i)) {
                minIndex = i;
                minValue = d.get(i);
            }
        }
        return minIndex;
    }
    
    private int getMaxIndex(List<Double> d) {
        int maxIndex = 0;
        double maxValue = d.get(maxIndex);
        for (int i = 1; i < d.size(); i++) {
            if (maxValue < d.get(i)) {
                maxIndex = i;
                maxValue = d.get(i);
            }
        }
        return maxIndex;
    }
    
}
