package by.vurtatoo.misoi;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import static by.vurtatoo.misoi.HomeController.*; 

@Controller
public class LabController {
	
    private final Logger logger = LoggerFactory.getLogger(getClass());
	
	
	@RequestMapping(value = "labAction3")
    public String post(Model model, String imagePath,Integer matrixSize, Double weight1, Double weight2) {
	    if (matrixSize == null) {
	        matrixSize = 3;
	    }
	    
	    if (weight1 == null) {
	        weight1 = 0.5D;
	    }
	    
	    if (weight2 == null) {
            weight2 = 2D;
        }
	    
	    model.addAttribute(ATTRIBUTE_TITLE_NAME,"lab3");
        model.addAttribute(ATTRIBUTE_LAB_NUMBER_NAME,3);
	    try {
            DifferenceOfGaussian differenceOfGaussian = new DifferenceOfGaussian(imagePath);
            model.addAttribute(ATTRIBUTE_IMAGE_PATH, differenceOfGaussian.getOutPutImagePath(matrixSize, weight1, weight2));
        } catch (Exception e) { 
            model.addAttribute(ATTRIBUTE_RESULT, e.getLocalizedMessage());
        } 
        return "lab3Result";
    }
	
	
    @RequestMapping(value = "lab4", method = RequestMethod.GET)
    public String getMainLab4(Model model) {
        return "labStart";
    }

}
