<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>${title}</title>
</head>
<body>
<img src="../tmp/${imagePath}" />
<b>Фильтр Difference of Gaussian (DoG)</b>
Фильтр Difference of Gaussian (DoG) нужен для идентификации краев. DoG похож на фильтры Laplacian of Gaussian (LoG) и Difference of Box (DoB) тем что в нем тоже используется двухэтаный процесс обнаружения краев.<br>
DoG обнаруживает края путем применения размытия изображения по Гауссу (Gaussian blur) с указанным значением theta (также известным как sigma или standard deviation). Результирующее изображение является размытой версией исходного изображения. Далее применяется другое размытие с "более резким" значением theta, которое размывает изображение меньше чем в предыдущем случае. Финальное изображение вычисляется путем замены каждого пикселя разностью между двумя размытыми изображениями...
</body>
</html>