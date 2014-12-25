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
	
	
	@RequestMapping(value = "lab3", method = RequestMethod.GET)
	public String getMainLab3(Model model) {
	    model.addAttribute(ATTRIBUTE_TITLE_NAME,"lab3");
	    model.addAttribute(ATTRIBUTE_LAB_NUMBER_NAME,3);
	    return "labStart";
	}
	
	@RequestMapping(value = "labAction3", method = RequestMethod.POST)
    public String post(Model model, String imagePath) {
	    try {
            DifferenceOfGaussian differenceOfGaussian = new DifferenceOfGaussian(imagePath);
            model.addAttribute(ATTRIBUTE_IMAGE_PATH, differenceOfGaussian.getOutPutImagePath());
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
