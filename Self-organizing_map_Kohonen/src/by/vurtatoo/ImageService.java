/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package by.vurtatoo;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.AbstractList;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;

/**
 *
 * @author user
 */
public class ImageService {

    void loadImages() throws Exception {
        System.out.println("You clicked me!");
        File folderImages = new File("images");
        if (folderImages.isDirectory()) {
            Storage.loadLettersImages = getImages(folderImages.listFiles());
        } else {
            throw new Exception("THIS IS NOT DERICTORY !!!! PATH  : " +  folderImages.getAbsolutePath());
        }
    }

    private List<Letter> getImages(File[] listFiles) {
        List<Letter> letters = new ArrayList(listFiles.length);
        for (File letterDirectory : listFiles) {
            if (letterDirectory.isDirectory()) {
                Letter letter = new Letter();
                letter.setName(letterDirectory.getName());
                //List<int[25][25]> images = new ArrayList<int[][]>();
                for (File letterFile : letterDirectory.listFiles()) {
                    try {
                        letter.addArrayLetterViews(getLetterMatrix(letterFile),letterFile.getAbsolutePath());
                    } catch (Exception ex) {
                        Logger.getLogger(ImageService.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
                letters.add(letter);
            } else {
                System.err.println("FAil Derictory " + letterDirectory.getAbsolutePath());
            }
        }
        return letters;
    }

    private int [][] getLetterMatrix(File letterFile) throws Exception {
        String errorMessage;
        if (letterFile.isFile()) {
            BufferedImage bufferedImage = ImageIO.read(letterFile);
            int width = bufferedImage.getWidth();
            int height = bufferedImage.getHeight();
            if (width == 25 && height == 25 ) {
                int [][] a = new int[25][25];
                for (int i = 0; i < width; i++) {
                    for (int j = 0; j < height; j++) {
                        int b = bufferedImage.getRGB(j, i);
                        if (b < -1) {
                            b = 1;
                        } else {
                            b = 0;
                        }
                        a[j][i] = b;
                       // System.out.print(b + " ");
                    }
                    //System.out.println(" ");
                }
                //System.out.println(" +  ");
                return a;
            } else {
                errorMessage = "Fail size image " + letterFile.getAbsolutePath();
            }
        } else {
            errorMessage = "This is not file " + letterFile.getAbsolutePath();
        }
        throw new Exception(errorMessage);
    }
    
   
    
}
