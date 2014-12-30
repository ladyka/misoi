/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package by.vurtatoo;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.Random;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.stream.DoubleStream;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.image.PixelWriter;
import javafx.scene.image.WritableImage;
import javax.imageio.ImageIO;

/**
 *
 * @author user
 */
public class FXMLDocumentController implements Initializable {

    @FXML
    private ImageView image_1_1;

    @FXML
    private ImageView image_1_2;

    @FXML
    private ImageView image_1_3;

    @FXML
    private ImageView image_2_1;

    @FXML
    private ImageView image_2_2;

    @FXML
    private ImageView image_2_3;

    @FXML
    private ImageView image_3_1;

    @FXML
    private ImageView image_3_2;

    @FXML
    private ImageView image_3_3;

    @FXML
    private ImageView testImage;

    private BufferedImage testBuffImage;

    @FXML
    private Slider noiseSlider;

    @FXML
    private TextArea textArea;

    @FXML
    private void loadImagesAction(ActionEvent event) {
        ImageService iService = new ImageService();
        try {
            iService.loadImages();
        } catch (Exception ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }

        try {
            image_1_1.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(0).getArrayLetterViews().get(0).getImagePath())));
            image_1_2.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(0).getArrayLetterViews().get(1).getImagePath())));
            image_1_3.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(0).getArrayLetterViews().get(2).getImagePath())));

            image_2_1.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(1).getArrayLetterViews().get(0).getImagePath())));
            image_2_2.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(1).getArrayLetterViews().get(1).getImagePath())));
            image_2_3.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(1).getArrayLetterViews().get(2).getImagePath())));

            image_3_1.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(2).getArrayLetterViews().get(0).getImagePath())));
            image_3_2.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(2).getArrayLetterViews().get(1).getImagePath())));
            image_3_3.setImage(new Image(new FileInputStream(Storage.loadLettersImages.get(2).getArrayLetterViews().get(2).getImagePath())));

        } catch (FileNotFoundException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }


    public void pressImage_1_1(ActionEvent event) {
        Storage.letterId = 0;
        Storage.imageId = 0;
        testImage.setImage(image_1_1.getImage());
        loadTestImage();
    }

    public void pressImage_1_2(ActionEvent event) {
        Storage.letterId = 0;
        Storage.imageId = 1;
        testImage.setImage(image_1_2.getImage());
        loadTestImage();
    }

    public void pressImage_1_3(ActionEvent event) {
        Storage.letterId = 0;
        Storage.imageId = 2;
        testImage.setImage(image_1_3.getImage());
        loadTestImage();
    }

    public void pressImage_2_1(ActionEvent event) {
        Storage.letterId = 1;
        Storage.imageId = 0;
        testImage.setImage(image_2_1.getImage());
        loadTestImage();
    }

    public void pressImage_2_2(ActionEvent event) {
        Storage.letterId = 1;
        Storage.imageId = 1;
        testImage.setImage(image_2_2.getImage());
        loadTestImage();
    }

    public void pressImage_2_3(ActionEvent event) {
        Storage.letterId = 1;
        Storage.imageId = 2;
        testImage.setImage(image_2_3.getImage());
        loadTestImage();
    }

    public void pressImage_3_1(ActionEvent event) {
        Storage.letterId = 2;
        Storage.imageId = 0;
        testImage.setImage(image_3_1.getImage());
        loadTestImage();
    }

    public void pressImage_3_2(ActionEvent event) {
        Storage.letterId = 2;
        Storage.imageId = 1;
        testImage.setImage(image_3_2.getImage());
        loadTestImage();
    }

    public void pressImage_3_3(ActionEvent event) {
        Storage.letterId = 2;
        Storage.imageId = 2;
        testImage.setImage(image_3_3.getImage());
        loadTestImage();
    }

    @FXML
    public void addNoise(ActionEvent event) {
            System.out.println(" " + noiseSlider.getValue());
            int noise = (int) noiseSlider.getValue();

            loadTestImage();

            int width = testBuffImage.getWidth();
            int height = testBuffImage.getHeight();
            int allPixelsCount = (int) (height * width);
            int countOfChange = (allPixelsCount / 100) * noise;

            Random r = new Random();

            for (int j = 0; j < countOfChange; j++) {
                int w = r.nextInt(width);
                int h = r.nextInt(height);
                int rgbPixel = testBuffImage.getRGB(h, w);
                if (rgbPixel == -16777216) {
                    rgbPixel = -1;
                } else {
                    rgbPixel = -16777216;
                }
                testBuffImage.setRGB(h, w, rgbPixel);
            }

            WritableImage wr = new WritableImage(width, height);
            PixelWriter pw = wr.getPixelWriter();
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    pw.setArgb(x, y, testBuffImage.getRGB(x, y));
                }
            }

            testImage.setImage(wr);
    }
    
    private void loadTestImage() {
        try {
            File fileImage = new File(Storage.loadLettersImages.get(Storage.letterId).getArrayLetterViews().get(Storage.imageId).getImagePath());
            testBuffImage = ImageIO.read(fileImage);
        } catch (IOException ex) {
            Logger.getLogger(FXMLDocumentController.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    @FXML
    public void learnAction(ActionEvent event) {
        Storage.n.learn(Storage.loadLettersImages);
        textArea.appendText("\nLearn .... Done\n");
    }

    @FXML
    public void testAction(ActionEvent event) {
        ImageService imageService = new ImageService();
        if (imageService.checkRangeImage(testBuffImage)) {
            int[][] arrayImage = imageService.convertToArray(testBuffImage);
            double[] outputLayer = Storage.n.calculate(Storage.n.convertArray(arrayImage));
            //Storage.n.
            double sum = DoubleStream.of(outputLayer).sum();
            textArea.appendText("Similarity:\n");
           textArea.appendText("1st class: "+ outputLayer[0] / sum + "\n");
           textArea.appendText("2st class: "+ outputLayer[1] / sum + "\n");
           textArea.appendText("3st class: "+ outputLayer[2] / sum + "\n");
            

            int maxIndex = 0;
            for (int i = 1; i < 3; i++) {
                if (outputLayer[maxIndex] < outputLayer[i]) {
                    maxIndex = i;
                }
            }

            textArea.appendText("Test class is : " + (maxIndex+1));
        } else {
            throw new RuntimeException("Error testImage range");
        }
        textArea.appendText("\n============================\n");
    }

}
