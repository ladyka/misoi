package by.vurtatoo.misoi;

public class GaussianCalculator {
    
    
    public static double[][] Calculate(int lenght, double weight) {
        double[][] Kernel = new double[lenght][lenght];
        double sumTotal = 0;

        int kernelRadius = lenght / 2;
        double distance = 0;

        double calculatedEuler = 1.0 / (2.0 * Math.PI * Math.pow(weight, 2));

        for (int filterY = -kernelRadius; filterY <= kernelRadius; filterY++) {
            for (int filterX = -kernelRadius; filterX <= kernelRadius; filterX++) {
                distance = ((filterX * filterX) + (filterY * filterY)) / (2 * (weight * weight));

                Kernel[filterY + kernelRadius][filterX + kernelRadius] = calculatedEuler * Math.exp(-distance);

                sumTotal += Kernel[filterY + kernelRadius][filterX + kernelRadius];
            }
        }

        for (int y = 0; y < lenght; y++) {
            for (int x = 0; x < lenght; x++) {
                Kernel[y][x] = Kernel[y][x] * (1.0 / sumTotal);
            }
        }

        return Kernel;
    }
}
