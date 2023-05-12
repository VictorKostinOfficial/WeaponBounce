# WeaponBounce

Тестовое задание

## Часть 1: Рефлексия

В задании сказано, что рука с оружием должна физически корректно отскакивать от препядствий:
1. Необходимо определить точку удара с объектом;
2. Обработать (Физически корректно) реакцию персонажа на удар по объекту;

Для определения точки соприкосновения можно использовать Line Trace, отлавливать BeginOverlap событие и обрабатывать реакцию (отскок). Однако в таком случае оруже будет проходить сквозь объекты.

<img width="301" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/e4f667ed-34cc-49d0-b412-1f155645e1d7">
<img width="295" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/3bcfd9c1-1af7-4f83-80ee-3a9d3ca9735f">
<img width="268" alt="image" src="https://github.com/VictorKostinOfficial/WeaponBounce/assets/122555487/624aa965-870d-449b-bdf6-cf558d22f505">
