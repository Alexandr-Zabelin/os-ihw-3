# os-ihw-3

# Забелин Александр Сергеевич, БПИ229
# Вариант 22

Задача о картинной галерее. Вахтер следит за тем, чтобы в
картинной галерее одновременно было не более 25 посетителей.
Для обозрения представлены 5 картин. Каждый посетитель случайно переходит от картины к картине, но если на желаемую картину любуются более пяти посетителей, он стоит в стороне и ждет,
пока число желающих увидеть эту картину не станет меньше. Посетитель покидает галерею по завершении осмотра всех картин.
Каждый посетитель уникальный, то есть имеет свой номер (например, PID). В галерею также пытаются постоянно зайти новые
посетители, которые ожидают своей очереди и разрешения от вахтера, если та заполнена.


## Логика программ

В качестве вахтера выступает сервер, который в каждом потоке отвечает одному клиенту. По факту, вахтер - просто счетчик, который считает, сколько в каждой из галерей находится сейчас посетителей, и в зависимости от этого разрешает или не разрешает войти следующему.

Клиент же представляет собой галерею, где каждый посетитель - отдельный поток. регулирование количества людей у каждой из картин происходит через соответствующей ей семафор.

## Сборка и запуск программ

Сборка производится через Makefile. Для того, чтобы собрать и запустить программу на оценку x необходимо:
1) Перейти в директорию x
2) Собрать исполняемые файлы через команду make
3) Запустить сервер и клиент(-ы) - ./TCPServer <port> и ./TCPClient <ip> <port> <galleryName> <visitorsCount> соответственно