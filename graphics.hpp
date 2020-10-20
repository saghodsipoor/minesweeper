#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "Game.hpp"

class Minesweeper
{
public:
  void start();
  
  Minesweeper(const Game& game = Game());
  ~Minesweeper();

private:
  void display();
  void load_sprites(); //2 53 135 33

  static constexpr unsigned cell_width_ = 16;

  std::unordered_map<std::string, sf::Sprite> sprites_;
  sf::Texture sprite_sheet_;
  sf::RenderWindow window_;
  Game game_;
};


#endif // GRAPHICS_HPP
