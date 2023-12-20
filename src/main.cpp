#include <iostream>
#include "raylib.h"

// v1.0.23

// ########### SETTINGS ###########
  // screen config // 
  const int screen_width = 1915;
  const int screen_height = 1000;
  const char screen_title[9] = "| Pong.|";
  const int game_speed = 60;
  // default player config //
  const int player_width = 40;
  const int player_height = 200;
  const int min_playerSpeedY = 4;
  const int max_playerSpeedY = 7;
  // default ball config //
  const int ball_width = 35;
  const int min_ballSpeedX = 7;
  const int max_ballSpeedX = 13;
  // sounds //
  Sound sound_menu;
  Sound sound_hit1;
  Sound sound_hit2;
  Sound sound_endgame;
  Sound sound_score;
  Sound sound_hit3;
  Sound music;
  // others //
  int score_one = 0, score_two = 0;
  int gameOver = 1;

void ResetScores(int *a, int *b) { *a = *b = 0; }

class Ball
{
  public:
  float x, y;
  float speedX, speedY;
  float width, height;

  void Draw() { DrawRectangle(x, y, width, height, WHITE); }

  void ResetBall()
  {
    x = GetScreenWidth()/2;
    y = GetScreenHeight()/2;
  }

  void Update()
  {
    x += speedX;
    y += speedY;

    if(width + x >= GetScreenWidth() || x <= 0){
      speedX *= -1;
      PlaySound(sound_hit3);
    }

    if(height + y >= GetScreenHeight() || y <= 0){
      speedY *= -1;
      PlaySound(sound_hit3);
    }

    if(x <= 0){
      score_two += 1;
      if (score_two >= 5){
        score_one = 0;
        score_two = 0;
        gameOver = 1;
      }
      ResetBall();
      speedX = -min_ballSpeedX + 1;
      speedY = min_ballSpeedX - 1;
      PlaySound(sound_score);
    } 

    if(width + x >= GetScreenWidth()){
      score_one += 1;
      if (score_one >= 5){
        score_one = 0;
        score_two = 0;
        gameOver = 1;
      }
      ResetBall();
      speedX = min_ballSpeedX - 1;
      speedY = -min_ballSpeedX + 1;
      PlaySound(sound_score);
    } 

  }
};

class Player
{
  public:
  float x, y;
  int speedY;
  float width, height;
  int playerNumber;

  void Draw() { DrawRectangle(x, y, width, height, WHITE); }

  void ResetPlayer(){
    y = GetScreenHeight()/2 - height/2;
  }

  void Update(int ball_y)
  {
    if(y <= 0) y = 0;
    if(height + y >= GetScreenHeight()) y = GetScreenHeight() - height;
    if(IsKeyDown(KEY_W) && playerNumber == 1) y -= speedY;
    if(IsKeyDown(KEY_S) && playerNumber == 1) y += speedY;
    if(IsKeyDown(KEY_UP) && playerNumber == 2) y -= speedY;
    if(IsKeyDown(KEY_DOWN) && playerNumber == 2) y += speedY;
    if (y + height / 2 > ball_y && playerNumber == 0) y -= speedY;
    if (y + height / 2 <= ball_y && playerNumber == 0) y += speedY;
    if (speedY < max_playerSpeedY  && playerNumber == 0) speedY = max_playerSpeedY;
  }
};

Ball ball;
Player one;
Player two;

