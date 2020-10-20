#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "graphics.hpp"

void Minesweeper::show_sprites_(sf::RenderWindow& window)
{
  window.clear( {190, 190, 190} );

  struct {int x, y;} origin = {cell_width_, cell_width_};
  
  game_.for_each_index([&](int i, int j){
    auto state = game_.cell_state({i,j});
    sprites_[state].setPosition(i*16 + origin.x, j*16 + origin.y);
    window.draw(sprites_[state]);
  });

  window.display();
}

void Minesweeper::load_sprites_()
{ 
  if (!sprite_sheet_.loadFromFile("sprites/sprites.png",
    sf::IntRect(2, 53, 135, 33) ))
      throw("Couldn't load sprites/sprites.png");
  
  // loading number sprite
  for (int i = 1; i < 9; ++i)
  {
    sf::Sprite sprite(sprite_sheet_, sf::IntRect(17 * (i-1), 17, 16, 16));
    sprites_.emplace(std::to_string(i), sprite);
  }
  // loading unclicked, mine, etc
  sf::Sprite sp_unclicked   (sprite_sheet_, sf::IntRect(0,0,16,16));
  sf::Sprite sp_blank       (sprite_sheet_, sf::IntRect(17,0,16,16));
  sf::Sprite sp_flagged     (sprite_sheet_, sf::IntRect(34,0,16,16));
  sf::Sprite sp_mine        (sprite_sheet_, sf::IntRect(85,0,16,16));
  sf::Sprite sp_mine_clicked(sprite_sheet_, sf::IntRect(102,0,16,16));
  
  sprites_.emplace("unclicked",     sp_unclicked);
  sprites_.emplace("blank",         sp_blank);
  sprites_.emplace("flagged",       sp_flagged);
  sprites_.emplace("bombed",        sp_mine);
  sprites_.emplace("bomb-clicked",  sp_mine_clicked);
}

Minesweeper::Minesweeper(const Game& game): game_(game)
{

  int w = cell_width_ * ( game_.size().w + 2); // 2 for border
  int h = cell_width_ * ( game_.size().h + 2); // 2 for border
  
  sf::RenderWindow window(sf::VideoMode(w, h), "Minesweeper",
    sf::Style::Titlebar | sf::Style::Close);
  
  load_sprites_();
  show_sprites_(window);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      
      if (event.type == sf::Event::KeyPressed)
        if (event.key.code == sf::Keyboard::R)
        {
          game_.reset();
          show_sprites_(window);
        }

      if (event.type == sf::Event::MouseButtonPressed)
      {
        if (!game_.is_on())
          break;

        Game::Index index =
          {event.mouseButton.x / 16 - 1, event.mouseButton.y / 16 - 1};
        
        // invalid cell
        if (!game_.index_is_valid(index))
          continue;

        if (event.mouseButton.button == sf::Mouse::Left)
          game_.visit(index);
        else
        if (event.mouseButton.button == sf::Mouse::Right)
          game_.toggle_flag(index);
        
        show_sprites_(window);
      }
    }
  }
}

Minesweeper::~Minesweeper()
{
}
