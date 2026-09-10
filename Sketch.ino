
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>


// ============================================================
//                         DISPLAY
// ============================================================

#define TFT_CS 53
#define TFT_DC 49

Adafruit_ILI9341 tft(
  TFT_CS,
  TFT_DC
);


// ============================================================
//                       SNAKE GAME
// ============================================================

class SnakeGame
{
public:

  SnakeGame(Adafruit_ILI9341 &display)
    : tft(display)
  {
    powered = false;

    snakeLength = 3;

    direction = RIGHT;
    nextDirection = RIGHT;

    state = WAITING;

    lastMoveTime = 0;
  }


  // ==========================================================
  //                       POWER
  // ==========================================================

  void power()
  {
    powered = !powered;


    if (powered)
    {
      state = WAITING;

      drawStartScreen();
    }
    else
    {
      state = WAITING;

      tft.fillScreen(
        ILI9341_BLACK
      );
    }
  }


  // ==========================================================
  //                          START
  // ==========================================================

  void start()
  {
    if (!powered)
    {
      return;
    }


    if (
      state == WAITING ||
      state == GAME_OVER
    )
    {
      reset();
    }
  }


  // ==========================================================
  //                          PAUSE
  // ==========================================================

  void pause()
  {
    if (!powered)
    {
      return;
    }


    if (state == PLAYING)
    {
      state = PAUSED;

      drawPauseScreen();
    }

    else if (state == PAUSED)
    {
      state = PLAYING;

      lastMoveTime =
        millis();

      drawFullGame();
    }
  }


  // ==========================================================
  //                            UP
  // ==========================================================

  void up()
  {
    if (
      powered &&
      state == PLAYING &&
      direction != DOWN
    )
    {
      nextDirection = UP;
    }
  }


  // ==========================================================
  //                           DOWN
  // ==========================================================

  void down()
  {
    if (
      powered &&
      state == PLAYING &&
      direction != UP
    )
    {
      nextDirection = DOWN;
    }
  }


  // ==========================================================
  //                           LEFT
  // ==========================================================

  void left()
  {
    if (
      powered &&
      state == PLAYING &&
      direction != RIGHT
    )
    {
      nextDirection = LEFT;
    }
  }


  // ==========================================================
  //                          RIGHT
  // ==========================================================

  void right()
  {
    if (
      powered &&
      state == PLAYING &&
      direction != LEFT
    )
    {
      nextDirection = RIGHT;
    }
  }


  // ==========================================================
  //                          UPDATE
  // ==========================================================

  void update()
  {
    if (!powered)
    {
      return;
    }


    if (state != PLAYING)
    {
      return;
    }


    unsigned long now =
      millis();


    if (
      now - lastMoveTime <
      MOVE_INTERVAL
    )
    {
      return;
    }


    lastMoveTime =
      now;


    move();
  }


private:

  // ==========================================================
  //                          TYPES
  // ==========================================================

  struct Point
  {
    int x;
    int y;
  };


  enum Direction
  {
    UP,
    DOWN,
    LEFT,
    RIGHT
  };


  enum State
  {
    WAITING,
    PLAYING,
    PAUSED,
    GAME_OVER
  };


  // ==========================================================
  //                         SETTINGS
  // ==========================================================

  static const int CELL_SIZE = 16;

  static const int FIELD_COLS = 20;
  static const int FIELD_ROWS = 15;

  static const int MAX_SNAKE = 300;

  static const unsigned long MOVE_INTERVAL = 180;


  // ==========================================================
  //                           DATA
  // ==========================================================

  Adafruit_ILI9341 &tft;


  bool powered;


  Point snake[MAX_SNAKE];

  Point food;


  int snakeLength;


  Direction direction;

  Direction nextDirection;


  State state;


  unsigned long lastMoveTime;


  // ==========================================================
  //                     IS SNAKE CELL
  // ==========================================================

  bool isSnakeCell(
    int x,
    int y
  )
  {
    for (
      int i = 0;
      i < snakeLength;
      i++
    )
    {
      if (
        snake[i].x == x &&
        snake[i].y == y
      )
      {
        return true;
      }
    }


    return false;
  }


  // ==========================================================
  //                       CREATE FOOD
  // ==========================================================

  void createFood()
  {
    do
    {
      food.x =
        random(
          0,
          FIELD_COLS
        );


      food.y =
        random(
          0,
          FIELD_ROWS
        );
    }
    while (
      isSnakeCell(
        food.x,
        food.y
      )
    );
  }


  // ==========================================================
  //                        DRAW CELL
  // ==========================================================