int main()
{
// ########## DEFINITIONS ##########
  // screen definitions //
  int screen_Xmiddle = screen_width / 2;
  int screen_Ymiddle = screen_height / 2;
  // ball definitions //
  ball.width = ball_width;
  ball.height = ball_width;  
  ball.x = screen_Xmiddle;
  ball.y = screen_Ymiddle;
  ball.speedX = min_ballSpeedX;
  ball.speedY = min_ballSpeedX;
  // player one definitions //
  one.width = player_width;
  one.height = player_height;
  one.x = one.width/2;
  one.y = screen_Ymiddle - one.height/2;
  one.speedY = min_playerSpeedY;
  one.playerNumber = 2;
  // player two definitions //
  two.width = player_width;
  two.height = player_height;
  two.x = screen_width - two.width*1.5;
  two.y = screen_Ymiddle - two.height/2;
  two.speedY = min_playerSpeedY;
  two.playerNumber = 0;

// ########### GAME LOOP ###########
  InitWindow(screen_width, screen_height, screen_title);
  InitAudioDevice();

  sound_menu = LoadSound("sounds/menuselect.wav");
  sound_hit1 = LoadSound("sounds/cinematic 1.wav");
  sound_hit2 = LoadSound("sounds/cinematic 2.wav");
  sound_hit3 = LoadSound("sounds/laser gun.wav");
  sound_score = LoadSound("sounds/score.wav");
  sound_endgame = LoadSound("sounds/gameover.wav");

  SetTargetFPS(game_speed);

  while (!WindowShouldClose()){
    if(IsKeyPressed(KEY_ENTER)) gameOver = 0;

    if(gameOver){
      BeginDrawing();
      ClearBackground(BLACK);
      DrawText(TextFormat("| PONG.|"), screen_width/3, screen_height/2 - screen_height/3, 150, WHITE);
      DrawText(TextFormat("Press Enter to play!"), screen_width/4 + screen_width/13, screen_height/2, 60, WHITE);
      DrawText(TextFormat("1 to set player one"), screen_width/3, screen_height/2 + 60, 60, WHITE);
      DrawText(TextFormat("2 to set player two"), screen_width/3, screen_height/2 + 120, 60, WHITE);
      DrawText(TextFormat("Made by: Eduardo Sandes"), screen_width - screen_width/6, screen_height - screen_height/20, 20, WHITE);

      DrawText(TextFormat("%i", one.playerNumber), screen_width/2 - 500, screen_height/6, 80, WHITE);
      DrawText(TextFormat("%i", two.playerNumber), screen_width - (screen_width/2 - 558), screen_height/6, 80, WHITE);

      if(IsKeyPressed(KEY_KP_1)){
        one.playerNumber ++;
        if(one.playerNumber >= 3) one.playerNumber = 0;
        PlaySound(sound_menu);
      }
      if(IsKeyPressed(KEY_KP_2)){
        two.playerNumber ++;
        if(two.playerNumber >= 3) two.playerNumber = 0;
        PlaySound(sound_menu);
      }
      EndDrawing();
    }

    if(!gameOver){
      BeginDrawing();

      // updating objects //
      ball.Update();
      one.Update(ball.y);
      two.Update(ball.y);

      // checking collisions //
      if (CheckCollisionRecs(Rectangle{ ball.x, ball.y, ball.width, ball.height }, Rectangle{ one.x, one.y, one.width, one.height })) {
        if(ball.speedX < 0){
          if(ball.speedX > -max_ballSpeedX) ball.speedX *= -1.1;
                  else ball.speedX = max_ballSpeedX;
          ball.speedY = (((ball.y + (ball.height)/2) - (one.y + (one.height/2)))/(one.height/2)) * (-ball.speedX);
        }
        PlaySound(sound_hit1);
      }


      if(CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.height}, Rectangle{two.x, two.y, two.width, two.height})){
        if(ball.speedX > 0){
          if(ball.speedX < max_ballSpeedX) ball.speedX *= -1.1;
          else ball.speedX = -max_ballSpeedX;
          ball.speedY = (((ball.y + (ball.height)/2) - (two.y + (two.height/2)))/(two.height/2)) * (ball.speedX);
        }
        PlaySound(sound_hit2);
      }

      // drawing frame //
      ClearBackground(BLACK);

      DrawRectangle(screen_Xmiddle-125, screen_Ymiddle-125, 250, 250, WHITE);
      DrawRectangle(screen_Xmiddle-124, screen_Ymiddle-124, 248, 248, BLACK);
      DrawLine(screen_Xmiddle, 0, screen_Xmiddle, screen_height, WHITE);

      ball.Draw();
      one.Draw();
      two.Draw();

      DrawText(TextFormat("%i", score_one), screen_width/2 - 100, screen_height/6, 80, WHITE);
      DrawText(TextFormat("%i", score_two), screen_width - (screen_width/2 - 58), screen_height/6, 80, WHITE);
      DrawText(TextFormat("%.2f", ball.speedX), screen_width - (screen_width/3), screen_height/2 + screen_height/6, 20, WHITE);
      DrawText(TextFormat("%.2f", ball.speedY), screen_width - (screen_width/3), screen_height/2 + screen_height/6 + 20, 20, WHITE);

      if(gameOver) PlaySound(sound_endgame);
      if(IsKeyPressed(KEY_P)) gameOver = 1;
      if(IsKeyPressed(KEY_R)){
        ball.ResetBall();
        one.ResetPlayer();
        two.ResetPlayer();
        ResetScores(&score_one, &score_two);
        gameOver = 1;
      }
      EndDrawing();
    }
  }
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
// Yep that's me, or at least who I thought I'd be.
// But life moves on, suddenly I'm 19.