<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>${title}</title>
</head>
<body>
<h1>${result}</h1>

<form action="labAction${labNumber}" method="post">
<input type="hidden" name="labNumber" value="${labNumber}" />
<br>
<input type="text" name="imagePath" />
<br>
Kernel Size
<br>
<input type="text" name="matrixSize" value="3" />
<br>
Weight 1
<br>
<input type="text" name="weight1" value="0.5"/>
<br>
Weight 2
<br>
<input type="text" name="weight2" value="2"/>

<input type="submit"/>

</form>

<br>

<hr>
<br>
<img src="../tmp/${imagePath}" width="600"/>
<br>
<br>
<b>Фильтр Difference of Gaussian (DoG)</b>
Фильтр Difference of Gaussian (DoG) нужен для идентификации краев. DoG похож на фильтры Laplacian of Gaussian (LoG) и Difference of Box (DoB) тем что в нем тоже используется двухэтаный процесс обнаружения краев.<br>
DoG обнаруживает края путем применения размытия изображения по Гауссу (Gaussian blur) с указанным значением theta (также известным как sigma или standard deviation). Результирующее изображение является размытой версией исходного изображения. Далее применяется другое размытие с "более резким" значением theta, которое размывает изображение меньше чем в предыдущем случае. Финальное изображение вычисляется путем замены каждого пикселя разностью между двумя размытыми изображениями...
</body>
</html>