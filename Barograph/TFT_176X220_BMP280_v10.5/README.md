# Barograph 24h

### A simple 24 hour barograph with a color graphic display. The pressure sensor BMP 280 and ArduinoPro 3.3V was used.

The 24 h barograph on the 2-inch TFT 220x176 display Open Smart c with the built-in driver LGDP4524. Controller -Atmega328P 3.3 V. Vertical autoscaling from the maximum to the minimum, when processing 220 pressure values ??in the array accumulated in 24 hours. In fact, there is averaging of each of the 220 values, since the pressure sensor is polled every second and only after integrating 393 values, every 393 seconds is recorded in the array for the graph. The shift of the graph is from right to left, so the pressure drop corresponds to the decline in the graph, respectively,  the growth of the pressure is indicated by the rise in the graph.

-----------------------------

Суточный барограф на 2-х дюймовом TFT 220х176 дисплее  Open Smart c со встроенным драйвером LGDP4524. 
Контроллер -Atmega328P 3.3 V.
Автомасштабирование по вертикали от максимума до минимума, при обработке 220 значений давления в массиве , накопленном за 24 часа.
На самом деле, происходит усреднение какждого из 220 значений, так как опрос датчика давления происходит каждую секунду и лишь после интегрирования 393 значений, каждые 393 секунды делается запись в массив для графика.
Сдвиг графика происходит справа налево, поэтому спад давления соответствует спаду на графике, соответственно рост даления отображается подъемом.

![front view](https://github.com/Aleksk1/Arduino/blob/master/Barograph/TFT_176X220_BMP280_v10.5/Pictures/IMG_20180130_144241res.jpg?raw=true)