  void drawCell(
    int x,
    int y,
    uint16_t color
  )
  {
    tft.fillRect(
      x * CELL_SIZE,
      y * CELL_SIZE,
      CELL_SIZE,
      CELL_SIZE,
      color
    );
  }


  // ==========================================================
  //                    DRAW FULL GAME
  // ==========================================================

  void drawFullGame()
  {
    tft.fillScreen(
      ILI9341_BLACK
    );


    // Еда

    drawCell(
      food.x,
      food.y,
      ILI9341_RED
    );


    // Змейка

    for (
      int i = 0;
      i < snakeLength;
      i++
    )
    {
      if (i == 0)
      {
        drawCell(
          snake[i].x,
          snake[i].y,
          ILI9341_GREEN
        );
      }
      else
      {
        drawCell(
          snake[i].x,
          snake[i].y,
          ILI9341_DARKGREEN
        );
      }
    }
  }


  // ==========================================================
  //                     START SCREEN
  // ==========================================================

  void drawStartScreen()
  {
    tft.fillScreen(
      ILI9341_BLACK
    );


    tft.setTextColor(
      ILI9341_GREEN
    );

    tft.setTextSize(4);

    tft.setCursor(
      95,
      60
    );

    tft.print(
      "SNAKE"
    );


    tft.setTextColor(
      ILI9341_WHITE
    );

    tft.setTextSize(2);

    tft.setCursor(
      75,
      130
    );

    tft.print(
      "PRESS START"
    );


    tft.setTextSize(1);

    tft.setCursor(
      65,
      180
    );

    tft.print(
      "ARROWS - MOVE   P - PAUSE"
    );
  }


  // ==========================================================
  //                     PAUSE SCREEN
  // ==========================================================

  void drawPauseScreen()
  {
    tft.fillRect(
      80,
      90,
      160,
      60,
      ILI9341_BLACK
    );


    tft.drawRect(
      80,
      90,
      160,
      60,
      ILI9341_WHITE
    );


    tft.setTextColor(
      ILI9341_YELLOW
    );

    tft.setTextSize(3);

    tft.setCursor(
      110,
      110
    );

    tft.print(
      "PAUSE"
    );
  }


  // ==========================================================
  //                    GAME OVER SCREEN
  // ==========================================================

  void drawGameOver()
  {
    tft.fillScreen(
      ILI9341_BLACK
    );


    tft.setTextColor(
      ILI9341_RED
    );

    tft.setTextSize(4);

    tft.setCursor(
      45,
      70
    );

    tft.print(
      "GAME OVER"
    );


    tft.setTextColor(
      ILI9341_WHITE
    );

    tft.setTextSize(2);

    tft.setCursor(
      70,
      140
    );

    tft.print(
      "PRESS START"
    );


    tft.setCursor(
      90,
      180
    );

    tft.print(
      "Score: "
    );

    tft.print(
      snakeLength - 3
    );
  }


  // ==========================================================
  //                          RESET
  // ==========================================================

  void reset()
  {
    snakeLength = 3;


    snake[0].x = 10;
    snake[0].y = 7;


    snake[1].x = 9;
    snake[1].y = 7;


    snake[2].x = 8;
    snake[2].y = 7;


    direction =
      RIGHT;

    nextDirection =
      RIGHT;


    createFood();


    state =
      PLAYING;


    lastMoveTime =
      millis();


    // Полностью экран рисуем только один раз
    // при начале новой игры

    drawFullGame();
  }


  // ==========================================================
  //                           MOVE
  // ==========================================================

