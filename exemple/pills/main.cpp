#include "lcd.h"
#include "fixmath.h"

#define TOP    1
#define BOTTOM 30

#define RADIUS 12
#define COUNT  50
#define PRECIS 3

#define X      0
#define Y      1
#define VX     2
#define VY     3

Fix16 balls[COUNT][4];  // Q 32.16
RGB colors[COUNT];
LCD lcd;
Rect view(1, TOP + 1, lcd.max_x() - 1, lcd.max_y() - BOTTOM - 1);  // ;

uint32_t rnd() {
  static int64_t next = 1;
  next = next * 1103515245 + 12345;
  return next >> 16;
}

int main(void) {
  STK_E;
  delay_ms(50);
  lcd.init();
  lcd.color(Blue);
  lcd.font(sans_24, 0, 0);
  // lcd.font(micro_5x6, 0, 0);
  lcd.clear();
  lcd.rect(view.min_x - 1, view.min_y - 1, view.width + 2, view.height + 2);

  for (int i = 0; i < COUNT; i++) {
    // Задание начальных значений
    constexpr fix16_t mask = 0x1ffff;
    balls[i][X] = Fix16((int16_t)(lcd.max_x() >> 1));
    balls[i][Y] = Fix16((int16_t)(lcd.max_y() >> 1));
    balls[i][VX] = ((rnd() & 0x1) ? (fix16_t)(-(rnd() & mask)) : (fix16_t)(rnd() & mask)) / PRECIS;
    balls[i][VY] = ((rnd() & 0x1) ? (fix16_t)(-(rnd() & mask)) : (fix16_t)(rnd() & mask)) / PRECIS;
    colors[i] = rnd();
  }

  // Квадрат минимального расстояния между шарами
  const Fix16 D2 = (int16_t)(RADIUS * RADIUS * 4);

loop:

  STK_C;
  for (int i = 0; i < COUNT; i++) {

    // Псевдонимы
    Fix16 &X0 = balls[i][X];
    Fix16 &Y0 = balls[i][Y];
    Fix16 &VX0 = balls[i][VX];
    Fix16 &VY0 = balls[i][VY];

    const Fix16 Xold = X0;
    const Fix16 Yold = Y0;

    for (auto k = 0; k < PRECIS; k++) {
      // Приращение координат
      X0 += VX0;
      Y0 += VY0;

      // Отражение от стенок: дополнено проверяется,
      // что действительно происходит сближение, а не отскок
      if (X0 > (int16_t)(view.max_x - RADIUS) && VX0 > (int16_t)0) VX0 = -VX0;
      if (X0 < (int16_t)(view.min_x + RADIUS) && VX0 < (int16_t)0) VX0 = -VX0;
      if (Y0 > (int16_t)(view.max_y - RADIUS) && VY0 > (int16_t)0) VY0 = -VY0;
      if (Y0 < (int16_t)(view.min_y + RADIUS) && VY0 < (int16_t)0) VY0 = -VY0;

      // Соударения между объектами
      for (int j = i + 1; j < COUNT; j++) {

        // Псевдонимы
        Fix16 &X1 = balls[j][X];
        Fix16 &Y1 = balls[j][Y];
        Fix16 &VX1 = balls[j][VX];
        Fix16 &VY1 = balls[j][VY];

        // Проверяем квадрат расстояния, чтобы не вычислять корень
        Fix16 DX = X1 - X0;
        Fix16 DY = Y1 - Y0;
        Fix16 S2 = DX * DX + DY * DY;

        // Слишком далеко: удар не происходит
        if (S2 > D2) continue;

        // Защита от больших погрешностей или переполнения
        if (S2 < 0.001) continue;

        // Проекции скоростей на прямую проходящею через центры масс
        // Делим сразу на квадрат расстояния, для
        // экономии на извлечении корня и 2ом делении
        Fix16 V0 = (VX0 * DX + VY0 * DY) / S2;
        Fix16 V1 = (VX1 * DX + VY1 * DY) / S2;

        // Вычисляем скорость сближения (делённую на расстояние)
        Fix16 DV = V1 - V0;

        // Проверка на сближение
        if (DV > (int16_t)0) continue;

        // Находим проекции скоростей сближения на оси координат
        // На расстояние мы уже разделили, поэтому тут просто умножение
        Fix16 DVX = DV * DX;
        Fix16 DVY = DV * DY;

        // Изменяем скорости сближения, на скорости удаления
        VX0 += DVX;
        VY0 += DVY;
        VX1 -= DVX;
        VY1 -= DVY;
      }
    }

    // Стираем устаревший объект с экрана
    lcd.color(Black);
    // lcd.fill((int)Xold - RADIUS , (int)Yold - RADIUS , (int)Xold + RADIUS , (int)Yold + RADIUS );
    lcd.circle_fill(Xold, Yold, RADIUS);
    // lcd.circle(Xold, Yold, RADIUS);

    // Отображаем объект в новом месте
    lcd.color(colors[i]);
    // lcd.fill((int)X0 - RADIUS + 1, (int)Y0 - RADIUS + 1, (int)X0 + RADIUS - 1, (int)Y0 + RADIUS - 1);
    lcd.circle_fill(X0, Y0, RADIUS);
    // lcd.circle(X0, Y0, RADIUS);
  }

  uint32_t fps = (F_CPU << 3) / STK_CNT;
  lcd.color(Aquamarine);
  lcd.viewport();
  lcd.at(10, lcd.max_y() - lcd.get_height());
  lcd.printf(P("FPS: %.1.3q  "), fps);
  lcd.viewport(&view);

  lcd.release();

  goto loop;
}
