package by.vurtatoo.misoi;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
public class Lab3 {
	
	private final Logger logger = LoggerFactory.getLogger(getClass());
	
	
	@RequestMapping(value = "lab3", method = RequestMethod.GET)
	public String getMainLab3() {
		// TODO Auto generated method stub
		return null;
	}

}