  void move()
  {
    // --------------------------------------------------------
    // Применяем выбранное направление
    // --------------------------------------------------------

    direction =
      nextDirection;


    // --------------------------------------------------------
    // Запоминаем старую голову и хвост
    // --------------------------------------------------------

    Point oldHead =
      snake[0];


    Point oldTail =
      snake[snakeLength - 1];


    // --------------------------------------------------------
    // Рассчитываем новую голову
    // --------------------------------------------------------

    Point newHead =
      snake[0];


    switch (direction)
    {
      case UP:

        newHead.y--;

        break;


      case DOWN:

        newHead.y++;

        break;


      case LEFT:

        newHead.x--;

        break;


      case RIGHT:

        newHead.x++;

        break;
    }


    // --------------------------------------------------------
    // Столкновение со стеной
    // --------------------------------------------------------

    if (
      newHead.x < 0 ||
      newHead.x >= FIELD_COLS ||
      newHead.y < 0 ||
      newHead.y >= FIELD_ROWS
    )
    {
      state =
        GAME_OVER;


      drawGameOver();


      return;
    }


    // --------------------------------------------------------
    // Проверяем еду
    // --------------------------------------------------------

    bool ateFood =
      newHead.x == food.x &&
      newHead.y == food.y;


    // --------------------------------------------------------
    // Проверяем столкновение с собой
    // --------------------------------------------------------

    int collisionLength =
      snakeLength;


    if (!ateFood)
    {
      collisionLength--;
    }


    for (
      int i = 0;
      i < collisionLength;
      i++
    )
    {
      if (
        snake[i].x == newHead.x &&
        snake[i].y == newHead.y
      )
      {
        state =
          GAME_OVER;


        drawGameOver();


        return;
      }
    }


    // --------------------------------------------------------
    // Если съели еду - увеличиваем длину
    // --------------------------------------------------------

    if (
      ateFood &&
      snakeLength < MAX_SNAKE
    )
    {
      snakeLength++;
    }


    // --------------------------------------------------------
    // Сдвигаем тело
    // --------------------------------------------------------

    for (
      int i = snakeLength - 1;
      i > 0;
      i--
    )
    {
      snake[i] =
        snake[i - 1];
    }


    // --------------------------------------------------------
    // Новая голова
    // --------------------------------------------------------

    snake[0] =
      newHead;


    // ========================================================
    //               РИСУЕМ ТОЛЬКО ИЗМЕНЕНИЯ
    // ========================================================


    // --------------------------------------------------------
    // Старая голова теперь стала телом
    // --------------------------------------------------------

    drawCell(
      oldHead.x,
      oldHead.y,
      ILI9341_DARKGREEN
    );


    // --------------------------------------------------------
    // Если еду НЕ съели,
    // старый хвост нужно стереть
    // --------------------------------------------------------

    if (!ateFood)
    {
      drawCell(
        oldTail.x,
        oldTail.y,
        ILI9341_BLACK
      );
    }


    // --------------------------------------------------------
    // Рисуем новую голову
    // --------------------------------------------------------

    drawCell(
      newHead.x,
      newHead.y,
      ILI9341_GREEN
    );


    // --------------------------------------------------------
    // Если съели еду -
    // создаём и рисуем новую
    // --------------------------------------------------------

    if (ateFood)
    {
      createFood();


      drawCell(
        food.x,
        food.y,
        ILI9341_RED
      );
    }
  }
};


// ============================================================
//                       ОБЪЕКТ ИГРЫ
// ============================================================

SnakeGame game(tft);

/* ============================================================

Ваша задача:

0.Внимательно прочитать код игры, понять из чего он состоит
1.Подключить кнопки и прописать им соответствующие пины
2.Написать две недрстающих функции: Controls_Init() и Controls_Update()
3.Забиндовать на кнопки функции из класса SnakeGame со ссылкой на объект игры game

Потребуются вам уже знакомые методы digitalRead() и pinMode() 
   ============================================================ */

// ============================================================
//                         CONTROLS 
// ============================================================

#define BTN_UP 20     
#define BTN_DOWN 17  
#define BTN_LEFT 19  
#define BTN_RIGHT 18  
#define BTN_START 16    
#define BTN_PAUSE 15   
#define BTN_ONOFF 14 

void Controls_Init()
{
pinMode(BTN_UP, INPUT_PULLUP);
pinMode(BTN_DOWN, INPUT_PULLUP);
pinMode(BTN_LEFT, INPUT_PULLUP);
pinMode(BTN_RIGHT, INPUT_PULLUP);
pinMode(BTN_START, INPUT_PULLUP);
pinMode(BTN_PAUSE, INPUT_PULLUP);
pinMode(BTN_ONOFF, INPUT_PULLUP);
}

void Controls_Update()
{
 if(digitalRead(BTN_ONOFF) == LOW) {
  game.power();
  delay(50);
 }
  if(digitalRead(BTN_START) == LOW) {
  game.start();
 }
  if(digitalRead(BTN_PAUSE) == LOW) {
  game.pause();
 }
  if(digitalRead(BTN_UP) == LOW) {
  game.up();
 }
  if(digitalRead(BTN_DOWN) == LOW) {
  game.down();
 }
  if(digitalRead(BTN_LEFT) == LOW) {
  game.left();
 }
  if(digitalRead(BTN_RIGHT) == LOW) {
  game.right();
 }
 

}


// ============================================================
//                          SETUP
// ============================================================

void setup()
{
  tft.begin();


  // 320 x 240

  tft.setRotation(1);


  // После запуска консоль выключена

  tft.fillScreen(
    ILI9341_BLACK
  );


  Controls_Init();
}


// ============================================================
//                           LOOP
// ============================================================

void loop()
{
  Controls_Update();

  game.update();
}
