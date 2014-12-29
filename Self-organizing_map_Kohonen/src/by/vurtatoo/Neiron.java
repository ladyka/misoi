/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package by.vurtatoo;

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
        List<Integer[]> a ;
        int[][] a = new int[1][2];
        int[] b = new int[Stream.of(a).flatMap(Stream::of).collect(Collectors.toList()).size()][]
        Stream.of(a).flatMap(Stream::of).collect(Collectors.toList()).toArray(b);
    }
    public void Learn(List<Bitmap> ListBtm)
		{
			
			
			List<double[]> listVector = ListBtm.ConvertAll(x => x.GetVector()).ToList();
			while(true)
			{
			var error = new List<double>();
			
			foreach(var source in listVector)
			{
				double[] y = calculate(source);
				List<double> d = Enumerable.Range(0,OutputNeuronCount)
					.Select(j => distanceEuclid(getW(j),source)*frec[j])
					.ToList();
				
				int dwin = d.IndexOf(d.Min());
				
				for(int j = 0 ;j < InputNeuronCount; j++)
				{
					_weights[j,dwin] += betta*source[j] - _weights[j,dwin];
				}
				
				
				frec[dwin]++;
				error.Add(distanceEuclid(getW(dwin), source));
			}
			
			if(error.Max() < Accuracy) break;
			}

		}
}
