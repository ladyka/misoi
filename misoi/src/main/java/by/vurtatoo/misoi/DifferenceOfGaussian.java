package by.vurtatoo.misoi;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ImageObserver;
import java.awt.image.ImageProducer;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class DifferenceOfGaussian {

    File imageOriginal = null;
    
    public DifferenceOfGaussian(String imagePath) throws Exception {
        imageOriginal = new File(imagePath);
        if (!imageOriginal.isFile()) {
            throw new Exception("THIS IS BAD LINK");
        }
    }

    public String getOutPutImagePath(int matrixSize, double weight1, double weight2) throws IOException {
        File fileOut = new File("/tmp/lab3_" + System.currentTimeMillis() + ".png");
        BufferedImage bufferedImageOriginal = ImageIO.read(imageOriginal);
        try {
            BufferedImage bufferedImageOut = DifferenceOfGaussianFilter(bufferedImageOriginal, matrixSize, weight1, weight2); 
            ImageIO.write(bufferedImageOut, "png" , fileOut);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        
        return fileOut.getName();
    }
    
    

    public BufferedImage DifferenceOfGaussianFilter(BufferedImage sourceBufferedImage, int matrixSize, double weight1, double weight2)
    {
        BufferedImage bufferedImageOut = new BufferedImage(sourceBufferedImage.getWidth(), sourceBufferedImage.getHeight(), BufferedImage.TYPE_INT_ARGB);
        double[][] kernel1 = GaussianCalculator.Calculate(matrixSize, (weight1 > weight2 ? weight1 : weight2));
        double[][] kernel2 = GaussianCalculator.Calculate(matrixSize, (weight1 > weight2 ? weight2 : weight1));

        byte[] grayscaleBuffer = new byte[sourceBufferedImage.getWidth() * sourceBufferedImage.getHeight()];

        int widght = sourceBufferedImage.getWidth();
        int height = sourceBufferedImage.getHeight();
        
        
        for (int i = 0; i < widght; i++) {
            for (int j = 0; j < height; j++) {
                int rgb = sourceBufferedImage.getRGB(i, j);
                rgb = getGray(rgb);
                grayscaleBuffer[i*height+j] = (byte) rgb;
            }
        }
        
        int color1 = 0;
        int color2 = 0;

        int filterOffset = (matrixSize - 1) / 2;
        int calcOffset = 0;

        for (int source = 0; source < grayscaleBuffer.length; source++)
        {
            color1 = 0;
            color2 = 0;

            for (int filterY = -filterOffset; filterY <= filterOffset; filterY++)
            {
                for (int filterX = -filterOffset; filterX <= filterOffset; filterX++)
                {

                    calcOffset = source + (filterX) + (filterY * sourceBufferedImage.getWidth());

                    calcOffset = (calcOffset < 0 ? 0 : (calcOffset >= grayscaleBuffer.length ? grayscaleBuffer.length - 1 : calcOffset));

                    color1 += (grayscaleBuffer[calcOffset]) * kernel1[filterY + filterOffset][filterX + filterOffset];

                    color2 += (grayscaleBuffer[calcOffset]) * kernel2[filterY + filterOffset][filterX + filterOffset];
                }
            }

            color1 = color1 - color2;
            color1 = ((color1 >= (weight1 - weight2)) ? 255 : 0);

            int i = source/height;
            int j = source % height;
            Color color = new Color(color1, color1, color1);
            if (j == 0) {
                System.out.println("");
            }
            System.out.println(color1 + " ");
            
            bufferedImageOut.setRGB(i, j, color.getRGB()); 

        }


        return bufferedImageOut;
    }

    private int getGray(int rgb) {
        return (getRed(rgb)*11+getGreen(rgb)*16+getBlue(rgb)*5)/32;
    }
    
    /**
     * Returns the red component in the range 0-255 in the default sRGB
     * space.
     * @return the red component.
     * @see #getRGB
     */
    public int getRed(int rgb) {
        return (rgb >> 16) & 0xFF;
    }

    /**
     * Returns the green component in the range 0-255 in the default sRGB
     * space.
     * @return the green component.
     * @see #getRGB
     */
    public int getGreen(int rgb) {
        return (rgb >> 8) & 0xFF;
    }

    /**
     * Returns the blue component in the range 0-255 in the default sRGB
     * space.
     * @return the blue component.
     * @see #getRGB
     */
    public int getBlue(int rgb) {
        return (rgb >> 0) & 0xFF;
    }

}
