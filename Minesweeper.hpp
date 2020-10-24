#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "Game.hpp"


class Minesweeper
{
public:
  void start();
  
  Minesweeper(Game& game);
  ~Minesweeper();

private:
  void display();
  void load_resources(); //2 53 135 33

  bool just_entered_the_game_ = true;
  bool just_finished_the_game_ = true;
  
  std::thread ending_thread_;

  static constexpr unsigned cell_width_ = 16;
  std::unordered_map<std::string, sf::Sprite> sprites_;
  sf::Texture sprite_sheet_;
  sf::Font font_;
  sf::RenderWindow window_;
  sf::Text welcome_text_;
  sf::Text win_text_;
  sf::Text lose_text_;

  Game *game_ { nullptr };
};


#endif // GRAPHICS_HPP
