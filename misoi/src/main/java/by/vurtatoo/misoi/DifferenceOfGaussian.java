package by.vurtatoo.misoi;

import java.io.File;
import java.io.IOException;

public class DifferenceOfGaussian {

    File imageOriginal = null;
    
    public DifferenceOfGaussian(String imagePath) throws Exception {
        imageOriginal = new File(imagePath);
        if (!imageOriginal.isFile()) {
            throw new Exception("THIS IS BAD LINK");
        }
    }

    public String getOutPutImagePath() {
        File file = new File(System.currentTimeMillis() + ".jpg");
        try {
            file.createNewFile();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return file.getName();
    }

}
